#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <stack>
#include <iomanip>
#include <fstream>
#include <stdexcept>

using namespace std;

// Thrown when the input stream ends (e.g. Ctrl+D), so the program can exit cleanly
// instead of looping forever on a closed stream.
struct InputClosed : runtime_error {
    InputClosed() : runtime_error("input closed") {}
};

// Safe input helpers: they re-prompt until the user enters something valid.
int readInt(const string& prompt, int minValue, int maxValue);
string readText(const string& prompt, size_t maxLength = 100);
bool readYesNo(const string& prompt);

struct Video {
    int Video_ID;
    string Movie_Title;
    string Genre;
    string Production;
    int Total_Copies;
    stack<int> Available_Copies;
    string Movie_Image_Filename;

    Video(int id, const string& title, const string& genre, const string& production,
          int copies, const string& image_filename);

    int availableCopies() const;
    void printInfo() const;
};

struct Customer {
    int Customer_ID;
    string Name;
    string Address;
    list<pair<int, int>> Rented_Videos;

    Customer(int id, const string& name, const string& address);
};

class VideoStore {
private:
    unordered_map<int, Video*> videos;
    unordered_map<int, Customer*> customers;
    int nextVideoID;
    int nextCustomerID;

    void saveCounter(const string& filename, int value) const;
    int loadCounter(const string& filename) const;

    Video* findVideo(int videoID) const;
    Customer* findCustomer(int customerID) const;

public:
    static const int MAX_COPIES = 100;

    VideoStore();
    ~VideoStore();

    // No copying: the store owns its Video and Customer pointers.
    VideoStore(const VideoStore&) = delete;
    VideoStore& operator=(const VideoStore&) = delete;

    void insertVideo(const string& title, const string& genre, const string& production,
                     int copies, const string& image_filename);
    void showAllVideos() const;
    void checkVideoAvailability(int videoID) const;
    void rentVideo(int customerID);
    void returnVideo(int customerID);
    void addCustomer(const string& name, const string& address);
    void showCustomerDetails(int customerID) const;
    void showRentedVideos(int customerID) const;
};

#endif
