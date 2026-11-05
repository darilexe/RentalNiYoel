# 🎬 RentalNiYoel: Video Rental System

A command-line video rental store written in C++. Staff can add movies with multiple copies, register customers, rent and return specific copies, and check what's available. Every video and customer is also written to its own text file, and ID counters are saved so IDs keep counting up between runs.

Built as a group project (July 2024) by Yoel Dwayne Reyes, Jimiel Balitayo, Justine Gabriel P. Rodriguez and Darryl Baranda.

## Features

- **Video inventory:** add a movie with its title, genre, production company, number of copies and a cover image filename. Each video gets an auto-generated ID.
- **Copy tracking:** every physical copy has its own copy ID, so the system knows exactly which copy each customer has.
- **Customers:** register customers with a name and address, view their details, and list the videos they're renting.
- **Renting and returning:** rent an available copy to a customer, or return it to the shelf.
- **Availability check:** see how many copies of a video are left.
- **File records:** each video and customer is written to its own file (`video_<id>.txt`, `customer_<id>.txt`). The next IDs are stored in `nextVideoID.txt` and `nextCustomerID.txt`, so IDs never repeat across runs.
- **Error handling:** every input is validated, and bad input gets a clear message and a second chance instead of a crash.

## Error handling

The first version had no input validation, so a single typo could break it. That's now fixed:

| Problem in the original | What happens now |
|---|---|
| Typing letters where a number was expected made the menu loop forever | Every number is read as a whole line and checked; the user is asked again |
| Numbers out of range (menu choice 9, `-5` copies, `0` copies) were accepted | Each prompt has a valid range, e.g. copies must be 1 to 100 |
| Returning a copy the customer never rented **added an extra copy** to the store | Only copies the customer actually has can be returned |
| Returning the same copy twice counted it twice | Once returned, a copy is removed from the customer's rentals |
| Looking up an unknown customer silently added an empty entry to the customer map | Lookups use `find()` and report "Customer not found" |
| Empty names and titles were accepted | Required text fields must not be blank |
| Ending the input (Ctrl+D) made the program loop forever | The program exits cleanly |
| File write failures and corrupted ID files were ignored | The user gets a warning, and a corrupted counter resets to 1 |
| Customer IDs restarted at 1 on every run and overwrote old customer files | The customer ID counter is saved like the video one |

## Data structures

| Structure | Used for | Why |
|---|---|---|
| `unordered_map<int, Video*>` | All videos, by video ID | Constant-time lookup when renting or returning |
| `unordered_map<int, Customer*>` | All customers, by customer ID | Constant-time customer lookup |
| `stack<int>` | A video's available copy IDs | Renting pops a copy off the shelf and returning pushes it back, so it always knows which copies are free |
| `list<pair<int, int>>` | A customer's rentals, as (video ID, copy ID) | Rentals can be removed from anywhere in the list when a copy comes back |

## Build and run

Requires a C++17 compiler (g++ or clang++).

```bash
g++ -std=c++17 -o rental main.cpp implementation.cpp
./rental
```

## Tests

`tests/run_tests.sh` feeds scripted input to the program and checks its output: 24 cases covering normal renting and returning plus every kind of bad input above. Each test must finish within 5 seconds, so an infinite loop counts as a failure.

```bash
tests/run_tests.sh
```

```
ok    rent a video
ok    return a rented video
ok    letters at the main menu
...
ok    return a copy the customer doesn't have
ok    return the same copy twice
ok    corrupted ID file

24 passed, 0 failed
```

## Example session

```
Video Rental System Menu:
1. Insert a new video
2. Rent a video
3. Return a video
4. Customer Maintenance
5. Show all videos
6. Check video availability
7. Exit
Enter your choice: 1
Enter Movie Title: Spirited Away
Enter Genre: Animation
Enter Production: Studio Ghibli
Enter Number of Copies: 2
Enter Movie Image Filename: spirited.jpg

Video ID Generated! Video ID: 1
...
Enter your choice: 2
Enter Customer ID: 1
Available Movies and Copies:
Video ID: 1, Title: Spirited Away, Available Copies: 2
Enter Video ID to Rent (0 to cancel): 1
Do you want to see the movie's information? (y/n): n
Video 'Spirited Away' copy 2 rented to Juan Dela Cruz
```

## Files

| File | What it is |
|---|---|
| `main.cpp` | The menu and main loop |
| `implementation.cpp` | The `VideoStore`, `Video` and `Customer` code |
| `data.h` | Struct and class declarations |
| `tests/run_tests.sh` | Automated tests |
| `Rental v2.cpp` | Earlier single-file version, kept for history (no error handling) |
| `a.cpp` | First single-file version, kept for history |
