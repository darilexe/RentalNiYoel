#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <stack>
#include <iomanip>
#include <fstream>

using namespace std;

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

    void saveNextVideoID();
    void loadNextVideoID();

public:
    VideoStore();
    ~VideoStore();

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

