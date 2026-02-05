#include "Assignment.h"
#include "funcHeader.h"
#include <iostream>
#include <cctype>
#include <chrono>
using namespace std;

namespace arr{

    //Changes....
    //Array size modification
    void initPassengerStore(int initialCapacity) {
        clearPassengerStore();
        if (initialCapacity < 1) initialCapacity = 1;
        passengerCapacity = initialCapacity;
        allPassengers = new Passenger*[passengerCapacity];
        passengerCount = 0;
    }

    void ensurePassengerCapacity(int minCapacity) {
        if (minCapacity <= passengerCapacity) return;

        Passenger** newArr = new Passenger*[minCapacity];
        for (int i = 0; i < passengerCount; i++) newArr[i] = allPassengers[i];
        delete[] allPassengers;
        allPassengers = newArr;
        passengerCapacity = minCapacity;
    }

    // Grow capacity by +1 whenever a reservation is made
    void ensurePassengerCapacityForOneMore() {
        if (passengerCount < passengerCapacity) return;
        ensurePassengerCapacity(passengerCapacity + 1);
    }

    void clearPassengerStore() {
        // Free Passenger objects
        if (allPassengers) {
            for (int i = 0; i < passengerCount; i++) {
                delete allPassengers[i];
            }
            delete[] allPassengers;
        }
        allPassengers = nullptr;
        passengerCount = 0;
        passengerCapacity = 0;
    }

    // Class priority
    int classPriority(const string& c) {
        if (c == "First") return 0;
        if (c == "Business") return 1;
        return 2; // Economy
    }

    bool flightClassLess(Passenger* a, Passenger* b) {
        if (a->flightID != b->flightID)
            return a->flightID < b->flightID;
        return classPriority(a->seatClass) < classPriority(b->seatClass);
    }

    void mergeByFlightClass(Passenger* arr[], int left, int mid, int right) {
        Passenger** temp = new Passenger*[right + 1];

        int i = left, j = mid + 1, k = left;

        while (i <= mid && j <= right) {
            if (flightClassLess(arr[i], arr[j]))
                temp[k++] = arr[i++];
            else
                temp[k++] = arr[j++];
        }

        while (i <= mid)
            temp[k++] = arr[i++];

        while (j <= right)
            temp[k++] = arr[j++];

        for (int x = left; x <= right; x++)
            arr[x] = temp[x];

        delete[] temp;
    }

    void mergeSortByFlightClass(Passenger* arr[], int left, int right) {
        if (left >= right) return;

        int mid = (left + right) / 2;
        mergeSortByFlightClass(arr, left, mid);
        mergeSortByFlightClass(arr, mid + 1, right);
        mergeByFlightClass(arr, left, mid, right);
    }


    //General Report
    int getMaxPassengerID(Passenger* arr[], int n) {
        int mx = arr[0]->passengerID;
        for (int i = 1; i < n; i++)
            if (arr[i]->passengerID > mx)
                mx = arr[i]->passengerID;
        return mx;
    }


    // Counting sort (LSD)
    using namespace std::chrono;

    static double g_countingMs = 0.0;

    void resetCountingTimeMs() {
        g_countingMs = 0.0;
    }

    double getCountingTimeMs() {
        return g_countingMs;
    }

    void countingSortByDigit(Passenger* arr[], int n, int exp) {
        auto t0 = high_resolution_clock::now();

        Passenger** output = new Passenger*[n];
        int count[10] = {0};

        for (int i = 0; i < n; i++) {
            int digit = (arr[i]->passengerID / exp) % 10;
            count[digit]++;
        }

        for (int i = 1; i < 10; i++)
            count[i] += count[i - 1];

        for (int i = n - 1; i >= 0; i--) {
            int digit = (arr[i]->passengerID / exp) % 10;
            output[count[digit] - 1] = arr[i];
            count[digit]--;
        }

        for (int i = 0; i < n; i++)
            arr[i] = output[i];

        delete[] output;

        // stop time
        auto t1 = high_resolution_clock::now();
        g_countingMs += duration<double, std::milli>(t1 - t0).count();
    }

    //General Rep
    void radixSortByID(Passenger* arr[], int n) {
        int maxID = getMaxPassengerID(arr, n);

        for (int exp = 1; maxID / exp > 0; exp *= 10)
            countingSortByDigit(arr, n, exp);
    }


    //FID filter
    int filterByFlightID(Passenger* src[], int srcCount,
                     Passenger* out[], const string& flightID) {
        int count = 0;
        for (int i = 0; i < srcCount; i++) {
            if (src[i] && src[i]->flightID == flightID) {
                out[count++] = src[i];
            }
        }
        return count;
    }

    //validFID (Passenger Manifest for Seat Occupancy)
    bool isValidFlightID_001_079(const std::string& flightID) {
        if (flightID.size() != 5) return false;
        if (flightID[0] != 'F' || flightID[1] != 'L') return false;
        if (!isdigit((unsigned char)flightID[2]) ||
            !isdigit((unsigned char)flightID[3]) ||
            !isdigit((unsigned char)flightID[4])) return false;

        int num = (flightID[2] - '0') * 100 + (flightID[3] - '0') * 10 + (flightID[4] - '0');
        return (num >= 1 && num <= 79);
    }

}
