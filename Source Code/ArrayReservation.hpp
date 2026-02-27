#ifndef ARRAY_RESERVATION_HPP
#define ARRAY_RESERVATION_HPP

#include "Passenger.hpp"
#include <string>

// Constants directly in the header
const int ARRAY_RES_ROWS = 400;
const int ARRAY_RES_COLS = 26;

inline int arrayResColIndex(char c) { return c - 'A'; }
inline char arrayResColChar(int i) { return 'A' + i; }

class ArrayReservation {
public:
    ArrayReservation();
    ~ArrayReservation();

    void init(const Passenger* passengers, int passengerCount, int maxID);

    // Technique 1 (Linear-tech insertion): NO index maintenance
    bool reserveNoIndex(const std::string& name, const std::string& cls, Passenger& out);

    // Technique 2 (Indexed-tech insertion): maintains ID->index table
    bool reserveWithIndex(const std::string& name, const std::string& cls, Passenger& out);

    const Passenger* searchLinear(const std::string& id) const;
    const Passenger* searchIndexed(const std::string& id) const;

    long memoryLinear() const;
    long memoryIndexed() const;

    int getPassengerCount() const { return size; }
    int getMaxID() const { return nextID - 1; }

private:
    bool reserveInternal(const std::string& name, const std::string& cls,
                         Passenger& out, bool maintainIndex);

private:
    Passenger arr[ARRAY_RES_ROWS * ARRAY_RES_COLS];
    int size;
    bool seatTaken[ARRAY_RES_ROWS][ARRAY_RES_COLS];
    int nextID;

    int* idToIndex; // heap (for numeric IDs)
};

#endif