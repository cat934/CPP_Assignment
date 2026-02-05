#include "LinkedListReservation.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Node* LinkedListReservation::head = nullptr;

void LinkedListReservation::loadFromFile(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file for linked list.\n";
        return;
    }

    string line;
    getline(file, line); // skip header

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

        Node* newNode = new Node{ p, head };
        head = newNode;
    }

    cout << "Linked List loaded from file.\n";
}

void LinkedListReservation::insertPassenger() {
    Passenger p;
    cout << "ID: "; cin >> p.passengerID;
    cout << "Name: "; cin.ignore(); getline(cin, p.name);
    cout << "Row: "; cin >> p.seatRow;
    cout << "Column: "; cin >> p.seatColumn;
    cout << "Class: "; cin >> p.seatClass;
    cout << "Flight ID: "; cin >> p.flightID;

    Node* newNode = new Node{ p, head };
    head = newNode;

    cout << "Passenger inserted.\n";
}

bool LinkedListReservation::cancelReservation(int id) {
    Node* curr = head;
    Node* prev = nullptr;

    while (curr) {
        if (curr->data.passengerID == id) {
            if (prev) prev->next = curr->next;
            else head = curr->next;
            delete curr;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

void LinkedListReservation::searchByRow(int row) {
    Node* curr = head;
    while (curr) {
        if (curr->data.seatRow == row) {
            cout << curr->data.passengerID << " " << curr->data.name << " "
                 << curr->data.flightID << endl;
        }
        curr = curr->next;
    }
}

void LinkedListReservation::searchByClass(const string& cls) {
    Node* curr = head;
    while (curr) {
        if (curr->data.seatClass == cls) {
            cout << curr->data.passengerID << " " << curr->data.name << " "
                 << curr->data.flightID << endl;
        }
        curr = curr->next;
    }
}

void LinkedListReservation::displayManifest() {
    Node* curr = head;
    while (curr) {
        cout << curr->data.passengerID << " "
             << curr->data.name << " "
             << curr->data.seatRow << curr->data.seatColumn << " "
             << curr->data.seatClass << " "
             << curr->data.flightID << endl;
        curr = curr->next;
    }
}

void LinkedListReservation::binarySearchByID(int id) {
    Node* curr = head;
    while (curr) {
        if (curr->data.passengerID == id) {
            cout << "Found: " << curr->data.name << endl;
            return;
        }
        curr = curr->next;
    }
    cout << "Passenger not found.\n";
}
