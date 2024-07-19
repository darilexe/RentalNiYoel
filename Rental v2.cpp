#include <iostream>
#include <string>
#include <list>
#include <unordered_map>
#include <stack>
#include <iomanip>
#include <fstream>

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

    void saveNextVideoID() {
        ofstream file("nextVideoID.txt");
        if (file.is_open()) {
            file << nextVideoID;
            file.close();
        }
    }

    void loadNextVideoID() {
        ifstream file("nextVideoID.txt");
        if (file.is_open()) {
            file >> nextVideoID;
            file.close();
        } else {
            nextVideoID = 1; // Default value if the file doesn't exist
        }
    }

public:
	
    VideoStore() : nextCustomerID(1) {
        loadNextVideoID();
    }
    
    ~VideoStore() {
        // Clean up dynamically allocated memory
        for (auto& pair : videos) {
            delete pair.second;
        }
        videos.clear();

        for (auto& pair : customers) {
            delete pair.second;
        }
        customers.clear();
    }

    // Insert a new video
    void insertVideo(const string& title, const string& genre, const string& production,
                     int copies, const string& image_filename) {
        int videoID = nextVideoID;
        Video* newVideo = new Video(videoID, title, genre, production, copies, image_filename);
        videos[newVideo->Video_ID] = newVideo;

        // Save to file
        ofstream fp("video_" + to_string(videoID) + ".txt");
        fp << "Video ID: " << videoID << endl;
        fp << "Title: " << title << endl;
        fp << "Genre: " << genre << endl;
        fp << "Production: " << production << endl;
        fp << "Copies: " << copies << endl;
        fp << "Image Filename: " << image_filename << endl;
	    
	cout <<"\nVideo ID Genrated! Video ID: " << newVideo->Video_ID << endl;
	    
        fp.close();

        nextVideoID++;
        saveNextVideoID();
    }

    // Show all videos
    void showAllVideos() const {
        cout << "All Videos in the Store:" << endl;
        for (const auto& pair : videos) {
            pair.second->printInfo();
            cout << endl;
        }
    }

    // Check video availability
    void checkVideoAvailability(int videoID) const {
        if (videos.find(videoID) != videos.end()) {
            const Video* video = videos.at(videoID);
            cout << "Video '" << video->Movie_Title << "' has " << video->availableCopies() << " copies available." << endl;
        } else {
            cout << "Video not found." << endl;
        }
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
                char rentDecision;
                cout << "Do you still want to rent this movie? (y/n): ";
                cin >> rentDecision;
                if (rentDecision != 'y' && rentDecision != 'Y') {
                    return;
                }
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
        cout << "\nCustomer ID Genrated! Customer ID: " << newCustomer->Customer_ID << endl;  
    }

    // Show customer details
    void showCustomerDetails(int customerID) const {
        if (customers.find(customerID) != customers.end()) {
            const Customer* customer = customers.at(customerID);
            cout << "Customer ID: " << customer->Customer_ID << endl
                 << "Name: " << customer->Name << endl
                 << "Address: " << customer->Address << endl;
        } else {
            cout << "Customer not found." << endl;
        }
    }

    // Print list of all videos rented by a customer
    void showRentedVideos(int customerID) const {
        if (customers.find(customerID) != customers.end()) {
            const Customer* customer = customers.at(customerID);
            cout << "Videos rented by " << customer->Name << ":" << endl;
            for (const auto& rented : customer->Rented_Videos) {
                int videoID = rented.first;
                int copyID = rented.second;
                Video* video = videos.at(videoID);
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
        cout << "4. Customer Maintenance" << endl;
        cout << "5. Show all videos" << endl;
        cout << "6. Check video availability" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string title, genre, production, image_filename;
                int copies;

                cout << "Enter Movie Title: ";
                cin.ignore(); // Clear the newline character from the buffer
                getline(cin, title);

                cout << "Enter Genre: ";
                getline(cin, genre);

                cout << "Enter Production: ";
                getline(cin, production);

                cout << "Enter Number of Copies: ";
                cin >> copies;

                cout << "Enter Movie Image Filename: ";
                cin.ignore(); // Clear the newline character from the buffer
                getline(cin, image_filename);

                store.insertVideo(title, genre, production, copies, image_filename);
                
                break;
            }

            case 2: {
                int customerID;
                char retry;
                do {
                  enterID:
		  cout << "Enter Customer ID: ";
        	  cin >> customerID;
            	  store.rentVideo(customerID);
                
                  cout << "Do you want to try again? (Y/N)" << endl;
                  cin >> retry;
        
                 if (retry == 'y' || retry == 'Y') {
                     goto enterID;
                   }
                } while (choice == 'y' || choice == 'Y');
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
                int customerChoice;
                cout << "Customer Maintenance Menu:" << endl;
                cout << "1. Add new customer" << endl;
                cout << "2. Show customer details" << endl;
                cout << "3. Show videos rented by customer" << endl;
                cout << "Enter your choice: ";
                cin >> customerChoice;

                switch (customerChoice) {
                    case 1: {
                        string name, address;
                        cout << "Enter Customer Name: ";
                        cin.ignore();
                        getline(cin, name);
                        cout << "Enter Customer Address: ";
                        getline(cin, address);
                        store.addCustomer(name, address);
                        break;
                    }

                    case 2: {
                        int customerID;
                        cout << "Enter Customer ID: ";
                        cin >> customerID;
                        store.showCustomerDetails(customerID);
                        break;
                    }

                    case 3: {
                        int customerID;
                        cout << "Enter Customer ID: ";
                        cin >> customerID;
                        store.showRentedVideos(customerID);
                        break;
                    }

                    default:
                        cout << "Invalid choice." << endl;
                        break;
                }
                break;
            }

            case 5:
                store.showAllVideos();
                break;

            case 6: {
                int videoID;
                cout << "Enter Video ID: ";
                cin >> videoID;
                store.checkVideoAvailability(videoID);
                break;
            }

            case 7:
                cout << "Exiting system." << endl;
                // Save to File.
                break;

            default:
                cout << "Invalid choice. Try again." << endl;
                break;
        }
    } while (choice != 7);

    return 0;
}

