#include "data.h"

#include <climits>

static void customerMenu(VideoStore& store) {
    cout << "Customer Maintenance Menu:" << endl;
    cout << "1. Add new customer" << endl;
    cout << "2. Show customer details" << endl;
    cout << "3. Show videos rented by customer" << endl;
    cout << "4. Back to main menu" << endl;

    switch (readInt("Enter your choice: ", 1, 4)) {
        case 1: {
            string name = readText("Enter Customer Name: ");
            string address = readText("Enter Customer Address: ");
            store.addCustomer(name, address);
            break;
        }
        case 2:
            store.showCustomerDetails(readInt("Enter Customer ID: ", 1, INT_MAX));
            break;
        case 3:
            store.showRentedVideos(readInt("Enter Customer ID: ", 1, INT_MAX));
            break;
        case 4:
            break;
    }
}

int main() {
    VideoStore store;
    int choice = 0;

    try {
        do {
            cout << "\nVideo Rental System Menu:" << endl;
            cout << "1. Insert a new video" << endl;
            cout << "2. Rent a video" << endl;
            cout << "3. Return a video" << endl;
            cout << "4. Customer Maintenance" << endl;
            cout << "5. Show all videos" << endl;
            cout << "6. Check video availability" << endl;
            cout << "7. Exit" << endl;
            choice = readInt("Enter your choice: ", 1, 7);

            switch (choice) {
                case 1: {
                    string title = readText("Enter Movie Title: ");
                    string genre = readText("Enter Genre: ");
                    string production = readText("Enter Production: ");
                    int copies = readInt("Enter Number of Copies: ", 1, VideoStore::MAX_COPIES);
                    string image_filename = readText("Enter Movie Image Filename: ");
                    store.insertVideo(title, genre, production, copies, image_filename);
                    break;
                }

                case 2: {
                    bool again;
                    do {
                        store.rentVideo(readInt("Enter Customer ID: ", 1, INT_MAX));
                        again = readYesNo("Do you want to rent another video? (y/n): ");
                    } while (again);
                    break;
                }

                case 3:
                    store.returnVideo(readInt("Enter Customer ID: ", 1, INT_MAX));
                    break;

                case 4:
                    customerMenu(store);
                    break;

                case 5:
                    store.showAllVideos();
                    break;

                case 6:
                    store.checkVideoAvailability(readInt("Enter Video ID: ", 1, INT_MAX));
                    break;

                case 7:
                    cout << "Exiting system." << endl;
                    break;
            }
        } while (choice != 7);
    } catch (const InputClosed&) {
        cout << "\nInput ended. Exiting system." << endl;
    } catch (const exception& e) {
        cerr << "\nUnexpected error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
