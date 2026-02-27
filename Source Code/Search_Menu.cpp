#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <iomanip>
#include <cmath>

#include "ArraySearch.hpp"
#include "LinkedListSearch.hpp"
#include "Passenger.hpp"

using namespace std;
using namespace chrono;

// Simple dynamic array for Passenger
class PassengerArray {
private:
    Passenger* data;
    int capacity;
    int count;
    
    void resize(int newCapacity) {
        Passenger* newData = new Passenger[newCapacity];
        for (int i = 0; i < count; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }
    
public:
    PassengerArray(int initialCapacity = 100) : capacity(initialCapacity), count(0) {
        data = new Passenger[capacity];
    }
    
    ~PassengerArray() {
        delete[] data;
    }
    
    void push_back(const Passenger& passenger) {
        if (count >= capacity) {
            resize(capacity * 2);
        }
        data[count++] = passenger;
    }
    
    Passenger& operator[](int index) {
        return data[index];
    }
    
    const Passenger& operator[](int index) const {
        return data[index];
    }
    
    int size() const {
        return count;
    }
    
    bool empty() const {
        return count == 0;
    }
    
    Passenger* getData() {
        return data;
    }
    
    const Passenger* getData() const {
        return data;
    }
};

// Print to console
void logOutput(const string& message, bool newline = true) {
    cout << message;
    if (newline) {
        cout << endl;
    }
}

// Print divider
void printLine() {
    logOutput("---------------------------------------------------------------");
}

// Memory used by a search function
size_t searchOverhead() {
    return sizeof(string) + sizeof(int) + sizeof(Passenger*);
}

// SIMPLE CSV LOADER for Search_Menu only
PassengerArray loadCSVForSearch(const string& filename) {
    PassengerArray passengers;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << "\n";
        return passengers;
    }
    
    string line;
    getline(file, line); // Skip header
    
    while (getline(file, line)) {
        stringstream ss(line);
        string passengerID, name, seatRowStr, seatColumn, seatClass;
        
        if (getline(ss, passengerID, ',') &&
            getline(ss, name, ',') &&
            getline(ss, seatRowStr, ',') &&
            getline(ss, seatColumn, ',') &&
            getline(ss, seatClass, ',')) {
            
            // Skip empty seats
            if (passengerID == "Empty" && name == "Empty") {
                continue;
            }
            
            // Convert seatRow from string to int
            int seatRow;
            try {
                seatRow = stoi(seatRowStr);
            } catch (const exception& e) {
                seatRow = 1;
            }
            
            // Convert seatColumn string to char
            char seatColChar = 'A';
            if (!seatColumn.empty()) {
                seatColChar = seatColumn[0];
            }
            
            // Create Passenger object
            passengers.push_back(Passenger(passengerID, name, seatRow, seatColChar, seatClass, false));
        }
    }
    
    file.close();
    return passengers;
}

void showSearchMenu() {
    cout << "\n===============================================================\n";
    cout << "          FLIGHT SYSTEM PERFORMANCE TEST REPORT                \n";
    cout << "===============================================================\n";

    // -------------------------
    // Setup systems
    // -------------------------
    ArrayPassengerSystem arraySys(10005);
    LinkedListPassengerSystem listSys;

    // -------------------------
    // Load CSV Data
    // -------------------------
    string filename = "Updated_Passenger_List.csv";
    cout << "[INFO] Loading data from: " << filename << endl;

    PassengerArray passengers = loadCSVForSearch(filename);
    
    if (passengers.empty()) {
        cout << "Error: Could not open file or file is empty." << endl;
        cout << "\nPress Enter to return to main menu...";
        cin.ignore();
        return;
    }

    // Load data into systems
    for (int i = 0; i < passengers.size(); i++) {
        const Passenger& p = passengers[i];
        arraySys.addPassenger(p.passengerID, p.name, p.seatRow, p.seatColumn, p.seatClass);
        listSys.addPassenger(p.passengerID, p.name, p.seatRow, p.seatColumn, p.seatClass);
    }

    cout << "[INFO] " << passengers.size() << " records loaded.\n" << endl;

    // -------------------------
    // User Input for Test IDs
    // -------------------------
    const int MAX_INPUTS = 50;
    string userTestIDs[MAX_INPUTS];
    int numTests = 0;

    cout << "[CONFIGURATION] Enter Passenger IDs to test (type 'done')" << endl;
    while (numTests < MAX_INPUTS) {
        cout << "Enter ID #" << (numTests + 1) << ": ";
        string input;
        cin >> input;
        if (input == "done") break;
        userTestIDs[numTests++] = input;
    }

    if (numTests == 0) {
        cout << "\nPress Enter to return to main menu...";
        cin.ignore();
        return;
    }

    // -------------------------
    // PHASE 1: UNSORTED SEARCH
    // -------------------------
    cout << "\nPHASE 1: UNSORTED LINEAR SEARCH" << endl;
    printLine();

    long long totalArrayUn = 0;
    long long totalListUn = 0;

    for (int i = 0; i < numTests; i++) {
        string id = userTestIDs[i];

        auto start = high_resolution_clock::now();
        arraySys.searchPassengerUnsorted(id);
        auto stop = high_resolution_clock::now();
        long long dArr = duration_cast<microseconds>(stop - start).count();
        totalArrayUn += dArr;

        start = high_resolution_clock::now();
        listSys.searchPassengerUnsorted(id);
        stop = high_resolution_clock::now();
        long long dList = duration_cast<microseconds>(stop - start).count();
        totalListUn += dList;

        cout << "Test " << i + 1 << " (" << id << "): Array=" << dArr
            << " us | List=" << dList << " us" << endl;
    }

    // -------------------------
    // Sorting
    // -------------------------
    cout << "\n[ACTION] Sorting data..." << endl;
    arraySys.sortData();
    listSys.sortData();
    cout << "[ACTION] Sorting complete.\n" << endl;

    // -------------------------
    // PHASE 2: SORTED SEARCH
    // -------------------------
    cout << "PHASE 2: SORTED & OPTIMIZED SEARCH" << endl;
    printLine();

    long long totalArrayLinear = 0, totalArrayBinary = 0;
    long long totalListLinear = 0, totalListJump = 0;

    for (int i = 0; i < numTests; i++) {
        string id = userTestIDs[i];

        auto start = high_resolution_clock::now();
        arraySys.searchPassengerSorted(id);
        auto stop = high_resolution_clock::now();
        long long dArrLinear = duration_cast<microseconds>(stop - start).count();
        totalArrayLinear += dArrLinear;

        start = high_resolution_clock::now();
        arraySys.searchPassengerBinary(id);
        stop = high_resolution_clock::now();
        long long dArrBinary = duration_cast<microseconds>(stop - start).count();
        totalArrayBinary += dArrBinary;

        start = high_resolution_clock::now();
        listSys.searchPassengerSorted(id);
        stop = high_resolution_clock::now();
        long long dListLinear = duration_cast<microseconds>(stop - start).count();
        totalListLinear += dListLinear;

        start = high_resolution_clock::now();
        listSys.searchPassengerJump(listSys.size(), id);
        stop = high_resolution_clock::now();
        long long dListJump = duration_cast<microseconds>(stop - start).count();
        totalListJump += dListJump;

        cout << "Test " << i + 1 << ": "
            << "ArrLinear=" << dArrLinear << " | ArrBinary=" << dArrBinary
            << " | ListLinear=" << dListLinear << " | ListJump=" << dListJump << endl;
    }

    // -------------------------
    // PHASE 3: SPACE COMPLEXITY
    // -------------------------
    cout << "\nPHASE 3: SPACE COMPLEXITY" << endl;
    printLine();

    size_t arrayMemory = arraySys.memoryUsage();
    size_t listMemory = listSys.memoryUsage();
    size_t searchMem = searchOverhead();

    cout << "Array Memory Used: " << arrayMemory << " bytes" << endl;
    cout << "Linked List Memory Used: " << listMemory << " bytes" << endl;
    cout << "Search Extra Memory: " << searchMem << " bytes" << endl;

    // -------------------------
    // FINAL SUMMARY
    // -------------------------
    cout << "\nFINAL SUMMARY" << endl;
    printLine();

    if (numTests > 0) {
        cout << "Unsorted Linear Avg (Array): " << (totalArrayUn / numTests) << " us" << endl;
        cout << "Unsorted Linear Avg (List): " << (totalListUn / numTests) << " us" << endl;
        cout << "Sorted Linear Avg (Array): " << (totalArrayLinear / numTests) << " us" << endl;
        cout << "Sorted Linear Avg (List): " << (totalListLinear / numTests) << " us" << endl;
        cout << "Binary Search Avg (Array): " << (totalArrayBinary / numTests) << " us" << endl;
        cout << "Jump Search Avg (List): " << (totalListJump / numTests) << " us" << endl;
    }

    cout << "\nPress Enter to return to main menu...";
    cin.ignore();
    cin.get();
}