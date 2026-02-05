#include "Assignment.h"
#include "funcHeader.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <string>
using namespace std;

namespace arr {

    using namespace std::chrono;

    void mergeSortByID(Passenger* arr[], int left, int right);
    int  binarySearchID(Passenger* arr[], int size, int targetID);
    void sortByRow(Passenger* arr[], int size);
    void mergeSortByFlightClass(Passenger* arr[], int left, int right);

// COLUMN SORT (Counting)
    //Changes
    void sortByColumn(Passenger* arr[], int size) {
        int count[MAX_COLS] = {0};
        Passenger** output = new Passenger*[size];

        for (int i = 0; i < size; i++)
            count[columnIndex(arr[i]->seatColumn)]++;

        for (int i = 1; i < MAX_COLS; i++)
            count[i] += count[i - 1];

        for (int i = size - 1; i >= 0; i--) {
            int c = columnIndex(arr[i]->seatColumn);
            output[count[c] - 1] = arr[i];
            count[c]--;
        }

        for (int i = 0; i < size; i++)
            arr[i] = output[i];

        delete[] output;
    }


    int binarySearchColumn(Passenger* arr[], int size, int target) {
        int left = 0, right = size - 1, pos = -1;

        while (left <= right) {
            int mid = (left + right) / 2;
            int midCol = columnIndex(arr[mid]->seatColumn);

            if (midCol == target) {
                pos = mid;
                right = mid - 1; // move LEFT for first match
            }
            else if (midCol < target)
                left = mid + 1;
            else
                right = mid - 1;
        }
        return pos;
    }

    int linearSearchColumn(Passenger* arr[], int size, int target) {
        for (int i = 0; i < size; i++) {
            if (columnIndex(arr[i]->seatColumn) == target)
                return i; // first match
        }
        return -1;
    }

    //Changes....
    void searchByColumn() {
        while (true) {
            char col;

            while (true) {
                cout << "Enter seat column (A-F, 0 to cancel): ";
                cin >> col;

                if (col == '0') return;

                if (col < 'A' || col > 'F') {
                    cout << "Invalid column. Please enter A-F.\n";
                    continue;
                }
                break;
            }

            Passenger** arr = new Passenger*[passengerCount];
            for (int i = 0; i < passengerCount; i++)
                arr[i] = allPassengers[i];

            sortByColumn(arr, passengerCount);

            int target = columnIndex(col);

            // ================= SPACE TRACKERS =================
            SpaceTracker spaceLinear("Search By Column - Linear");
            SpaceTracker spaceBinary("Search By Column - Binary");

            // -------- Linear search time (only) --------
            spaceLinear.addVar<int>(); // i
            spaceLinear.addVar<int>(); // target
            auto startLinear = high_resolution_clock::now();
            int idxL = linearSearchColumn(arr, passengerCount, target);
            auto endLinear = high_resolution_clock::now();
            long long usLinear = duration_cast<microseconds>(endLinear - startLinear).count();
            // -------- Binary search time (only) --------
            spaceBinary.addVar<int>();
            spaceBinary.addVar<int>();
            spaceBinary.addVar<int>();
            auto startBinary = high_resolution_clock::now();
            int idxB = binarySearchColumn(arr, passengerCount, target);
            auto endBinary = high_resolution_clock::now();
            long long usBinary = duration_cast<microseconds>(endBinary - startBinary).count();
            int idx = idxB;

            if (idx == -1) {
                cout << "No passengers found in column " << col << ".\n";
                cout << fixed << setprecision(12);
                cout << "\nSearch Time (Linear): " << usLinear << " us";
                cout << "\nSearch Time (Binary): " << usBinary << " us\n\n";

                cout << endl;

                spaceLinear.stop();
                spaceBinary.stop();

                delete[] arr;
                continue;
            }

            int start = idx, end = idx;
            while (end < passengerCount &&
                   columnIndex(arr[end]->seatColumn) == target)
                end++;

            sortByRow(arr + start, end - start);
            mergeSortByID(arr, start, end - 1);

            cout << "\nPassengers in column " << col << ":\n";
            for (int i = start; i < end; i++) {
                cout << arr[i]->passengerID << " | "
                     << arr[i]->name << " | "
                     << arr[i]->seatRow << arr[i]->seatColumn << " | "
                     << arr[i]->seatClass << " | "
                     << arr[i]->flightID << endl;
            }

            cout << fixed << setprecision(12);
            cout << "\nSearch Time (Linear): " << usLinear << " us";
            cout << "\nSearch Time (Binary): " << usBinary << " us\n\n";

            spaceLinear.stop();
            spaceBinary.stop();
            cout << endl;

            delete[] arr;
        }
    }


// ROW SEARCH
     void sortByRow(Passenger* arr[], int size) {
         int count[MAX_ROWS + 1] = {0};
         Passenger** output = new Passenger*[size];

         for (int i = 0; i < size; i++)
             count[arr[i]->seatRow]++;

         for (int i = 1; i <= MAX_ROWS; i++)
             count[i] += count[i - 1];

         for (int i = size - 1; i >= 0; i--) {
             int r = arr[i]->seatRow;
             output[count[r] - 1] = arr[i];
             count[r]--;
         }

         for (int i = 0; i < size; i++)
             arr[i] = output[i];

         delete[] output;
     }

     int binarySearchRow(Passenger* arr[], int size, int row) {
         int l = 0, r = size - 1, pos = -1;
         while (l <= r) {
             int m = (l + r) / 2;
             if (arr[m]->seatRow == row) {
                 pos = m;
                 r = m - 1;
             }
             else if (arr[m]->seatRow < row)
                 l = m + 1;
             else
                 r = m - 1;
         }
         return pos;
     }

     int linearSearchRow(Passenger* arr[], int size, int row) {
         for (int i = 0; i < size; i++) {
             if (arr[i]->seatRow == row)
                 return i; // first match
         }
         return -1;
     }

    void searchByRow()
    {

        while (true)
        {
            int row;

            while (true)
            {
                cout << "Enter seat row (1-" << MAX_ROWS << ", 0 to cancel): ";
                cin >> row;

                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number.\n";
                    continue;
                }

                if (row == 0) return;

                if (row < 1 || row > MAX_ROWS)
                {
                    cout << "Row out of range.\n";
                    continue;
                }
                break;
            }

            // Build array
            Passenger** arr = new Passenger*[passengerCount];
            for (int i = 0; i < passengerCount; i++)
                arr[i] = allPassengers[i];

            // Pre-sort (NOT TIMED)
            sortByRow(arr, passengerCount);

            SpaceTracker spaceLinear("Search By Row - Linear");
            SpaceTracker spaceBinary("Search By Row - Binary");

            // ================= LINEAR SEARCH (search only) =================
            spaceLinear.addVar<int>(); // i
            spaceLinear.addVar<int>(); // row

            auto startLinear = high_resolution_clock::now();
            int idxL = linearSearchRow(arr, passengerCount, row);
            auto endLinear = high_resolution_clock::now();

            long long usLinear =
                duration_cast<microseconds>(endLinear - startLinear).count();

            // ================= BINARY SEARCH (search only) =================
            spaceBinary.addVar<int>();
            spaceBinary.addVar<int>();
            spaceBinary.addVar<int>();

            auto startBinary = high_resolution_clock::now();
            int idxB = binarySearchRow(arr, passengerCount, row);
            auto endBinary = high_resolution_clock::now();

            long long usBinary =
                duration_cast<microseconds>(endBinary - startBinary).count();

            int idx = idxB;

            if (idx == -1)
            {
                cout << "No passengers found.\n";
                cout << "\nSearch Time (Linear): " << usLinear << " us";
                cout << "\nSearch Time (Binary): " << usBinary << " us\n\n";

                spaceLinear.stop();
                spaceBinary.stop();
                delete[] arr;
                continue;
            }

            // Find range (NOT TIMED)
            int start = idx, end = idx;
            while (end < passengerCount && arr[end]->seatRow == row)
                end++;

            // Sort for display (NOT TIMED)
            mergeSortByID(arr, start, end - 1);

            cout << "\nPassengers in Row " << row << ":\n";
            for (int i = start; i < end; i++)
            {
                cout << arr[i]->passengerID << " | "
                     << arr[i]->name << " | "
                     << arr[i]->seatRow << arr[i]->seatColumn << " | "
                     << arr[i]->seatClass << " | "
                     << arr[i]->flightID << endl;
            }

            cout << "\nSearch Time (Linear): " << usLinear << " us";
            cout << "\nSearch Time (Binary): " << usBinary << " us\n\n";

            spaceLinear.stop();
            spaceBinary.stop();

            delete[] arr;
        }
    }

// INDEX SEARCH
    void searchBySeatIndex()
    {

        while (true)
        {
            int row;
            char col;

            // -------- Input --------
            while (true)
            {
                cout << "Enter seat row (1-" << MAX_ROWS << ", 0 to cancel): ";
                cin >> row;

                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number.\n";
                    continue;
                }
                if (row == 0) return;
                if (row < 1 || row > MAX_ROWS)
                {
                    cout << "Row out of range.\n";
                    continue;
                }
                break;
            }

            while (true)
            {
                cout << "Enter seat column (A-F, 0 to cancel): ";
                cin >> col;

                if (col == '0') return;
                if (col < 'A' || col > 'F')
                {
                    cout << "Invalid column.\n";
                    continue;
                }
                break;
            }

            cout << "\nSeat " << row << col << ":\n";
            cout << "ID | Name | Seat | Class | Flight\n";
            cout << "----------------------------------\n";

            // ================= LINEAR SEARCH =================
            SpaceTracker spaceLinear("Seat Index - Linear");
            spaceLinear.addVar<int>();   // i
            spaceLinear.addVar<int>();   // row
            spaceLinear.addVar<char>();  // col

            bool foundLinear = false;

            auto startLinear = high_resolution_clock::now();
            for (int i = 0; i < passengerCount; i++)
            {
                if (allPassengers[i]->seatRow == row &&
                    allPassengers[i]->seatColumn == col)
                {
                    cout << allPassengers[i]->passengerID << " | "
                         << allPassengers[i]->name << " | "
                         << row << col << " | "
                         << allPassengers[i]->seatClass << " | "
                         << allPassengers[i]->flightID << endl;

                    foundLinear = true;
                }
            }
            auto endLinear = high_resolution_clock::now();

            long long usLinear =
                duration_cast<microseconds>(endLinear - startLinear).count();

            if (!foundLinear)
                cout << "(Linear) Seat is empty.\n";

            // ================= BINARY SEARCH (ROW) =================
            Passenger** arr = new Passenger*[passengerCount];
            for (int i = 0; i < passengerCount; i++)
                arr[i] = allPassengers[i];

            sortByRow(arr, passengerCount); // preprocessing (NOT timed)

            SpaceTracker spaceBinaryRow("Seat Index - Binary Row");
            spaceBinaryRow.addVar<int>();
            spaceBinaryRow.addVar<int>();
            spaceBinaryRow.addVar<int>();

            auto startRow = high_resolution_clock::now();
            int idxRow = binarySearchRow(arr, passengerCount, row);
            auto endRow = high_resolution_clock::now();

            long long usBinaryRow =
                duration_cast<microseconds>(endRow - startRow).count();

            if (idxRow == -1)
            {
                cout << "\nSearch Time (Linear SeatIndex): " << usLinear << " us";
                cout << "\nSearch Time (Binary Row): " << usBinaryRow << " us";
                cout << "\nSearch Time (Binary Col): 0 us";
                cout << "\nSearch Time (Binary Total): " << usBinaryRow << " us\n\n";

                spaceLinear.stop();
                spaceBinaryRow.stop();
                delete[] arr;
                continue;
            }

            // ================= BINARY SEARCH (COLUMN) =================
            int rowStart = idxRow, rowEnd = idxRow;
            while (rowEnd < passengerCount && arr[rowEnd]->seatRow == row)
                rowEnd++;

            sortByColumn(arr + rowStart, rowEnd - rowStart); // NOT timed

            SpaceTracker spaceBinaryCol("Seat Index - Binary Column");
            spaceBinaryCol.addVar<int>();
            spaceBinaryCol.addVar<int>();
            spaceBinaryCol.addVar<int>();

            int targetCol = columnIndex(col);

            auto startCol = high_resolution_clock::now();
            int idxColLocal =
                binarySearchColumn(arr + rowStart, rowEnd - rowStart, targetCol);
            auto endCol = high_resolution_clock::now();

            long long usBinaryCol =
                duration_cast<microseconds>(endCol - startCol).count();

            if (idxColLocal == -1)
            {
                cout << "(Binary) Seat is empty.\n";
                cout << "\nSearch Time (Linear SeatIndex): " << usLinear << " us";
                cout << "\nSearch Time (Binary Row): " << usBinaryRow << " us";
                cout << "\nSearch Time (Binary Col): " << usBinaryCol << " us";
                cout << "\nSearch Time (Binary Total): "
                     << (usBinaryRow + usBinaryCol) << " us\n\n";

                spaceLinear.stop();
                spaceBinaryRow.stop();
                spaceBinaryCol.stop();
                delete[] arr;
                continue;
            }

            // ================= OUTPUT =================
            int colStart = rowStart + idxColLocal;
            int colEnd = colStart;
            while (colEnd < rowEnd &&
                   columnIndex(arr[colEnd]->seatColumn) == targetCol)
                colEnd++;

            cout << "\n(Binary) Matches:\n";
            for (int i = colStart; i < colEnd; i++)
            {
                cout << arr[i]->passengerID << " | "
                     << arr[i]->name << " | "
                     << arr[i]->seatRow << arr[i]->seatColumn << " | "
                     << arr[i]->seatClass << " | "
                     << arr[i]->flightID << endl;
            }

            cout << "\nSearch Time (Linear SeatIndex): " << usLinear << " us";
            cout << "\nSearch Time (Binary Row): " << usBinaryRow << " us";
            cout << "\nSearch Time (Binary Col): " << usBinaryCol << " us";
            cout << "\nSearch Time (Binary Total): "
                 << (usBinaryRow + usBinaryCol) << " us\n\n";

            spaceLinear.stop();
            spaceBinaryRow.stop();
            spaceBinaryCol.stop();

            delete[] arr;
        }
    }


// CLASS SEARCH
    int classIndex(const string& c) {
        if (c == "Economy") return 0;
        if (c == "Business") return 1;
        if (c == "First") return 2;
        return -1; // First
    }

    void sortByClass(Passenger* arr[], int size) {
        int count[3] = {0};

        Passenger** output = new Passenger*[size];

        for (int i = 0; i < size; i++)
            count[classIndex(arr[i]->seatClass)]++;

        for (int i = 1; i < 3; i++)
            count[i] += count[i - 1];

        for (int i = size - 1; i >= 0; i--) {
            int c = classIndex(arr[i]->seatClass);
            output[count[c] - 1] = arr[i];
            count[c]--;
        }

        for (int i = 0; i < size; i++)
            arr[i] = output[i];

        delete[] output;
    }

    int binarySearchClass(Passenger* arr[], int size, int target) {
        int l = 0, r = size - 1, pos = -1;
        while (l <= r) {
            int m = (l + r) / 2;
            int midClass = classIndex(arr[m]->seatClass);
            if (midClass == target) {
                pos = m;
                r = m - 1;
            }
            else if (midClass < target)
                l = m + 1;
            else
                r = m - 1;
        }
        return pos;
    }

    int linearSearchClass(Passenger* arr[], int size, int target) {
        for (int i = 0; i < size; i++) {
            if (classIndex(arr[i]->seatClass) == target)
                return i; // first match
        }
        return -1;
    }

    void searchByClass() {
        while (true) {
            string cls;
            int target;

            while (true) {
                cout << "Enter class (Economy / Business / First, 0 to cancel): ";
                cin >> cls;

                if (cls == "0") return;

                target = classIndex(cls);
                if (target != -1) break;

                cout << "Invalid class. Try again.\n";
            }

            Passenger** arr = new Passenger*[passengerCount];
            for (int i = 0; i < passengerCount; i++)
                arr[i] = allPassengers[i];

            sortByClass(arr, passengerCount);

            SpaceTracker spaceLinear("Search By Class - Linear");
            SpaceTracker spaceBinary("Search By Class - Binary");

            // ====== SEARCH-ONLY timing (Linear) ======
            spaceLinear.addVar<int>();
            spaceLinear.addVar<int>();
            auto startLinear = high_resolution_clock::now();
            int idxL = linearSearchClass(arr, passengerCount, target);
            auto endLinear = high_resolution_clock::now();
            long long usLinear =
            duration_cast<microseconds>(endLinear - startLinear).count();
            // ====== SEARCH-ONLY timing (Binary) ======
            spaceBinary.addVar<int>();
            spaceBinary.addVar<int>();
            spaceBinary.addVar<int>();
            auto startBinary = high_resolution_clock::now();
            int idxB = binarySearchClass(arr, passengerCount, target);
            auto endBinary = high_resolution_clock::now();

            long long usBinary =
            duration_cast<microseconds>(endBinary - startBinary).count();
            int idx = idxB; // 用 binary 的 idx 来输出

            if (idx == -1) {
                cout << "No passengers found.\n";
                cout << fixed << setprecision(12);
                cout << "\nSearch Time (Linear): " << usLinear << " us";
                cout << "\nSearch Time (Binary): " << usBinary << " us\n\n";

                spaceLinear.stop();
                spaceBinary.stop();
                cout << endl;

                delete[] arr;
                continue;
            }

            int start = idx, end = idx;
            while (end < passengerCount && classIndex(arr[end]->seatClass) == target)
                end++;

            mergeSortByID(arr, start, end - 1);

            cout << "\nPassengers in " << cls << " class:\n";
            for (int i = start; i < end; i++) {
                cout << arr[i]->passengerID << " | "
                     << arr[i]->name << " | "
                     << arr[i]->seatRow << arr[i]->seatColumn << " | "
                     << arr[i]->seatClass << " | "
                     << arr[i]->flightID << endl;
            }

            cout << fixed << setprecision(12);
            cout << "\nSearch Time (Linear): " << usLinear << " us";
            cout << "\nSearch Time (Binary): " << usBinary << " us\n\n";
            cout << endl;

            spaceLinear.stop();
            spaceBinary.stop();
            cout << endl;

            delete[] arr;
        }
    }


// NAME SEARCH
    bool startsWith(const string& full, const string& prefix) {
        if (prefix.length() > full.length()) return false;
        return full.compare(0, prefix.length(), prefix) == 0;
    }

    bool nameLess(const string& a, const string& b) {
        return a < b;
    }

    //Divide array recursively
    void mergeByName(Passenger* arr[], int left, int mid, int right) {
        Passenger** temp = new Passenger*[right + 1];

        int i = left;
        int j = mid + 1;
        int k = left;

        while (i <= mid && j <= right) {
            if (nameLess(arr[i]->name, arr[j]->name))
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

    //Combine
    void mergeSortByName(Passenger* arr[], int left, int right) {
        if (left >= right) return;

        int mid = (left + right) / 2;
        mergeSortByName(arr, left, mid);
        mergeSortByName(arr, mid + 1, right);
        mergeByName(arr, left, mid, right);
    }

    int binarySearchNamePrefix(Passenger* arr[], int size, const string& prefix) {
        int left = 0, right = size - 1, pos = -1;

        while (left <= right) {
            int mid = (left + right) / 2;

            if (startsWith(arr[mid]->name, prefix)) {
                pos = mid;
                right = mid - 1;
            }
            else if (arr[mid]->name < prefix) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        return pos;
    }

    int linearSearchNamePrefix(Passenger* arr[], int size, const string& prefix) {
        for (int i = 0; i < size; i++) {
            if (startsWith(arr[i]->name, prefix))
                return i; // first match
        }
        return -1;
    }


    void searchByName() {
        while (true) {
            if (passengerCount == 0) {
                cout << "No passengers loaded.\n";
                return;
            }

            string target;

            cout << "Enter passenger name (prefix, 0 to cancel): ";
            getline(cin >> ws, target);

            if (target == "0") return;

            while (target.empty()) {
                cout << "Input cannot be empty.\n";
                cout << "Enter passenger name (prefix, 0 to cancel): ";
                getline(cin, target);

                if (target == "0") return;
            }

            Passenger** arr = new Passenger*[passengerCount];
            for (int i = 0; i < passengerCount; i++)
                arr[i] = allPassengers[i];

            mergeSortByName(arr, 0, passengerCount - 1);

            SpaceTracker spaceLinear("Search By Name - Linear");
            SpaceTracker spaceBinary("Search By Name - Binary");

            spaceLinear.addVar<int>();
            spaceLinear.addVar<string>();
            // ===== Search-only timing (Linear) =====
            auto startLinear = high_resolution_clock::now();
            int idxL = linearSearchNamePrefix(arr, passengerCount, target);
            auto endLinear = high_resolution_clock::now();

            long long usLinear =
            duration_cast<microseconds>(endLinear - startLinear).count();

            // ===== Search-only timing (Binary) =====
            spaceBinary.addVar<int>();
            spaceBinary.addVar<int>();
            spaceBinary.addVar<int>();

            auto startBinary = high_resolution_clock::now();
            int idxB = binarySearchNamePrefix(arr, passengerCount, target);
            auto endBinary = high_resolution_clock::now();

            long long usBinary =
            duration_cast<microseconds>(endBinary - startBinary).count();

            int idx = idxB; // 用 binary 的 idx 作为输出起点

            if (idx == -1) {
                cout << "No matching passengers found.\n";
                cout << fixed << setprecision(12);
                cout << "\nSearch Time (Linear): " << usLinear << " us";
                cout << "\nSearch Time (Binary): " << usBinary << " us\n\n";

                spaceLinear.stop();
                spaceBinary.stop();
                cout << endl;

                delete[] arr;
                continue;
            }

            int start = idx, end = idx;
            while (end < passengerCount && startsWith(arr[end]->name, target))
                end++;

            mergeSortByFlightClass(arr, start, end - 1);

            cout << "\nMatching passengers:\n";
            for (int i = start; i < end; i++) {
                cout << arr[i]->passengerID << " | "
                     << arr[i]->name << " | "
                     << arr[i]->seatRow << arr[i]->seatColumn << " | "
                     << arr[i]->seatClass << " | "
                     << arr[i]->flightID << endl;
            }

            cout << fixed << setprecision(12);
            cout << "\nSearch Time (Linear): " << usLinear << " us";
            cout << "\nSearch Time (Binary): " << usBinary << " us\n\n";

            spaceLinear.stop();
            spaceBinary.stop();
            cout << endl;

            delete[] arr;
        }
    }



// ID SEARCH
    void mergeByID(Passenger* arr[], int left, int mid, int right) {
        Passenger** temp = new Passenger*[right + 1];

        int i = left;
        int j = mid + 1;
        int k = left;

        while (i <= mid && j <= right) {
            if (arr[i]->passengerID <= arr[j]->passengerID)
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


    void mergeSortByID(Passenger* arr[], int left, int right) {
        if (left >= right) return;

        int mid = (left + right) / 2;
        mergeSortByID(arr, left, mid);
        mergeSortByID(arr, mid + 1, right);
        mergeByID(arr, left, mid, right);
    }


    int binarySearchID(Passenger* arr[], int size, int targetID) {
        int left = 0, right = size - 1, pos = -1;

        while (left <= right) {
            int mid = (left + right) / 2;

            if (arr[mid]->passengerID == targetID) {
                pos = mid;
                right = mid - 1;  // move LEFT
            }
            else if (arr[mid]->passengerID < targetID)
                left = mid + 1;
            else
                right = mid - 1;
        }
        return pos;
    }

    int linearSearchID(Passenger* arr[], int size, int targetID) {
        for (int i = 0; i < size; i++) {
            if (arr[i]->passengerID == targetID)
                return i; // first match
        }
        return -1;
    }

    void searchByID() {
        while (true) {
            if (passengerCount == 0) {
                cout << "No passengers loaded.\n";
                return;
            }

            int targetID;

            while (true) {
                cout << "Enter Passenger ID (0 to cancel): ";
                cin >> targetID;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number.\n";
                    continue;
                }

                if (targetID == 0) return;
                break;
            }

            Passenger** arr = new Passenger*[passengerCount];
            for (int i = 0; i < passengerCount; i++)
                arr[i] = allPassengers[i];

            mergeSortByID(arr, 0, passengerCount - 1);

            SpaceTracker spaceLinear("Search By ID - Linear");
            SpaceTracker spaceBinary("Search By ID - Binary");

            // ===== Search-only timing (Linear) =====
            spaceLinear.addVar<int>();
            spaceLinear.addVar<int>();

            auto startLinear = high_resolution_clock::now();
            int idxL = linearSearchID(arr, passengerCount, targetID);
            auto endLinear = high_resolution_clock::now();

            long long usLinear = duration_cast<microseconds>(endLinear - startLinear).count();
            // ===== Search-only timing (Binary) =====
            spaceBinary.addVar<int>();
            spaceBinary.addVar<int>();
            spaceBinary.addVar<int>();

            auto startBinary = high_resolution_clock::now();
            int idxB = binarySearchID(arr, passengerCount, targetID);
            auto endBinary = high_resolution_clock::now();

            long long usBinary =
            duration_cast<microseconds>(endBinary - startBinary).count();
            int idx = idxB;

            if (idx == -1) {
                cout << "Passenger ID " << targetID << " not found.\n";
                cout << fixed << setprecision(12);
                cout << "\nSearch Time (Linear): " << usLinear << " us";
                cout << "\nSearch Time (Binary): " << usBinary << " us\n\n";

                spaceLinear.stop();
                spaceBinary.stop();
                cout << endl;

                delete[] arr;
                continue;
            }

            cout << "\nPassenger found:\n";
            cout << arr[idx]->passengerID << " | "
                 << arr[idx]->name << " | "
                 << arr[idx]->seatRow << arr[idx]->seatColumn << " | "
                 << arr[idx]->seatClass << " | "
                 << arr[idx]->flightID << endl;

            cout << fixed << setprecision(12);
            cout << "\nSearch Time (Linear): " << usLinear << " us";
            cout << "\nSearch Time (Binary): " << usBinary << " us\n\n";

            spaceLinear.stop();
            spaceBinary.stop();
            cout << endl;

            delete[] arr;
        }
    }


// FID SEARCH

    void mergeByFlightID(Passenger* arr[], int left, int mid, int right) {
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

    void mergeSortByFlightID(Passenger* arr[], int left, int right) {
        if (left >= right) return;
        int mid = (left + right) / 2;
        mergeSortByFlightID(arr, left, mid);
        mergeSortByFlightID(arr, mid + 1, right);
        mergeByFlightID(arr, left, mid, right);
    }

    int binarySearchFirstFlightID(Passenger* arr[], int size, const std::string& flightID) {
        int target = flightIDToNum(flightID);

        int l = 0, r = size - 1, pos = -1;
        while (l <= r) {
            int m = (l + r) / 2;
            int midVal = flightIDToNum(arr[m]->flightID);

            if (midVal == target) {
                pos = m;
                r = m - 1; // 往左找第一笔
            } else if (midVal < target) {
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        return pos;
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

    void searchByFlightID() {
        while (true) {
            if (passengerCount == 0) {
                cout << "No passengers loaded.\n";
                return;
            }

            string flightID;
            cout << "Enter Flight ID (FL001-FL079, 0 to cancel): ";
            cin >> flightID;

            if (flightID == "0") return;

            if (!arr::isValidFlightID_001_079(flightID)) {
                cout << "Invalid Flight ID.\n";
                continue;
            }

            buildAndPrintManifestForFlight(flightID);
            cout << endl;
        }
    }

}
