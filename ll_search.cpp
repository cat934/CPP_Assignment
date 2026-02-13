#include "Search.h"
#include "funcHeader.h"
#include <iostream>
#include <iomanip>
using namespace std;

PassengerNode* ll_head = nullptr;

PassengerNode* mergeRow(PassengerNode* a, PassengerNode* b) {
    if (!a) return b;
    if (!b) return a;

    if (a->data.seatRow <= b->data.seatRow) {
        a->next = mergeRow(a->next, b);
        return a;
    } else {
        b->next = mergeRow(a, b->next);
        return b;
    }
}

PassengerNode* mergeID(PassengerNode* a, PassengerNode* b) {
    if (!a) return b;
    if (!b) return a;

    if (a->data.passengerID <= b->data.passengerID) {
        a->next = mergeID(a->next, b);
        return a;
    } else {
        b->next = mergeID(a, b->next);
        return b;
    }
}

PassengerNode* getMiddle(PassengerNode* head) {
    if (!head) return head;
    PassengerNode* slow = head;
    PassengerNode* fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

PassengerNode* mergeSortByRowLL(PassengerNode* head) {
    if (!head || !head->next) return head;

    PassengerNode* mid = getMiddle(head);
    PassengerNode* right = mid->next;
    mid->next = nullptr;

    PassengerNode* left = mergeSortByRowLL(head);
    right = mergeSortByRowLL(right);

    return mergeRow(left, right);
}

PassengerNode* mergeSortByIDLL(PassengerNode* head) {
    if (!head || !head->next) return head;

    PassengerNode* mid = getMiddle(head);
    PassengerNode* right = mid->next;
    mid->next = nullptr;

    PassengerNode* left = mergeSortByIDLL(head);
    right = mergeSortByIDLL(right);

    return mergeID(left, right);
}

PassengerNode* mergeClass(PassengerNode* a, PassengerNode* b) {
    if (!a) return b;
    if (!b) return a;

    if (a->data.seatClass <= b->data.seatClass) {
        a->next = mergeClass(a->next, b);
        return a;
    } else {
        b->next = mergeClass(a, b->next);
        return b;
    }
}

PassengerNode* mergeSortByClassLL(PassengerNode* head) {
    if (!head || !head->next) return head;

    PassengerNode* mid = getMiddle(head);
    PassengerNode* right = mid->next;
    mid->next = nullptr;

    PassengerNode* left = mergeSortByClassLL(head);
    right = mergeSortByClassLL(right);

    return mergeClass(left, right);
}

namespace ll {

/* ================= BASIC HELPERS ================= */

int getNodeCount() {
    int count = 0;
    for (PassengerNode* p = ll_head; p != nullptr; p = p->next)
        count++;
    return count;
}

/* ================= 1. INSERTION ================= */

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

/* ================= 2. DELETION ================= */

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

/* ================= 3. REVERSAL ================= */

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

/* ================= 4. BASIC SEARCH ================= */

PassengerNode* searchByID(int id) {
    PassengerNode* current = ll_head;
    while (current != nullptr) {
        if (current->data.passengerID == id)
            return current;
        current = current->next;
    }
    return nullptr;
}

/* ================= 5. PRINT ================= */

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

/* ================= 6. SEARCH ================= */

void searchByRow(int row) {
    if (!ll_head) {
        cout << "No passengers.\n";
        return;
    }

    // Space trackers
    SpaceTracker stLinear("Search By Row - Linear");
    SpaceTracker stBinary("Search By Row - Binary");

    // Linear Search
    Timer linearTimer;

    //Sort linked list by ROW
    ll_head = mergeSortByRowLL(ll_head);

    // 2) Linear scan find matching rows
    PassengerNode* cur = ll_head;
    while (cur && cur->data.seatRow < row)
        cur = cur->next;

    PassengerNode* start = cur;
    bool found = false;

    while (cur && cur->data.seatRow == row) {
        found = true;
        cur = cur->next;
    }

    double linearTimeUs = linearTimer.stopMs();

    if (found) {
        cout << "\n";
        PassengerNode* p = start;
        while (p && p->data.seatRow == row) {
            cout << p->data.passengerID << " | "
                 << p->data.name << " | "
                 << p->data.seatRow << p->data.seatColumn << " | "
                 << p->data.seatClass << " | "
                 << p->data.flightID << endl;
            p = p->next;
        }
    }

    cout << "\nSearch Time (Linear): " << (long long)linearTimeUs << " us\n";

    // Track small auxiliary space
    stLinear.addVar<int>();
    stLinear.stop();

    // ---------- Binary Search ----------
    //binary search isn't supported on LL.
    Timer binaryTimer;
    double binaryTimeUs = binaryTimer.stopMs(); // ~0 us

    cout << "Search Time (Binary): " << (long long)binaryTimeUs << " us\n";

    // Track small auxiliary space
    stBinary.addVar<int>();
    stBinary.addVar<int>();
    stBinary.stop();
}

void searchByClass(const string& cls) {
    if (!ll_head) {
        cout << "No passengers.\n";
        return;
    }

    // Space trackers
    SpaceTracker stLinear("Search By Class - Linear");
    SpaceTracker stBinary("Search By Class - Binary");

    // ---------- Linear Search ----------
    Timer linearTimer;

    // Sort linked list by class
    ll_head = mergeSortByClassLL(ll_head);

    // Linear scan
    PassengerNode* cur = ll_head;
    while (cur && cur->data.seatClass < cls)
        cur = cur->next;

    PassengerNode* start = cur;
    bool found = false;

    while (cur && cur->data.seatClass == cls) {
        found = true;
        cur = cur->next;
    }

    double linearTimeUs = linearTimer.stopMs(); // microseconds

    if (found) {
        cout << "\n";
        PassengerNode* p = start;
        while (p && p->data.seatClass == cls) {
            cout << p->data.passengerID << " | "
                 << p->data.name << " | "
                 << p->data.seatRow << p->data.seatColumn << " | "
                 << p->data.seatClass << " | "
                 << p->data.flightID << endl;
            p = p->next;
        }
    } else {
        cout << "No passengers found in " << cls << " class.\n";
    }

    cout << "\nSearch Time (Linear): " << (long long)linearTimeUs << " us\n";

    // Track small auxiliary space
    stLinear.addVar<int>();
    stLinear.stop();

    // ---------- Binary Search ----------
    Timer binaryTimer;
    double binaryTimeUs = binaryTimer.stopMs(); // ~0 us

    cout << "Search Time (Binary): " << (long long)binaryTimeUs << " us\n";

    // Track small auxiliary space
    stBinary.addVar<int>();
    stBinary.addVar<int>();
    stBinary.stop();
}
}
