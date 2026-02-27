#include "Report_Menu.hpp"
#include <iomanip>
#include <cstring>

// Wrapper to launch the reporting module
void showFlightSystemMainMenu() {
    showMainMenu();
}

// Platform-agnostic console clearing
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// UI Utility: Pauses execution until user intervention
void pressEnterToContinue() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

// ==================== MENUS ====================

// The central hub for the reporting application
void showMainMenu() {
    int mainChoice;
    bool exitToMainProgram = false;
    
    while (!exitToMainProgram) {
        clearScreen();
        std::cout << "===========================================\n";
        std::cout << "   FLIGHT SEATING & MANIFEST MODULE\n";
        std::cout << "===========================================\n\n";
        std::cout << "MAIN MENU:\n";
        std::cout << "1. Array-Based System\n";
        std::cout << "2. Linked List-Based System\n";
        std::cout << "3. Technique Demonstration\n";
        std::cout << "4. Performance Comparison\n";
        std::cout << "5. Return to Main Program\n\n";
        
        std::cout << "Enter your choice (1-5): ";
        
        // Input Validation: Check if input is an integer
        if (!(std::cin >> mainChoice)) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard Invalid input
            std::cout << "Invalid input. Please enter a number.\n";
            pressEnterToContinue();
            continue;
        }
        
        switch (mainChoice) {
            case 1:
                showArrayMenu();
                break;
                
            case 2:
                showLinkedListMenu();
                break;
                
            case 3:
                showTechniqueMenu();
                break;
                
            case 4:
                showComparisonMenu();
                break;
                
            case 5:
                exitToMainProgram = true;
                break;
                
            default:
                std::cout << "\nInvalid choice! Please enter 1-5.\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                pressEnterToContinue();
        }
    }
}

// Sub-menu for Array-specific operations (Grid view)
void showArrayMenu() {
    int choice;
    bool backToMain = false;
    
    while (!backToMain) {
        clearScreen();
        std::cout << "===================================\n";
        std::cout << "   ARRAY-BASED SYSTEM MENU\n";
        std::cout << "===================================\n\n";
        
        // Display context (Passenger count)
        std::cout << "SYSTEM INFO:\n";
        std::cout << "Passengers: " << (arraySystem ? arraySystem->getPassengerCount() : 0)
                  << " / " << (arraySystem ? arraySystem->getTotalSeats() : 0) << "\n";
        std::cout << "Data Structure: 2D Array (400 × 26)\n\n";
        
        std::cout << "DISPLAY OPERATIONS:\n";
        std::cout << "1. Display Seating Chart\n";
        std::cout << "2. Display Passenger Manifest\n";
        std::cout << "3. Return to Main Menu\n\n";
        
        std::cout << "Enter your choice (1-3): ";
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            pressEnterToContinue();
            continue;
        }
        
        switch (choice) {
            case 1:
                clearScreen();
                std::cout << "ARRAY SYSTEM - SEATING CHART\n";
                std::cout << "============================\n\n";
                if (arraySystem) arraySystem->displaySeatingChart();
                pressEnterToContinue();
                break;
                
            case 2:
                clearScreen();
                std::cout << "ARRAY SYSTEM - PASSENGER MANIFEST\n";
                std::cout << "==================================\n\n";
                if (arraySystem) arraySystem->displayPassengerManifest();
                pressEnterToContinue();
                break;
                
            case 3:
                backToMain = true;
                break;
                
            default:
                std::cout << "\nInvalid choice! Please enter 1-3.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                pressEnterToContinue();
        }
    }
}

// Sub-menu for Linked List operations (Dynamic nodes)
void showLinkedListMenu() {
    int choice;
    bool backToMain = false;
    
    while (!backToMain) {
        clearScreen();
        std::cout << "===================================\n";
        std::cout << "   LINKED LIST-BASED SYSTEM MENU\n";
        std::cout << "===================================\n\n";
        
        std::cout << "SYSTEM INFO:\n";
        std::cout << "Passengers: " << (linkedListSystem ? linkedListSystem->getPassengerCount() : 0)
                  << " / " << (linkedListSystem ? linkedListSystem->getTotalSeats() : 0) << "\n";
        std::cout << "Data Structure: Singly Linked List\n\n";
        
        std::cout << "DISPLAY OPERATIONS:\n";
        std::cout << "1. Display Seating Chart\n";
        std::cout << "2. Display Passenger Manifest\n";
        std::cout << "3. Return to Main Menu\n\n";
        
        std::cout << "Enter your choice (1-3): ";
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            pressEnterToContinue();
            continue;
        }
        
        switch (choice) {
            case 1:
                clearScreen();
                std::cout << "LINKED LIST SYSTEM - SEATING CHART\n";
                std::cout << "===================================\n\n";
                if (linkedListSystem) linkedListSystem->displaySeatingChart();
                pressEnterToContinue();
                break;
                
            case 2:
                clearScreen();
                std::cout << "LINKED LIST SYSTEM - PASSENGER MANIFEST\n";
                std::cout << "=========================================\n\n";
                if (linkedListSystem) linkedListSystem->displayPassengerManifest();
                pressEnterToContinue();
                break;
                
            case 3:
                backToMain = true;
                break;
                
            default:
                std::cout << "\nInvalid choice! Please enter 1-3.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                pressEnterToContinue();
        }
    }
}

// Menu for individual algorithm demonstrations
void showTechniqueMenu() {
    int choice;
    bool backToMain = false;
    
    while (!backToMain) {
        clearScreen();
        std::cout << "===================================\n";
        std::cout << "   TECHNIQUE DEMONSTRATION\n";
        std::cout << "===================================\n\n";
        std::cout << "DEMONSTRATION OPTIONS:\n";
        std::cout << "1. Show Array Techniques Demo\n";
        std::cout << "2. Show Linked List Techniques Demo\n";
        std::cout << "3. Return to Main Menu\n\n";
        
        std::cout << "Enter your choice (1-3): ";
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            pressEnterToContinue();
            continue;
        }
        
        switch (choice) {
            case 1:
                demonstrateArrayTechniques();
                break;
                
            case 2:
                demonstrateLinkedListTechniques();
                break;
                
            case 3:
                backToMain = true;
                break;
                
            default:
                std::cout << "\nInvalid choice! Please enter 1-3.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                pressEnterToContinue();
        }
    }
}

// ==================== BENCHMARKING FUNCTIONS ====================

// Demonstrates Linear Search (O(n)) vs Binary Search (O(log n))
void demonstrateArrayTechniques() {
    clearScreen();
    std::cout << "ARRAY TECHNIQUES DEMONSTRATION\n";
    std::cout << "================================\n\n";
    
    if (!arraySystem || arraySystem->getPassengerCount() == 0) {
        std::cout << "No data available for demonstration.\n";
        pressEnterToContinue();
        return;
    }
    
    std::cout << "Testing with " << arraySystem->getPassengerCount() << " passengers\n";
    std::cout << "Searching 10 random IDs\n\n";
    
    // Retrieve sample IDs for testing
    int testCount = 0;
    char** testIDs = arraySystem->getTestIDs(10, &testCount);
    
    if (!testIDs || testCount == 0) {
        std::cout << "No test IDs available.\n";
        pressEnterToContinue();
        return;
    }
    
    // --- Test 1: Linear Search ---
    std::cout << "1. LINEAR SEARCH (O(n)):\n";
    double linearTotal = 0;
    double linearMin = std::numeric_limits<double>::max();
    double linearMax = 0;
    std::string linearMinID, linearMaxID;
    
    for (int i = 0; i < testCount; i++) {
        double time = arraySystem->measureLinearSearchTime(testIDs[i]);
        linearTotal += time;
        if (time < linearMin) { 
            linearMin = time; 
            linearMinID = testIDs[i]; 
        }
        if (time > linearMax) { 
            linearMax = time; 
            linearMaxID = testIDs[i]; 
        }
    }
    
    std::cout << "   Average Time: " << std::fixed << std::setprecision(2) 
              << (testCount > 0 ? linearTotal / testCount : 0) << " us\n";
    std::cout << "   Min Time: " << linearMin << " us (ID: " << linearMinID << ")\n";
    std::cout << "   Max Time: " << linearMax << " us (ID: " << linearMaxID << ")\n\n";
    
    // --- Test 2: Binary Search (Requires Sorting) ---
    std::cout << "2. BINARY SEARCH (O(log n)):\n";
    arraySystem->sortByPassengerID(); // Pre-requisite: Data must be sorted
    
    double binaryTotal = 0;
    double binaryMin = std::numeric_limits<double>::max();
    double binaryMax = 0;
    std::string binaryMinID, binaryMaxID;
    
    for (int i = 0; i < testCount; i++) {
        double time = arraySystem->measureBinarySearchTime(testIDs[i]);
        binaryTotal += time;
        if (time < binaryMin) { 
            binaryMin = time; 
            binaryMinID = testIDs[i]; 
        }
        if (time > binaryMax) { 
            binaryMax = time; 
            binaryMaxID = testIDs[i]; 
        }
    }
    
    std::cout << "   Average Time: " << (testCount > 0 ? binaryTotal / testCount : 0) << " us\n";
    std::cout << "   Min Time: " << binaryMin << " us (ID: " << binaryMinID << ")\n";
    std::cout << "   Max Time: " << binaryMax << " us (ID: " << binaryMaxID << ")\n\n";
    
    // --- Test 3: Memory ---
    size_t arrayMem = arraySystem->calculateMemoryUsage();
    std::cout << "3. MEMORY USAGE:\n";
    std::cout << "   Total Memory: " << arrayMem << " bytes (" 
              << std::fixed << std::setprecision(2) << (arrayMem / 1024.0) << " KB)\n";
    std::cout << "   Per Passenger: " 
              << (arraySystem->getPassengerCount() > 0 ? arrayMem / arraySystem->getPassengerCount() : 0) 
              << " bytes\n";
    
    // Clean up dynamically allocated test IDs
    for (int i = 0; i < testCount; i++) {
        delete[] testIDs[i];
    }
    delete[] testIDs;
    
    pressEnterToContinue();
}

// Demonstrates Linear Search (O(n)) vs Hash Map Lookup (O(1))
void demonstrateLinkedListTechniques() {
    clearScreen();
    std::cout << "LINKED LIST TECHNIQUES DEMONSTRATION\n";
    std::cout << "=======================================\n\n";
    
    if (!linkedListSystem || linkedListSystem->getPassengerCount() == 0) {
        std::cout << "No data available for demonstration.\n";
        pressEnterToContinue();
        return;
    }
    
    std::cout << "Testing with " << linkedListSystem->getPassengerCount() << " passengers\n";
    std::cout << "Searching 10 random IDs\n\n";
    
    // Retrieve sample IDs
    int testCount = 0;
    char** testIDs = linkedListSystem->getTestIDs(10, &testCount);
    
    if (!testIDs || testCount == 0) {
        std::cout << "No test IDs available.\n";
        pressEnterToContinue();
        return;
    }
    
    // --- Test 1: Linear Search ---
    std::cout << "1. LINEAR SEARCH (O(n)):\n";
    double linearTotal = 0;
    double linearMin = std::numeric_limits<double>::max();
    double linearMax = 0;
    std::string linearMinID, linearMaxID;
    
    for (int i = 0; i < testCount; i++) {
        double time = linkedListSystem->measureLinearSearchTime(testIDs[i]);
        linearTotal += time;
        if (time < linearMin) { 
            linearMin = time; 
            linearMinID = testIDs[i]; 
        }
        if (time > linearMax) { 
            linearMax = time; 
            linearMaxID = testIDs[i]; 
        }
    }
    
    std::cout << "   Average Time: " << std::fixed << std::setprecision(2) 
              << (testCount > 0 ? linearTotal / testCount : 0) << " us\n";
    std::cout << "   Min Time: " << linearMin << " us (ID: " << linearMinID << ")\n";
    std::cout << "   Max Time: " << linearMax << " us (ID: " << linearMaxID << ")\n\n";
    
    // --- Test 2: Hash Search (O(1)) ---
    std::cout << "2. HASH MAP LOOKUP (O(1)):\n";
    double hashTotal = 0;
    double hashMin = std::numeric_limits<double>::max();
    double hashMax = 0;
    std::string hashMinID, hashMaxID;
    
    for (int i = 0; i < testCount; i++) {
        double time = linkedListSystem->measureHashSearchTime(testIDs[i]);
        hashTotal += time;
        if (time < hashMin) { 
            hashMin = time; 
            hashMinID = testIDs[i]; 
        }
        if (time > hashMax) { 
            hashMax = time; 
            hashMaxID = testIDs[i]; 
        }
    }
    
    std::cout << "   Average Time: " << (testCount > 0 ? hashTotal / testCount : 0) << " us\n";
    std::cout << "   Min Time: " << hashMin << " us (ID: " << hashMinID << ")\n";
    std::cout << "   Max Time: " << hashMax << " us (ID: " << hashMaxID << ")\n\n";
    
    // --- Test 3: Memory ---
    size_t llMem = linkedListSystem->calculateMemoryUsage();
    std::cout << "3. MEMORY USAGE:\n";
    std::cout << "   Total Memory: " << llMem << " bytes (" 
              << std::fixed << std::setprecision(2) << (llMem / 1024.0) << " KB)\n";
    std::cout << "   Per Passenger: " 
              << (linkedListSystem->getPassengerCount() > 0 ? llMem / linkedListSystem->getPassengerCount() : 0) 
              << " bytes\n";
    std::cout << "   Node Overhead: " << sizeof(ListNode) << " bytes per node\n";
    
    // Clean up test IDs
    for (int i = 0; i < testCount; i++) {
        delete[] testIDs[i];
    }
    delete[] testIDs;
    
    pressEnterToContinue();
}

// Sub-menu for specific comparison tasks
void compareManifestSubMenu() {
    int subChoice;
    bool backToComparison = false;
    
    while (!backToComparison) {
        clearScreen();
        std::cout << "MANIFEST GENERATION COMPARISON\n";
        std::cout << "===============================\n\n";
        if (arraySystem) {
            std::cout << "Testing with " << arraySystem->getPassengerCount() << " passengers\n\n";
        }
        std::cout << "COMPARISON OPTIONS:\n";
        std::cout << "1. Compare Unsorted Manifest Display\n";
        std::cout << "2. Compare Sorted Manifest Display\n";
        std::cout << "3. Back to Performance Comparison Menu\n\n";
        std::cout << "Enter your choice (1-3): ";
        
        if (!(std::cin >> subChoice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            pressEnterToContinue();
            continue;
        }
        
        switch (subChoice) {
            case 1:
                clearScreen();
                if (arraySystem && linkedListSystem) {
                    PerformanceComparator::compareUnsortedManifest(*arraySystem, *linkedListSystem);
                }
                pressEnterToContinue();
                break;
                
            case 2:
                clearScreen();
                if (arraySystem && linkedListSystem) {
                    PerformanceComparator::compareSortedManifest(*arraySystem, *linkedListSystem);
                }
                pressEnterToContinue();
                break;
                
            case 3:
                backToComparison = true;
                break;
                
            default:
                std::cout << "\nInvalid choice! Please enter 1-3.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                pressEnterToContinue();
        }
    }
}

// Menu for side-by-side performance comparisons
void showComparisonMenu() {
    int choice;
    bool backToMain = false;
    
    while (!backToMain) {
        clearScreen();
        std::cout << "===================================\n";
        std::cout << "   PERFORMANCE COMPARISON\n";
        std::cout << "===================================\n\n";
        
        if (arraySystem) {
            std::cout << "Testing with " << arraySystem->getPassengerCount() 
                      << " passengers\n\n";
        }
        
        std::cout << "COMPARISON OPTIONS:\n";
        std::cout << "1. Compare Seating Chart Display (Time + Space)\n";
        std::cout << "2. Compare Manifest Generation\n";
        std::cout << "3. Compare All Operations (Complete Analysis)\n";
        std::cout << "4. Return to Main Menu\n\n";
        
        std::cout << "Enter your choice (1-4): ";
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            pressEnterToContinue();
            continue;
        }
        
        switch (choice) {
            case 1:
                clearScreen();
                if (arraySystem && linkedListSystem) {
                    PerformanceComparator::compareSeatingChart(*arraySystem, *linkedListSystem);
                }
                pressEnterToContinue();
                break;
                
            case 2:
                compareManifestSubMenu();
                break;
                
            case 3:
                clearScreen();
                if (arraySystem && linkedListSystem) {
                    PerformanceComparator::compareAllOperations(*arraySystem, *linkedListSystem);
                }
                pressEnterToContinue();
                break;
                
            case 4:
                backToMain = true;
                break;
                
            default:
                std::cout << "\nInvalid choice! Please enter 1-4.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                pressEnterToContinue();
        }
    }
}