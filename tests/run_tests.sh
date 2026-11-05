#!/usr/bin/env bash
# Feeds scripted input to the rental system and checks the output.
# Each test runs in a fresh temporary folder and must finish within 5 seconds,
# so an infinite loop counts as a failure.
#
# Usage: tests/run_tests.sh [path/to/rental]   (builds ./rental if no path is given)

set -u
cd "$(dirname "$0")/.."

BIN="${1:-}"
if [ -z "$BIN" ]; then
    g++ -std=c++17 -Wall -Wextra -o rental main.cpp implementation.cpp || exit 1
    BIN="./rental"
fi
BIN="$(cd "$(dirname "$BIN")" && pwd)/$(basename "$BIN")"

pass=0
fail=0

# check NAME INPUT EXPECTED_TEXT [MUST_NOT_CONTAIN]
check() {
    local name="$1" input="$2" expected="$3" forbidden="${4:-}"
    local dir out status
    dir="$(mktemp -d)"
    out="$(cd "$dir" && printf '%b' "$input" | timeout 5 "$BIN" 2>&1)"
    status=$?
    rm -rf "$dir"

    if [ $status -eq 124 ]; then
        echo "FAIL  $name (timed out: infinite loop?)"
        fail=$((fail + 1))
    elif ! grep -qF -- "$expected" <<<"$out"; then
        echo "FAIL  $name (expected: $expected)"
        fail=$((fail + 1))
    elif [ -n "$forbidden" ] && grep -qF -- "$forbidden" <<<"$out"; then
        echo "FAIL  $name (should not contain: $forbidden)"
        fail=$((fail + 1))
    else
        echo "ok    $name"
        pass=$((pass + 1))
    fi
}

ADD_VIDEO='1\nSpirited Away\nAnimation\nStudio Ghibli\n2\nspirited.jpg\n'
ADD_CUSTOMER='4\n1\nJuan Dela Cruz\nManila\n'

# Happy path
check "rent a video" \
    "${ADD_VIDEO}${ADD_CUSTOMER}2\n1\n1\nn\nn\n7\n" \
    "copy 2 rented to Juan Dela Cruz"
check "return a rented video" \
    "${ADD_VIDEO}${ADD_CUSTOMER}2\n1\n1\nn\nn\n3\n1\n1\n2\n6\n1\n7\n" \
    "has 2 of 2 copies available"

# Bad menu input
check "letters at the main menu" "abc\n7\n" "'abc' is not a whole number"
check "number out of range at the menu" "9\n7\n" "Please enter a number from 1 to 7."
check "number with trailing text" "2abc\n7\n" "'2abc' is not a whole number"
check "huge number" "99999999999999999999\n7\n" "That number is too large"
check "empty input" "\n7\n" "Please enter a number."
check "input ends (Ctrl+D)" "1\nTitle\n" "Input ended. Exiting system."

# Bad video input
check "empty movie title" "1\n\n   \nTitle\nG\nP\n1\nimg\n7\n" "This field can't be empty."
check "zero copies" "1\nTitle\nG\nP\n0\n1\nimg\n7\n" "Please enter a number from 1 to 100."
check "negative copies" "1\nTitle\nG\nP\n-5\n1\nimg\n7\n" "Please enter a number from 1 to 100."
check "letters for copies" "1\nTitle\nG\nP\nfive\n1\nimg\n7\n" "'five' is not a whole number"

# Renting
check "rent with unknown customer" "${ADD_VIDEO}2\n42\nn\n7\n" "Customer not found."
check "rent when the store is empty" "${ADD_CUSTOMER}2\n1\nn\n7\n" "There are no videos in the store yet."
check "rent unknown video, then cancel" "${ADD_VIDEO}${ADD_CUSTOMER}2\n1\n99\n0\nn\n7\n" "No video with ID 99"
check "rent when all copies are out" \
    "1\nSolo\nG\nP\n1\nimg\n${ADD_CUSTOMER}2\n1\n1\nn\ny\n1\n1\nn\n7\n" \
    "all copies of 'Solo' are currently rented out"
check "invalid y/n answer" "${ADD_VIDEO}${ADD_CUSTOMER}2\n1\n1\nmaybe\nn\nn\n7\n" "Please answer y or n."

# Returning
check "return with nothing rented" "${ADD_CUSTOMER}3\n1\n7\n" "has no videos to return."
check "return a copy the customer doesn't have" \
    "${ADD_VIDEO}${ADD_CUSTOMER}2\n1\n1\nn\nn\n3\n1\n1\n99\n6\n1\n7\n" \
    "doesn't have copy 99 of video 1" "has 2 of 2 copies"
check "return the same copy twice" \
    "${ADD_VIDEO}${ADD_CUSTOMER}2\n1\n1\nn\nn\n3\n1\n1\n2\n3\n1\n6\n1\n7\n" \
    "has no videos to return." "has 3 of 2"

# Other lookups
check "unknown video availability" "6\n5\n7\n" "Video not found."
check "show videos when empty" "5\n7\n" "No videos in the store yet."
check "customer submenu out of range" "4\n9\n4\n7\n" "Please enter a number from 1 to 4."

# A corrupted ID file should reset to 1 with a warning, not crash or use garbage.
dir="$(mktemp -d)"
echo "garbage" > "$dir/nextVideoID.txt"
out="$(cd "$dir" && printf '1\nA\nB\nC\n1\nimg\n7\n' | timeout 5 "$BIN" 2>&1)"
rm -rf "$dir"
if grep -qF "is corrupted. Starting IDs from 1." <<<"$out" && grep -qF "Video ID: 1" <<<"$out"; then
    echo "ok    corrupted ID file"
    pass=$((pass + 1))
else
    echo "FAIL  corrupted ID file"
    fail=$((fail + 1))
fi

echo
echo "$pass passed, $fail failed"
[ $fail -eq 0 ]
