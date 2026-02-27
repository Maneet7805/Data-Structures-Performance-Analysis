#ifndef LINKEDLISTSEARCH_HPP
#define LINKEDLISTSEARCH_HPP

#include <iostream>
#include <cmath>
#include "Passenger.hpp"
using namespace std;

class LinkedListPassengerSystem {
private:
    SimpleNode* head; 

public:
    // Constructor
    LinkedListPassengerSystem() : head(nullptr) {}
    LinkedListPassengerSystem(const LinkedListPassengerSystem&) = delete;
    LinkedListPassengerSystem& operator=(const LinkedListPassengerSystem&) = delete;

    // Insert at head (O(1)) 
    void addPassenger(const string& id, const string& n, int r, char c, const string& cls) {
        Passenger p(id, n, r, c, cls, false);
        head = new SimpleNode(p, head);
    }

    // Returns number of nodes (O(n)) 
    int size() const {
        int count = 0;
        SimpleNode* cur = head;
        while (cur) {
            count++;
            cur = cur->next;
        }
        return count;
    }

    // --- UNSORTED LINEAR SEARCH (O(n)) ---
    bool searchPassengerUnsorted(const string& targetID) {
        SimpleNode* cur = head;
        while (cur) {
            if (cur->data.passengerID == targetID) return true;
            cur = cur->next;
        }
        return false;
    }

    // --- SORTED LINEAR SEARCH WITH EARLY EXIT ---2
    bool searchPassengerSorted(const string& targetID) {
        SimpleNode* cur = head;
        while (cur) {
            if (cur->data.passengerID == targetID) return true;
            if (cur->data.passengerID > targetID) break;
            cur = cur->next;
        }
        return false;
    }

    // --- JUMP SEARCH FOR LINKED LIST (O(√n)) ---
    bool searchPassengerJump(int listSize, const string& targetID) {
        if (!head || listSize <= 0) return false;

        int step = sqrt(listSize);
        SimpleNode* prev = head;
        SimpleNode* cur = head;

        // Jump in blocks 
        while (cur->next && cur->data.passengerID < targetID) {
            int block = 0;
            prev = cur;
            while (cur->next && block < step) {
                cur = cur->next;
                block++;
            }
            if (cur->data.passengerID >= targetID) break;
        }

        // Linear search inside block 
        SimpleNode* temp = prev;
        while (temp != cur->next) {
            if (temp->data.passengerID == targetID) return true;
            temp = temp->next;
        }
        return false;
    }

    // --- SELECTION SORT (O(n²)) ---
    void sortData() {
        for (SimpleNode* i = head; i; i = i->next) {
            SimpleNode* minNode = i;
            for (SimpleNode* j = i->next; j; j = j->next) {
                if (j->data.passengerID < minNode->data.passengerID)
                    minNode = j;
            }
            if (minNode != i) {
                // Swap passenger data only 
                Passenger temp = i->data;
                i->data = minNode->data;
                minNode->data = temp;
            }
        }
        cout << "[LINKED LIST] Sorted.\n";
    }

    // Destructor: free all nodes 
    ~LinkedListPassengerSystem() {
        while (head) {
            SimpleNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // --- SPACE COMPLEXITY ---
    size_t memoryUsage() const {
        int nodeCount = size();
        return nodeCount * sizeof(SimpleNode);
    }
};

#endif 