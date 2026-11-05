#include "data.h"

#include <algorithm>
#include <climits>
#include <vector>

// ---------- Input helpers ----------

static string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Read one whole line. Reading lines (instead of `cin >> x`) means bad input
// can never leave the stream stuck in a failed state.
static string readLine(const string& prompt) {
    cout << prompt;
    string line;
    if (!getline(cin, line)) {
        throw InputClosed();
    }
    return trim(line);
}

int readInt(const string& prompt, int minValue, int maxValue) {
    while (true) {
        string line = readLine(prompt);
        if (line.empty()) {
            cout << "Please enter a number." << endl;
            continue;
        }
        try {
            size_t used = 0;
            long long value = stoll(line, &used);
            if (used != line.size()) {
                throw invalid_argument("trailing characters");
            }
            if (value < minValue || value > maxValue) {
                cout << "Please enter a number from " << minValue << " to " << maxValue << "." << endl;
                continue;
            }
            return static_cast<int>(value);
        } catch (const invalid_argument&) {
            cout << "'" << line << "' is not a whole number. Please try again." << endl;
        } catch (const out_of_range&) {
            cout << "That number is too large. Please enter a number from " << minValue << " to " << maxValue
                 << "." << endl;
        }
    }
}

string readText(const string& prompt, size_t maxLength) {
    while (true) {
        string line = readLine(prompt);
        if (line.empty()) {
            cout << "This field can't be empty. Please try again." << endl;
        } else if (line.size() > maxLength) {
            cout << "Please keep it under " << maxLength << " characters." << endl;
        } else {
            return line;
        }
    }
}

bool readYesNo(const string& prompt) {
    while (true) {
        string line = readLine(prompt);
        if (line == "y" || line == "Y" || line == "yes" || line == "Yes" || line == "YES") {
            return true;
        }
        if (line == "n" || line == "N" || line == "no" || line == "No" || line == "NO") {
            return false;
        }
        cout << "Please answer y or n." << endl;
    }
}

// ---------- Video and Customer ----------

Video::Video(int id, const string& title, const string& genre, const string& production,
             int copies, const string& image_filename)
    : Video_ID(id), Movie_Title(title), Genre(genre), Production(production),
      Total_Copies(copies), Movie_Image_Filename(image_filename) {
    for (int i = 1; i <= copies; ++i) {
        Available_Copies.push(i);
    }
}

int Video::availableCopies() const {
    return static_cast<int>(Available_Copies.size());
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

// ---------- VideoStore ----------

VideoStore::VideoStore()
    : nextVideoID(loadCounter("nextVideoID.txt")), nextCustomerID(loadCounter("nextCustomerID.txt")) {}

VideoStore::~VideoStore() {
    for (auto& entry : videos) {
        delete entry.second;
    }
    for (auto& entry : customers) {
        delete entry.second;
    }
}

void VideoStore::saveCounter(const string& filename, int value) const {
    ofstream file(filename);
    if (!file) {
        cout << "Warning: could not save " << filename << ". IDs may repeat next time." << endl;
        return;
    }
    file << value;
}

// Load the next ID from a file. A missing or corrupted file starts again from 1.
int VideoStore::loadCounter(const string& filename) const {
    ifstream file(filename);
    if (!file) {
        return 1;
    }
    long long value = 0;
    if (!(file >> value) || value < 1 || value >= INT_MAX) {
        cout << "Warning: " << filename << " is corrupted. Starting IDs from 1." << endl;
        return 1;
    }
    return static_cast<int>(value);
}

// Use find() rather than operator[]: operator[] would silently insert a null
// entry for every unknown ID that's looked up.
Video* VideoStore::findVideo(int videoID) const {
    auto it = videos.find(videoID);
    return it == videos.end() ? nullptr : it->second;
}

Customer* VideoStore::findCustomer(int customerID) const {
    auto it = customers.find(customerID);
    return it == customers.end() ? nullptr : it->second;
}

void VideoStore::insertVideo(const string& title, const string& genre, const string& production,
                             int copies, const string& image_filename) {
    if (copies < 1 || copies > MAX_COPIES) {
        cout << "Cannot add video: number of copies must be from 1 to " << MAX_COPIES << "." << endl;
        return;
    }
    if (nextVideoID == INT_MAX) {
        cout << "Cannot add video: no more video IDs available." << endl;
        return;
    }

    int videoID = nextVideoID;
    videos[videoID] = new Video(videoID, title, genre, production, copies, image_filename);

    ofstream fp("video_" + to_string(videoID) + ".txt");
    if (fp) {
        fp << "Video ID: " << videoID << endl;
        fp << "Title: " << title << endl;
        fp << "Genre: " << genre << endl;
        fp << "Production: " << production << endl;
        fp << "Copies: " << copies << endl;
        fp << "Image Filename: " << image_filename << endl;
    }
    if (!fp) {
        cout << "Warning: could not save the video record to a file." << endl;
    }

    cout << "\nVideo ID Generated! Video ID: " << videoID << endl;

    nextVideoID++;
    saveCounter("nextVideoID.txt", nextVideoID);
}

void VideoStore::showAllVideos() const {
    if (videos.empty()) {
        cout << "No videos in the store yet." << endl;
        return;
    }

    // Show videos in ID order (unordered_map has no fixed order).
    vector<int> ids;
    for (const auto& entry : videos) {
        ids.push_back(entry.first);
    }
    sort(ids.begin(), ids.end());

    cout << "All Videos in the Store:" << endl;
    for (int id : ids) {
        videos.at(id)->printInfo();
        cout << endl;
    }
}

void VideoStore::checkVideoAvailability(int videoID) const {
    const Video* video = findVideo(videoID);
    if (!video) {
        cout << "Video not found." << endl;
        return;
    }
    cout << "Video '" << video->Movie_Title << "' has " << video->availableCopies() << " of "
         << video->Total_Copies << " copies available." << endl;
}

void VideoStore::rentVideo(int customerID) {
    Customer* customer = findCustomer(customerID);
    if (!customer) {
        cout << "Customer not found." << endl;
        return;
    }
    if (videos.empty()) {
        cout << "There are no videos in the store yet." << endl;
        return;
    }

    vector<int> ids;
    for (const auto& entry : videos) {
        ids.push_back(entry.first);
    }
    sort(ids.begin(), ids.end());

    cout << "Available Movies and Copies:" << endl;
    for (int id : ids) {
        const Video* video = videos.at(id);
        cout << "Video ID: " << video->Video_ID << ", Title: " << video->Movie_Title
             << ", Available Copies: " << video->availableCopies() << endl;
    }

    Video* video = nullptr;
    while (!video) {
        int videoID = readInt("Enter Video ID to Rent (0 to cancel): ", 0, INT_MAX);
        if (videoID == 0) {
            cout << "Rental cancelled." << endl;
            return;
        }
        video = findVideo(videoID);
        if (!video) {
            cout << "No video with ID " << videoID << ". Please choose one from the list." << endl;
        }
    }

    if (video->Available_Copies.empty()) {
        cout << "Sorry, all copies of '" << video->Movie_Title << "' are currently rented out." << endl;
        return;
    }

    if (readYesNo("Do you want to see the movie's information? (y/n): ")) {
        video->printInfo();
        if (!readYesNo("Do you still want to rent this movie? (y/n): ")) {
            cout << "Rental cancelled." << endl;
            return;
        }
    }

    int copyID = video->Available_Copies.top();
    video->Available_Copies.pop();
    customer->Rented_Videos.emplace_back(video->Video_ID, copyID);
    cout << "Video '" << video->Movie_Title << "' copy " << copyID << " rented to " << customer->Name << endl;
}

void VideoStore::returnVideo(int customerID) {
    Customer* customer = findCustomer(customerID);
    if (!customer) {
        cout << "Customer not found." << endl;
        return;
    }
    if (customer->Rented_Videos.empty()) {
        cout << customer->Name << " has no videos to return." << endl;
        return;
    }

    cout << "Videos rented by " << customer->Name << ":" << endl;
    for (const auto& rented : customer->Rented_Videos) {
        const Video* video = findVideo(rented.first);
        cout << " - " << (video ? video->Movie_Title : "(unknown video)") << " (Video ID: " << rented.first
             << ", Copy ID: " << rented.second << ")" << endl;
    }

    int videoID = readInt("Enter Video ID to Return: ", 1, INT_MAX);
    int copyID = readInt("Enter Copy ID to Return: ", 1, INT_MAX);

    // Only accept a copy this customer actually has. Without this check, returning
    // a made-up copy ID would add an extra copy that never existed.
    auto it = find(customer->Rented_Videos.begin(), customer->Rented_Videos.end(), make_pair(videoID, copyID));
    if (it == customer->Rented_Videos.end()) {
        cout << "Cannot return: " << customer->Name << " doesn't have copy " << copyID << " of video "
             << videoID << "." << endl;
        return;
    }

    Video* video = findVideo(videoID);
    customer->Rented_Videos.erase(it);
    if (video) {
        video->Available_Copies.push(copyID);
        cout << "Video '" << video->Movie_Title << "' copy " << copyID << " returned by " << customer->Name << endl;
    }
}

void VideoStore::addCustomer(const string& name, const string& address) {
    if (nextCustomerID == INT_MAX) {
        cout << "Cannot add customer: no more customer IDs available." << endl;
        return;
    }

    int customerID = nextCustomerID++;
    customers[customerID] = new Customer(customerID, name, address);
    saveCounter("nextCustomerID.txt", nextCustomerID);
    cout << "\nCustomer ID Generated! Customer ID: " << customerID << endl;

    ofstream fp("customer_" + to_string(customerID) + ".txt");
    if (fp) {
        fp << "Customer ID: " << customerID << endl;
        fp << "Name: " << name << endl;
        fp << "Address: " << address << endl;
    }
    if (!fp) {
        cout << "Warning: could not save the customer record to a file." << endl;
    }
}

void VideoStore::showCustomerDetails(int customerID) const {
    const Customer* customer = findCustomer(customerID);
    if (!customer) {
        cout << "Customer not found." << endl;
        return;
    }
    cout << "Customer ID: " << customer->Customer_ID << endl
         << "Name: " << customer->Name << endl
         << "Address: " << customer->Address << endl;
}

void VideoStore::showRentedVideos(int customerID) const {
    const Customer* customer = findCustomer(customerID);
    if (!customer) {
        cout << "Customer not found." << endl;
        return;
    }
    if (customer->Rented_Videos.empty()) {
        cout << customer->Name << " isn't renting any videos." << endl;
        return;
    }
    cout << "Videos rented by " << customer->Name << ":" << endl;
    for (const auto& rented : customer->Rented_Videos) {
        const Video* video = findVideo(rented.first);
        cout << " - " << (video ? video->Movie_Title : "(unknown video)") << " (Video ID: " << rented.first
             << ", Copy ID: " << rented.second << ")" << endl;
    }
}
