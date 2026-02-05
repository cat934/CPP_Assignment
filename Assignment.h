#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <string>

struct Passenger {
        int passengerID;
        std::string name;
        int seatRow;
        char seatColumn;
        std::string seatClass;
        std::string flightID;
    };

namespace arr {

    // Seating layout
    const int MAX_ROWS  = 30;
    const int MAX_COLS  = 6;   // A-F
    const int MAX_SEATS = MAX_ROWS * MAX_COLS;

    // Helpers
    inline int columnIndex(char c) { return c - 'A'; }
    inline int seatIndex(int row, char col) {
        return (row - 1) * MAX_COLS + columnIndex(col);
    }
    inline int indexToRow(int idx) { return (idx / MAX_COLS) + 1; }
    inline char indexToCol(int idx) { return static_cast<char>('A' + (idx % MAX_COLS)); }

}

//Linked List
// ADD THIS: Node for linked list implementation
struct PassengerNode {
    Passenger data;           // The passenger data
    PassengerNode* next;

    // Optional: constructor for convenience
    PassengerNode(const Passenger& p) : data(p), next(nullptr) {}
};


#endif
