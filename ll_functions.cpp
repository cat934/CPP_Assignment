#include "funcHeader.h"
#include "Assignment.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
using namespace std;

// Allow using array namespace function
using arr::isValidFlightID_001_079;

// External declaration
extern PassengerNode* ll_head;

namespace ll {

    // ==================== 1. LOAD FROM FILE ====================
    void loadFromFile(const string& filename) {
        Timer timer;

        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            return;
        }

        string line;
        getline(file, line); // Skip header

        int count = 0;
        while (getline(file, line)) {
            stringstream ss(line);
            string idStr, name, rowStr, colStr, classStr, flightID;

            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, rowStr, ',');
            getline(ss, colStr, ',');
            getline(ss, classStr, ',');
            getline(ss, flightID);

            Passenger p{
                stoi(idStr),
                name,
                stoi(rowStr),
                colStr[0],
                classStr,
                flightID
            };

            // Add to linked list
            PassengerNode* newNode = new PassengerNode(p);
            newNode->next = ll_head;
            ll_head = newNode;
            count++;
        }

        double ms = timer.stopMs();
        cout << count << " passengers loaded into linked list.\n";
        cout << "Time Taken: " << ms << " ms\n";
    }

    // ==================== 2. CANCEL RESERVATION ====================
    void cancelReservation() {
        Timer timer;

        if (ll_head == nullptr) {
            cout << "No reservations to cancel.\n";
            return;
        }

        int targetID;
        cout << "Enter Passenger ID to cancel (0 to cancel): ";
        cin >> targetID;

        if (targetID == 0) {
            cout << "Cancellation cancelled.\n";
            return;
        }

        // Handle head deletion
        if (ll_head->data.passengerID == targetID) {
            PassengerNode* temp = ll_head;
            ll_head = ll_head->next;
            delete temp;
            cout << "Reservation cancelled successfully.\n";
            double ms = timer.stopMs();
            cout << "Time Taken: " << ms << " ms\n";
            return;
        }

        // Search for node to delete
        PassengerNode* current = ll_head;
        PassengerNode* prev = nullptr;

        while (current != nullptr && current->data.passengerID != targetID) {
            prev = current;
            current = current->next;
        }

        if (current == nullptr) {
            cout << "Passenger ID " << targetID << " not found.\n";
            return;
        }

        // Remove the node
        prev->next = current->next;
        delete current;

        cout << "Reservation cancelled successfully.\n";
        double ms = timer.stopMs();
        cout << "Time Taken: " << ms << " ms\n";
    }

    // ==================== 3. DISPLAY MANIFEST ====================
    void displayManifest() {
        Timer timer;

        if (ll_head == nullptr) {
            cout << "No passengers in manifest.\n";
            return;
        }

        cout << "\n===== LINKED LIST PASSENGER MANIFEST =====\n";
        cout << left << setw(10) << "ID"
             << setw(20) << "Name"
             << setw(10) << "Seat"
             << setw(12) << "Class"
             << setw(10) << "Flight" << endl;
        cout << string(62, '-') << endl;

        int count = 0;
        PassengerNode* current = ll_head;
        while (current != nullptr) {
            cout << left << setw(10) << current->data.passengerID
                 << setw(20) << current->data.name
                 << setw(10) << (to_string(current->data.seatRow) + current->data.seatColumn)
                 << setw(12) << current->data.seatClass
                 << setw(10) << current->data.flightID << endl;

            current = current->next;
            count++;
        }

        cout << "\nTotal passengers: " << count << endl;
        double ms = timer.stopMs();
        cout << "Time Taken: " << ms << " ms\n";
    }

    // ==================== 4. DISPLAY SEATING CHART ====================
    void displaySeatingChart() {
        Timer timer;

        string flightID;
        cout << "Enter Flight ID (FL001-FL079, 0 to cancel): ";
        cin >> flightID;

        if (flightID == "0") {
            cout << "Operation cancelled.\n";
            return;
        }

        // Validate flight ID
        if (!arr::isValidFlightID_001_079(flightID)) {
            cout << "Invalid Flight ID.\n";
            return;
        }

        cout << "\nSeating Chart for Flight " << flightID << ":\n";
        cout << "   A  B  C  D  E  F\n";

        bool hasPassengers = false;

        // Check each seat
        for (int row = 1; row <= 30; row++) {
            cout << setw(2) << row << " ";

            for (char col = 'A'; col <= 'F'; col++) {
                bool occupied = false;

                // Check linked list for this seat
                PassengerNode* current = ll_head;
                while (current != nullptr) {
                    if (current->data.flightID == flightID &&
                        current->data.seatRow == row &&
                        current->data.seatColumn == col) {
                        occupied = true;
                        hasPassengers = true;
                        break;
                    }
                    current = current->next;
                }

                cout << (occupied ? "[X] " : "[ ] ");
            }
            cout << endl;

            // Add section labels
            if (row == 3) cout << "   --- First Class ---\n";
            if (row == 10) cout << "   --- Business Class ---\n";
        }

        if (!hasPassengers) {
            cout << "\nNo passengers on this flight.\n";
        }

        double ms = timer.stopMs();
        cout << "\nTime Taken: " << ms << " ms\n";
    }

    // ==================== 5. ADD PASSENGER (for menu) ====================
    void addPassenger() {
        Timer timer;

        Passenger p;

        cout << "\n=== ADD NEW PASSENGER ===\n";

        // Generate ID
        int maxID = 0;
        PassengerNode* current = ll_head;
        while (current != nullptr) {
            if (current->data.passengerID > maxID)
                maxID = current->data.passengerID;
            current = current->next;
        }
        p.passengerID = maxID + 1;

        // === NAME INPUT ===
        while (true) {
            cout << "Name (0 to cancel): ";
            cin.ignore();  // Clear input buffer
            getline(cin, p.name);

            if (p.name == "0") {
                cout << "Add passenger cancelled.\n";
                return;
            }

            if (p.name.empty()) {
                cout << "Name cannot be empty.\n";
                continue;
            }
            break;
        }

        // === FLIGHT ID INPUT ===
        while (true) {
            cout << "Flight ID (FL001-FL079, 0 to cancel): ";
            cin >> p.flightID;

            if (p.flightID == "0") {
                cout << "Add passenger cancelled.\n";
                return;
            }

            // Use the array version's validation function
            if (!arr::isValidFlightID_001_079(p.flightID)) {
                cout << "Invalid Flight ID. Must be FL001 to FL079.\n";
                continue;
            }
            break;
        }

        // === SEAT CLASS INPUT ===
        int classChoice;
        while (true) {
            cout << "1. Economy\n";
            cout << "2. Business\n";
            cout << "3. First\n";
            cout << "Please choose class (0 to cancel): ";
            cin >> classChoice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            if (classChoice == 0) {
                cout << "Add passenger cancelled.\n";
                return;
            }

            if (classChoice < 1 || classChoice > 3) {
                cout << "Invalid choice. Please enter 1-3.\n";
                continue;
            }
            break;
        }

        // Set seat class based on choice
        if (classChoice == 1) p.seatClass = "Economy";
        else if (classChoice == 2) p.seatClass = "Business";
        else p.seatClass = "First";

        // === SEAT ROW INPUT (with class validation) ===
        int startRow, endRow;
        // Get row range based on class
        if (p.seatClass == "First") {
            startRow = 1; endRow = 3;
        } else if (p.seatClass == "Business") {
            startRow = 4; endRow = 10;
        } else { // Economy
            startRow = 11; endRow = 30;
        }

        while (true) {
            cout << "Seat Row (" << startRow << "-" << endRow << ", 0 to cancel): ";
            cin >> p.seatRow;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            if (p.seatRow == 0) {
                cout << "Add passenger cancelled.\n";
                return;
            }

            // Check if row matches class
            bool validRow = false;
            if (p.seatClass == "First" && p.seatRow >= 1 && p.seatRow <= 3) validRow = true;
            else if (p.seatClass == "Business" && p.seatRow >= 4 && p.seatRow <= 10) validRow = true;
            else if (p.seatClass == "Economy" && p.seatRow >= 11 && p.seatRow <= 30) validRow = true;

            if (!validRow) {
                cout << "Row does not match " << p.seatClass << " section.\n";
                continue;
            }
            break;
        }

        // === SEAT COLUMN INPUT ===
        while (true) {
            cout << "Seat Column (A-F, 0 to cancel): ";
            cin >> p.seatColumn;

            if (p.seatColumn == '0') {
                cout << "Add passenger cancelled.\n";
                return;
            }

            // Convert to uppercase if needed
            if (p.seatColumn >= 'a' && p.seatColumn <= 'f') {
                p.seatColumn = toupper(p.seatColumn);
            }

            if (p.seatColumn < 'A' || p.seatColumn > 'F') {
                cout << "Invalid column. Please enter A-F.\n";
                continue;
            }
            break;
        }

        // === CHECK IF SEAT IS ALREADY BOOKED ===
        if (isSeatBooked(p.seatRow, p.seatColumn, p.flightID)) {
            cout << "Seat " << p.seatRow << p.seatColumn
                 << " is already booked for flight " << p.flightID << ".\n";
            cout << "Please try again with a different seat.\n";
            double ms = timer.stopMs();
            cout << "Time Taken: " << ms << " ms\n";
            return;
        }

        // === ADD TO LINKED LIST ===
        PassengerNode* newNode = new PassengerNode(p);
        newNode->next = ll_head;
        ll_head = newNode;

        cout << "\nPassenger added successfully!\n";
        cout << "Passenger ID: " << p.passengerID << "\n";
        cout << "Name: " << p.name << "\n";
        cout << "Flight: " << p.flightID << "\n";
        cout << "Seat: " << p.seatRow << p.seatColumn << "\n";
        cout << "Class: " << p.seatClass << "\n";

        double ms = timer.stopMs();
        cout << "\nTime Taken: " << ms << " ms\n";
    }

    bool isSeatBooked(int row, char col, const string& flightID) {
        PassengerNode* current = ll_head;
        while (current != nullptr) {
            if (current->data.flightID == flightID &&
                current->data.seatRow == row &&
                current->data.seatColumn == col) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // ==================== 6. INPUT VALIDATION HELPERS ====================
    int readIntWithValidation(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }

    string readStringWithValidation(const string& prompt) {
        string value;
        cout << prompt;
        getline(cin, value);
        return value;
    }
}
