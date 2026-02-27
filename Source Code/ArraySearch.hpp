#ifndef ARRAYSEARCH_HPP
#define ARRAYSEARCH_HPP

#include <iostream>
#include "Passenger.hpp"
using namespace std;

class ArrayPassengerSystem {
private:
    Passenger* dataArray;  
    int capacity;
    int count;

public:
    // Constructor: allocate memory for the array
    ArrayPassengerSystem(int size) {
        capacity = size;
        count = 0;
        dataArray = new Passenger[capacity];
    }

    // Adds a passenger to the array (if not full)
    void addPassenger(const string& id, const string& n, int r, char c, const string& cls) {
        if (count < capacity) {
            dataArray[count] = Passenger(id, n, r, c, cls, false);
            count++;
        }
    }

    // --- UNSORTED LINEAR SEARCH (O(n)) ---
    // Identical algorithm to original SET 2
    void searchPassengerUnsorted(const string& targetID) {
        for (int i = 0; i < count; i++) {
            if (dataArray[i].passengerID == targetID)
                return; // stop when found
        }
    }

    // --- SORTED LINEAR SEARCH WITH EARLY EXIT ---
    // Identical algorithm to original SET 2
    void searchPassengerSorted(const string& targetID) {
        for (int i = 0; i < count; i++) {
            if (dataArray[i].passengerID == targetID) return;
            if (dataArray[i].passengerID > targetID) break;
        }
    }

    // --- BINARY SEARCH (O(log n)) ---
    // Identical algorithm to original SET 2
    void searchPassengerBinary(const string& targetID) {
        int left = 0, right = count - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (dataArray[mid].passengerID == targetID) return;
            else if (dataArray[mid].passengerID < targetID) left = mid + 1;
            else right = mid - 1;
        }
    }

    // --- SELECTION SORT (O(n²)) ---
    // Identical algorithm to original SET 2
    void sortData() {
        for (int i = 0; i < count - 1; i++) {
            int minIndex = i;
            for (int j = i + 1; j < count; j++) {
                if (dataArray[j].passengerID < dataArray[minIndex].passengerID)
                    minIndex = j;
            }
            if (minIndex != i) {
                // Swap passengers 
                Passenger temp = dataArray[i];
                dataArray[i] = dataArray[minIndex];
                dataArray[minIndex] = temp;
            }
        }
        cout << "[ARRAY] Sorted by PassengerID." << endl;
    }

    // Destructor: free dynamic memory 
    ~ArrayPassengerSystem() {
        delete[] dataArray;
    }

    // --- SPACE COMPLEXITY MEASUREMENT ---
    // Same calculation approach, different Passenger size
    size_t memoryUsage() const {
        return sizeof(Passenger) * capacity;
    }
};

#endif 