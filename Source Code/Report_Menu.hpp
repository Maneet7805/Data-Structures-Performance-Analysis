#ifndef REPORT_MENU_HPP
#define REPORT_MENU_HPP

#include "Seat_Manifest.hpp"
#include <iostream>
#include <cstdlib>
#include <limits>

// EXTERN DECLARATIONS
extern ArrayDisplaySystem* arraySystem;
extern LinkedListDisplaySystem* linkedListSystem;
extern Passenger* loadedPassengers;
extern int loadedPassengerCount;

// Function declarations
void showFlightSystemMainMenu();
void showMainMenu();
void showArrayMenu();
void showLinkedListMenu();
void showTechniqueMenu();
void showComparisonMenu();
void runComparison(int choice);
void demonstrateArrayTechniques();
void demonstrateLinkedListTechniques();
void clearScreen();
void pressEnterToContinue();
void compareManifestSubMenu();

#endif