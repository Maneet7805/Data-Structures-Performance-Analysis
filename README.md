# Data-Structures-Performance-Analysis
C++ implementation comparing array-based vs linked-list data structures in a flight reservation system, with performance analysis of search and sort operations.

## Project Overview

This Flight Reservation System demonstrates and compares two fundamental data structures:
- **2D Array-based implementation**
- **Linked List-based implementation**

The system manages passenger reservations for a large aircraft (400 rows × 26 seats) and provides performance comparisons between different data structure operations.

## Educational Purpose

This project was created to demonstrate:
1. **Data Structure Trade-offs**: Compare array vs linked list performance
2. **Search Algorithms**: Linear search, binary search, hash-based lookup
3. **Sorting Algorithms**: Merge sort implementation for both structures
4. **Memory Management**: Manual memory allocation without STL containers
5. **Real-world Application**: Flight reservation system with CSV data

## Features

### Core Functionality
- **Seating Chart Display**: Visual representation with color-coded seats
  - `X` (red) = Reserved seat
  - `O` (green) = Available seat
- **Passenger Manifest**: Sorted list of all passengers
- **Multiple Search Methods**:
  - Linear search (common to both)
  - Binary search (array-specific)
  - Hash map lookup (linked list-specific)
- **Direct Seat Access**: Quick lookup by row and column

### Performance Analysis
- **Time measurements** for all operations (microseconds)
- **Memory usage** calculations (bytes and KB)
- **Side-by-side comparison** tables
- **Multiple test scenarios**:
  - Seating chart generation
  - Unsorted manifest display
  - Sorted manifest display
  - Search operations


