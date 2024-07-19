#include "data.h"

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
                } while (retry == 'y' || retry == 'Y');
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
                break;

            default:
                cout << "Invalid choice. Try again." << endl;
                break;
        }
    } while (choice != 7);

    return 0;
}

