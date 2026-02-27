#include "ArrayReservation.hpp"
#include <cstring>
#include <algorithm>
#include <string>

ArrayReservation::ArrayReservation()
    : size(0), nextID(0), idToIndex(nullptr) {
    
    // Create ID to index mapping
    const int MAX_ID_LIMIT = 200000;
    idToIndex = new int[MAX_ID_LIMIT + 1];
    for (int i = 0; i <= MAX_ID_LIMIT; i++)
        idToIndex[i] = -1;
    
    // Initialize seat grid - USE NEW CONSTANT NAMES
    for (int r = 0; r < ARRAY_RES_ROWS; r++)
        for (int c = 0; c < ARRAY_RES_COLS; c++)
            seatTaken[r][c] = false;
}

ArrayReservation::~ArrayReservation() {
    delete[] idToIndex;
}

void ArrayReservation::init(const Passenger* passengers, int passengerCount, int maxID) {
    size = 0;
    nextID = maxID + 1;
    
    // Clear ID mapping
    const int MAX_ID_LIMIT = 200000;
    for (int i = 0; i <= MAX_ID_LIMIT; i++)
        idToIndex[i] = -1;
    
    // Initialize seats - USE NEW CONSTANT NAMES
    for (int r = 0; r < ARRAY_RES_ROWS; r++)
        for (int c = 0; c < ARRAY_RES_COLS; c++)
            seatTaken[r][c] = false;
    
    // Load passengers
    for (int i = 0; i < passengerCount; i++) {
        const Passenger& p = passengers[i];
        if (!p.isEmpty) {
            arr[size] = p;
            
            // Convert string ID to int for index
            try {
                int id = std::stoi(p.passengerID);
                if (id >= 0 && id <= MAX_ID_LIMIT)
                    idToIndex[id] = size;
            } catch (...) {
                // ID is not numeric, skip index
            }
            
            // Mark seat as taken - USE NEW CONSTANT NAMES
            int r = p.seatRow - 1;
            int c = p.seatColumn - 'A';
            if (r >= 0 && r < ARRAY_RES_ROWS && c >= 0 && c < ARRAY_RES_COLS) {
                seatTaken[r][c] = true;
            }
            
            size++;
        }
    }
}

bool ArrayReservation::reserveNoIndex(const std::string& name, const std::string& cls, Passenger& out) {
    return reserveInternal(name, cls, out, false);
}

bool ArrayReservation::reserveWithIndex(const std::string& name, const std::string& cls, Passenger& out) {
    return reserveInternal(name, cls, out, true);
}

bool ArrayReservation::reserveInternal(const std::string& name, const std::string& cls,
                                       Passenger& out, bool maintainIndex) {
    // USE NEW CONSTANT NAMES
    for (int r = 0; r < ARRAY_RES_ROWS; r++) {
        for (int c = 0; c < ARRAY_RES_COLS; c++) {
            if (!seatTaken[r][c]) {
                Passenger p;
                p.passengerID = std::to_string(nextID++);
                p.name = name;
                p.seatRow = r + 1;
                // USE arrayResColChar (not resColChar)
                p.seatColumn = arrayResColChar(c);
                p.seatClass = cls;
                p.isEmpty = false;

                seatTaken[r][c] = true;
                arr[size] = p;

                if (maintainIndex) {
                    try {
                        int id = std::stoi(p.passengerID);
                        const int MAX_ID_LIMIT = 200000;
                        if (id >= 0 && id <= MAX_ID_LIMIT)
                            idToIndex[id] = size;
                    } catch (...) {
                        // ID is not numeric, skip index
                    }
                }

                size++;
                out = p;
                return true;
            }
        }
    }
    return false;
}

const Passenger* ArrayReservation::searchLinear(const std::string& id) const {
    for (int i = 0; i < size; i++) {
        if (arr[i].passengerID == id && !arr[i].isEmpty)
            return &arr[i];
    }
    return nullptr;
}

const Passenger* ArrayReservation::searchIndexed(const std::string& id) const {
    try {
        int numID = std::stoi(id);
        const int MAX_ID_LIMIT = 200000;
        if (numID < 0 || numID > MAX_ID_LIMIT) return nullptr;
        
        int idx = idToIndex[numID];
        if (idx < 0 || idx >= size) return nullptr;
        return &arr[idx];
    } catch (...) {
        return nullptr;
    }
}

long ArrayReservation::memoryLinear() const {
    return (long)sizeof(arr) + (long)sizeof(seatTaken);
}

long ArrayReservation::memoryIndexed() const {
    const int MAX_ID_LIMIT = 200000;
    return memoryLinear() + (long)(MAX_ID_LIMIT + 1) * (long)sizeof(int);
}