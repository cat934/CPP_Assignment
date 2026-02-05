#include "Assignment.h"
#include "funcHeader.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

namespace arr {
    void delPassenger(SearchAlgorithm algo) {
        Timer timer;
        int targetID;
        cout << "Enter Passenger ID to delete: ";
        cin >> targetID;

        int index = -1;
        double timeTaken = 0.0;

        SpaceMeter space; // Track memory usage
        space.add(sizeof(int)); // targetID
        space.add(sizeof(int)); // index
        space.add(sizeof(double)); // timeTaken

        // linear
        if (algo == LINEAR_SEARCH) {
            Timer timer;
            for (int i = 0; i < passengerCount; i++) {
                    space.add(sizeof(int)); // loop  i
                    Passenger* p = allPassengers[i];
                    space.add(sizeof(Passenger*));

                if (allPassengers[i]->passengerID == targetID) {
                    index = i;
                    break;
                }
            }
            timeTaken = timer.stopMs();
        }

        //Binary
        else if (algo == BINARY_SEARCH) {
            Timer timer;

            // Must sort Passenger* array first
            mergeSortByID(allPassengers, 0, passengerCount - 1);

            space.add(sizeof(int) * 3); // left, right, mid
            space.add(sizeof(Passenger*));

            index = binarySearchID(allPassengers, passengerCount, targetID);
            timeTaken = timer.stopMs();
        }

        if (index == -1) {
            cout << "Passenger not found.\n";
            cout << fixed << setprecision(3);
            cout << "Time Taken: " << timeTaken << " ms\n\n";
            return;
        }

        Passenger* p = allPassengers[index];

        delete p;

        for (int i = index; i < passengerCount - 1; i++)
            allPassengers[i] = allPassengers[i + 1];

        passengerCount--;
        savePassenger();

        cout << "Passenger deleted successfully.\n";
        cout << fixed << setprecision(3);
        cout << "Time Taken ("
                << (algo == LINEAR_SEARCH ? "Linear Search" : "Binary Search")
                << "): " << timeTaken << " ms\n";
        cout << "Peak Space Used: " << space.peak() << " bytes\n\n";
    }

}
