#include "Search.h"
#include "funcHeader.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Linked list head - DEFINE IT HERE (only once in entire project)
PassengerNode* ll_head = nullptr;

namespace ll {

/* ================= BASIC HELPERS ================= */

int getNodeCount() {
    int count = 0;
    for (PassengerNode* p = ll_head; p != nullptr; p = p->next)
        count++;
    return count;
}

/* ================= 1. INSERTION (You do this) ================= */

void insertAtBeginning(Passenger data) {
    PassengerNode* newNode = new PassengerNode(data);
    newNode->next = ll_head;
    ll_head = newNode;
}

void insertAtEnd(Passenger data) {
    PassengerNode* newNode = new PassengerNode(data);
    if (ll_head == nullptr) {
        ll_head = newNode;
        return;
    }
    PassengerNode* temp = ll_head;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
    temp->next = newNode;
}

void insertAtPosition(Passenger data, int pos) {
    if (pos <= 1 || ll_head == nullptr) {
        insertAtBeginning(data);
        return;
    }

    PassengerNode* temp = ll_head;
    int count = 1;

    while (temp->next != nullptr && count < pos - 1) {
        temp = temp->next;
        count++;
    }

    PassengerNode* newNode = new PassengerNode(data);
    newNode->next = temp->next;
    temp->next = newNode;
}

/* ================= 2. DELETION (You do this) ================= */

void deleteNode(int passengerID) {
    if (ll_head == nullptr) return;

    // Delete head
    if (ll_head->data.passengerID == passengerID) {
        PassengerNode* temp = ll_head;
        ll_head = ll_head->next;
        delete temp;
        return;
    }

    // Delete middle/end
    PassengerNode* temp = ll_head;
    while (temp->next != nullptr && temp->next->data.passengerID != passengerID) {
        temp = temp->next;
    }

    if (temp->next != nullptr) {
        PassengerNode* toDelete = temp->next;
        temp->next = temp->next->next;
        delete toDelete;
    }
}

/* ================= 3. REVERSAL (You do this) ================= */

void reverseLinkedList() {
    PassengerNode *prev = nullptr, *current = ll_head, *next = nullptr;

    while (current != nullptr) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    ll_head = prev;
}

/* ================= 4. BASIC SEARCH (You do this - simple version) ================= */

PassengerNode* searchByID(int id) {
    PassengerNode* current = ll_head;
    while (current != nullptr) {
        if (current->data.passengerID == id)
            return current;
        current = current->next;
    }
    return nullptr;
}

/* ================= 5. PRINT (You do this - basic version) ================= */

void printGeneralReport() {
    if (ll_head == nullptr) {
        cout << "No passengers.\n";
        return;
    }

    cout << "\n===== PASSENGER REPORT =====\n";
    PassengerNode* current = ll_head;
    while (current != nullptr) {
        cout << current->data.passengerID << " | "
             << current->data.name << " | "
             << current->data.seatRow << current->data.seatColumn << " | "
             << current->data.seatClass << endl;
        current = current->next;
    }
    cout << "Total: " << getNodeCount() << endl;
}

/* ================= TODO: JIASHUEN ADDS THESE ================= */
// void searchByRow(int row) - enhance with better output
void searchByRow(int row) {
    Timer timer;
    bool found = false;

    cout << "\nPassengers in Row " << row << ":\n";
    cout << "ID | Name | Seat | Class | Flight\n";
    cout << "----------------------------------\n";

    for (PassengerNode* cur = ll_head; cur != nullptr; cur = cur->next) {
        if (cur->data.seatRow == row) {
            cout << cur->data.passengerID << " | "
                 << cur->data.name << " | "
                 << cur->data.seatRow << cur->data.seatColumn << " | "
                 << cur->data.seatClass << " | "
                 << cur->data.flightID << endl;
            found = true;
        }
    }

    double ms = timer.stopMs();
    if (!found) cout << "No passengers found.\n";

    cout << "\nTime Taken (Linear): " << ms << " ms\n\n";
}

// void searchByClass(string cls) - new search
void searchByClass(const string& cls) {
    Timer timer;
    bool found = false;

    cout << "\nPassengers in " << cls << " class:\n";
    cout << "ID | Name | Seat | Flight\n";
    cout << "----------------------------------\n";

    for (PassengerNode* cur = ll_head; cur != nullptr; cur = cur->next) {
        if (cur->data.seatClass == cls) {
            cout << cur->data.passengerID << " | "
                 << cur->data.name << " | "
                 << cur->data.seatRow << cur->data.seatColumn << " | "
                 << cur->data.flightID << endl;
            found = true;
        }
    }

    double ms = timer.stopMs();
    if (!found) cout << "No passengers found.\n";

    cout << "\nTime Taken (Linear): " << ms << " ms\n\n";
}

// Better menu interface
// Input validation
// Timer for performance measurement

} // namespace ll
