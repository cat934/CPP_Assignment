#ifndef SEARCH_H
#define SEARCH_H

#include "Assignment.h"
#include <string>
#include <chrono>
#include <iostream>
#include <cstddef>

using std::string;

class Timer {
    private:
        using clock = std::chrono::steady_clock;
        clock::time_point start;

    public:
        Timer() {
            start = clock::now();
        }

    // millisec
    double stopMs() {
        auto end = clock::now();
        return std::chrono::duration<double, std::micro>(end - start).count();
    }

};


class SpaceTracker {
    private:
        std::string label;
        long long bytesUsed;
        bool active;

    public:
        SpaceTracker(const std::string& name)
            : label(name), bytesUsed(0), active(true)
        {
            std::cout << "\n[SpaceTracker START] " << label << std::endl;
        }

        // Record raw bytes
        void addBytes(long long bytes) {
            if (active)
                bytesUsed += bytes;
        }

        // Record N elements of type T
        template <typename T>
        void addArray(int count) {
            if (active)
                bytesUsed += sizeof(T) * count;
        }

        // Record a single variable
        template <typename T>
        void addVar() {
            if (active)
                bytesUsed += sizeof(T);
        }

        void stop() {
            if (!active) return;
            active = false;

            std::cout << "[SpaceTracker END] " << label << std::endl;
            std::cout << "Auxiliary Space Used: "
                      << bytesUsed << " bytes ("
                      << bytesUsed / 1024.0 << " KB)\n";
        }

        long long getBytes() const {
            return bytesUsed;
        }
};

struct SpaceMeter {
    size_t currentBytes = 0;
    size_t peakBytes = 0;

    void add(size_t bytes) {
        currentBytes += bytes;
        if (currentBytes > peakBytes) peakBytes = currentBytes;
    }

    void remove(size_t bytes) {
        if (bytes > currentBytes) currentBytes = 0;
        else currentBytes -= bytes;
    }

    size_t peak() const { return peakBytes; }
};


namespace arr {
    enum SearchAlgorithm {
        LINEAR_SEARCH,
        BINARY_SEARCH
    };

    extern Passenger** allPassengers;
    extern int passengerCount;
    extern int passengerCapacity;

    // File loading (1D only)
    void loadPassengers(const std::string& filename);

    // Dynamic passenger store
    void initPassengerStore(int initialCapacity);
    void ensurePassengerCapacity(int minCapacity);
    void ensurePassengerCapacityForOneMore();
    void clearPassengerStore();

    // Search (reports / filtering)
    void searchByColumn();
    void searchByRow();
    void searchBySeatIndex();
    void searchByClass();
    void searchByName();
    void searchByID();
    void searchByFlightID();

    // Reports / Manifest
    void printGeneralReport();
    void printGeneralReportSorted(int sortChoice);
    void buildAndPrintManifestForFlight(const std::string& flightID);

    // Sorting helpers
    void resetCountingTimeMs();
    double getCountingTimeMs();
    void radixSortByID(Passenger* arr[], int n);
    void mergeSortByName(Passenger* arr[], int left, int right);
    void mergeSortByID(Passenger* arr[], int left, int right);
    void mergeSortByFlightClass(Passenger* arr[], int left, int right);

    // Seat occupancy (1D seat index)
    void displaySeating(const std::string& flightID);
    double linearDisplaySeating(const std::string& flightID, bool occupied1D[MAX_SEATS]);
    double binaryDisplaySeating(const std::string& flightID, bool occupied1D[MAX_SEATS]);

    // Sort / Filter helpers
    void sortByColumn(Passenger* arr[], int size);
    void sortByRow(Passenger* arr[], int size);
    void sortByClass(Passenger* arr[], int size);
    int classPriority(const string& c);
    int flightIDToNum(const string& fid);
    void mergeSortByFlightID(Passenger* arr[], int left, int right);
    int binarySearchFirstFlightID(Passenger* arr[], int size, const std::string& flightID);


    int filterByFlightID(Passenger* src[], int srcCount, Passenger* out[], const std::string& flightID);

    // Flight ID validation
    bool isValidFlightID_001_079(const std::string& flightID);

    // Reservation
    int generateNewID(SearchAlgorithm algo);
    void addPassenger(SearchAlgorithm algo);
    void savePassenger();
    bool isSeatBooked(int row, char col, const std::string& flightID);

    // Cancellation
    void delPassenger(SearchAlgorithm algo);
    int binarySearchID(Passenger* arr[], int size, int targetID);

}

namespace ll {

    // Search functions (with parameters)
    void searchByColumn();
    void searchByRow(int row);              // ADD PARAMETER
    void searchBySeatIndex();
    void searchByClass(const string& cls);  // ADD PARAMETER
    void searchByName();
    void searchByID();
    void searchByFlightID();

    // Reports
    void printGeneralReport();
    void printGeneralReportSorted(int sortChoice);
    void buildAndPrintManifestForFlight(const string& flightID);

    // Seat occupancy
    void displaySeatingChart();  // RENAME from displaySeating to displaySeatingChart
    void displayManifest();      // ADD THIS

    // Sort helpers
    void mergeSortByName(PassengerNode* arr[], int left, int right);
    void mergeSortByID(PassengerNode* arr[], int left, int right);
    void mergeSortByFlightClass(PassengerNode* arr[], int left, int right);

    // Filter helpers
    int filterByFlightID(
        PassengerNode* src[], int srcCount,
        PassengerNode* out[], const string& flightID
    );

    // Add/Delete
    void addPassenger();
    void cancelReservation();    // RENAME from delPassenger to cancelReservation
    bool isSeatBooked(int row, char col, const string& flightID);
    void loadFromFile(const string& filename);  // ADD THIS

    // Utility
    int getNodeCount();
    PassengerNode** getAllNodesArray(); // Convert linked list to array for sorting
    void freeNodesArray(PassengerNode* arr[]);

    // Additional functions you implemented
    void insertAtBeginning(Passenger data);
    void insertAtEnd(Passenger data);
    void insertAtPosition(Passenger data, int pos);
    void deleteNode(int passengerID);
    void reverseLinkedList();
    PassengerNode* searchByID(int id);

}

#endif



