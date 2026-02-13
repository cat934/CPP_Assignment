#include "Assignment.h"
#include "funcHeader.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

namespace arr {

    double totalIDSearchTime = 0.0;
    size_t totalIDSearchSpace = 0;

    double totalSeatSearchTime = 0.0;
    size_t totalSeatSearchSpace = 0;

    // generate new id
    int generateNewID_Linear() {
        Timer timer;
        SpaceMeter space;
        int maxID = 0;
        space.add(sizeof(int) * 2); // maxID + temp

        for (int i = 0; i < passengerCount; i++)
            maxID = max(maxID, allPassengers[i]->passengerID);

        double us = timer.stopMs() * 1000.0;
        totalIDSearchTime += us;
        totalIDSearchSpace = max(totalIDSearchSpace, space.peak());

        return maxID + 1;
    }

    int generateNewID_Binary() {
        if (passengerCount == 0) return 1;

        // 确保按ID排
        mergeSortByID(allPassengers, 0, passengerCount - 1);

        Timer timer;
        SpaceMeter space;
        int left = 0, right = passengerCount - 1;
        int maxID = -1;
        space.add(sizeof(int) * 3); // left, right, mid

        while (left <= right) {
            int mid = (left + right) / 2;
            maxID = allPassengers[mid]->passengerID;
            left = mid + 1;
        }

        double us = timer.stopMs() * 1000.0;
        totalIDSearchTime += us;
        totalIDSearchSpace = max(totalIDSearchSpace, space.peak());

        return maxID + 1;
    }

    int generateNewID(SearchAlgorithm algo) {
        if (algo == LINEAR_SEARCH)
            return generateNewID_Linear();
        else
            return generateNewID_Binary();
    }

    // check seat
    bool isSeatBooked_Linear(int row, char col, const string& flightID) {
        Timer timer;
        SpaceMeter space;
        space.add(sizeof(Passenger*));

        for (int i = 0; i < passengerCount; i++) {
            Passenger* p = allPassengers[i];
            if (p->seatRow == row && p->seatColumn == col && p->flightID == flightID) {
                double us = timer.stopMs() * 1000.0;
                totalSeatSearchTime += us;
                totalSeatSearchSpace = max(totalSeatSearchSpace, space.peak());
                return true;
            }
        }

        double us = timer.stopMs() * 1000.0;
        totalSeatSearchTime += us;
        totalSeatSearchSpace = max(totalSeatSearchSpace, space.peak());
        return false;
    }

    bool isSeatBooked_Binary(int row, char col, const string& flightID) {
        if (passengerCount == 0)
            return false;

        vector<Passenger*> temp(allPassengers, allPassengers + passengerCount);
        sort(temp.begin(), temp.end(), [](Passenger* a, Passenger* b) {
             if (a->flightID != b->flightID)
                return a->flightID < b->flightID;
             if (a->seatRow != b->seatRow) return a->seatRow < b->seatRow;
             return a->seatColumn < b->seatColumn;
         });

        Timer timer;
        SpaceMeter space;
        int left = 0, right = temp.size() - 1;
        space.add(sizeof(int) * 3 + sizeof(Passenger*));

        while (left <= right) {
            int mid = (left + right) / 2;
            Passenger* p = temp[mid];

            if (p->flightID == flightID && p->seatRow == row && p->seatColumn == col) {
                double us = timer.stopMs() * 1000.0;
                totalSeatSearchTime += us;
                totalSeatSearchSpace = max(totalSeatSearchSpace, space.peak());
                return true;
            }

            if (p->flightID < flightID ||
                (p->flightID == flightID && p->seatRow < row) ||
                (p->flightID == flightID && p->seatRow == row && p->seatColumn < col))
                left = mid + 1;
            else
                right = mid - 1;
        }

        double us = timer.stopMs() * 1000.0;
        totalSeatSearchTime += us;
        totalSeatSearchSpace = max(totalSeatSearchSpace, space.peak());
        return false;
    }

    bool isSeatBooked(int row, char col, const string& flightID, SearchAlgorithm algo) {
        if (algo == LINEAR_SEARCH)
            return isSeatBooked_Linear(row, col, flightID);
        else
            return isSeatBooked_Binary(row, col, flightID);
    }


    void savePassenger() {
        ofstream file("Flite_passenger_Dataset_FIDs.csv");
        if (!file.is_open()) {
            cout << "Error saving file.\n";
            return;
        }

        file << "PassengerID,Name,SeatRow,SeatColumn,Class,FlightID\n";
        for (int i = 0; i < passengerCount; i++) {
            Passenger* p = allPassengers[i];
            file << p->passengerID << ","
                 << p->name << ","
                 << p->seatRow << ","
                 << p->seatColumn << ","
                 << p->seatClass << ","
                 << p->flightID << "\n";
        }
        file.close();
    }

    static void getClassRowRange(const string& seatClass, int& startRow, int& endRow) {
        if (seatClass == "First") {
                startRow = 1;
                endRow = 3;
        }
        else if (seatClass == "Business") {
                startRow = 4;
                endRow = 10;
        }
        else {
                startRow = 11;
                endRow = 30;
        }
    }

    static bool isRowMatchingClass(int row, const string& seatClass) {
        int s, e;
        getClassRowRange(seatClass, s, e);
        return row >= s && row <= e;
    }

    static bool hasAvailableSeatInClass(const string& flightID, const string& seatClass, SearchAlgorithm algo) {
        int s, e;
        getClassRowRange(seatClass, s, e);

        for (int r = s; r <= e; r++) {
            for (char c = 'A'; c <= 'F'; c++) {
                if (!isSeatBooked(r, c, flightID, algo)) {
                    return true;
                }
            }
        }
        return false;
    }


    void addPassenger(SearchAlgorithm algo) {
        while (true) {
            Passenger* p = new Passenger;
            p->passengerID = generateNewID(algo);

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Name (0 to cancel): ";
            getline(cin, p->name);
            if (p->name == "0") {
                    delete p; return;
            }
            while (p->name.empty()) {
                    cout << "Name cannot be empty: "; getline(cin, p->name);
            }

            while (true) {
                cout << "Flight ID (FL001-FL079, 0 to cancel): ";
                cin >> p->flightID;
                if (p->flightID == "0") {
                        delete p;
                        return;
                }
                if (!arr::isValidFlightID_001_079(p->flightID)) {
                        cout << "Invalid Flight ID.\n";
                        continue;
                }

                int choice;
                cout << "1.Economy\n2.Business\n3.First\nChoose class: ";
                cin >> choice;
                if (choice == 0) {
                        delete p;
                        return;
                }
                p->seatClass = (choice == 1) ? "Economy" : (choice == 2) ? "Business" : "First";

                if (!hasAvailableSeatInClass(p->flightID, p->seatClass, algo)) {
                    cout << "No available seats, choose another.\n";
                    continue;
                }
                break;
            }

            while (true) {
                int s, e;
                getClassRowRange(p->seatClass, s, e);
                cout << "Seat Row (" << s << "-" << e << "): ";
                cin >> p->seatRow;
                if (!isRowMatchingClass(p->seatRow, p->seatClass)) {
                        cout << "Row mismatch.\n";
                        continue;
                }

                cout << "Seat Column (A-F): ";
                cin >> p->seatColumn;
                if (isSeatBooked(p->seatRow, p->seatColumn, p->flightID, algo)) {
                    cout << "Seat booked. Choose another.\n";
                    continue;
                }
                break;
            }

            ensurePassengerCapacityForOneMore();
            allPassengers[passengerCount++] = p;
            savePassenger();
            cout << "Passenger added successful. ID = " << p->passengerID << "\n";

            // 打印总数
            cout << fixed << setprecision(3);
            cout << "Total search time for generate new ID: " << totalIDSearchTime << " us \nTotal space: " << totalIDSearchSpace << " bytes\n";
            cout << "Total search time for check seat: " << totalSeatSearchTime << " us \nTotal space: " << totalSeatSearchSpace << " bytes\n";

            break;
        }
    }

}

