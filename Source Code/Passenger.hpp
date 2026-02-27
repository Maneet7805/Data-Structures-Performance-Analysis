#ifndef PASSENGER_HPP
#define PASSENGER_HPP

#include <string>

struct Passenger {
    std::string passengerID;
    std::string name;
    int seatRow;
    char seatColumn;
    std::string seatClass;
    bool isEmpty;
    
    Passenger(std::string id = "", std::string n = "", int row = 0, 
              char col = 'A', std::string cls = "", bool empty = false)
        : passengerID(id), name(n), seatRow(row), seatColumn(col), 
          seatClass(cls), isEmpty(empty) {}

    std::string getSeat() const {
        return std::to_string(seatRow) + seatColumn;
    }

    bool operator<(const Passenger& other) const {
        if (seatRow != other.seatRow) return seatRow < other.seatRow;
        return seatColumn < other.seatColumn;
    }
};

struct SimpleNode {
    Passenger data;
    SimpleNode* next;
    
    SimpleNode(const Passenger& p, SimpleNode* n = nullptr) 
        : data(p), next(n) {}
};

#endif