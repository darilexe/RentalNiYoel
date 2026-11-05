# 🎬 RentalNiYoel: Video Rental System

A command-line video rental store written in C++. Staff can add movies with multiple copies, register customers, rent and return specific copies, and check what's available. Videos and customers are saved to text files so the data is kept between runs.

Built as a group project (July 2024) by Yoel Dwayne Reyes, Jimiel Balitayo, Justine Gabriel P. Rodriguez and Darryl Baranda.

## Features

- **Video inventory:** add a movie with its title, genre, production company, number of copies and a cover image filename. Each video gets an auto-generated ID.
- **Copy tracking:** every physical copy has its own copy ID, so the system knows exactly which copy each customer has.
- **Customers:** register customers with a name and address, view their details, and list the videos they're renting.
- **Renting and returning:** rent an available copy to a customer, or return it to the shelf.
- **Availability check:** see how many copies of a video are left.
- **File storage:** each video and customer is written to its own file (`video_<id>.txt`, `customer_<id>.txt`), and the next video ID is stored in `nextVideoID.txt` so IDs keep counting up across runs.

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
Enter Video ID to Rent: 1
Video 'Spirited Away' copy 2 rented to Juan Dela Cruz
```

## Files

| File | What it is |
|---|---|
| `main.cpp` | The menu and main loop |
| `implementation.cpp` | The `VideoStore`, `Video` and `Customer` code |
| `data.h` | Struct and class declarations |
| `Rental v2.cpp` | Earlier single-file version with file handling |
| `a.cpp` | First single-file version, before file handling was added |
