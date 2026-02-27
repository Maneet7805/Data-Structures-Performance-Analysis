#include "Cancellation_Menu.hpp"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;
using namespace chrono;

void showCancellationMenu(const char* file) {
    int cancelChoice;

    do {
        cout << "\n==================\n";
        cout << "  CANCELLATION\n";
        cout << "==================\n";
        cout << "\nChoose Cancellation Type:\n";
        cout << "0. Return to Main Menu\n";
        cout << "1. Array-based Cancellation\n";
        cout << "2. Linked List-based Cancellation\n";
        cout << "3. Array vs Linked List Comparison Table\n";
        cout << "Enter your choice: ";
        
        if (!(cin >> cancelChoice)) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
            cout << "Invalid input! Please enter a number 0-3.\n";
            continue; // Skip to next iteration
        }
        
        // Clear the newline left in buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (cancelChoice) {
        case 1: {
            string id;
            cout << "\nEnter Passenger ID to cancel: ";
            getline(cin, id); // Use getline for strings

            ArrayCancellation arrLinear(10400);
            arrLinear.loadFromFile(file);
            arrLinear.sortByID();

            size_t memArrayLin = arrLinear.getCapacity() * sizeof(Passenger);

            auto start = high_resolution_clock::now();
            bool linearSuccess = arrLinear.cancelByLinear(id);
            auto end = high_resolution_clock::now();

            cout << "\n[ARRAY - LINEAR SEARCH]\n";
            cout << "Time: "
                << duration_cast<microseconds>(end - start).count()
                << " microseconds\n";
            cout << "Memory: "
                << memArrayLin << " bytes ("
                << memArrayLin / 1024.0 << " KB)\n";
            cout << "Result: "
                << (linearSuccess ? "Found & Canceled" : "Not Found") << endl;

            ArrayCancellation arrBinary(10400);
            arrBinary.loadFromFile(file);
            arrBinary.sortByID();

            size_t memArrayBin = arrBinary.getCapacity() * sizeof(Passenger);

            start = high_resolution_clock::now();
            bool binarySuccess = arrBinary.cancelByBinary(id);
            end = high_resolution_clock::now();

            cout << "\n[ARRAY - BINARY SEARCH]\n";
            cout << "Time: "
                << duration_cast<microseconds>(end - start).count()
                << " microseconds\n";
            cout << "Memory: "
                << memArrayBin << " bytes ("
                << memArrayBin / 1024.0 << " KB)\n";
            cout << "Result: "
                << (binarySuccess ? "Found & Canceled" : "Not Found") << endl;

            break;
        }

        case 2: {
            string id;
            cout << "\nEnter Passenger ID to cancel: ";
            getline(cin, id); // Use getline for strings

            LinkedListCancellation llLinear;
            llLinear.loadFromFile(file);

            int n3 = llLinear.getSize();
            size_t memListLin = n3 * (sizeof(SimpleNode) + sizeof(Passenger));

            auto start = high_resolution_clock::now();
            bool llLinearSuccess = llLinear.cancelByLinear(id);
            auto end = high_resolution_clock::now();

            cout << "\n[LINKED LIST - LINEAR SEARCH]\n";
            cout << "Time: "
                << duration_cast<microseconds>(end - start).count()
                << " microseconds\n";
            cout << "Memory: "
                << memListLin << " bytes ("
                << memListLin / 1024.0 << " KB)\n";
            cout << "Result: "
                << (llLinearSuccess ? "Found & Canceled" : "Not Found") << endl;

            LinkedListCancellation llSentinel;
            llSentinel.loadFromFile(file);

            int n4 = llSentinel.getSize();
            size_t memListSen = n4 * (sizeof(SimpleNode) + sizeof(Passenger));

            start = high_resolution_clock::now();
            bool llSentinelSuccess = llSentinel.cancelBySentinel(id);
            end = high_resolution_clock::now();

            cout << "\n[LINKED LIST - SENTINEL SEARCH]\n";
            cout << "Time: "
                << duration_cast<microseconds>(end - start).count()
                << " microseconds\n";
            cout << "Memory: "
                << memListSen << " bytes ("
                << memListSen / 1024.0 << " KB)\n";
            cout << "Result: "
                << (llSentinelSuccess ? "Found & Canceled" : "Not Found") << endl;

            break;
        }

        case 3: {
            string id;
            cout << "\nEnter Passenger ID to test: ";
            getline(cin, id); // Use getline for strings

            ArrayCancellation arrTest(10400);
            arrTest.loadFromFile(file);

            size_t memArr = arrTest.getCapacity() * sizeof(Passenger);

            auto start = high_resolution_clock::now();
            bool arrResult = arrTest.cancelByLinear(id);
            auto end = high_resolution_clock::now();

            auto arrTime = duration_cast<microseconds>(end - start).count();

            LinkedListCancellation llTest;
            llTest.loadFromFile(file);

            int nList = llTest.getSize();
            size_t memList = nList * (sizeof(SimpleNode) + sizeof(Passenger));

            start = high_resolution_clock::now();
            bool llResult = llTest.cancelByLinear(id);
            end = high_resolution_clock::now();

            auto llTime = duration_cast<microseconds>(end - start).count();

            cout << "\n========================================\n";
            cout << "   LINEAR SEARCH PERFORMANCE COMPARISON\n";
            cout << "========================================\n";
            cout << "Structure      Time (us)     Memory (KB)\n";
            cout << "----------------------------------------\n";
            cout << "Array           "
                << arrTime << "           "
                << memArr / 1024.0 << endl;
            cout << "Linked List     "
                << llTime << "           "
                << memList / 1024.0 << endl;
            cout << "----------------------------------------\n";

            cout << "\nResult Check:\n";
            cout << "Array: "
                << (arrResult ? "Found & Canceled" : "Not Found") << endl;
            cout << "Linked List: "
                << (llResult ? "Found & Canceled" : "Not Found") << endl;

            break;
        }

        case 0:
            cout << "Returning to main menu...\n";
            break;

        default:
            cout << "Invalid cancellation type! Please enter 0-3.\n";
        }

    } while (cancelChoice != 0);
}