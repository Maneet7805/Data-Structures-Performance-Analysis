#ifndef LINKEDLIST_RESERVATION_HPP
#define LINKEDLIST_RESERVATION_HPP

#include "Passenger.hpp"
#include <string>

const int LL_RES_ROWS = 400;
const int LL_RES_COLS = 26;

inline int llResColIndex(char c) { return c - 'A'; }
inline char llResColChar(int i) { return 'A' + i; }

class LinkedListReservation {
public:
    LinkedListReservation();
    ~LinkedListReservation();

    void init(const Passenger* passengers, int passengerCount, int maxID);

    // Technique 1 (Linear-tech insertion): NO index maintenance
    bool reserveNoIndex(const std::string& name, const std::string& cls, Passenger& out);

    // Technique 2 (Indexed-tech insertion): maintains ID->node table
    bool reserveWithIndex(const std::string& name, const std::string& cls, Passenger& out);

    const Passenger* searchLinear(const std::string& id) const;
    const Passenger* searchIndexed(const std::string& id) const;

    long memoryLinear() const;
    long memoryIndexed() const;

    int getPassengerCount() const { return nodeCount; }
    int getMaxID() const { return nextID - 1; }

private:
    bool reserveInternal(const std::string& name, const std::string& cls,
                         Passenger& out, bool maintainIndex);

private:
    struct Node {
        Passenger data;
        Node* next;
    };

    Node* head;
    int nodeCount;
    bool seatTaken[LL_RES_ROWS][LL_RES_COLS];
    int nextID;

    Node** idToNode; // heap (for numeric IDs)
};

#endif