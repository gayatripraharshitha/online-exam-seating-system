# Online Exam Seating Arrangement System

A C-based examination seating management system that automates student registration, examination-center preference management, seat allocation, authentication, audit history, and examination record archiving.

## Overview

The Online Exam Seating Arrangement System is an academic project developed in C to demonstrate the practical application of data structures, algorithms, file handling, and system-state management to an examination seating problem.

Students do not select individual seats. Instead, they submit an ordered list of examination-center preferences. The examination authority manages the examination centers, controls the preference window, approves submitted preferences, generates the seating allocation, and locks the final allocation.

The system also maintains student-specific history and supports archiving of completed examinations before starting a new examination cycle.

## Objectives

- Automate examination-center and seat allocation
- Allow students to submit prioritized examination-center preferences
- Prevent direct student selection of individual seats
- Allocate seats based on submitted preferences and available capacity
- Provide fallback allocation when preferred centers are unavailable
- Maintain student-specific activity history
- Provide authority-controlled examination management
- Preserve completed examination records
- Support multiple examination cycles
- Demonstrate practical Data Structures and Algorithms concepts

## System Features

### Student Module

The student module provides the following functionality:

- Student registration
- Password-based authentication
- Candidate ID-based login
- Examination-center listing
- Submission of three examination-center preferences
- Modification of preferences while the preference window is open
- Viewing submitted preferences
- Viewing personal examination allocation
- Viewing personal activity history
- Logout

Students can access only their own allocation and history.

### Authority Module

The authority module provides:

- Authority authentication
- Examination-center creation
- Examination-center information display
- Preference-window management
- Preference approval
- Candidate search
- System dashboard
- Audit-history viewing
- Allocation generation
- Final allocation locking
- Examination archiving
- Examination reset

### Authority Credentials

The current academic demonstration version uses the following credentials:

Username:

    admin

Password:

    admin123

These credentials are intended only for the demonstration version of the project.

## Examination Workflow

The system follows a controlled examination workflow:

    1. Create examination centers
    2. Open the preference window
    3. Register students
    4. Collect student preferences
    5. Close the preference window
    6. Approve submitted preferences
    7. Generate the seating allocation
    8. Review the generated allocation
    9. Lock the final allocation
    10. Archive the examination
    11. Reset the system for a new examination

This workflow prevents students from modifying their preferences after the authority has approved them and prevents further allocation changes after the final allocation has been locked.

## Allocation Algorithm

Each student submits three examination-center preferences.

The allocation process evaluates the preferences in order:

    First Preference
          |
          v
    Second Preference
          |
          v
    Third Preference
          |
          v
    Fallback Center

For each student, the system attempts to allocate the first preferred center with available usable capacity.

If the first preference is full, the second preference is considered. If the second preference is also full, the third preference is considered.

If all three preferred centers are full, the system performs a sequential search through the available centers and attempts to allocate the student to another center with usable capacity.

Students do not select individual seats. The system assigns the next available seat automatically.

The current implementation uses a priority-based greedy allocation strategy. It selects the first feasible center from the student's ordered preferences rather than performing a global optimization across all students.

## Seat Management

The current implementation represents an examination hall using a two-dimensional seat structure.

    Rows: 6
    Columns: 6
    Total seats: 36

A reserve capacity of 10 percent is maintained.

    Total seats: 36
    Reserve seats: 3
    Usable seats: 33

Seats are represented using row and column coordinates.

Examples:

    R1-C1
    R1-C2
    R1-C3
    R2-C1

The seat position is calculated from a sequential seat number using arithmetic indexing.

    Row    = ((S - 1) / COLS) + 1
    Column = ((S - 1) % COLS) + 1

This provides constant-time conversion from a sequential seat position to its row and column representation.

## Data Structures

The project currently uses the following data structures.

### Structures

C structures are used to represent:

- Student records
- Seat records
- History records

Structures allow related attributes to be grouped into logical entities.

### One-Dimensional Arrays

Arrays are used to store:

- Student records
- Examination-center names
- Center allocation counters
- Student history records

### Two-Dimensional Arrays

A two-dimensional array is used to represent the examination seating layout.

    Seat seats[ROWS][COLS];

This provides a grid-based representation of rows and columns within an examination hall.

## Algorithms and DSA Concepts

The implementation demonstrates the following concepts.

### Linear Search

Candidate IDs are located using sequential comparison through the student array.

Time complexity:

    O(N)

where N is the number of registered students.

### Sequential Traversal

Sequential traversal is used for:

- Displaying examination centers
- Processing student records
- Calculating dashboard statistics
- Displaying history
- Generating allocations
- Creating archive records

### Priority-Based Greedy Allocation

The allocation algorithm processes each student's preferences in priority order and selects the first feasible center.

Each student has at most three preferred centers, so checking the preference list requires constant time with respect to the number of centers.

### Fallback Search

When all preferred centers are unavailable, the system sequentially searches the examination centers for available usable capacity.

Worst-case complexity:

    O(C)

where C is the number of examination centers.

### Arithmetic Indexing

Seat numbers are mapped to row and column positions using integer division and modulo operations.

Time complexity:

    O(1)

### State Management

The system maintains explicit states for the examination workflow.

Student allocation states:

    PENDING
    ALLOCATED
    LOCKED

The system also maintains states for:

- Preference-window availability
- Preference approval
- Allocation locking

These states control which operations are permitted at each stage.

### File Handling

File operations are used to archive completed examination records.

The archive file is:

    exam_archive.txt

## Complexity Analysis

| Operation | Technique | Complexity |
|---|---|---|
| Candidate search | Linear search | O(N) |
| Center traversal | Sequential traversal | O(C) |
| Preference validation | Constant-time comparison | O(1) |
| Seat generation | Arithmetic indexing | O(1) |
| Preference checking | Fixed-size priority list | O(1) |
| Fallback center search | Sequential search | O(C) |
| Complete allocation | Student processing with fallback search | O(NC) worst case |
| History traversal | Sequential traversal | O(H) |
| Archive generation | Sequential traversal | O(N + H) |

Where:

- N = number of students
- C = number of examination centers
- H = number of history records

## Authentication and Access Control

### Student Authentication

Students create a password during registration and authenticate using:

    Candidate ID
    Password

Students can view only their own:

- Preferences
- Allocation
- History

### Authority Authentication

The authority uses the predefined demonstration credentials and has access to system-wide examination management operations.

## History and Audit Trail

The system maintains activity history for each student.

Recorded events can include:

- Student registration
- Preference submission or update
- Preference approval
- Seat allocation
- Allocation failure
- Final allocation locking

The student interface exposes only the logged-in student's history.

The authority interface can display the complete audit history.

## Examination Archiving

Before resetting the active examination, the system can archive the current examination records.

Archived information includes:

- Candidate ID
- Student name
- Branch
- Examination center
- Hall
- Seat assignment
- Student history
- Examination-center information

The records are stored in:

    exam_archive.txt

The archive mechanism allows the active examination data to be reset without immediately losing the previous examination records.

## Examination Reset

The authority can archive the current examination and reset the active system.

The reset process:

1. Archives the current examination
2. Clears active student records
3. Clears current examination centers
4. Resets allocation states
5. Prepares the system for a new examination

A new examination can therefore use a different set of examination centers.

## Technologies

- C
- GCC
- Visual Studio Code

## Compilation and Execution

Open a terminal in the project directory.

Compile the program:

    gcc onlineseatingarr.c -o exam

Run on Windows PowerShell:

    .\exam.exe

## Project Structure

    online-exam-seating-system/
    |
    |-- onlineseatingarr.c
    |-- README.md
    |-- .gitignore

The program may generate the following file during the archive operation:

    exam_archive.txt

This file is excluded from version control through `.gitignore` because it may contain examination records.

## Example

Suppose the authority creates the following examination centers:

    1. Chennai
    2. Bangalore
    3. Kolkata
    4. Dubai
    5. Nagaland

A student submits:

    First Preference: Bangalore
    Second Preference: Chennai
    Third Preference: Dubai

The system first attempts to allocate the student to Bangalore.

If Bangalore has no remaining usable capacity, the system checks Chennai. If Chennai is also unavailable, it checks Dubai.

If all three preferred centers are unavailable, the system searches the remaining centers for usable capacity.

The student does not select an individual seat.

## Design Considerations

The current implementation emphasizes:

- Controlled examination workflow
- Priority-based allocation
- Array-based data management
- Student-specific access to records
- Reserve capacity
- Allocation locking
- Examination history
- File-based archiving
- Repeatable examination cycles
- Clear algorithmic behavior

## Future Improvements

Potential extensions include:

- Hash-table-based Candidate ID lookup
- Multiple halls per examination center
- Dynamic hall and seat configuration
- More advanced allocation and fairness policies
- Database integration
- Stronger password protection
- Encrypted credential storage
- Web-based student and authority interfaces
- Automated examination reports
- Advanced optimization-based seat allocation

These features are not part of the current implementation.

## Project Type

Academic Data Structures and C Programming Project