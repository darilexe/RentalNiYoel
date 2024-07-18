#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <stack>
#include <iomanip>

using namespace std;

// Define Video structure
struct Video {
    int Video_ID;
    string Movie_Title;
    string Genre;
    string Production;
    int Total_Copies;
    stack<int> Available_Copies; // Stack to manage available copies
    string Movie_Image_Filename;

    Video(int id, const string& title, const string& genre, const string& production,
          int copies, const string& image_filename)
        : Video_ID(id), Movie_Title(title), Genre(genre), Production(production),
          Total_Copies(copies), Movie_Image_Filename(image_filename) {
        for (int i = 1; i <= copies; ++i) {
            Available_Copies.push(i);
        }
    }

    int availableCopies() const {
        return Available_Copies.size();
    }

    void printInfo() const {
        cout << "Video ID: " << Video_ID << endl
             << "Title: " << Movie_Title << endl
             << "Genre: " << Genre << endl
             << "Production: " << Production << endl
             << "Total Copies: " << Total_Copies << endl
             << "Available Copies: " << availableCopies() << endl
             << "Movie Image Filename: " << Movie_Image_Filename << endl;
    }
};

// Define Customer structure
struct Customer {
    int Customer_ID;
    string Name;
    string Address;
    list<pair<int, int>> Rented_Videos; // List of pairs (Video_ID, Copy_ID)

    Customer(int id, const string& name, const string& address)
        : Customer_ID(id), Name(name), Address(address) {}
};

// Video Store class
class VideoStore {
private:
    unordered_map<int, Video*> videos; // Map to store videos by Video_ID
    unordered_map<int, Customer*> customers; // Map to store customers by Customer_ID
    int nextVideoID; // Track the next available Video ID
    int nextCustomerID; // Track the next available Customer ID

public:
    VideoStore() : nextVideoID(1), nextCustomerID(1) {}

    // Insert a new video
    void insertVideo(const string& title, const string& genre, const string& production,
                     int copies, const string& image_filename) {
        Video* newVideo = new Video(nextVideoID++, title, genre, production, copies, image_filename);
        videos[newVideo->Video_ID] = newVideo;
    }

    // Rent a video
    void rentVideo(int customerID) {
        Customer* customer = customers[customerID];

        if (customer) {
            cout << "Available Movies and Copies:" << endl;
            for (const auto& pair : videos) {
                Video* video = pair.second;
                cout << "Video ID: " << video->Video_ID 
                     << ", Title: " << video->Movie_Title 
                     << ", Available Copies: " << video->availableCopies() << endl;
            }

            int videoID;
            cout << "Enter Video ID to Rent: ";
            cin >> videoID;

            if (videos.find(videoID) == videos.end()) {
                cout << "Invalid Video ID. Returning to main menu." << endl;
                return;
            }

            char showInfo;
            cout << "Do you want to see the movie's information? (y/n): ";
            cin >> showInfo;

            if (showInfo == 'y' || showInfo == 'Y') {
                videos[videoID]->printInfo();
            }

            Video* video = videos[videoID];

            if (video && !video->Available_Copies.empty()) {
                int copyID = video->Available_Copies.top();
                video->Available_Copies.pop();
                customer->Rented_Videos.emplace_back(videoID, copyID);
                cout << "Video '" << video->Movie_Title << "' copy " << copyID << " rented to " << customer->Name << endl;
            } else {
                cout << "Cannot rent video. No copies available or invalid Video ID." << endl;
            }
        } else {
            cout << "Customer not found." << endl;
        }
    }

    // Return a video
    void returnVideo(int customerID) {
        Customer* customer = customers[customerID];

        if (customer) {
            cout << "Videos rented by " << customer->Name << ":" << endl;
            for (const auto& rented : customer->Rented_Videos) {
                int videoID = rented.first;
                int copyID = rented.second;
                Video* video = videos[videoID];
                if (video) {
                    cout << " - " << video->Movie_Title << " (Video ID: " << video->Video_ID << ", Copy ID: " << copyID << ")" << endl;
                }
            }

            int videoID, copyID;
            cout << "Enter Video ID to Return: ";
            cin >> videoID;

            cout << "Enter Copy ID to Return: ";
            cin >> copyID;

            Video* video = videos[videoID];
            if (video) {
                video->Available_Copies.push(copyID);
                customer->Rented_Videos.remove_if([videoID, copyID](const pair<int, int>& p) {
                    return p.first == videoID && p.second == copyID;
                });
                cout << "Video '" << video->Movie_Title << "' copy " << copyID << " returned by " << customer->Name << endl;
            } else {
                cout << "Cannot return video. Invalid Video ID or Copy ID." << endl;
            }
        } else {
            cout << "Customer not found." << endl;
        }
    }

    // Add a new customer
    void addCustomer(const string& name, const string& address) {
        Customer* newCustomer = new Customer(nextCustomerID++, name, address);
        customers[newCustomer->Customer_ID] = newCustomer;
    }

    // Print list of all videos rented by a customer
    void printRentedVideos(int customerID) {
        Customer* customer = customers[customerID];
        if (customer) {
            cout << "Videos rented by " << customer->Name << ":" << endl;
            for (const auto& rented : customer->Rented_Videos) {
                int videoID = rented.first;
                int copyID = rented.second;
                Video* video = videos[videoID];
                if (video) {
                    cout << " - " << video->Movie_Title << " (Video ID: " << video->Video_ID << ", Copy ID: " << copyID << ")" << endl;
                }
            }
        } else {
            cout << "Customer not found." << endl;
        }
    }
};

int main() {
    VideoStore store;
    int choice;

    do {
        cout << "\nVideo Rental System Menu:" << endl;
        cout << "1. Insert a new video" << endl;
        cout << "2. Rent a video" << endl;
        cout << "3. Return a video" << endl;
        cout << "4. Add a new customer" << endl;
        cout << "5. Print list of videos rented by a customer" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string title, genre, production, image_filename;
                int copies;

                cout << "Enter Movie Title: ";
                cin.ignore();
                getline(cin, title);

                cout << "Enter Genre: ";
                getline(cin, genre);

                cout << "Enter Production: ";
                getline(cin, production);

                cout << "Enter Number of Copies: ";
                cin >> copies;

                cout << "Enter Movie Image Filename: ";
                cin.ignore();
                getline(cin, image_filename);

                store.insertVideo(title, genre, production, copies, image_filename);
                break;
            }
            case 2: {
                int customerID;
                cout << "Enter Customer ID: ";
                cin >> customerID;

                store.rentVideo(customerID);
                break;
            }
            case 3: {
                int customerID;
                cout << "Enter Customer ID: ";
                cin >> customerID;

                store.returnVideo(customerID);
                break;
            }
            case 4: {
                string name, address;
                cout << "Enter Customer Name: ";
                cin.ignore();
                getline(cin, name);

                cout << "Enter Customer Address: ";
                getline(cin, address);

                store.addCustomer(name, address);
                break;
            }
            case 5: {
                int customerID;
                cout << "Enter Customer ID to Print Rented Videos: ";
                cin >> customerID;

                store.printRentedVideos(customerID);
                break;
            }
            case 6:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a number from 1 to 6." << endl;
                break;
        }

    } while (choice != 6);

    return 0;
}

