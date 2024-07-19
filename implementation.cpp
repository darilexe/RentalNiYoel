#include "data.h"

Video::Video(int id, const string& title, const string& genre, const string& production,
             int copies, const string& image_filename)
    : Video_ID(id), Movie_Title(title), Genre(genre), Production(production),
      Total_Copies(copies), Movie_Image_Filename(image_filename) {
    for (int i = 1; i <= copies; ++i) {
        Available_Copies.push(i);
    }
}

int Video::availableCopies() const {
    return Available_Copies.size();
}

void Video::printInfo() const {
    cout << "Video ID: " << Video_ID << endl
         << "Title: " << Movie_Title << endl
         << "Genre: " << Genre << endl
         << "Production: " << Production << endl
         << "Total Copies: " << Total_Copies << endl
         << "Available Copies: " << availableCopies() << endl
         << "Movie Image Filename: " << Movie_Image_Filename << endl;
}

Customer::Customer(int id, const string& name, const string& address)
    : Customer_ID(id), Name(name), Address(address) {}

VideoStore::VideoStore() : nextCustomerID(1) {
    loadNextVideoID();
}

VideoStore::~VideoStore() {
    for (auto& pair : videos) {
        delete pair.second;
    }
    videos.clear();

    for (auto& pair : customers) {
        delete pair.second;
    }
    customers.clear();
}

void VideoStore::saveNextVideoID() {
    ofstream file("nextVideoID.txt");
    if (file.is_open()) {
        file << nextVideoID;
        file.close();
    }
}

void VideoStore::loadNextVideoID() {
    ifstream file("nextVideoID.txt");
    if (file.is_open()) {
        file >> nextVideoID;
        file.close();
    } else {
        nextVideoID = 1;
    }
}

void VideoStore::insertVideo(const string& title, const string& genre, const string& production,
                             int copies, const string& image_filename) {
    int videoID = nextVideoID;
    Video* newVideo = new Video(videoID, title, genre, production, copies, image_filename);
    videos[newVideo->Video_ID] = newVideo;

    ofstream fp("video_" + to_string(videoID) + ".txt");
    fp << "Video ID: " << videoID << endl;
    fp << "Title: " << title << endl;
    fp << "Genre: " << genre << endl;
    fp << "Production: " << production << endl;
    fp << "Copies: " << copies << endl;
    fp << "Image Filename: " << image_filename << endl;

    cout << "\nVideo ID Generated! Video ID: " << newVideo->Video_ID << endl;

    fp.close();

    nextVideoID++;
    saveNextVideoID();
}

void VideoStore::showAllVideos() const {
    cout << "All Videos in the Store:" << endl;
    for (const auto& pair : videos) {
        pair.second->printInfo();
        cout << endl;
    }
}

void VideoStore::checkVideoAvailability(int videoID) const {
    if (videos.find(videoID) != videos.end()) {
        const Video* video = videos.at(videoID);
        cout << "Video '" << video->Movie_Title << "' has " << video->availableCopies() << " copies available." << endl;
    } else {
        cout << "Video not found." << endl;
    }
}

void VideoStore::rentVideo(int customerID) {
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

void VideoStore::returnVideo(int customerID) {
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

void VideoStore::addCustomer(const string& name, const string& address) {
    Customer* newCustomer = new Customer(nextCustomerID++, name, address);
    customers[newCustomer->Customer_ID] = newCustomer;
    cout << "\nCustomer ID Generated! Customer ID: " << newCustomer->Customer_ID << endl;
    int customerID = newCustomer->Customer_ID;

    ofstream fp("customer_" + to_string(customerID) + ".txt");
    fp << "Customer ID: " << customerID << endl;
    fp << "Name: " << name << endl;
    fp << "Address: " << address << endl;
    fp.close();
}

void VideoStore::showCustomerDetails(int customerID) const {
    if (customers.find(customerID) != customers.end()) {
        const Customer* customer = customers.at(customerID);
        cout << "Customer ID: " << customer->Customer_ID << endl
             << "Name: " << customer->Name << endl
             << "Address: " << customer->Address << endl;
    } else {
        cout << "Customer not found." << endl;
    }
}

void VideoStore::showRentedVideos(int customerID) const {
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

