#include "Reservation_Menu.hpp"
#include "ArrayReservation.hpp"
#include "LinkedListReservation.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstring>
#include <cctype>

// Static helper functions
static void bar(char ch = '=', int n = 60) {
    for (int i = 0; i < n; i++) std::cout << ch;
    std::cout << "\n";
}

static void header(const char* t) { 
    bar('='); 
    std::cout << t << "\n"; 
    bar('='); 
}

static void sub(const char* t) { 
    bar('-'); 
    std::cout << t << "\n"; 
    bar('-'); 
}

static long long usSince(const std::chrono::steady_clock::time_point& a,
                         const std::chrono::steady_clock::time_point& b) {
    return std::chrono::duration_cast<std::chrono::microseconds>(b - a).count();
}

static double fasterPct(double slower, double faster) {
    if (slower <= 0.0) return 0.0;
    return ((slower - faster) / slower) * 100.0;
}

static void winnerLine(const char* label, long long arr_us, long long list_us) {
    std::cout << label << "\n";
    std::cout << "Array Time      : " << arr_us << " microseconds\n";
    std::cout << "Linked List Time: " << list_us << " microseconds\n";

    if (arr_us < list_us) {
        std::cout << "Array is " << std::fixed << std::setprecision(1)
                  << fasterPct((double)list_us, (double)arr_us) << "% faster\n";
    } else if (list_us < arr_us) {
        std::cout << "Linked List is " << std::fixed << std::setprecision(1)
                  << fasterPct((double)arr_us, (double)list_us) << "% faster\n";
    } else {
        std::cout << "Tie\n";
    }
    std::cout << "\n";
}

static void readLine(const char* prompt, char* buf, int n) {
    std::cout << prompt;
    std::cin.getline(buf, n);
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(100000, '\n');
        buf[0] = '\0';
    }
}

int readChoice() {
    char buffer[32];
    std::cout << "Choice: ";
    std::cin.getline(buffer, 32);

    if (buffer[0] == '\0') {
        std::cout << "ERROR: No input entered. Please try again.\n\n";
        return -1;
    }

    for (int i = 0; buffer[i]; i++) {
        if (!isdigit(buffer[i])) {
            std::cout << "ERROR: Invalid input. Numbers only.\n\n";
            return -1;
        }
    }

    int choice = atoi(buffer);

    if (choice < 0 || choice > 3) {
        std::cout << "ERROR: Invalid choice. Please select 0 to 3.\n\n";
        return -1;
    }

    return choice;
}

// Find max ID in passengers
int findMaxID(const Passenger* passengers, int passengerCount) {
    int maxID = 0;
    for (int i = 0; i < passengerCount; i++) {
        const Passenger& p = passengers[i];
        if (!p.isEmpty) {
            try {
                int id = std::stoi(p.passengerID);
                if (id > maxID) maxID = id;
            } catch (...) {
                // Skip non-numeric IDs
            }
        }
    }
    return maxID;
}

// Main reservation menu function
void showReservationMenu(const Passenger* loadedPassengers, int loadedPassengerCount) {
    header("FLIGHT RESERVATION SYSTEM (INSERTION ONLY)");
    std::cout << "Seats: 400 rows x A-Z columns (10400 seats)\n";
    std::cout << "Dataset: Name=\"Empty\" means available seat\n\n";

    // Count occupied passengers
    int occupiedCount = 0;
    for (int i = 0; i < loadedPassengerCount; i++) {
        if (!loadedPassengers[i].isEmpty) {
            occupiedCount++;
        }
    }
    
    int maxID = findMaxID(loadedPassengers, loadedPassengerCount);

    ArrayReservation arr;
    LinkedListReservation list;

    arr.init(loadedPassengers, loadedPassengerCount, maxID);
    list.init(loadedPassengers, loadedPassengerCount, maxID);

    sub("DATASET LOADED");
    std::cout << "Occupied passengers: " << occupiedCount << "\n";
    std::cout << "Max PassengerID    : " << maxID << "\n\n";

    while (true) {
        sub("MAIN MENU");
        std::cout << "1. Reservation (Array)\n";
        std::cout << "2. Reservation (Linked List)\n";
        std::cout << "3. Performance Comparison (Insertion + Search + Memory)\n";
        std::cout << "0. Exit\n";
        int choice = readChoice();
        if (choice == -1) continue;
        if (choice == 0) break;

        if (choice == 1 || choice == 2) {
            char name[50], cls[12];
            readLine("Enter passenger name: ", name, 50);
            readLine("Enter class (Economy/Business/First): ", cls, 12);

            Passenger p{};
            bool ok = (choice == 1) ? arr.reserveWithIndex(name, cls, p)
                                    : list.reserveWithIndex(name, cls, p);

            if (!ok) {
                std::cout << "ERROR: No empty seats available.\n\n";
            } else {
                sub(choice == 1 ? "RESERVATION RESULT (ARRAY)" : "RESERVATION RESULT (LINKED LIST)");
                std::cout << "Seat: " << p.seatRow << p.seatColumn << "\n";
                std::cout << "ID  : " << p.passengerID << "\n\n";
            }
        } else if (choice == 3) {
            header("PERFORMANCE COMPARISON (MICROSECONDS + % FASTER)");

            const int ROWS = 400;
            const int COLS = 26;
            int available = (ROWS * COLS) - occupiedCount;
            int N = 300;
            if (N > available) N = available;
            if (N < 50) N = (available > 0 ? available : 50);

            std::cout << "Benchmark iterations: " << N << "\n";
            std::cout << "Timing uses TOTAL microseconds for N operations.\n\n";

            ArrayReservation* A = new ArrayReservation();
            LinkedListReservation* L = new LinkedListReservation();
            ArrayReservation* A2 = new ArrayReservation();
            LinkedListReservation* L2 = new LinkedListReservation();

            A->init(loadedPassengers, loadedPassengerCount, maxID);
            L->init(loadedPassengers, loadedPassengerCount, maxID);
            A2->init(loadedPassengers, loadedPassengerCount, maxID);
            L2->init(loadedPassengers, loadedPassengerCount, maxID);

            // Get first passenger ID for search test
            std::string searchID = "";
            for (int i = 0; i < loadedPassengerCount; i++) {
                const Passenger& p = loadedPassengers[i];
                if (!p.isEmpty) {
                    searchID = p.passengerID;
                    break;
                }
            }
            if (searchID.empty()) searchID = std::to_string(maxID);
            
            Passenger tmp{};

            // INSERTION: Linear-tech
            auto a1 = std::chrono::steady_clock::now();
            for (int i = 0; i < N; i++) A->reserveNoIndex("Bench", "Economy", tmp);
            auto a2t = std::chrono::steady_clock::now();

            auto l1 = std::chrono::steady_clock::now();
            for (int i = 0; i < N; i++) L->reserveNoIndex("Bench", "Economy", tmp);
            auto l2t = std::chrono::steady_clock::now();

            long long arrInsLin_us = usSince(a1, a2t);
            long long listInsLin_us = usSince(l1, l2t);

            sub("INSERTION (LINEAR TECHNIQUE: NO INDEX UPDATE)");
            winnerLine("Insertion Time", arrInsLin_us, listInsLin_us);

            // INSERTION: Indexed-tech
            auto a3 = std::chrono::steady_clock::now();
            for (int i = 0; i < N; i++) A2->reserveWithIndex("Bench", "Economy", tmp);
            auto a4t = std::chrono::steady_clock::now();

            auto l3 = std::chrono::steady_clock::now();
            for (int i = 0; i < N; i++) L2->reserveWithIndex("Bench", "Economy", tmp);
            auto l4t = std::chrono::steady_clock::now();

            long long arrInsIdx_us = usSince(a3, a4t);
            long long listInsIdx_us = usSince(l3, l4t);

            sub("INSERTION (INDEXED TECHNIQUE: MAINTAIN ID INDEX)");
            winnerLine("Insertion Time", arrInsIdx_us, listInsIdx_us);

            // SEARCH: Linear
            auto a5 = std::chrono::steady_clock::now();
            for (int i = 0; i < N; i++) (void)A2->searchLinear(searchID);
            auto a6t = std::chrono::steady_clock::now();

            auto l5 = std::chrono::steady_clock::now();
            for (int i = 0; i < N; i++) (void)L2->searchLinear(searchID);
            auto l6t = std::chrono::steady_clock::now();

            long long arrSearchLin_us = usSince(a5, a6t);
            long long listSearchLin_us = usSince(l5, l6t);

            sub("SEARCH (LINEAR)");
            winnerLine("Search Time", arrSearchLin_us, listSearchLin_us);

            // SEARCH: Indexed
            auto a7 = std::chrono::steady_clock::now();
            for (int i = 0; i < N; i++) (void)A2->searchIndexed(searchID);
            auto a8t = std::chrono::steady_clock::now();

            auto l7 = std::chrono::steady_clock::now();
            for (int i = 0; i < N; i++) (void)L2->searchIndexed(searchID);
            auto l8t = std::chrono::steady_clock::now();

            long long arrSearchIdx_us = usSince(a7, a8t);
            long long listSearchIdx_us = usSince(l7, l8t);

            sub("SEARCH (INDEXED)");
            winnerLine("Search Time", arrSearchIdx_us, listSearchIdx_us);

            // MEMORY
            sub("MEMORY USAGE COMPARISON");

            std::cout << std::left << std::setw(25) << "Data Structure"
                      << std::setw(15) << "Technique"
                      << std::setw(15) << "Bytes"
                      << "KB\n";
            bar('-');

            auto printMem = [](const char* ds, const char* tech, long bytes) {
                double kb = bytes / 1024.0;
                std::cout << std::left << std::setw(25) << ds
                          << std::setw(15) << tech
                          << std::setw(15) << bytes
                          << std::fixed << std::setprecision(1) << kb << "\n";
            };

            printMem("Array", "Linear", A->memoryLinear());
            printMem("Array", "Indexed", A2->memoryIndexed());
            printMem("Linked List", "Linear", L->memoryLinear());
            printMem("Linked List", "Indexed", L2->memoryIndexed());

            bar('-');

            std::cout << "Analysis:\n";
            std::cout << "1. Indexed technique consumes more memory due to ID lookup tables.\n";
            std::cout << "2. Linked list uses additional memory for node pointers.\n";

            delete A;
            delete L;
            delete A2;
            delete L2;
        }
    }
}