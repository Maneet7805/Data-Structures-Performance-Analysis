#ifndef SEAT_MANIFEST_HPP
#define SEAT_MANIFEST_HPP

#include "Passenger.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>

// ANSI Color Codes
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"

// Forward declarations for merge sort
void mergeSort(Passenger* arr, int left, int right, bool sortByID = false);
void merge(Passenger* arr, int left, int mid, int right, bool sortByID);
int comparePassengers(const Passenger& a, const Passenger& b, bool sortByID);

class ArrayDisplaySystem {
private:
    Passenger*** seatGrid;  
    Passenger* passengerList;  
    Passenger* sortedByID;     
    int totalRows;
    int seatsPerRow;
    int passengerCount;
    int capacity;
    bool isSorted;
    
    // Helper methods for dynamic arrays
    void resizePassengerList(int newCapacity);
    void resizeSortedArray();
    
public:
    ArrayDisplaySystem(int rows = 400, int seats = 26);
    ~ArrayDisplaySystem();
    
    void loadPassengersFromArray(const Passenger* passengers, int count);
    
    // Core display functions
    void displaySeatingChart() const;
    void displayPassengerManifest() const;
    
    // TECHNIQUE 1: Linear Search (Common)
    Passenger* linearSearchByID(const std::string& passengerID) const;
    
    // TECHNIQUE 2: Binary Search (Array-specific)
    Passenger* binarySearchByID(const std::string& passengerID);
    void sortByPassengerID();  // For binary search preparation
    
    // TECHNIQUE 3: Direct Seat Access (Array-specific)
    Passenger* getSeatDirect(int row, char col) const;
    std::string getSeatStatus(int row, char col) const;
    
    // Common sorting (using merge sort)
    void sortBySeatCommon();
    
    // Performance measurement
    double measureLinearSearchTime(const std::string& id) const;
    double measureBinarySearchTime(const std::string& id);
    double measureSeatingChartTime() const;
    double measureUnsortedManifestTime() const;
    double measureSortedManifestTime() const;
    double measureSortingTime() const;
    double measureDisplayOnlyTime() const;
    
    // Test data generation
    char** getTestIDs(int count, int* actualCount) const;
    
    // Memory usage
    size_t calculateMemoryUsage() const;
    
    // Getters
    int getPassengerCount() const { return passengerCount; }
    int getTotalSeats() const { return totalRows * seatsPerRow; }
    int getMaxRows() const { return totalRows; }
    int getMaxSeatsPerRow() const { return seatsPerRow; }
    bool getIsSorted() const { return isSorted; }
};

// Node for linked list
struct ListNode {
    Passenger data;
    ListNode* next;
    
    ListNode(const Passenger& p, ListNode* n = nullptr) : data(p), next(n) {}
};

// Hash map entry for ID lookup
struct HashEntry {
    std::string key;
    Passenger* value;
    HashEntry* next;
    
    HashEntry(const std::string& k, Passenger* v, HashEntry* n = nullptr) 
        : key(k), value(v), next(n) {}
};

// Simple hash map class
class SimpleHashMap {
private:
    static const int TABLE_SIZE = 10007;
    HashEntry** table;
    
public:
    SimpleHashMap();
    ~SimpleHashMap();
    
    void insert(const std::string& key, Passenger* value);
    Passenger* find(const std::string& key) const;
    void clear();
};

// Linked List-based implementation class
class LinkedListDisplaySystem {
private:
    ListNode* head;
    SimpleHashMap seatMap;   
    SimpleHashMap idMap;      
    int passengerCount;
    int totalRows;
    int seatsPerRow;
    
public:
    LinkedListDisplaySystem(int rows = 400, int seats = 26);
    ~LinkedListDisplaySystem();
    
    void loadPassengersFromArray(const Passenger* passengers, int count);
    
    // Core display functions
    void displaySeatingChart() const;
    void displayPassengerManifest() const;
    
    // TECHNIQUE 1: Linear Search (Common)
    Passenger* linearSearchByID(const std::string& passengerID) const;
    
    // TECHNIQUE 2: Hash Map Lookup (Linked List-specific)
    Passenger* hashSearchByID(const std::string& passengerID) const;
    
    // Common sorting (using merge sort on array copy)
    void sortBySeatCommon();
    
    // Performance measurement
    double measureLinearSearchTime(const std::string& id) const;
    double measureHashSearchTime(const std::string& id) const;
    double measureSeatingChartTime() const;
    double measureUnsortedManifestTime() const;
    double measureSortedManifestTime() const;
    double measureSortingTime() const;
    double measureDisplayOnlyTime() const;
    
    // Test data generation
    char** getTestIDs(int count, int* actualCount) const;
    
    // Memory usage
    size_t calculateMemoryUsage() const;
    
    // Getters
    int getPassengerCount() const { return passengerCount; }
    int getTotalSeats() const { return totalRows * seatsPerRow; }
    int getMaxRows() const { return totalRows; }
    int getMaxSeatsPerRow() const { return seatsPerRow; }
    ListNode* getHead() const { return head; }
};

// Performance comparison utility
class PerformanceComparator {
public:
    // Core comparison functions
    static void compareSeatingChart(const ArrayDisplaySystem& arraySys, 
                                   const LinkedListDisplaySystem& llSys);
    
    static void compareUnsortedManifest(const ArrayDisplaySystem& arraySys, 
                                       const LinkedListDisplaySystem& llSys);
    
    static void compareSortedManifest(const ArrayDisplaySystem& arraySys, 
                                     const LinkedListDisplaySystem& llSys);
    
    static void compareAllOperations(const ArrayDisplaySystem& arraySys, 
                                    const LinkedListDisplaySystem& llSys);
    
    // Helper function for table display
    static void printTableRow(const std::string& label, double arrayVal, double llVal, const std::string& unit = "us");
    static void printMemoryRow(const std::string& label, size_t arrayVal, size_t llVal, bool inKB = false);
};

// Helper functions
Passenger* readPassengersFromCSV(const char* filename, int* passengerCount);
char intToSeatLetter(int num);
std::string getClassFromRow(int row);

#endif 