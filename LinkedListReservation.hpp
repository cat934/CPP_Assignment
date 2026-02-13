#ifndef LINKEDLISTRESERVATION_HPP
#define LINKEDLISTRESERVATION_HPP

#include <string>
#include "Assignment.h"   // <-- Passenger is defined here

using namespace std;

struct Node {
    Passenger data;
    Node* next;
};

class LinkedListReservation {
public:
    static void loadFromFile(const char* filename);

    static void insertPassenger();
    static bool cancelReservation(int id);
    static void searchByRow(int row);
    static void searchByClass(const string& cls);
    static void displayManifest();
    static void binarySearchByID(int id);

private:
    static Node* head;
};

#endif

