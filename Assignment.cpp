#include "Assignment.h"
#include "funcHeader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cctype>
using namespace std;

namespace arr {
    // Single definitions for globals declared in headers
    Passenger** allPassengers = nullptr;
    int passengerCount = 0;
    int passengerCapacity = 0;

    void loadPassengers(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // skip header

    int recordCount = 0;
    while (getline(file, line)) {
        if (!line.empty()) recordCount++;
    }

    arr::initPassengerStore(recordCount);

    file.clear();
    file.seekg(0);
    getline(file, line); // skip header again

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, name, rowStr, colStr, classStr, flightID;

        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, rowStr, ',');
        getline(ss, colStr, ',');
        getline(ss, classStr, ',');
        getline(ss, flightID);

        Passenger* p = new Passenger{
            stoi(idStr),
            name,
            stoi(rowStr),
            colStr[0],
            classStr,
            flightID
        };

        // Ensure capacity then append
        ensurePassengerCapacityForOneMore();
        allPassengers[passengerCount++] = p;
    }

    cout << passengerCount << " passengers loaded successfully.\n";
    }
}

static int readInt() {
    int x;
    while (true) {
        cin >> x;
        if (!cin.fail()) return x;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number: ";
    }
}




int main() {
    string filename = "Flite_passenger_Dataset_FIDs.csv";
    arr::loadPassengers(filename);

    int choice;

        do {

        cout << "\n===== AIRLINE RESERVATION SYSTEM =====\n";
        cout << "1. Array-Based System\n";
        cout << "2. Linked List-Based System\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        choice = readInt();

        if (choice != 0 && choice != 1 && choice != 2) {
            cout << "Invalid option! Please enter 0, 1, or 2.\n";
            continue;
        }

        switch (choice) {

        // Arr
        case 1: {
            int arrChoice;
            do {
                cout << "\n--- ARRAY-BASED MENU ---\n";
                cout << "1. Reservation\n";
                cout << "2. Cancellation\n";
                cout << "3. Passenger Manifest & Reports\n";
                cout << "4. Seat Occupancy\n";
                cout << "0. Back\n";
                cout << "Enter choice: ";
                arrChoice = readInt();

                if (arrChoice < 0 || arrChoice > 4) {
                    cout << "Invalid choice! Please enter 0-4.\n";
                    continue;
                }

                switch (arrChoice) {

                case 1: {
                   int algoChoice;
                    do {
                        cout << "\nChoose search method:\n";
                        cout << "1. Linear Search\n";
                        cout << "2. Binary Search\n";
                        cout << "0. Back\n";
                        cout << "Choice: ";

                        algoChoice = readInt();

                        if (algoChoice < 0 || algoChoice > 2)
                            cout << "Invalid choice. Please enter 0, 1 or 2.\n";

                    } while (algoChoice < 0 || algoChoice > 2);

                    if (algoChoice == 0) break;

                    if (algoChoice == 1) {
                        arr::addPassenger(arr::LINEAR_SEARCH);
                    } else {
                        arr::addPassenger(arr::BINARY_SEARCH);
                    }
                    break;
                }
                case 2: {
                    int algoChoice;
                    do {
                        cout << "\nChoose search method:\n";
                        cout << "1. Linear Search\n";
                        cout << "2. Binary Search\n";
                        cout << "0. Back\n";
                        cout << "Choice: ";

                        algoChoice = readInt();

                        if (algoChoice < 0 || algoChoice > 2)
                            cout << "Invalid choice. Please enter 0, 1 or 2.\n";

                    } while (algoChoice < 0 || algoChoice > 2);

                    if (algoChoice == 0) break;

                    if (algoChoice == 1) {
                        arr::delPassenger(arr::LINEAR_SEARCH);
                    } else {
                        arr::delPassenger(arr::BINARY_SEARCH);
                    }
                    break;
                }
                case 3: {
                    int reportChoice;
                    do {
                        cout << "\n--- MANIFEST & REPORT MENU ---\n";
                        cout << "1. General Report\n";
                        cout << "2. Filter Records\n";
                        cout << "3. Sort Records\n";
                        cout << "0. Back\n";
                        cout << "Enter choice: ";
                        reportChoice = readInt();

                        if (reportChoice < 0 || reportChoice > 3) {
                            cout << "Invalid choice! Please enter 0-3.\n";
                            continue;
                        }

                        switch (reportChoice) {

                        case 1:
                            arr::printGeneralReport();
                            break;

                        case 2: {
                            int filterChoice;
                            do {
                                cout << "\n--- FILTER MENU ---\n";
                                cout << "1. Seat Column\n";
                                cout << "2. Seat Row\n";
                                cout << "3. Seat Index\n";
                                cout << "4. Class\n";
                                cout << "5. Flight ID\n";
                                cout << "6. Passenger Name\n";
                                cout << "7. Passenger ID\n";
                                cout << "0. Back\n";
                                cout << "Enter choice: ";
                                filterChoice = readInt();

                                if (filterChoice < 0 || filterChoice > 7) {
                                    cout << "Invalid choice! Please enter 0-6.\n";
                                    continue;
                                }

                                switch (filterChoice) {
                                    case 1: arr::searchByColumn(); break;
                                    case 2: arr::searchByRow(); break;
                                    case 3: arr::searchBySeatIndex(); break;
                                    case 4: arr::searchByClass(); break;
                                    case 5: arr::searchByFlightID(); break;
                                    case 6: arr::searchByName(); break;
                                    case 7: arr::searchByID(); break;
                                    case 0: break;
                                    default: cout << "Invalid choice!\n";
                                }
                            } while (filterChoice != 0);
                            break;
                        }

                        case 3: {
                            int sortChoice;
                            do {
                                cout << "\n--- SORT MENU ---\n";
                                cout << "1. Seat Index\n";
                                cout << "2. Seat Column\n";
                                cout << "3. Seat Row\n";
                                cout << "4. Class\n";
                                cout << "5. Flight ID\n";
                                cout << "6. Name\n";
                                cout << "0. Back\n";
                                cout << "Enter choice: ";

                                sortChoice = readInt();

                                if (sortChoice == 0) break;
                                if (sortChoice < 1 || sortChoice > 6) {
                                    cout << "Invalid choice! Please enter 0-6.\n";
                                    continue;
                                }

                                arr::printGeneralReportSorted(sortChoice);

                            } while (true);
                            break;
                        }

                        case 0: break;
                        default: cout << "Invalid choice!\n";
                        }

                    } while (reportChoice != 0);
                    break;
                }

                case 4: {
                    string flightID;
                    do {
                        cout << "\nEnter Flight ID (FL001-FL079, 0 to back): ";
                        cin >> flightID;

                        if (flightID == "0") break;

                        if (!arr::isValidFlightID_001_079(flightID)) {
                            cout << "Invalid Flight ID. Please enter FL001 to FL079.\n";
                            continue;
                        }

                        arr::displaySeating(flightID);

                        char view;
                        while (true) {
                            cout << "View passenger manifest? (y/n): ";
                            cin >> view;

                            if (view == 'y' || view == 'Y' || view == 'n' || view == 'N')
                                break;

                            cout << "Invalid input. Please enter 'y' or 'n'.\n";
                        }

                        if (view == 'y' || view == 'Y')
                            arr::buildAndPrintManifestForFlight(flightID);

                    } while (true);
                    break;
                }

                case 0: break;
                default: cout << "Invalid choice!\n";
                }

            } while (arrChoice != 0);
            break;
        }
        //linked list
        case 2: {
            int llChoice;
            do {
                cout << "\n--- LINKED LIST MENU ---\n";
                cout << "1. Add Passenger\n";
                cout << "2. Cancel Reservation\n";
                cout << "3. Search by Row\n";
                cout << "4. Search by Class\n";
                cout << "5. Display Manifest\n";
                cout << "6. Display Seating Chart\n";
                cout << "7. Binary Search by PassengerID\n";
                cout << "0. Back\n";
                cout << "Enter choice: ";

                llChoice = readInt();

                if (llChoice < 0 || llChoice > 7) {
                    cout << "Invalid choice! Please enter 0–7.\n";
                    continue;
                }

                switch (llChoice) {
                case 1:
                    ll::addPassenger();
                    break;
                case 2:
                    ll::cancelReservation();
                    break;
                case 3: {
                    int row;
                    cout << "Enter row to search: ";
                    cin >> row;
                    ll::searchByRow(row);
                    break;
                }
                case 4: {
                    string cls;
                    cout << "Enter class to search: ";
                    cin >> cls;
                    ll::searchByClass(cls);
                    break;
                }
                case 5:
                    ll::displayManifest();
                    break;
                case 6:
                    ll::displaySeatingChart();
                    break;
                case 7: {
                    int id;
                    cout << "Enter Passenger ID to search: ";
                    cin >> id;
                    // Call binary search function here
                    cout << "(Binary search function call)\n";
                    break;
                }
                case 0:
                break;
                default:
                    cout << "Invalid choice!\n";
                }

                } while (llChoice != 0);
            break;
        }
        }  // <-- ADD THIS if it's missing (closes the outer switch)

    } while (choice != 0);

    return 0;
}  // End of main()
