#ifndef CANCELLATION_HPP
#define CANCELLATION_HPP
#include "Passenger.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/*
=======================================
        ARRAY CANCELLATION
=======================================
*/

class ArrayCancellation {
private:
    Passenger* arr;
    int size;
    int capacity;

public:
    ArrayCancellation(int cap) {
        capacity = cap;
        size = 0;
        arr = new Passenger[capacity];
    }

    ~ArrayCancellation() {
        delete[] arr;
    }

    void loadFromFile(const char* filename) {
        std::ifstream file(filename);
        std::string line;
        std::getline(file, line); 

        while (std::getline(file, line)) {
            Passenger p;
            std::string token;
            std::stringstream ss(line);

            std::getline(ss, p.passengerID, ',');
            std::getline(ss, p.name, ',');
            
            std::getline(ss, token, ','); 
            p.seatRow = std::stoi(token);
            
            std::getline(ss, token, ','); 
            p.seatColumn = token[0];
            
            std::getline(ss, p.seatClass, ',');

            p.passengerID.erase(0, p.passengerID.find_first_not_of(" \t"));
            p.passengerID.erase(p.passengerID.find_last_not_of(" \t") + 1);
            
            p.name.erase(0, p.name.find_first_not_of(" \t"));
            p.name.erase(p.name.find_last_not_of(" \t") + 1);
            
            p.seatClass.erase(0, p.seatClass.find_first_not_of(" \t"));
            p.seatClass.erase(p.seatClass.find_last_not_of(" \t") + 1);
            
            // Check for empty seat
            p.isEmpty = (p.passengerID == "Empty" || p.passengerID == "EMPTY");

            if (size < capacity) {
                arr[size++] = p;
            }
        }
        file.close();
    }

    int linearSearch(const std::string& id) {
        for (int i = 0; i < size; i++) {
            if (arr[i].passengerID == id && !arr[i].isEmpty) {
                return i;
            }
        }
        return -1;
    }

    int binarySearch(const std::string& id) {
        int low = 0, high = size - 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (arr[mid].passengerID == id && !arr[mid].isEmpty) return mid;
            else if (id < arr[mid].passengerID) high = mid - 1;
            else low = mid + 1;
        }
        return -1;
    }

    bool cancelByLinear(const std::string& id) {
        int index = linearSearch(id);
        if (index == -1) return false;

        for (int i = index; i < size - 1; i++)
            arr[i] = arr[i + 1];

        size--;
        return true;
    }

    bool cancelByBinary(const std::string& id) {
        int index = binarySearch(id);
        if (index == -1) return false;

        for (int i = index; i < size - 1; i++)
            arr[i] = arr[i + 1];

        size--;
        return true;
    }

    void sortByID() {
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (arr[j].passengerID > arr[j + 1].passengerID) {
                    Passenger temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }

    int getSize() const {
        return size;
    }
    
    int getCapacity() const {
        return capacity;
    }
    
    // Helper to get array for iteration
    const Passenger* getArray() const {
        return arr;
    }
};

/*
=======================================
       LINKED LIST CANCELLATION
=======================================
*/

class LinkedListCancellation {
private:
    SimpleNode* head;

public:
    LinkedListCancellation() {
        head = nullptr;
    }
    
    ~LinkedListCancellation() {
        SimpleNode* current = head;
        while (current != nullptr) {
            SimpleNode* next = current->next;
            delete current;
            current = next;
        }
    }

    void loadFromFile(const char* filename) {
        std::ifstream file(filename);
        std::string line;
        std::getline(file, line); 

        while (std::getline(file, line)) {
            Passenger p;
            std::string token;
            std::stringstream ss(line);

            std::getline(ss, p.passengerID, ',');
            std::getline(ss, p.name, ',');
            
            std::getline(ss, token, ','); 
            p.seatRow = std::stoi(token);
            
            std::getline(ss, token, ','); 
            p.seatColumn = token[0];
            
            std::getline(ss, p.seatClass, ',');
            
            p.passengerID.erase(0, p.passengerID.find_first_not_of(" \t"));
            p.passengerID.erase(p.passengerID.find_last_not_of(" \t") + 1);
            
            p.name.erase(0, p.name.find_first_not_of(" \t"));
            p.name.erase(p.name.find_last_not_of(" \t") + 1);
            
            p.seatClass.erase(0, p.seatClass.find_first_not_of(" \t"));
            p.seatClass.erase(p.seatClass.find_last_not_of(" \t") + 1);
            
            // Check for empty seat
            p.isEmpty = (p.passengerID == "Empty" || p.passengerID == "EMPTY");

            SimpleNode* newNode = new SimpleNode(p, head);
            head = newNode;
        }
        file.close();
    }

    SimpleNode* linearSearch(const std::string& id) {
        SimpleNode* temp = head;
        while (temp != nullptr) {
            if (temp->data.passengerID == id && !temp->data.isEmpty)
                return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    SimpleNode* sentinelSearch(const std::string& id) {
        if (head == nullptr) return nullptr;

        Passenger sentinelPassenger;
        sentinelPassenger.passengerID = id;
        sentinelPassenger.name = "";
        sentinelPassenger.seatRow = 0;
        sentinelPassenger.seatColumn = 'A';
        sentinelPassenger.seatClass = "";
        sentinelPassenger.isEmpty = false;
        
        SimpleNode* sentinel = new SimpleNode(sentinelPassenger, nullptr);

        SimpleNode* last = head;
        while (last->next != nullptr) {
            last = last->next;
        }

        last->next = sentinel;

        SimpleNode* current = head;
        while (current->data.passengerID != id && !current->data.isEmpty) {
            current = current->next;
        }

        last->next = nullptr;
        
        SimpleNode* result = nullptr;
        if (current != sentinel) {
            result = current;
        }
        
        delete sentinel;
        return result;
    }

    bool cancelByLinear(const std::string& id) {
        SimpleNode* temp = head;
        SimpleNode* prev = nullptr;

        while (temp != nullptr && (temp->data.passengerID != id || temp->data.isEmpty)) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr) return false;

        if (prev == nullptr)
            head = temp->next;
        else
            prev->next = temp->next;

        delete temp;
        return true;
    }

    bool cancelBySentinel(const std::string& id) {
        SimpleNode* nodeToDelete = sentinelSearch(id);
        if (nodeToDelete == nullptr) return false;
        
        return cancelByLinear(id);
    }

    int getSize() const {
        int count = 0;
        SimpleNode* temp = head;
        while (temp != nullptr) {
            if (!temp->data.isEmpty) {
                count++;
            }
            temp = temp->next;
        }
        return count;
    }
    
    // Helper to get head for iteration
    SimpleNode* getHead() const {
        return head;
    }
};

#endif