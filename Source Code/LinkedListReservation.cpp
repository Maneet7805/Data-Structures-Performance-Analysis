#include "LinkedListReservation.hpp"
#include <cstring>
#include <algorithm>
#include <string>

LinkedListReservation::LinkedListReservation()
    : head(nullptr), nodeCount(0), nextID(0), idToNode(nullptr) {
    
    // Create ID to node mapping
    const int MAX_ID_LIMIT = 200000;
    idToNode = new Node*[MAX_ID_LIMIT + 1];
    for (int i = 0; i <= MAX_ID_LIMIT; i++)
        idToNode[i] = nullptr;
    
    // Initialize seat grid
    for (int r = 0; r < LL_RES_ROWS; r++)
        for (int c = 0; c < LL_RES_COLS; c++)
            seatTaken[r][c] = false;
}

LinkedListReservation::~LinkedListReservation() {
    Node* cur = head;
    while (cur) {
        Node* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    delete[] idToNode;
}

void LinkedListReservation::init(const Passenger* passengers, int passengerCount, int maxID) {
    // Clear existing list
    Node* cur = head;
    while (cur) {
        Node* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head = nullptr;
    nodeCount = 0;
    nextID = maxID + 1;
    
    // Clear ID mapping
    const int MAX_ID_LIMIT = 200000;
    for (int i = 0; i <= MAX_ID_LIMIT; i++)
        idToNode[i] = nullptr;
    
    // Initialize seats
    for (int r = 0; r < LL_RES_ROWS; r++)
        for (int c = 0; c < LL_RES_COLS; c++)
            seatTaken[r][c] = false;
    
    // Build list from passengers
    for (int i = 0; i < passengerCount; i++) {
        const Passenger& p = passengers[i];
        if (!p.isEmpty) {
            Node* n = new Node{p, head};
            head = n;
            
            // Mark seat as taken 
            int r = p.seatRow - 1;
            int c = p.seatColumn - 'A';
            if (r >= 0 && r < LL_RES_ROWS && c >= 0 && c < LL_RES_COLS) {
                seatTaken[r][c] = true;
            }
            
            // Add to index if ID is numeric
            try {
                int id = std::stoi(p.passengerID);
                if (id >= 0 && id <= MAX_ID_LIMIT)
                    idToNode[id] = n;
            } catch (...) {
                // ID is not numeric, skip index
            }
            
            nodeCount++;
        }
    }
}

bool LinkedListReservation::reserveNoIndex(const std::string& name, const std::string& cls, Passenger& out) {
    return reserveInternal(name, cls, out, false);
}

bool LinkedListReservation::reserveWithIndex(const std::string& name, const std::string& cls, Passenger& out) {
    return reserveInternal(name, cls, out, true);
}

bool LinkedListReservation::reserveInternal(const std::string& name, const std::string& cls,
                                            Passenger& out, bool maintainIndex) {
    // USE NEW CONSTANT NAMES
    for (int r = 0; r < LL_RES_ROWS; r++) {
        for (int c = 0; c < LL_RES_COLS; c++) {
            if (!seatTaken[r][c]) {
                Passenger p;
                p.passengerID = std::to_string(nextID++);
                p.name = name;
                p.seatRow = r + 1;
                p.seatColumn = llResColChar(c);
                p.seatClass = cls;
                p.isEmpty = false;

                seatTaken[r][c] = true;

                Node* n = new Node{p, head};
                head = n;

                if (maintainIndex) {
                    try {
                        int id = std::stoi(p.passengerID);
                        const int MAX_ID_LIMIT = 200000;
                        if (id >= 0 && id <= MAX_ID_LIMIT)
                            idToNode[id] = n;
                    } catch (...) {
                        // ID is not numeric, skip index
                    }
                }

                nodeCount++;
                out = p;
                return true;
            }
        }
    }
    return false;
}

const Passenger* LinkedListReservation::searchLinear(const std::string& id) const {
    for (Node* n = head; n; n = n->next) {
        if (n->data.passengerID == id && !n->data.isEmpty)
            return &n->data;
    }
    return nullptr;
}

const Passenger* LinkedListReservation::searchIndexed(const std::string& id) const {
    try {
        int numID = std::stoi(id);
        const int MAX_ID_LIMIT = 200000;
        if (numID < 0 || numID > MAX_ID_LIMIT) return nullptr;
        
        Node* node = idToNode[numID];
        return node ? &node->data : nullptr;
    } catch (...) {
        return nullptr;
    }
}

long LinkedListReservation::memoryLinear() const {
    return (long)(nodeCount * sizeof(Node)) + (long)sizeof(seatTaken);
}

long LinkedListReservation::memoryIndexed() const {
    const int MAX_ID_LIMIT = 200000;
    return memoryLinear() + (long)(MAX_ID_LIMIT + 1) * (long)sizeof(Node*);
}