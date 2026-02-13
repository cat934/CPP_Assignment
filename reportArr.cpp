#include "funcHeader.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

namespace arr {
    using namespace std::chrono;
    //Bubble Helper
    static inline void swapP(Passenger*& a, Passenger*& b) {
        Passenger* t = a; a = b; b = t;
    }

    //General report
    void bubbleSortByID(Passenger* arr[], int n) {
        for (int i = 0; i < n - 1; i++) {
            bool swapped = false;
            for (int j = 0; j < n - 1 - i; j++) {
                if (arr[j]->passengerID > arr[j + 1]->passengerID) {
                    swapP(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    void printGeneralReport()
    {
        using namespace std::chrono;

        if (passengerCount == 0) {
            cout << "No passengers loaded.\n";
            return;
        }

        Passenger** arrRadix  = new Passenger*[passengerCount];
        Passenger** arrBubble = new Passenger*[passengerCount];

        for (int i = 0; i < passengerCount; i++) {
            arrRadix[i]  = allPassengers[i];
            arrBubble[i] = allPassengers[i];
        }

        long long usRadixTotal = 0;
        long long usCountingOnly = 0;
        long long usBubble = 0;

        SpaceTracker spaceRadix("General Report - Radix Sort");
        SpaceTracker spaceBubble("General Report - Bubble Sort");

        // ---- Space tracking ----
        spaceRadix.addArray<Passenger*>(passengerCount); // output array
        spaceRadix.addArray<int>(10);                     // digit count (0–9)
        spaceRadix.addVar<int>();                         // exp variable

        spaceBubble.addVar<int>();
        spaceBubble.addVar<int>();
        spaceBubble.addVar<Passenger*>();

        // ================= RADIX SORT (TIMED) =================
        arr::resetCountingTimeMs(); // resets internal counting sort timer

        auto startRadix = high_resolution_clock::now();
        radixSortByID(arrRadix, passengerCount);
        auto endRadix = high_resolution_clock::now();

        usRadixTotal =
            duration_cast<microseconds>(endRadix - startRadix).count();

        // countingSortByDigit tracks its own time in milliseconds
        usCountingOnly =
            static_cast<long long>(arr::getCountingTimeMs() * 1000.0);

        // ================= BUBBLE SORT (TIMED) =================
        auto startBubble = high_resolution_clock::now();
        bubbleSortByID(arrBubble, passengerCount);
        auto endBubble = high_resolution_clock::now();

        usBubble =
            duration_cast<microseconds>(endBubble - startBubble).count();

        // ================= OUTPUT =================
        cout << "\n===== GENERAL PASSENGER REPORT =====\n";
        cout << "ID | Name | Seat | Class | Flight\n";
        cout << "----------------------------------\n";

        for (int i = 0; i < passengerCount; i++) {
            cout << arrRadix[i]->passengerID << " | "
                 << arrRadix[i]->name << " | "
                 << arrRadix[i]->seatRow << arrRadix[i]->seatColumn << " | "
                 << arrRadix[i]->seatClass << " | "
                 << arrRadix[i]->flightID << "\n";
        }

        cout << "\nTime Taken (Radix TOTAL):            " << usRadixTotal << " us";
        cout << "\nTime Taken (Counting Sort By Digit): " << usCountingOnly << " us";
        cout << "\nTime Taken (Bubble):                " << usBubble << " us\n\n";

        spaceRadix.stop();
        spaceBubble.stop();

        delete[] arrRadix;
        delete[] arrBubble;
    }

    //Seat occupancy report
    static void printHeader() {
        cout << "      A   B   C   D   E   F\n";
    }

    static void printSection(const bool occupied[MAX_SEATS],
                         const string& title,
                         int startRowInclusive, int endRowInclusive) {
        cout << "\n" << title << " (Rows " << startRowInclusive << "-" << endRowInclusive << ")\n";
        printHeader();

        for (int r = startRowInclusive; r <= endRowInclusive; r++) {
            cout << setw(3) << r << "  ";
            for (int c = 0; c < MAX_COLS; c++) {
                const int idx = seatIndex(r, static_cast<char>('A' + c));
                cout << (occupied[idx] ? "[X] " : "[.] ");
            }
            cout << "\n";
        }
    }

    double linearDisplaySeating(const std::string& flightID, bool occupied[MAX_SEATS]) {
        for (int i = 0; i < MAX_SEATS; i++) occupied[i] = false;

        auto startLinear = high_resolution_clock::now();
        for (int i = 0; i < passengerCount; i++) {
            Passenger* p = allPassengers[i];
            if (p && p->flightID == flightID) {
                int r = p->seatRow - 1;
                int c = p->seatColumn - 'A';
                if (r >= 0 && r < MAX_ROWS && c >= 0 && c < MAX_COLS)
                    occupied[r * MAX_COLS + c] = true;
            }
        }
        auto endLinear = high_resolution_clock::now();
        long long usLinear = duration_cast<microseconds>(endLinear - startLinear).count();
        return usLinear;
    }

    double binaryDisplaySeating(const std::string& flightID, bool occupied[MAX_SEATS]) {
        for (int i = 0; i < MAX_SEATS; i++) occupied[i] = false;

        Passenger** arr = new Passenger*[arr::passengerCount];
        for (int i = 0; i < arr::passengerCount; i++) arr[i] = arr::allPassengers[i];
        if (arr::passengerCount > 1) mergeSortByFlightID(arr, 0, arr::passengerCount - 1);

        auto startBinary = high_resolution_clock::now();
        int first = binarySearchFirstFlightID(arr, arr::passengerCount, flightID);
        if (first != -1) {
            int target = flightIDToNum(flightID);
            for (int i = first; i < arr::passengerCount && flightIDToNum(arr[i]->flightID) == target; i++) {
                Passenger* p = arr[i];
                int r = p->seatRow - 1;
                int c = p->seatColumn - 'A';
                if (r >= 0 && r < MAX_ROWS && c >= 0 && c < MAX_COLS)
                    occupied[r * MAX_COLS + c] = true;
            }
        }
        auto endBinary = high_resolution_clock::now();
        long long usBinary =
            duration_cast<microseconds>(endBinary - startBinary).count();

        delete[] arr;
        return usBinary;
    }

    void displaySeating(const string& flightID) {
        bool occupiedLinear[MAX_SEATS];
        bool occupiedBinary[MAX_SEATS];

        SpaceTracker spaceLinear("Seat Occupancy - Linear");
        SpaceTracker spaceBinary("Seat Occupancy - Binary");

        spaceLinear.addVar<int>();
        spaceLinear.addVar<int>();

        spaceBinary.addVar<int>();
        spaceBinary.addVar<int>();
        spaceBinary.addVar<int>();

        // Run BOTH algorithms (time is search-only)
        double usLinear = linearDisplaySeating(flightID, occupiedLinear);
        double usBinary = binaryDisplaySeating(flightID, occupiedBinary);

        // Print using ONE result set (linear), because output is the same
        cout << "\nSeat Occupancy for Flight " << flightID << "\n";

        // Class sections
        printSection(occupiedLinear, "First Class",    1,  3);
        printSection(occupiedLinear, "Business Class", 4, 10);
        printSection(occupiedLinear, "Economy Class", 11, 30);

        cout << fixed << setprecision(12);
        cout << "\nSearch Time (Linear): " << usLinear << " us";
        cout << "\nSearch Time (Binary): " << usBinary << " us\n\n";

        spaceLinear.stop();
        spaceBinary.stop();
        cout << endl;
    }


    //Sorted Report
    //Index (Bubble)
    void bubbleSortBySeatIndex(Passenger* arr[], int n) {
        for (int i = 0; i < n - 1; i++) {
            bool swapped = false;
            for (int j = 0; j < n - 1 - i; j++) {
                Passenger* a = arr[j];
                Passenger* b = arr[j + 1];

                bool needSwap =
                    (a->seatRow > b->seatRow) ||
                    (a->seatRow == b->seatRow &&
                     columnIndex(a->seatColumn) > columnIndex(b->seatColumn));

                if (needSwap) {
                    swapP(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    //Column (Bubble)
    void bubbleSortByColumn(Passenger* arr[], int n) {
        for (int i = 0; i < n - 1; i++) {
            bool swapped = false;
            for (int j = 0; j < n - 1 - i; j++) {
                if (columnIndex(arr[j]->seatColumn) > columnIndex(arr[j + 1]->seatColumn)) {
                    swapP(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    //Row (Bubble)
    void bubbleSortByRow(Passenger* arr[], int n) {
        for (int i = 0; i < n - 1; i++) {
            bool swapped = false;
            for (int j = 0; j < n - 1 - i; j++) {
                if (arr[j]->seatRow > arr[j + 1]->seatRow) {
                    swapP(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    //Class (Bubble)
    void bubbleSortByClass(Passenger* arr[], int n) {
        for (int i = 0; i < n - 1; i++) {
            bool swapped = false;

            for (int j = 0; j < n - 1 - i; j++) {
                int r1 = classPriority(arr[j]->seatClass);
                int r2 = classPriority(arr[j + 1]->seatClass);
                if (r1 > r2) {
                    swapP(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }

            if (!swapped) break;
        }
    }

    //FID (Bubble)
    void bubbleSortByFlightID(Passenger* arr[], int n) {
        for (int i = 0; i < n - 1; i++) {
            bool swapped = false;
            for (int j = 0; j < n - 1 - i; j++) {
                if (flightIDToNum(arr[j]->flightID) > flightIDToNum(arr[j + 1]->flightID)) {
                    swapP(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    //Name (Bubble)
    void bubbleSortByName(Passenger* arr[], int n) {
        for (int i = 0; i < n - 1; i++) {
            bool swapped = false;
            for (int j = 0; j < n - 1 - i; j++) {
                if (arr[j]->name > arr[j + 1]->name) { // string compare
                    swapP(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    void printGeneralReportSorted(int sortChoice)
    {
        using namespace std::chrono;

        if (passengerCount == 0) {
            cout << "No passengers loaded.\n";
            return;
        }

        Passenger** arrA = new Passenger*[passengerCount]; // Algorithm A
        Passenger** arrB = new Passenger*[passengerCount]; // Bubble

        for (int i = 0; i < passengerCount; i++) {
            arrA[i] = allPassengers[i];
            arrB[i] = allPassengers[i];
        }

        long long usA = 0;
        long long usB = 0;

        const char* algoAName =
            (sortChoice == 5 || sortChoice == 6) ? "Merge Sort" : "Counting Sort";

        // ================= SPACE TRACKING =================
        SpaceTracker spaceA("Sorted Report - Algorithm A");
        SpaceTracker spaceB("Sorted Report - Bubble Sort");

        if (sortChoice >= 1 && sortChoice <= 4) {
            // Counting-based
            spaceA.addArray<int>(passengerCount);
            spaceA.addArray<int>(MAX_ROWS + 1);
        } else {
            // Merge-based
            spaceA.addArray<Passenger*>(passengerCount);
            spaceA.addVar<int>();
        }

        spaceB.addVar<int>();
        spaceB.addVar<int>();
        spaceB.addVar<Passenger*>();

        // ================= ALGORITHM A (TIMED) =================
        auto startA = high_resolution_clock::now();

        switch (sortChoice) {
            case 1:
                sortByColumn(arrA, passengerCount);
                sortByRow(arrA, passengerCount);
                break;
            case 2:
                sortByColumn(arrA, passengerCount);
                break;
            case 3:
                sortByRow(arrA, passengerCount);
                break;
            case 4:
                sortByClass(arrA, passengerCount);
                break;
            case 5:
                mergeSortByFlightID(arrA, 0, passengerCount - 1);
                break;
            case 6:
                mergeSortByName(arrA, 0, passengerCount - 1);
                break;
            default:
                break;
        }

        auto endA = high_resolution_clock::now();
        usA = duration_cast<microseconds>(endA - startA).count();

        // ================= BUBBLE SORT (TIMED) =================
        auto startB = high_resolution_clock::now();

        switch (sortChoice) {
            case 1: bubbleSortBySeatIndex(arrB, passengerCount); break;
            case 2: bubbleSortByColumn(arrB, passengerCount);    break;
            case 3: bubbleSortByRow(arrB, passengerCount);       break;
            case 4: bubbleSortByClass(arrB, passengerCount);     break;
            case 5: bubbleSortByFlightID(arrB, passengerCount);  break;
            case 6: bubbleSortByName(arrB, passengerCount);      break;
            default: break;
        }

        auto endB = high_resolution_clock::now();
        usB = duration_cast<microseconds>(endB - startB).count();

        // ================= OUTPUT =================
        cout << "\n--- GENERAL REPORT (Sorted) ---\n";
        cout << "ID | Name | Seat | Class | Flight\n";
        cout << "----------------------------------\n";

        for (int i = 0; i < passengerCount; i++) {
            cout << arrA[i]->passengerID << " | "
                 << arrA[i]->name << " | "
                 << arrA[i]->seatRow << arrA[i]->seatColumn << " | "
                 << arrA[i]->seatClass << " | "
                 << arrA[i]->flightID << "\n";
        }

        cout << "\nTime Taken (" << algoAName << "): " << usA << " us";
        cout << "\nTime Taken (Bubble): " << usB << " us\n\n";

        spaceA.stop();
        spaceB.stop();

        delete[] arrA;
        delete[] arrB;
    }

    int flightIDToNum(const std::string& fid) {
        return (fid[2]-'0')*100 + (fid[3]-'0')*10 + (fid[4]-'0');
    }

    static void mergeByFlightID(Passenger* arr[], int left, int mid, int right) {
        Passenger** tmp = new Passenger*[right - left + 1];

        int i = left, j = mid + 1, k = 0;

        while (i <= mid && j <= right) {
            int a = flightIDToNum(arr[i]->flightID);
            int b = flightIDToNum(arr[j]->flightID);

            if (a <= b) tmp[k++] = arr[i++];
            else        tmp[k++] = arr[j++];
        }

        while (i <= mid)  tmp[k++] = arr[i++];
        while (j <= right) tmp[k++] = arr[j++];

        for (int x = 0; x < k; x++)
            arr[left + x] = tmp[x];

        delete[] tmp;
    }

    static int binaryFilterByFlightID_Sorted(Passenger* sortedArr[], int size,
                                         Passenger* out[], const std::string& flightID)
    {
        int first = binarySearchFirstFlightID(sortedArr, size, flightID);
        if (first == -1) return 0;

        int count = 0;
        for (int i = first; i < size && flightIDToNum(sortedArr[i]->flightID) == flightIDToNum(flightID); i++) {
            out[count++] = sortedArr[i];
        }
        return count;
    }


    void buildAndPrintManifestForFlight(const string& flightID) {
        // Linear
        Passenger** tempLinear = new Passenger*[passengerCount];

        SpaceTracker spaceLinear("Filter By FlightID - Linear");
        spaceLinear.addVar<int>();
        spaceLinear.addVar<int>();
        spaceLinear.addVar<string>();

        auto startLinear = high_resolution_clock::now();
        int countLinear = arr::filterByFlightID(allPassengers, passengerCount, tempLinear, flightID);
        auto endLinear = high_resolution_clock::now();
        long long usLinear = duration_cast<microseconds>(endLinear - startLinear).count();
        // FID Sort
        Passenger** sortedByFID = new Passenger*[passengerCount];
        for (int i = 0; i < passengerCount; i++) sortedByFID[i] = allPassengers[i];

        if (passengerCount > 1)
            mergeSortByFlightID(sortedByFID, 0, passengerCount - 1);

        // Binary
        Passenger** tempBinary = new Passenger*[passengerCount];

        SpaceTracker spaceBinary("Filter By FlightID - Binary");
        spaceBinary.addVar<int>();  // left
        spaceBinary.addVar<int>();  // right
        spaceBinary.addVar<int>();  // mid
        spaceBinary.addVar<int>();

        auto startBinary = high_resolution_clock::now();
        int countBinary = binaryFilterByFlightID_Sorted(sortedByFID, passengerCount, tempBinary, flightID);
        auto endBinary = high_resolution_clock::now();
        long long usBinary = duration_cast<microseconds>(endBinary - startBinary).count();
        delete[] sortedByFID;

        if (countLinear == 0) {
            cout << "No passengers found for Flight " << flightID << ".\n";
            cout << fixed << setprecision(12);
            cout << "\nSearch Time (Linear): " << usLinear << " us";
            cout << "\nSearch Time (Binary): " << usBinary << " us\n\n";

            spaceLinear.stop();
            spaceBinary.stop();
            cout << endl;

            delete[] tempLinear;
            delete[] tempBinary;
            return;
        }

        // Sort for Printing
        mergeSortByFlightClass(tempLinear, 0, countLinear - 1);

        int start = 0;
        while (start < countLinear) {
            int end = start;
            while (end < countLinear && tempLinear[end]->seatClass == tempLinear[start]->seatClass) {
                end++;
            }

            sortByColumn(tempLinear + start, end - start);
            sortByRow(tempLinear + start, end - start);

            start = end;
        }

        cout << "\nPassenger Manifest for Flight " << flightID << ":\n";
        cout << "ID | Name | Seat | Class | Flight\n";
        cout << "----------------------------------\n";

        for (int i = 0; i < countLinear; i++) {
            cout << tempLinear[i]->passengerID << " | "
                 << tempLinear[i]->name << " | "
                 << tempLinear[i]->seatRow << tempLinear[i]->seatColumn << " | "
                 << tempLinear[i]->seatClass << " | "
                 << tempLinear[i]->flightID << endl;
        }

        cout << fixed << setprecision(12);
        cout << "\nSearch Time (Linear): " << usLinear << " us";
        cout << "\nSearch Time (Binary): " << usBinary << " us\n\n";

        spaceLinear.stop();
        spaceBinary.stop();
        cout << endl;

        delete[] tempLinear;
        delete[] tempBinary;
    }


}
