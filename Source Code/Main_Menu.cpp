#include <iostream>
#include <chrono>
#include <string>
#include <iomanip>
#include <cstring>
#include "Passenger.hpp" 
#include "Reservation_Menu.hpp" 
#include "Cancellation.hpp"
#include "Cancellation_Menu.hpp"
#include "Search_Menu.hpp"
#include "Seat_Manifest.hpp"
#include "Report_Menu.hpp"

using namespace std;
using namespace chrono;

// GLOBAL VARIABLE DEFINITIONS 
ArrayDisplaySystem* arraySystem = nullptr;
LinkedListDisplaySystem* linkedListSystem = nullptr;
Passenger* loadedPassengers = nullptr;
int loadedPassengerCount = 0;

// GLOBAL FUNCTION DEFINITIONS 
void initializeFlightSystem() {
    if (arraySystem) delete arraySystem;
    if (linkedListSystem) delete linkedListSystem;
    
    // Initialize systems with loaded passengers
    arraySystem = new ArrayDisplaySystem(400, 26);
    linkedListSystem = new LinkedListDisplaySystem(400, 26);
    
    if (loadedPassengers && loadedPassengerCount > 0) {
        arraySystem->loadPassengersFromArray(loadedPassengers, loadedPassengerCount);
        linkedListSystem->loadPassengersFromArray(loadedPassengers, loadedPassengerCount);
    }
}

// Count occupied passengers function 
int countOccupiedPassengers(const Passenger* passengers, int count) {
    int occupied = 0;
    for (int i = 0; i < count; i++) {
        if (!passengers[i].isEmpty) {
            occupied++;
        }
    }
    return occupied;
}

int main() {
    const char* file = "Updated_Passenger_List.csv";
    
    // Load passengers from CSV
    loadedPassengers = readPassengersFromCSV(file, &loadedPassengerCount);
    
    if (!loadedPassengers || loadedPassengerCount == 0) {
        cout << "No CSV file found or file is empty.\n";
        return 1;
    }
    
    // Initialize flight system
    initializeFlightSystem();
    
    // Calculate statistics
    int actualPassengers = countOccupiedPassengers(loadedPassengers, loadedPassengerCount);
    int emptySeats = loadedPassengerCount - actualPassengers;
    int totalSeats = 400 * 26; 
    
    cout << "=========================================\n";
    cout << "  FLIGHT RESERVATION SYSTEM\n";
    cout << "=========================================\n";
    cout << "Total Records: " << loadedPassengerCount << "\n";
    cout << "Passengers: " << actualPassengers << "\n";
    cout << "Empty Seats: " << emptySeats << "\n";
    cout << "Aircraft: 400 rows × 26 seats\n";
    cout << "Total Capacity: " << totalSeats << " seats\n";
    cout << "Seat Utilization: " 
         << std::fixed << std::setprecision(1)
         << (actualPassengers * 100.0 / totalSeats) << "%\n";
    cout << "=========================================\n";
    
    int choice;

    do {
        cout << "\n=========================================\n";
        cout << "  FLIGHT RESERVATION SYSTEM - MAIN MENU\n";
        cout << "=========================================\n";
        cout << "0. Exit Program\n";
        cout << "1. Reservation\n";
        cout << "2. Cancellation\n";
        cout << "3. Search & Lookup\n";
        cout << "4. Manifest & Seat Report\n\n";
        cout << "Enter your choice (0-4): ";
        cin >> choice;
        cin.ignore(); // Clear newline from buffer

        switch (choice) {
        case 1: {
            showReservationMenu(loadedPassengers, loadedPassengerCount);
            break;
        }

        case 2:
            showCancellationMenu(file);
            break;

        case 3:
            showSearchMenu();
            break;

        case 4: {
            if (!arraySystem || !linkedListSystem) {
                initializeFlightSystem();
            }
            clearScreen(); 
            showFlightSystemMainMenu();
            break;
        }

        case 0:
            cout << "\n=== EXITING PROGRAM ===\n";
            cout << "Thank you for using the Flight Reservation System!\n";
            break;

        default:
            cout << "\nInvalid choice! Please enter 0-4.\n";
            break;
        }

    } while (choice != 0);

    if (arraySystem) delete arraySystem;
    if (linkedListSystem) delete linkedListSystem;
    if (loadedPassengers) delete[] loadedPassengers;

    return 0;
}