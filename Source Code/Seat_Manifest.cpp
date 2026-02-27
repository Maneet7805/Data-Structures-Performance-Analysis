#include "Seat_Manifest.hpp"
#include <cstring>
#include <cstdlib>
#include <ctime>

// ==================== SORTING ALGORITHMS ====================

// Comparator: Sorts by Passenger ID (lexicographical) or Seat Position (Row primary, Column secondary)
int comparePassengers(const Passenger& a, const Passenger& b, bool sortByID = false) {
    if (sortByID) {
        return a.passengerID.compare(b.passengerID);
    } else {
        if (a.seatRow != b.seatRow) return a.seatRow - b.seatRow;
        return a.seatColumn - b.seatColumn;
    }
}

// Standard Merge function for Merge Sort
void merge(Passenger* arr, int left, int mid, int right, bool sortByID) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    // Create temporary arrays
    Passenger* leftArr = new Passenger[n1];
    Passenger* rightArr = new Passenger[n2];
    
    // Copy data to temp arrays
    for (int i = 0; i < n1; i++) leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++) rightArr[j] = arr[mid + 1 + j];
    
    // Merge temp arrays back into main array
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (comparePassengers(leftArr[i], rightArr[j], sortByID) <= 0) {
            arr[k] = leftArr[i++];
        } else {
            arr[k] = rightArr[j++];
        }
        k++;
    }
    
    // Copy remaining elements
    while (i < n1) arr[k++] = leftArr[i++];
    while (j < n2) arr[k++] = rightArr[j++];
    
    // Clean up memory
    delete[] leftArr;
    delete[] rightArr;
}

// Recursive Merge Sort implementation
void mergeSort(Passenger* arr, int left, int right, bool sortByID) {
    if (left >= right) return;
    
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid, sortByID);
    mergeSort(arr, mid + 1, right, sortByID);
    merge(arr, left, mid, right, sortByID);
}

// ==================== HELPER FUNCTIONS ====================

// Determine flight class based on row ranges
std::string getClassFromRow(int row) {
    if (row >= 1 && row <= 50) return "Business";
    if (row >= 51 && row <= 150) return "First";
    return "Economy";  
}

// Convert zero-indexed integer to seat character (0 -> 'A')
char intToSeatLetter(int num) {
    return static_cast<char>('A' + num);
}

// ==================== FILE I/O ====================

// Reads CSV data. Performs two passes: 1. Count lines 2. Parse data
Passenger* readPassengersFromCSV(const char* filename, int* passengerCount) {
    std::ifstream file(filename);
    *passengerCount = 0;
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return nullptr;
    }
    
    // Pass 1: Count lines to determine array size
    std::string line;
    int lineCount = -1; // Start at -1 to skip header
    while (std::getline(file, line)) lineCount++;
    file.clear();
    file.seekg(0);
    
    // Allocate exact memory needed
    Passenger* passengers = new Passenger[lineCount];
    std::getline(file, line); // Skip header
    
    int index = 0;
    int emptySeats = 0;
    
    // Pass 2: Parse CSV fields
    while (std::getline(file, line) && index < lineCount) {
        std::stringstream ss(line);
        std::string passengerID, name, seatRowStr, seatColumn, seatClass;
        
        if (std::getline(ss, passengerID, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, seatRowStr, ',') &&
            std::getline(ss, seatColumn, ',') &&
            std::getline(ss, seatClass, ',')) {
            
            // Trim whitespace from all fields
            passengerID.erase(0, passengerID.find_first_not_of(" \t"));
            passengerID.erase(passengerID.find_last_not_of(" \t") + 1);
            
            name.erase(0, name.find_first_not_of(" \t"));
            name.erase(name.find_last_not_of(" \t") + 1);
            
            seatRowStr.erase(0, seatRowStr.find_first_not_of(" \t"));
            seatRowStr.erase(seatRowStr.find_last_not_of(" \t") + 1);
            
            seatColumn.erase(0, seatColumn.find_first_not_of(" \t"));
            seatColumn.erase(seatColumn.find_last_not_of(" \t") + 1);
            
            seatClass.erase(0, seatClass.find_first_not_of(" \t"));
            seatClass.erase(seatClass.find_last_not_of(" \t") + 1);
            
            // Standardize empty seat markers
            bool isEmptySeat = (passengerID == "Empty" && name == "Empty");
            
            if (isEmptySeat) {
                passengerID = "EMPTY";
                name = "Empty Seat";
                emptySeats++;
            }
            
            // Safely convert row string to integer
            int seatRow;
            try {
                seatRow = std::stoi(seatRowStr);
            } catch (const std::exception& e) {
                seatRow = 1;
            }
            
            // Store valid passenger data
            if (!seatColumn.empty()) {
                char seatColChar = seatColumn[0];
                
                // Auto-fill class if missing
                if (seatClass.empty() || seatClass == "Empty") {
                    seatClass = getClassFromRow(seatRow);
                }
                
                passengers[index++] = Passenger(passengerID, name, seatRow, seatColChar, seatClass, isEmptySeat);
            }
        }
    }
    
    file.close();
    *passengerCount = index;
    
    // Output load statistics
    std::cout << "\n=========================================\n";
    std::cout << "Loaded Dataset: " << *passengerCount << " records\n";
    std::cout << "Empty Seats: " << emptySeats << "\n";
    std::cout << "=========================================\n";
    
    return passengers;
}

// ==================== SIMPLE HASH MAP ====================

// Initialize hash table with null pointers
SimpleHashMap::SimpleHashMap() {
    table = new HashEntry*[TABLE_SIZE]();
}

SimpleHashMap::~SimpleHashMap() {
    clear();
    delete[] table;
}

// Insert using polynomial rolling hash and chaining for collisions
void SimpleHashMap::insert(const std::string& key, Passenger* value) {
    size_t hash = 0;
    for (char c : key) {
        hash = hash * 31 + c;
    }
    int index = hash % TABLE_SIZE;
    
    // Add new entry to the front of the linked list at this bucket
    table[index] = new HashEntry(key, value, table[index]);
}

// Find value by key using the same hash logic
Passenger* SimpleHashMap::find(const std::string& key) const {
    size_t hash = 0;
    for (char c : key) {
        hash = hash * 31 + c;
    }
    int index = hash % TABLE_SIZE;
    
    HashEntry* entry = table[index];
    while (entry) {
        if (entry->key == key) {
            return entry->value;
        }
        entry = entry->next;
    }
    return nullptr;
}

// Clean up all dynamically allocated entries
void SimpleHashMap::clear() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashEntry* entry = table[i];
        while (entry) {
            HashEntry* next = entry->next;
            delete entry;
            entry = next;
        }
        table[i] = nullptr;
    }
}

// ==================== ARRAY DISPLAY SYSTEM ====================

ArrayDisplaySystem::ArrayDisplaySystem(int rows, int seats) 
    : totalRows(rows), seatsPerRow(seats), passengerCount(0), capacity(1000), isSorted(false) {
    
    // Allocate 2D grid (array of pointers) for spatial lookups
    seatGrid = new Passenger**[totalRows];
    for (int i = 0; i < totalRows; i++) {
        seatGrid[i] = new Passenger*[seatsPerRow];
        for (int j = 0; j < seatsPerRow; j++) {
            seatGrid[i][j] = nullptr;
        }
    }
    
    // Allocate 1D array for linear storage
    passengerList = new Passenger[capacity];
    sortedByID = nullptr;
}

ArrayDisplaySystem::~ArrayDisplaySystem() {
    for (int i = 0; i < totalRows; i++) {
        delete[] seatGrid[i];
    }
    delete[] seatGrid;
    delete[] passengerList;
    delete[] sortedByID;
}

// Dynamic resizing for the main passenger list
void ArrayDisplaySystem::resizePassengerList(int newCapacity) {
    Passenger* newList = new Passenger[newCapacity];
    for (int i = 0; i < passengerCount; i++) {
        newList[i] = passengerList[i];
    }
    delete[] passengerList;
    passengerList = newList;
    capacity = newCapacity;
}

// Dynamic resizing for the sorted optimization array
void ArrayDisplaySystem::resizeSortedArray() {
    delete[] sortedByID;
    sortedByID = new Passenger[passengerCount];
}

// Populates both the linear list and the 2D grid from input array
void ArrayDisplaySystem::loadPassengersFromArray(const Passenger* passengers, int count) {
    // Reset grid
    for (int i = 0; i < totalRows; i++) {
        for (int j = 0; j < seatsPerRow; j++) {
            seatGrid[i][j] = nullptr;
        }
    }
    
    if (count > capacity) {
        resizePassengerList(count * 2);
    }
    
    passengerCount = 0;
    for (int i = 0; i < count; i++) {
        if (passengers[i].isEmpty) continue;
        
        // Store in 1D list
        passengerList[passengerCount] = passengers[i];
        
        // Map to 2D grid coordinates (0-indexed)
        int rowIndex = passengers[i].seatRow - 1;
        int colIndex = passengers[i].seatColumn - 'A';
        
        if (rowIndex >= 0 && rowIndex < totalRows && 
            colIndex >= 0 && colIndex < seatsPerRow) {
            seatGrid[rowIndex][colIndex] = &passengerList[passengerCount];
        }
        
        passengerCount++;
    }
    
    isSorted = false; // Invalidate sorted cache
}

// O(N) search through unsorted list
Passenger* ArrayDisplaySystem::linearSearchByID(const std::string& passengerID) const {
    for (int i = 0; i < passengerCount; i++) {
        if (passengerList[i].passengerID == passengerID && !passengerList[i].isEmpty) {
            return const_cast<Passenger*>(&passengerList[i]);
        }
    }
    return nullptr;
}

// Prepares the secondary array for binary search
void ArrayDisplaySystem::sortByPassengerID() {
    if (isSorted) return;
    
    if (!sortedByID || passengerCount > capacity) {
        resizeSortedArray();
    }
    
    // Extract non-empty passengers
    int sortedCount = 0;
    for (int i = 0; i < passengerCount; i++) {
        if (!passengerList[i].isEmpty) {
            sortedByID[sortedCount++] = passengerList[i];
        }
    }
    
    mergeSort(sortedByID, 0, sortedCount - 1, true); // Sort by ID
    isSorted = true;
}

// O(log N) search (requires sorting first)
Passenger* ArrayDisplaySystem::binarySearchByID(const std::string& passengerID) {
    if (!isSorted) {
        sortByPassengerID();
    }
    
    int sortedCount = 0;
    while (sortedCount < passengerCount && !sortedByID[sortedCount].isEmpty) {
        sortedCount++;
    }
    
    // Standard Binary Search implementation
    int low = 0;
    int high = sortedCount - 1;
    
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = passengerID.compare(sortedByID[mid].passengerID);
        
        if (cmp == 0) {
            return &sortedByID[mid];
        } else if (cmp < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return nullptr;
}

// Sorts the main list by Seat Row/Col for manifest display
void ArrayDisplaySystem::sortBySeatCommon() {
    if (passengerCount <= 0) return;
    
    int nonEmptyCount = 0;
    for (int i = 0; i < passengerCount; i++) {
        if (!passengerList[i].isEmpty) nonEmptyCount++;
    }
    
    if (nonEmptyCount <= 0) return;
    
    // Sort in temporary array to avoid modifying core list structure
    Passenger* tempArray = new Passenger[nonEmptyCount];
    int index = 0;
    for (int i = 0; i < passengerCount; i++) {
        if (!passengerList[i].isEmpty) {
            tempArray[index++] = passengerList[i];
        }
    }
    
    mergeSort(tempArray, 0, nonEmptyCount - 1, false); // Sort by Seat
    
    delete[] tempArray;
}

// O(1) access via 2D array
Passenger* ArrayDisplaySystem::getSeatDirect(int row, char col) const {
    int rowIndex = row - 1;
    int colIndex = col - 'A';
    
    if (rowIndex >= 0 && rowIndex < totalRows && 
        colIndex >= 0 && colIndex < seatsPerRow) {
        return seatGrid[rowIndex][colIndex];
    }
    return nullptr;
}

std::string ArrayDisplaySystem::getSeatStatus(int row, char col) const {
    Passenger* passenger = getSeatDirect(row, col);
    if (passenger == nullptr) {
        return "Available";
    } else if (passenger->isEmpty) {
        return "Available (Empty)";
    } else {
        return "Reserved by " + passenger->passengerID;
    }
}

// Visualizes the 2D grid
void ArrayDisplaySystem::displaySeatingChart() const {
    // Calculate display bounds based on actual data
    int maxUsedRow = 0;
    int maxUsedCol = 0;
    
    for (int row = 0; row < totalRows; row++) {
        for (int col = 0; col < seatsPerRow; col++) {
            if (seatGrid[row][col] != nullptr) {
                if (row + 1 > maxUsedRow) maxUsedRow = row + 1;
                if (col + 1 > maxUsedCol) maxUsedCol = col + 1;
            }
        }
    }
    
    // Set display limits
    int displayRows = std::min(maxUsedRow + 5, totalRows);
    int displayCols = std::min(maxUsedCol + 3, seatsPerRow);
    
    std::cout << "SEATING CHART (" << COLOR_RED << "X" << COLOR_RESET << " = Reserved, " 
              << COLOR_GREEN << "O" << COLOR_RESET << " = Available)\n";
    std::cout << "===============================================\n\n";
    
    // Print Header
    std::cout << "     ";
    for (int col = 0; col < displayCols; col++) {
        std::cout << char('A' + col) << " ";
    }
    std::cout << "\n";
    
    // Print Rows
    for (int row = 0; row < displayRows; row++) {
        std::cout << std::setw(3) << (row + 1) << ": ";
        
        for (int col = 0; col < displayCols; col++) {
            Passenger* passenger = seatGrid[row][col];
            
            if (passenger == nullptr || passenger->isEmpty) {
                std::cout << COLOR_GREEN << "O " << COLOR_RESET;
            } else {
                std::cout << COLOR_RED << "X " << COLOR_RESET;
            }
        }
        std::cout << "\n";
    }
    
    // Calculate and print statistics
    int reserved = 0;
    int available = 0;
    
    for (int row = 0; row < totalRows; row++) {
        for (int col = 0; col < seatsPerRow; col++) {
            if (seatGrid[row][col] == nullptr || seatGrid[row][col]->isEmpty) {
                available++;
            } else {
                reserved++;
            }
        }
    }
    
    std::cout << "\nSEAT STATISTICS:\n";
    std::cout << "" << COLOR_RED << "Reserved: " << reserved << " seats" << COLOR_RESET << "\n";
    std::cout << "" << COLOR_GREEN << "Available: " << available << " seats" << COLOR_RESET << "\n";
    std::cout << "Total: " << totalRows * seatsPerRow << " seats\n";
    
    std::cout << "\nLEGEND:\n";
    std::cout << COLOR_RED << "X" << COLOR_RESET << " - Reserved Seat\n";
    std::cout << COLOR_GREEN << "O" << COLOR_RESET << " - Available Seat\n";
}

// Displays list of passengers sorted by seat number
void ArrayDisplaySystem::displayPassengerManifest() const {
    if (passengerCount == 0) {
        std::cout << "No passengers in the manifest.\n";
        return;
    }
    
    // Copy to temp array for sorting to preserve original order
    Passenger* sortedList = new Passenger[passengerCount];
    int sortedCount = 0;
    
    for (int i = 0; i < passengerCount; i++) {
        if (!passengerList[i].isEmpty) {
            sortedList[sortedCount++] = passengerList[i];
        }
    }
    
    if (sortedCount > 0) {
        mergeSort(sortedList, 0, sortedCount - 1, false);
    }
    
    std::cout << "PASSENGER MANIFEST (Sorted by Seat)\n";
    std::cout << "====================================\n";
    std::cout << "Total Passengers: " << sortedCount << "\n\n";
    
    std::cout << std::left << std::setw(10) << "Seat" 
              << std::setw(15) << "Passenger ID" 
              << std::setw(25) << "Name" 
              << std::setw(12) << "Class" 
              << "\n";
    std::cout << std::string(62, '-') << "\n";
    
    int count = 0;
    for (int i = 0; i < sortedCount; i++) {
        std::string seat = std::to_string(sortedList[i].seatRow) + sortedList[i].seatColumn;
        std::cout << std::left << std::setw(10) << seat
                  << std::setw(15) << sortedList[i].passengerID
                  << std::setw(25) << sortedList[i].name
                  << std::setw(12) << sortedList[i].seatClass
                  << "\n";
        count++;
        
        // Pagination (limit to 50)
        if (count >= 50 && sortedCount > 50) {
            std::cout << "... and " << (sortedCount - 50) << " more passengers\n";
            break;
        }
    }
    
    delete[] sortedList;
}

// --- Measurement Functions (using high_resolution_clock) ---
double ArrayDisplaySystem::measureLinearSearchTime(const std::string& id) const {
    auto start = std::chrono::high_resolution_clock::now();
    linearSearchByID(id);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count() * 1000000; // Microseconds
}

double ArrayDisplaySystem::measureBinarySearchTime(const std::string& id) {
    auto start = std::chrono::high_resolution_clock::now();
    binarySearchByID(id);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count() * 1000000;
}

double ArrayDisplaySystem::measureSeatingChartTime() const {
    auto start = std::chrono::high_resolution_clock::now();
    
    // Simulate traversal of grid
    int occupiedCount = 0;
    for (int row = 0; row < std::min(50, totalRows); row++) {
        for (int col = 0; col < std::min(10, seatsPerRow); col++) {
            Passenger* passenger = seatGrid[row][col];
            if (passenger != nullptr && !passenger->isEmpty) {
                occupiedCount++;
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count() * 1000000;
}

double ArrayDisplaySystem::measureUnsortedManifestTime() const {
    auto start = std::chrono::high_resolution_clock::now();
    
    // Simulate linear scan
    int nonEmptyCount = 0;
    for (int i = 0; i < passengerCount; i++) {
        if (!passengerList[i].isEmpty) nonEmptyCount++;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count() * 1000000;
}

double ArrayDisplaySystem::measureSortedManifestTime() const {
    auto start = std::chrono::high_resolution_clock::now();
    
    if (passengerCount == 0) return 0.0;
    
    // Include allocation, copy, and sort time
    int nonEmptyCount = 0;
    for (int i = 0; i < passengerCount; i++) {
        if (!passengerList[i].isEmpty) nonEmptyCount++;
    }
    
    if (nonEmptyCount == 0) return 0.0;
    
    Passenger* tempArray = new Passenger[nonEmptyCount];
    int index = 0;
    for (int i = 0; i < passengerCount; i++) {
        if (!passengerList[i].isEmpty) {
            tempArray[index++] = passengerList[i];
        }
    }
    
    mergeSort(tempArray, 0, nonEmptyCount - 1, false);
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    
    delete[] tempArray;
    return duration.count() * 1000000;
}

double ArrayDisplaySystem::measureSortingTime() const {
    return measureSortedManifestTime(); 
}

double ArrayDisplaySystem::measureDisplayOnlyTime() const {
    auto start = std::chrono::high_resolution_clock::now();
    
    if (passengerCount == 0) return 0.0;
    
    // Simulate string formatting cost
    for (int i = 0; i < passengerCount; i++) {
        if (!passengerList[i].isEmpty) {
            std::string seat = std::to_string(passengerList[i].seatRow) + passengerList[i].seatColumn;
            std::string dummy = seat + passengerList[i].passengerID + passengerList[i].name + passengerList[i].seatClass;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count() * 1000000;
}

// Generate test data for benchmarks
char** ArrayDisplaySystem::getTestIDs(int count, int* actualCount) const {
    char** testIDs = nullptr;
    *actualCount = 0;
    
    if (passengerCount == 0 || count <= 0) return testIDs;
    
    int nonEmptyCount = 0;
    for (int i = 0; i < passengerCount; i++) {
        if (!passengerList[i].isEmpty) nonEmptyCount++;
    }
    
    // Allocate array of strings
    if (nonEmptyCount <= count) {
        testIDs = new char*[nonEmptyCount];
        *actualCount = nonEmptyCount;
        int index = 0;
        for (int i = 0; i < passengerCount && index < nonEmptyCount; i++) {
            if (!passengerList[i].isEmpty) {
                testIDs[index] = new char[passengerList[i].passengerID.length() + 1];
                strcpy(testIDs[index], passengerList[i].passengerID.c_str());
                index++;
            }
        }
        return testIDs;
    }
    
    // Return a subset
    testIDs = new char*[count];
    *actualCount = count;
    int selected = 0;
    
    for (int i = 0; i < passengerCount && selected < count; i++) {
        if (!passengerList[i].isEmpty) {
            testIDs[selected] = new char[passengerList[i].passengerID.length() + 1];
            strcpy(testIDs[selected], passengerList[i].passengerID.c_str());
            selected++;
        }
    }
    
    return testIDs;
}

size_t ArrayDisplaySystem::calculateMemoryUsage() const {
    size_t memory = 0;
    // 1. 1D Array storage
    memory += sizeof(Passenger) * capacity;
    
    // 2. 2D Grid pointers
    memory += sizeof(Passenger***) + sizeof(Passenger**) * totalRows;
    for (int i = 0; i < totalRows; i++) {
        memory += sizeof(Passenger*) * seatsPerRow;
    }
    
    // 3. Sorted array storage
    if (sortedByID) {
        memory += sizeof(Passenger) * capacity;
    }
    
    return memory;
}

// ==================== LINKED LIST DISPLAY SYSTEM ====================
// Uses Linked List for storage + Hash Maps for O(1) lookups
LinkedListDisplaySystem::LinkedListDisplaySystem(int rows, int seats) 
    : head(nullptr), passengerCount(0), totalRows(rows), seatsPerRow(seats) {}

LinkedListDisplaySystem::~LinkedListDisplaySystem() {
    ListNode* current = head;
    while (current) {
        ListNode* next = current->next;
        delete current;
        current = next;
    }
}

void LinkedListDisplaySystem::loadPassengersFromArray(const Passenger* passengers, int count) {
    // Clear list
    ListNode* current = head;
    while (current) {
        ListNode* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    passengerCount = 0;
    
    // Clear lookup maps
    seatMap.clear();
    idMap.clear();
    
    // Build list in reverse (so inserting at head maintains order)
    for (int i = count - 1; i >= 0; i--) {
        if (passengers[i].isEmpty) continue;
        
        ListNode* newNode = new ListNode(passengers[i], head);
        head = newNode;
        passengerCount++;
        
        // Populate hash maps
        std::string seatKey = std::to_string(passengers[i].seatRow) + passengers[i].seatColumn;
        seatMap.insert(seatKey, &newNode->data);
        idMap.insert(passengers[i].passengerID, &newNode->data);
    }
}

// O(N) traversal search
Passenger* LinkedListDisplaySystem::linearSearchByID(const std::string& passengerID) const {
    ListNode* current = head;
    while (current) {
        if (current->data.passengerID == passengerID && !current->data.isEmpty) {
            return &current->data;
        }
        current = current->next;
    }
    return nullptr;
}

// O(1) average case search via Hash Map
Passenger* LinkedListDisplaySystem::hashSearchByID(const std::string& passengerID) const {
    return idMap.find(passengerID);
}

void LinkedListDisplaySystem::sortBySeatCommon() {
    if (!head || passengerCount <= 0) return;
    
    // Must copy to array to sort efficiently
    Passenger* tempArray = new Passenger[passengerCount];
    ListNode* current = head;
    int index = 0;
    
    while (current) {
        if (!current->data.isEmpty) {
            tempArray[index++] = current->data;
        }
        current = current->next;
    }
    
    mergeSort(tempArray, 0, index - 1, false);
    
    delete[] tempArray;
}

void LinkedListDisplaySystem::displaySeatingChart() const {
    if (!head) {
        std::cout << "No passengers in the system.\n";
        return;
    }
    
    // 1. Determine grid dimensions by traversing list
    int maxRow = 0;
    int maxCol = 0;
    
    ListNode* current = head;
    while (current) {
        if (current->data.seatRow > maxRow) maxRow = current->data.seatRow;
        int colIndex = current->data.seatColumn - 'A';
        if (colIndex > maxCol) maxCol = colIndex;
        current = current->next;
    }
    
    int displayRows = std::min(maxRow + 5, totalRows);
    int displayCols = std::min(maxCol + 3, seatsPerRow);
    
    // 2. Allocate temp 2D grid (overhead for linked list approach)
    Passenger*** tempGrid = new Passenger**[displayRows];
    for (int i = 0; i < displayRows; i++) {
        tempGrid[i] = new Passenger*[displayCols];
        for (int j = 0; j < displayCols; j++) {
            tempGrid[i][j] = nullptr;
        }
    }
    
    // 3. Map list nodes to temp grid
    current = head;
    while (current) {
        int rowIndex = current->data.seatRow - 1;
        int colIndex = current->data.seatColumn - 'A';
        
        if (rowIndex >= 0 && rowIndex < displayRows && 
            colIndex >= 0 && colIndex < displayCols) {
            tempGrid[rowIndex][colIndex] = &current->data;
        }
        current = current->next;
    }
    
    // 4. Render Grid
    std::cout << "SEATING CHART (" << COLOR_RED << "X" << COLOR_RESET << " = Reserved, " 
              << COLOR_GREEN << "O" << COLOR_RESET << " = Available)\n";
    std::cout << "===============================================\n\n";
    
    std::cout << "     ";
    for (int col = 0; col < displayCols; col++) {
        std::cout << char('A' + col) << " ";
    }
    std::cout << "\n";
    
    for (int row = 0; row < displayRows; row++) {
        std::cout << std::setw(3) << (row + 1) << ": ";
        
        for (int col = 0; col < displayCols; col++) {
            Passenger* passenger = tempGrid[row][col];
            
            if (passenger == nullptr || passenger->isEmpty) {
                std::cout << COLOR_GREEN << "O " << COLOR_RESET;
            } else {
                std::cout << COLOR_RED << "X " << COLOR_RESET;
            }
        }
        std::cout << "\n";
    }
    
    // 5. Cleanup temp grid
    for (int i = 0; i < displayRows; i++) {
        delete[] tempGrid[i];
    }
    delete[] tempGrid;
    
    std::cout << "\nSEAT STATISTICS:\n";
    std::cout << "Total Passengers: " << passengerCount << "\n";
    std::cout << "Total Seats: " << totalRows * seatsPerRow << "\n";
    
    std::cout << "\nLEGEND:\n";
    std::cout << COLOR_RED << "X" << COLOR_RESET << " - Reserved Seat\n";
    std::cout << COLOR_GREEN << "O" << COLOR_RESET << " - Available Seat\n";
}

void LinkedListDisplaySystem::displayPassengerManifest() const {
    if (!head) {
        std::cout << "No passengers in the manifest.\n";
        return;
    }
    
    int nonEmptyCount = 0;
    ListNode* current = head;
    while (current) {
        if (!current->data.isEmpty) nonEmptyCount++;
        current = current->next;
    }
    
    if (nonEmptyCount == 0) {
        std::cout << "No passengers in the manifest.\n";
        return;
    }
    
    // Convert List to Array -> Sort -> Display
    Passenger* tempArray = new Passenger[nonEmptyCount];
    current = head;
    int index = 0;
    
    while (current && index < nonEmptyCount) {
        if (!current->data.isEmpty) {
            tempArray[index++] = current->data;
        }
        current = current->next;
    }
    
    mergeSort(tempArray, 0, nonEmptyCount - 1, false);
    
    std::cout << "PASSENGER MANIFEST (Sorted by Seat)\n";
    std::cout << "====================================\n";
    std::cout << "Total Passengers: " << nonEmptyCount << "\n\n";
    
    std::cout << std::left << std::setw(10) << "Seat" 
              << std::setw(15) << "Passenger ID" 
              << std::setw(25) << "Name" 
              << std::setw(12) << "Class" 
              << "\n";
    std::cout << std::string(62, '-') << "\n";
    
    int count = 0;
    for (int i = 0; i < nonEmptyCount; i++) {
        std::string seat = std::to_string(tempArray[i].seatRow) + tempArray[i].seatColumn;
        std::cout << std::left << std::setw(10) << seat
                  << std::setw(15) << tempArray[i].passengerID
                  << std::setw(25) << tempArray[i].name
                  << std::setw(12) << tempArray[i].seatClass
                  << "\n";
        count++;
        
        if (count >= 50 && nonEmptyCount > 50) {
            std::cout << "... and " << (nonEmptyCount - 50) << " more passengers\n";
            break;
        }
    }
    
    delete[] tempArray;
}

// --- Measurement Functions ---
double LinkedListDisplaySystem::measureLinearSearchTime(const std::string& id) const {
    auto start = std::chrono::high_resolution_clock::now();
    linearSearchByID(id);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count() * 1000000;
}

double LinkedListDisplaySystem::measureHashSearchTime(const std::string& id) const {
    auto start = std::chrono::high_resolution_clock::now();
    hashSearchByID(id);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count() * 1000000;
}

double LinkedListDisplaySystem::measureSeatingChartTime() const {
    auto start = std::chrono::high_resolution_clock::now();
    
    if (!head) return 0.0;
    
    // Simulate list traversal (expensive compared to Array Grid)
    ListNode* current = head;
    int count = 0;
    while (current && count < 1000) {
        count++;
        current = current->next;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count() * 1000000;
}

double LinkedListDisplaySystem::measureUnsortedManifestTime() const {
    auto start = std::chrono::high_resolution_clock::now();
    
    if (!head) return 0.0;
    
    int nonEmptyCount = 0;
    ListNode* current = head;
    while (current) {
        if (!current->data.isEmpty) nonEmptyCount++;
        current = current->next;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count() * 1000000;
}

double LinkedListDisplaySystem::measureSortedManifestTime() const {
    return measureSortingTime();
}

double LinkedListDisplaySystem::measureSortingTime() const {
    auto start = std::chrono::high_resolution_clock::now();
    
    if (!head) return 0.0;
    
    // Count
    int nonEmptyCount = 0;
    ListNode* current = head;
    while (current) {
        if (!current->data.isEmpty) nonEmptyCount++;
        current = current->next;
    }
    
    if (nonEmptyCount == 0) return 0.0;
    
    // Copy to Array
    Passenger* tempArray = new Passenger[nonEmptyCount];
    current = head;
    int index = 0;
    
    while (current && index < nonEmptyCount) {
        if (!current->data.isEmpty) {
            tempArray[index++] = current->data;
        }
        current = current->next;
    }
    
    // Sort
    mergeSort(tempArray, 0, nonEmptyCount - 1, false);
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    
    delete[] tempArray;
    return duration.count() * 1000000;
}

double LinkedListDisplaySystem::measureDisplayOnlyTime() const {
    auto start = std::chrono::high_resolution_clock::now();
    
    if (!head) return 0.0;
    
    ListNode* current = head;
    while (current) {
        if (!current->data.isEmpty) {
            std::string seat = std::to_string(current->data.seatRow) + current->data.seatColumn;
            std::string dummy = seat + current->data.passengerID + current->data.name + current->data.seatClass;
        }
        current = current->next;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    return duration.count() * 1000000;
}

// Generate Test IDs
char** LinkedListDisplaySystem::getTestIDs(int count, int* actualCount) const {
    char** testIDs = nullptr;
    *actualCount = 0;
    
    if (passengerCount == 0 || count <= 0) return testIDs;
    
    int nonEmptyCount = 0;
    ListNode* current = head;
    while (current) {
        if (!current->data.isEmpty) nonEmptyCount++;
        current = current->next;
    }
    
    if (nonEmptyCount <= count) {
        testIDs = new char*[nonEmptyCount];
        *actualCount = nonEmptyCount;
        current = head;
        int index = 0;
        while (current && index < nonEmptyCount) {
            if (!current->data.isEmpty) {
                testIDs[index] = new char[current->data.passengerID.length() + 1];
                strcpy(testIDs[index], current->data.passengerID.c_str());
                index++;
            }
            current = current->next;
        }
        return testIDs;
    }
    
    // Subset
    testIDs = new char*[count];
    *actualCount = count;
    current = head;
    int selected = 0;
    
    while (current && selected < count) {
        if (!current->data.isEmpty) {
            testIDs[selected] = new char[current->data.passengerID.length() + 1];
            strcpy(testIDs[selected], current->data.passengerID.c_str());
            selected++;
        }
        current = current->next;
    }
    
    return testIDs;
}

size_t LinkedListDisplaySystem::calculateMemoryUsage() const {
    size_t memory = 0;
    
    // 1. Linked List Nodes
    ListNode* current = head;
    while (current) {
        memory += sizeof(ListNode);
        current = current->next;
    }
    
    // 2. Hash Maps (Seat + ID maps)
    memory += sizeof(SimpleHashMap) * 2;
    
    return memory;
}

// ==================== PERFORMANCE COMPARATOR ====================

// Formats comparison rows for time data
void PerformanceComparator::printTableRow(const std::string& label, double arrayVal, double llVal, const std::string& unit) {
    std::cout << "| " << std::left << std::setw(30) << label 
              << " | " << std::right << std::setw(15) << std::fixed << std::setprecision(2) << arrayVal 
              << " " << unit
              << " | " << std::right << std::setw(15) << llVal 
              << " " << unit << " |\n";
}

// Formats comparison rows for memory data
void PerformanceComparator::printMemoryRow(const std::string& label, size_t arrayVal, size_t llVal, bool inKB) {
    if (inKB) {
        std::cout << "| " << std::left << std::setw(30) << label 
                  << " | " << std::right << std::setw(15) << std::fixed << std::setprecision(2) << (arrayVal / 1024.0)
                  << " | " << std::right << std::setw(15) << (llVal / 1024.0) << " |\n";
    } else {
        std::cout << "| " << std::left << std::setw(30) << label 
                  << " | " << std::right << std::setw(15) << arrayVal 
                  << " | " << std::right << std::setw(15) << llVal << " |\n";
    }
}

// Benchmark: Seating Chart Construction (Array Grid vs Linked List Traversal)
void PerformanceComparator::compareSeatingChart(const ArrayDisplaySystem& arraySys, 
                                               const LinkedListDisplaySystem& llSys) {
    std::cout << "PERFORMANCE COMPARISON: SEATING CHART DISPLAY\n";
    std::cout << "==============================================\n\n";
    
    std::cout << "Testing with " << arraySys.getPassengerCount() << " passengers\n\n";
    
    double arrayTime = arraySys.measureSeatingChartTime();
    double llTime = llSys.measureSeatingChartTime();
    size_t arrayMem = arraySys.calculateMemoryUsage();
    size_t llMem = llSys.calculateMemoryUsage();
    
    std::cout << std::string(73, '=') << "\n";
    std::cout << "| " << std::left << std::setw(30) << "METRIC" 
              << " | " << std::left << std::setw(15) << "ARRAY SYSTEM"
              << " | " << std::left << std::setw(15) << "LINKED LIST SYSTEM" << " |\n";
    std::cout << std::string(73, '=') << "\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Time" 
              << " | " << std::right << std::setw(12) << std::fixed << std::setprecision(2) << arrayTime 
              << " us"
              << " | " << std::right << std::setw(15) << llTime << " us" << " |\n";
    
    std::cout << std::string(73, '-') << "\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Total Memory (bytes)" 
              << " | " << std::right << std::setw(15) << arrayMem 
              << " | " << std::right << std::setw(18) << llMem << " |\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Memory (KB)" 
              << " | " << std::right << std::setw(15) << std::fixed << std::setprecision(2) << (arrayMem / 1024.0)
              << " | " << std::right << std::setw(18) << (llMem / 1024.0) << " |\n";
    
    int arrayPassengerCount = arraySys.getPassengerCount();
    int llPassengerCount = llSys.getPassengerCount();
    
    std::cout << "| " << std::left << std::setw(30) << "Memory per Passenger" 
              << " | " << std::right << std::setw(15) 
              << (arrayPassengerCount > 0 ? arrayMem / arrayPassengerCount : 0)
              << " | " << std::right << std::setw(18) 
              << (llPassengerCount > 0 ? llMem / llPassengerCount : 0) << " |\n";
    
    std::cout << std::string(73, '=') << "\n";
}

// Benchmark: Iterating through Unsorted data
void PerformanceComparator::compareUnsortedManifest(const ArrayDisplaySystem& arraySys, 
                                                   const LinkedListDisplaySystem& llSys) {
    std::cout << "PERFORMANCE COMPARISON: UNSORTED MANIFEST DISPLAY\n";
    std::cout << "==================================================\n\n";
    
    std::cout << "Testing with " << arraySys.getPassengerCount() << " passengers\n\n";
    
    double arrayTime = arraySys.measureUnsortedManifestTime();
    double llTime = llSys.measureUnsortedManifestTime();
    size_t arrayMem = arraySys.calculateMemoryUsage();
    size_t llMem = llSys.calculateMemoryUsage();
    
    std::cout << std::string(73, '=') << "\n";
    std::cout << "| " << std::left << std::setw(30) << "METRIC" 
              << " | " << std::left << std::setw(15) << "ARRAY SYSTEM"
              << " | " << std::left << std::setw(15) << "LINKED LIST SYSTEM" << " |\n";
    std::cout << std::string(73, '=') << "\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Display Time" 
              << " | " << std::right << std::setw(12) << std::fixed << std::setprecision(2) << arrayTime 
              << " us"
              << " | " << std::right << std::setw(15) << llTime << " us" << " |\n";
    
    std::cout << std::string(73, '-') << "\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Total Memory (bytes)" 
              << " | " << std::right << std::setw(15) << arrayMem 
              << " | " << std::right << std::setw(18) << llMem << " |\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Memory (KB)" 
              << " | " << std::right << std::setw(15) << std::fixed << std::setprecision(2) << (arrayMem / 1024.0)
              << " | " << std::right << std::setw(18) << (llMem / 1024.0) << " |\n";
    
    int arrayPassengerCount = arraySys.getPassengerCount();
    int llPassengerCount = llSys.getPassengerCount();
    
    std::cout << "| " << std::left << std::setw(30) << "Memory per Passenger" 
              << " | " << std::right << std::setw(15) 
              << (arrayPassengerCount > 0 ? arrayMem / arrayPassengerCount : 0)
              << " | " << std::right << std::setw(18) 
              << (llPassengerCount > 0 ? llMem / llPassengerCount : 0) << " |\n";
    
    std::cout << std::string(73, '=') << "\n";
}

// Benchmark: Sorting + Displaying data
void PerformanceComparator::compareSortedManifest(const ArrayDisplaySystem& arraySys, 
                                                 const LinkedListDisplaySystem& llSys) {
    std::cout << "PERFORMANCE COMPARISON: SORTED MANIFEST DISPLAY\n";
    std::cout << "================================================\n\n";
    
    std::cout << "Testing with " << arraySys.getPassengerCount() << " passengers\n";
    std::cout << "Using merge sort algorithm\n\n";
    
    // Decompose timing components
    double arrayTotalTime = arraySys.measureSortedManifestTime();
    double arraySortTime = arraySys.measureSortingTime();
    double arrayDisplayTime = arraySys.measureDisplayOnlyTime();
    
    double llTotalTime = llSys.measureSortedManifestTime();
    double llSortTime = llSys.measureSortingTime();
    double llDisplayTime = llSys.measureDisplayOnlyTime();
    
    if (arrayDisplayTime < 0) arrayDisplayTime = 0;
    if (llDisplayTime < 0) llDisplayTime = 0;
    
    size_t arrayMem = arraySys.calculateMemoryUsage();
    size_t llMem = llSys.calculateMemoryUsage();
    
    std::cout << std::string(73, '=') << "\n";
    std::cout << "| " << std::left << std::setw(30) << "METRIC" 
              << " | " << std::left << std::setw(15) << "ARRAY SYSTEM"
              << " | " << std::left << std::setw(15) << "LINKED LIST SYSTEM" << " |\n";
    std::cout << std::string(73, '=') << "\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Total Time" 
              << " | " << std::right << std::setw(12) << std::fixed << std::setprecision(2) << arrayTotalTime 
              << " us"
              << " | " << std::right << std::setw(15) << llTotalTime << " us" << " |\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Sorting Time" 
              << " | " << std::right << std::setw(12) << arraySortTime 
              << " us"
              << " | " << std::right << std::setw(15) << llSortTime << " us" << " |\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Display Time (only)" 
              << " | " << std::right << std::setw(12) << arrayDisplayTime 
              << " us"
              << " | " << std::right << std::setw(15) << llDisplayTime << " us" << " |\n";
    
    std::cout << std::string(73, '-') << "\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Total Memory (bytes)" 
              << " | " << std::right << std::setw(15) << arrayMem 
              << " | " << std::right << std::setw(18) << llMem << " |\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Memory (KB)" 
              << " | " << std::right << std::setw(15) << std::fixed << std::setprecision(2) << (arrayMem / 1024.0)
              << " | " << std::right << std::setw(18) << (llMem / 1024.0) << " |\n";
    
    int arrayPassengerCount = arraySys.getPassengerCount();
    int llPassengerCount = llSys.getPassengerCount();
    
    std::cout << "| " << std::left << std::setw(30) << "Memory per Passenger" 
              << " | " << std::right << std::setw(15) 
              << (arrayPassengerCount > 0 ? arrayMem / arrayPassengerCount : 0)
              << " | " << std::right << std::setw(18) 
              << (llPassengerCount > 0 ? llMem / llPassengerCount : 0) << " |\n";
    
    std::cout << std::string(73, '=') << "\n";
}

// Summary Report: Aggregates all benchmarks
void PerformanceComparator::compareAllOperations(const ArrayDisplaySystem& arraySys, 
                                                const LinkedListDisplaySystem& llSys) {
    std::cout << "COMPREHENSIVE PERFORMANCE COMPARISON: ALL OPERATIONS\n";
    std::cout << "====================================================\n\n";
    
    std::cout << "Testing with " << arraySys.getPassengerCount() << " passengers\n\n";
    
    double arraySeatingTime = arraySys.measureSeatingChartTime();
    double llSeatingTime = llSys.measureSeatingChartTime();
    
    double arrayUnsortedTime = arraySys.measureUnsortedManifestTime();
    double llUnsortedTime = llSys.measureUnsortedManifestTime();
    
    double arraySortedTime = arraySys.measureSortedManifestTime();
    double llSortedTime = llSys.measureSortedManifestTime();
    
    double arraySortTime = arraySys.measureSortingTime();
    double llSortTime = llSys.measureSortingTime();
    
    double arrayDisplayTime = arraySys.measureDisplayOnlyTime();
    double llDisplayTime = llSys.measureDisplayOnlyTime();
    
    size_t arrayMem = arraySys.calculateMemoryUsage();
    size_t llMem = llSys.calculateMemoryUsage();
    
    double totalArrayTime = arraySeatingTime + arrayUnsortedTime + arraySortedTime;
    double totalLlTime = llSeatingTime + llUnsortedTime + llSortedTime;
    
    std::cout << std::string(73, '=') << "\n";
    std::cout << "| " << std::left << std::setw(30) << "OPERATION" 
              << " | " << std::left << std::setw(15) << "ARRAY (us)"
              << " | " << std::left << std::setw(18) << "LINKED LIST (us)" << " |\n";
    std::cout << std::string(73, '=') << "\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Seating Chart Display" 
              << " | " << std::right << std::setw(15) << std::fixed << std::setprecision(2) << arraySeatingTime
              << " | " << std::right << std::setw(18) << llSeatingTime << " |\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Unsorted Manifest" 
              << " | " << std::right << std::setw(15) << arrayUnsortedTime
              << " | " << std::right << std::setw(18) << llUnsortedTime << " |\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Sorted Manifest" 
              << " | " << std::right << std::setw(15) << arraySortedTime
              << " | " << std::right << std::setw(18) << llSortedTime << " |\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Sorting Only" 
              << " | " << std::right << std::setw(15) << arraySortTime
              << " | " << std::right << std::setw(18) << llSortTime << " |\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Display Only" 
              << " | " << std::right << std::setw(15) << arrayDisplayTime
              << " | " << std::right << std::setw(18) << llDisplayTime << " |\n";
    
    std::cout << std::string(73, '-') << "\n";
    
    std::cout << "| " << std::left << std::setw(30) << "TOTAL TIME" 
              << " | " << std::right << std::setw(15) << totalArrayTime
              << " | " << std::right << std::setw(18) << totalLlTime << " |\n";
    
    std::cout << std::string(73, '=') << "\n\n";
    
    std::cout << std::string(73, '=') << "\n";
    std::cout << "| " << std::left << std::setw(30) << "MEMORY USAGE" 
              << " | " << std::left << std::setw(15) << "ARRAY"
              << " | " << std::left << std::setw(18) << "LINKED LIST" << " |\n";
    std::cout << std::string(73, '=') << "\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Total (bytes)" 
              << " | " << std::right << std::setw(15) << arrayMem 
              << " | " << std::right << std::setw(18) << llMem << " |\n";
    
    std::cout << "| " << std::left << std::setw(30) << "Total (KB)" 
              << " | " << std::right << std::setw(15) << std::fixed << std::setprecision(2) << (arrayMem / 1024.0)
              << " | " << std::right << std::setw(18) << (llMem / 1024.0) << " |\n";
    
    int arrayPassengerCount = arraySys.getPassengerCount();
    int llPassengerCount = llSys.getPassengerCount();
    
    std::cout << "| " << std::left << std::setw(30) << "Per Passenger (bytes)" 
              << " | " << std::right << std::setw(15) 
              << (arrayPassengerCount > 0 ? arrayMem / arrayPassengerCount : 0)
              << " | " << std::right << std::setw(18) 
              << (llPassengerCount > 0 ? llMem / llPassengerCount : 0) << " |\n";
    
    std::cout << std::string(73, '=') << "\n";
}