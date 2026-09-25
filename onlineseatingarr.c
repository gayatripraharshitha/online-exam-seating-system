#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_CENTERS 20
#define MAX_HISTORY 50

#define ROWS 6
#define COLS 6

#define AVAILABLE 0
#define OCCUPIED 1

#define PENDING 0
#define ALLOCATED 1
#define LOCKED 2
void generateAllocation();
void lockAllocation();
void archiveAndReset();

/* ================= STUDENT ================= */

typedef struct
{
    char id[20];
    char password[30];
    char name[50];
    char branch[30];

    /* Center preference numbers */
    int preference[3];

    char allocatedCenter[30];
    char hall[20];
    char seat[10];

    int status;

} Student;


/* ================= HISTORY ================= */

typedef struct
{
    char action[100];

} History;


/* ================= SEAT ================= */

typedef struct
{
    int status;
    char studentId[20];

} Seat;


/* ================= GLOBAL DATA ================= */

Student students[MAX_STUDENTS];

int studentCount = 0;


char centers[MAX_CENTERS][30];

int centerCount = 0;


History history[MAX_STUDENTS][MAX_HISTORY];

int historyCount[MAX_STUDENTS];


Seat seats[ROWS][COLS];


/* ================= SYSTEM CONTROL ================= */

int preferenceWindowOpen = 0;

int preferencesApproved = 0;

int allocationLocked = 0;


/* ================= RESERVE CAPACITY ================= */

int reservePercent = 10;


/* ================= ADMIN LOGIN ================= */

const char ADMIN_ID[] = "admin";

const char ADMIN_PASSWORD[] = "admin123";


/* ================================================= */
/*                  UTILITY FUNCTIONS                */
/* ================================================= */


/* Find student by ID */

int findStudent(char id[])
{
    int i;

    for(i = 0; i < studentCount; i++)
    {
        if(strcmp(students[i].id, id) == 0)
        {
            return i;
        }
    }

    return -1;
}


/* Add an entry to a student's history */

void addHistory(int studentIndex, const char action[])
{
    if(studentIndex < 0 || studentIndex >= MAX_STUDENTS)
    {
        return;
    }

    if(historyCount[studentIndex] >= MAX_HISTORY)
    {
        return;
    }

    strcpy(
        history[studentIndex][historyCount[studentIndex]].action,
        action
    );

    historyCount[studentIndex]++;
}


/* Initialize all seats */

void initializeSeats()
{
    int i;
    int j;

    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS; j++)
        {
            seats[i][j].status = AVAILABLE;

            strcpy(
                seats[i][j].studentId,
                "-"
            );
        }
    }
}


/* Initialize history counters */

void initializeHistory()
{
    int i;

    for(i = 0; i < MAX_STUDENTS; i++)
    {
        historyCount[i] = 0;
    }
}


/* Initialize entire system */

void initializeSystem()
{
    initializeSeats();

    initializeHistory();

    studentCount = 0;

    centerCount = 0;

    preferenceWindowOpen = 0;

    preferencesApproved = 0;

    allocationLocked = 0;
}


/* Calculate usable seats after reserve capacity */

int getUsableSeats()
{
    int totalSeats;
    int reserveSeats;

    totalSeats = ROWS * COLS;

    reserveSeats =
        (totalSeats * reservePercent) / 100;

    return totalSeats - reserveSeats;
}
/* ================================================= */
/*                  STUDENT FUNCTIONS                */
/* ================================================= */


/* Display available exam centers */

void displayCenters()
{
    int i;

    if(centerCount == 0)
    {
        printf("\nNo exam centers have been created yet.\n");
        return;
    }

    printf("\n========== AVAILABLE EXAM CENTERS ==========\n");

    for(i = 0; i < centerCount; i++)
    {
        printf("%d. %s\n", i + 1, centers[i]);
    }

    printf("============================================\n");
}


/* Validate whether three preferences are valid */

int validPreferences(int p1, int p2, int p3)
{
    if(centerCount < 3)
    {
        return 0;
    }

    if(p1 < 1 || p1 > centerCount)
    {
        return 0;
    }

    if(p2 < 1 || p2 > centerCount)
    {
        return 0;
    }

    if(p3 < 1 || p3 > centerCount)
    {
        return 0;
    }

    /* Preferences must be different */

    if(p1 == p2 || p1 == p3 || p2 == p3)
    {
        return 0;
    }

    return 1;
}


/* Student registration */

void studentRegistration()
{
    Student newStudent;

    char confirmPassword[30];

    if(studentCount >= MAX_STUDENTS)
    {
        printf("\nStudent registration limit reached.\n");
        return;
    }

    printf("\n========== STUDENT REGISTRATION ==========\n");

    printf("Enter Candidate ID: ");
    scanf("%19s", newStudent.id);

    /* Check duplicate ID */

    if(findStudent(newStudent.id) != -1)
    {
        printf("\nCandidate ID already exists.\n");
        return;
    }

    printf("Enter Name: ");
    scanf(" %[^\n]", newStudent.name);

    printf("Enter Branch: ");
    scanf(" %[^\n]", newStudent.branch);

    printf("Create Password: ");
    scanf("%29s", newStudent.password);

    printf("Confirm Password: ");
    scanf("%29s", confirmPassword);

    if(strcmp(newStudent.password, confirmPassword) != 0)
    {
        printf("\nPasswords do not match.\n");
        return;
    }

    /* Default values */

    newStudent.preference[0] = -1;
    newStudent.preference[1] = -1;
    newStudent.preference[2] = -1;

    strcpy(newStudent.allocatedCenter, "Not Allocated");
    strcpy(newStudent.hall, "Not Assigned");
    strcpy(newStudent.seat, "Not Assigned");

    newStudent.status = PENDING;

    students[studentCount] = newStudent;

    addHistory(
        studentCount,
        "Student registered successfully"
    );

    studentCount++;

    printf("\nRegistration successful.\n");
    printf("Your Candidate ID is: %s\n", newStudent.id);
}


/* Student login */

int studentLogin()
{
    char id[20];
    char password[30];

    int index;

    printf("\n========== STUDENT LOGIN ==========\n");

    printf("Candidate ID: ");
    scanf("%19s", id);

    printf("Password: ");
    scanf("%29s", password);

    index = findStudent(id);

    if(index == -1)
    {
        printf("\nInvalid Candidate ID or Password.\n");
        return -1;
    }

    if(strcmp(students[index].password, password) != 0)
    {
        printf("\nInvalid Candidate ID or Password.\n");
        return -1;
    }

    printf("\nLogin successful. Welcome, %s!\n",
           students[index].name);

    return index;
}


/* Submit or update center preferences */

void submitPreferences(int studentIndex)
{
    int p1;
    int p2;
    int p3;

    if(studentIndex < 0 || studentIndex >= studentCount)
    {
        return;
    }

    if(!preferenceWindowOpen)
    {
        printf("\nPreference submission is currently CLOSED.\n");
        return;
    }

    if(preferencesApproved)
    {
        printf("\nPreferences have already been approved by the authority.\n");
        printf("Changes are no longer allowed.\n");
        return;
    }

    if(centerCount < 3)
    {
        printf("\nAt least 3 exam centers are required.\n");
        printf("Please contact the exam authority.\n");
        return;
    }

    displayCenters();

    printf("\nEnter your center priorities.\n");
    printf("1 = Highest priority\n");
    printf("2 = Second priority\n");
    printf("3 = Third priority\n\n");

    printf("First preference: ");

    if(scanf("%d", &p1) != 1)
    {
        printf("\nInvalid input.\n");
        while(getchar() != '\n');
        return;
    }

    printf("Second preference: ");

    if(scanf("%d", &p2) != 1)
    {
        printf("\nInvalid input.\n");
        while(getchar() != '\n');
        return;
    }

    printf("Third preference: ");

    if(scanf("%d", &p3) != 1)
    {
        printf("\nInvalid input.\n");
        while(getchar() != '\n');
        return;
    }

    if(!validPreferences(p1, p2, p3))
    {
        printf("\nInvalid preferences.\n");
        printf("Make sure all three numbers are valid and different.\n");
        return;
    }

    students[studentIndex].preference[0] = p1;
    students[studentIndex].preference[1] = p2;
    students[studentIndex].preference[2] = p3;

    addHistory(
        studentIndex,
        "Exam center preferences submitted/updated"
    );

    printf("\nPreferences saved successfully.\n");

    printf("1st Priority : %s\n",
           centers[p1 - 1]);

    printf("2nd Priority : %s\n",
           centers[p2 - 1]);

    printf("3rd Priority : %s\n",
           centers[p3 - 1]);
}


/* View student's own preferences */

void viewPreferences(int studentIndex)
{
    int p1;
    int p2;
    int p3;

    if(studentIndex < 0 || studentIndex >= studentCount)
    {
        return;
    }

    p1 = students[studentIndex].preference[0];
    p2 = students[studentIndex].preference[1];
    p3 = students[studentIndex].preference[2];

    printf("\n========== MY PREFERENCES ==========\n");

    if(p1 == -1)
    {
        printf("Preferences not submitted yet.\n");
    }
    else
    {
        printf("1st Priority : %s\n", centers[p1 - 1]);
        printf("2nd Priority : %s\n", centers[p2 - 1]);
        printf("3rd Priority : %s\n", centers[p3 - 1]);
    }

    printf("====================================\n");
}


/* View student's own allocation */

void viewAllocation(int studentIndex)
{
    if(studentIndex < 0 || studentIndex >= studentCount)
    {
        return;
    }

    printf("\n========== MY EXAM ALLOCATION ==========\n");

    printf("Candidate ID : %s\n",
           students[studentIndex].id);

    printf("Name         : %s\n",
           students[studentIndex].name);

    if(students[studentIndex].status == PENDING)
    {
        printf("Status       : Allocation Pending\n");
    }
    else if(students[studentIndex].status == ALLOCATED)
    {
        printf("Status       : Allocated\n");

        printf("Center       : %s\n",
               students[studentIndex].allocatedCenter);

        printf("Hall         : %s\n",
               students[studentIndex].hall);

        printf("Seat         : %s\n",
               students[studentIndex].seat);
    }
    else if(students[studentIndex].status == LOCKED)
    {
        printf("Status       : Allocation Locked\n");

        printf("Center       : %s\n",
               students[studentIndex].allocatedCenter);

        printf("Hall         : %s\n",
               students[studentIndex].hall);

        printf("Seat         : %s\n",
               students[studentIndex].seat);
    }

    printf("========================================\n");
}


/* View student's own history */

void viewStudentHistory(int studentIndex)
{
    int i;

    if(studentIndex < 0 || studentIndex >= studentCount)
    {
        return;
    }

    printf("\n========== MY HISTORY ==========\n");

    if(historyCount[studentIndex] == 0)
    {
        printf("No history available.\n");
    }
    else
    {
        for(i = 0; i < historyCount[studentIndex]; i++)
        {
            printf("%d. %s\n",
                   i + 1,
                   history[studentIndex][i].action);
        }
    }

    printf("===============================\n");
}


/* Student portal */

void studentPortal()
{
    int studentIndex;

    int choice;

    studentIndex = studentLogin();

    if(studentIndex == -1)
    {
        return;
    }

    do
    {
        printf("\n\n========== STUDENT PORTAL ==========\n");

        printf("1. View Available Centers\n");
        printf("2. Submit / Update Preferences\n");
        printf("3. View My Preferences\n");
        printf("4. View My Allocation\n");
        printf("5. View My History\n");
        printf("6. Logout\n");

        printf("=====================================\n");

        printf("Enter choice: ");

        if(scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input.\n");
            while(getchar() != '\n');
            continue;
        }

        switch(choice)
        {
            case 1:
                displayCenters();
                break;

            case 2:
                submitPreferences(studentIndex);
                break;

            case 3:
                viewPreferences(studentIndex);
                break;

            case 4:
                viewAllocation(studentIndex);
                break;

            case 5:
                viewStudentHistory(studentIndex);
                break;

            case 6:
                printf("\nLogged out successfully.\n");
                break;

            default:
                printf("\nInvalid choice.\n");
        }

    } while(choice != 6);
}
/* ================================================= */
/*                 AUTHORITY FUNCTIONS               */
/* ================================================= */


/* Authority login */

int authorityLogin()
{
    char username[30];
    char password[30];

    printf("\n========== AUTHORITY LOGIN ==========\n");

    printf("Username: ");
    scanf("%29s", username);

    printf("Password: ");
    scanf("%29s", password);

    if(strcmp(username, ADMIN_ID) == 0 &&
       strcmp(password, ADMIN_PASSWORD) == 0)
    {
        printf("\nAuthority login successful.\n");
        return 1;
    }

    printf("\nInvalid authority credentials.\n");

    return 0;
}


/* Add exam centers */

void addCenters()
{
    int number;
    int i;

    if(centerCount > 0)
    {
        printf("\nCenters have already been created for this exam.\n");
        printf("Reset the exam before creating a new center setup.\n");
        return;
    }

    printf("\n========== CREATE EXAM CENTERS ==========\n");

    printf("Enter number of centers: ");

    if(scanf("%d", &number) != 1)
    {
        printf("\nInvalid input.\n");
        while(getchar() != '\n');
        return;
    }

    if(number < 3 || number > MAX_CENTERS)
    {
        printf("\nNumber of centers must be between 3 and %d.\n",
               MAX_CENTERS);
        return;
    }

    for(i = 0; i < number; i++)
    {
        printf("Enter name for Center %d: ", i + 1);

        scanf(" %29[^\n]", centers[i]);
    }

    centerCount = number;

    printf("\n%d exam centers created successfully.\n",
           centerCount);
}


/* Open preference window */

void openPreferenceWindow()
{
    if(centerCount < 3)
    {
        printf("\nCreate at least 3 centers first.\n");
        return;
    }

    if(allocationLocked)
    {
        printf("\nAllocation is already locked.\n");
        return;
    }

    if(preferencesApproved)
    {
        printf("\nPreferences have already been approved.\n");
        return;
    }

    preferenceWindowOpen = 1;

    printf("\nPreference window is now OPEN.\n");
    printf("Students can submit or update their priorities.\n");
}


/* Close preference window */

void closePreferenceWindow()
{
    if(!preferenceWindowOpen)
    {
        printf("\nPreference window is already closed.\n");
        return;
    }

    preferenceWindowOpen = 0;

    printf("\nPreference window CLOSED.\n");
    printf("Students can no longer modify their preferences.\n");
}


/* Approve all submitted preferences */

void approvePreferences()
{
    int i;
    int approvedCount = 0;
    int incompleteCount = 0;

    if(centerCount < 3)
    {
        printf("\nNo valid center configuration exists.\n");
        return;
    }

    if(preferenceWindowOpen)
    {
        printf("\nClose the preference window first.\n");
        return;
    }

    if(studentCount == 0)
    {
        printf("\nNo students are registered.\n");
        return;
    }

    if(preferencesApproved)
    {
        printf("\nPreferences are already approved.\n");
        return;
    }

    printf("\n========== PREFERENCE APPROVAL ==========\n");

    for(i = 0; i < studentCount; i++)
    {
        if(students[i].preference[0] != -1 &&
           students[i].preference[1] != -1 &&
           students[i].preference[2] != -1)
        {
            approvedCount++;

            addHistory(
                i,
                "Center preferences approved by authority"
            );
        }
        else
        {
            incompleteCount++;
        }
    }

    if(approvedCount == 0)
    {
        printf("\nNo complete preferences were submitted.\n");
        return;
    }

    preferencesApproved = 1;

    printf("\nPreference approval completed.\n");

    printf("Complete preferences : %d\n", approvedCount);
    printf("Incomplete preferences: %d\n", incompleteCount);

    if(incompleteCount > 0)
    {
        printf("\nStudents with incomplete preferences may remain\n");
        printf("unallocated during the allocation process.\n");
    }
}


/* Search for a student */

void searchCandidate()
{
    char id[20];

    int index;

    printf("\n========== SEARCH CANDIDATE ==========\n");

    printf("Enter Candidate ID: ");
    scanf("%19s", id);

    index = findStudent(id);

    if(index == -1)
    {
        printf("\nCandidate not found.\n");
        return;
    }

    printf("\nCandidate found.\n");

    printf("\nCandidate ID : %s\n",
           students[index].id);

    printf("Name         : %s\n",
           students[index].name);

    printf("Branch       : %s\n",
           students[index].branch);

    printf("Status       : ");

    if(students[index].status == PENDING)
    {
        printf("Pending\n");
    }
    else if(students[index].status == ALLOCATED)
    {
        printf("Allocated\n");
    }
    else
    {
        printf("Locked\n");
    }

    if(students[index].preference[0] != -1)
    {
        printf("\nPreferences:\n");

        printf("1. %s\n",
               centers[students[index].preference[0] - 1]);

        printf("2. %s\n",
               centers[students[index].preference[1] - 1]);

        printf("3. %s\n",
               centers[students[index].preference[2] - 1]);
    }

    printf("\nAllocation:\n");

    printf("Center : %s\n",
           students[index].allocatedCenter);

    printf("Hall   : %s\n",
           students[index].hall);

    printf("Seat   : %s\n",
           students[index].seat);

    printf("======================================\n");
}


/* Display authority audit history */

void displayAuditHistory()
{
    int i;
    int j;

    printf("\n========== SYSTEM AUDIT HISTORY ==========\n");

    if(studentCount == 0)
    {
        printf("No student records available.\n");
        return;
    }

    for(i = 0; i < studentCount; i++)
    {
        printf("\nCandidate: %s (%s)\n",
               students[i].id,
               students[i].name);

        if(historyCount[i] == 0)
        {
            printf("  No history.\n");
        }
        else
        {
            for(j = 0; j < historyCount[i]; j++)
            {
                printf("  %d. %s\n",
                       j + 1,
                       history[i][j].action);
            }
        }
    }

    printf("\n===========================================\n");
}


/* Display authority dashboard */

void authorityDashboard()
{
    int i;

    int allocated = 0;
    int pending = 0;

    printf("\n========== AUTHORITY DASHBOARD ==========\n");

    printf("Total Students : %d\n", studentCount);

    printf("Total Centers  : %d\n", centerCount);

    for(i = 0; i < studentCount; i++)
    {
        if(students[i].status == PENDING)
        {
            pending++;
        }
        else
        {
            allocated++;
        }
    }

    printf("Allocated      : %d\n", allocated);

    printf("Pending        : %d\n", pending);

    printf("\nPreference Window : ");

    if(preferenceWindowOpen)
    {
        printf("OPEN\n");
    }
    else
    {
        printf("CLOSED\n");
    }

    printf("Preferences      : ");

    if(preferencesApproved)
    {
        printf("APPROVED\n");
    }
    else
    {
        printf("NOT APPROVED\n");
    }

    printf("Allocation       : ");

    if(allocationLocked)
    {
        printf("LOCKED\n");
    }
    else
    {
        printf("NOT LOCKED\n");
    }

    printf("Reserve Capacity : %d%%\n",
           reservePercent);

    printf("Usable Seats/Hall: %d\n",
           getUsableSeats());

    printf("=========================================\n");
}


/* Display center information */

void displayCenterInformation()
{
    int i;

    printf("\n========== CENTER INFORMATION ==========\n");

    if(centerCount == 0)
    {
        printf("No centers created.\n");
        return;
    }

    for(i = 0; i < centerCount; i++)
    {
        printf("%d. %s\n",
               i + 1,
               centers[i]);

        printf("   Hall Capacity : %d\n",
               ROWS * COLS);

        printf("   Reserve Seats : %d\n",
               ((ROWS * COLS) * reservePercent) / 100);

        printf("   Usable Seats  : %d\n",
               getUsableSeats());
    }

    printf("=========================================\n");
}


/* Authority portal */

void authorityPortal()
{
    int choice;

    if(!authorityLogin())
    {
        return;
    }

    do
    {
        printf("\n\n========== AUTHORITY PORTAL ==========\n");

        printf("1.  Create Exam Centers\n");
        printf("2.  View Center Information\n");
        printf("3.  Open Preference Window\n");
        printf("4.  Close Preference Window\n");
        printf("5.  Approve Preferences\n");
        printf("6.  Search Candidate\n");
        printf("7.  View Audit History\n");
        printf("8.  View Dashboard\n");
        printf("9.  Generate Allocation\n");
        printf("10. Lock Final Allocation\n");
        printf("11. Archive & Reset Exam\n");
        printf("12. Logout\n");

        printf("======================================\n");

        printf("Enter choice: ");

        if(scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input.\n");
            while(getchar() != '\n');
            continue;
        }

        switch(choice)
        {
            case 1:
                addCenters();
                break;

            case 2:
                displayCenterInformation();
                break;

            case 3:
                openPreferenceWindow();
                break;

            case 4:
                closePreferenceWindow();
                break;

            case 5:
                approvePreferences();
                break;

            case 6:
                searchCandidate();
                break;

            case 7:
                displayAuditHistory();
                break;

            case 8:
                authorityDashboard();
                break;

            case 9:
                generateAllocation();
                break;

            case 10:
                lockAllocation();
                break;

            case 11:
                archiveAndReset();
                break;

            case 12:
                printf("\nAuthority logged out successfully.\n");
                break;

            default:
                printf("\nInvalid choice.\n");
        }

    } while(choice != 12);
}
/* ================================================= */
/*              ALLOCATION FUNCTIONS                 */
/* ================================================= */


/* Track allocated seats separately for each center */

int centerAllocated[MAX_CENTERS];


/* Reset center allocation counters */

void initializeCenterAllocation()
{
    int i;

    for(i = 0; i < MAX_CENTERS; i++)
    {
        centerAllocated[i] = 0;
    }
}


/* Find next available center with capacity */

int findFallbackCenter()
{
    int i;

    for(i = 0; i < centerCount; i++)
    {
        if(centerAllocated[i] < getUsableSeats())
        {
            return i;
        }
    }

    return -1;
}


/* Assign a seat number based on allocation count */

void assignSeat(int centerIndex, int studentIndex)
{
    int seatNumber;
    int row;
    int column;

    seatNumber = centerAllocated[centerIndex] + 1;

    row = ((seatNumber - 1) / COLS) + 1;

    column = ((seatNumber - 1) % COLS) + 1;

    strcpy(
        students[studentIndex].allocatedCenter,
        centers[centerIndex]
    );

    strcpy(
        students[studentIndex].hall,
        "Hall-1"
    );

    sprintf(
        students[studentIndex].seat,
        "R%d-C%d",
        row,
        column
    );

    centerAllocated[centerIndex]++;
}


/* Allocate one student according to preferences */

int allocateStudent(int studentIndex)
{
    int i;
    int preferenceCenter;
    int selectedCenter = -1;

    /*
       Try preferences in priority order:
       Preference 1 -> Preference 2 -> Preference 3
    */

    for(i = 0; i < 3; i++)
    {
        preferenceCenter =
            students[studentIndex].preference[i] - 1;

        if(preferenceCenter >= 0 &&
           preferenceCenter < centerCount)
        {
            if(centerAllocated[preferenceCenter] <
               getUsableSeats())
            {
                selectedCenter = preferenceCenter;
                break;
            }
        }
    }


    /*
       If all preferred centers are full,
       use another available center.
    */

    if(selectedCenter == -1)
    {
        selectedCenter = findFallbackCenter();
    }


    /* No capacity anywhere */

    if(selectedCenter == -1)
    {
        return 0;
    }


    /* Assign seat */

    assignSeat(
        selectedCenter,
        studentIndex
    );


    students[studentIndex].status = ALLOCATED;


    addHistory(
        studentIndex,
        "Exam seat allocated"
    );


    return 1;
}


/* Generate complete exam allocation */

void generateAllocation()
{
    int i;

    int allocatedCount = 0;
    int failedCount = 0;


    if(centerCount < 3)
    {
        printf("\nCreate exam centers first.\n");
        return;
    }


    if(preferenceWindowOpen)
    {
        printf("\nClose the preference window first.\n");
        return;
    }


    if(!preferencesApproved)
    {
        printf("\nPreferences must be approved first.\n");
        return;
    }


    if(allocationLocked)
    {
        printf("\nAllocation is already locked.\n");
        return;
    }


    if(studentCount == 0)
    {
        printf("\nNo students are registered.\n");
        return;
    }


    printf("\n========== GENERATING ALLOCATION ==========\n");


    /*
       Start allocation from a clean state.
    */

    initializeCenterAllocation();


    for(i = 0; i < studentCount; i++)
    {
        /*
           Reset previous allocation information
           before generating a fresh allocation.
        */

        students[i].status = PENDING;

        strcpy(
            students[i].allocatedCenter,
            "Not Allocated"
        );

        strcpy(
            students[i].hall,
            "Not Assigned"
        );

        strcpy(
            students[i].seat,
            "Not Assigned"
        );
    }


    /*
       Allocate students one by one.
    */

    for(i = 0; i < studentCount; i++)
    {
        /*
           Students without complete preferences
           are not automatically assigned.
        */

        if(students[i].preference[0] == -1 ||
           students[i].preference[1] == -1 ||
           students[i].preference[2] == -1)
        {
            failedCount++;

            addHistory(
                i,
                "Allocation failed: incomplete preferences"
            );

            continue;
        }


        if(allocateStudent(i))
        {
            allocatedCount++;
        }
        else
        {
            failedCount++;

            addHistory(
                i,
                "Allocation failed: no usable seat available"
            );
        }
    }


    printf("\nAllocation generation completed.\n");

    printf("Successfully allocated : %d\n",
           allocatedCount);

    printf("Not allocated          : %d\n",
           failedCount);


    printf("\n========== CENTER UTILIZATION ==========\n");

    for(i = 0; i < centerCount; i++)
    {
        printf("%s : %d / %d usable seats occupied\n",
               centers[i],
               centerAllocated[i],
               getUsableSeats());
    }

    printf("========================================\n");
}


/* Lock final allocation */

void lockAllocation()
{
    int i;

    if(!preferencesApproved)
    {
        printf("\nPreferences must be approved first.\n");
        return;
    }


    if(preferenceWindowOpen)
    {
        printf("\nClose the preference window first.\n");
        return;
    }


    if(allocationLocked)
    {
        printf("\nAllocation is already locked.\n");
        return;
    }


    /*
       Make sure allocation has actually been generated.
    */

    for(i = 0; i < studentCount; i++)
    {
        if(students[i].status == ALLOCATED)
        {
            allocationLocked = 1;
            break;
        }
    }


    if(!allocationLocked)
    {
        printf("\nNo allocation has been generated yet.\n");
        return;
    }


    /*
       Mark allocated students as LOCKED.
    */

    for(i = 0; i < studentCount; i++)
    {
        if(students[i].status == ALLOCATED)
        {
            students[i].status = LOCKED;

            addHistory(
                i,
                "Final exam allocation locked"
            );
        }
    }


    printf("\n========================================\n");
    printf(" FINAL ALLOCATION SUCCESSFULLY LOCKED\n");
    printf("========================================\n");

    printf("Students can now view their final\n");
    printf("exam center, hall and seat assignment.\n");
}


/* Display final allocation report */

void displayAllocationReport()
{
    int i;

    printf("\n========== FINAL ALLOCATION REPORT ==========\n");

    if(studentCount == 0)
    {
        printf("No students available.\n");
        return;
    }


    for(i = 0; i < studentCount; i++)
    {
        printf("\nCandidate ID : %s\n",
               students[i].id);

        printf("Name         : %s\n",
               students[i].name);

        printf("Status       : ");


        if(students[i].status == PENDING)
        {
            printf("NOT ALLOCATED\n");
        }
        else if(students[i].status == ALLOCATED)
        {
            printf("ALLOCATED\n");
        }
        else
        {
            printf("LOCKED\n");
        }


        printf("Center       : %s\n",
               students[i].allocatedCenter);

        printf("Hall         : %s\n",
               students[i].hall);

        printf("Seat         : %s\n",
               students[i].seat);

        printf("---------------------------------------------\n");
    }
}


/* ================================================= */
/*                    ARCHIVE                         */
/* ================================================= */


/*
   Save the current examination records to a file
   before resetting the system.
*/

void archiveCurrentExam()
{
    FILE *file;

    int i;
    int j;


    file = fopen("exam_archive.txt", "a");


    if(file == NULL)
    {
        printf("\nUnable to create archive file.\n");
        return;
    }


    fprintf(
        file,
        "\n============================================\n"
    );

    fprintf(
        file,
        "          EXAMINATION ARCHIVE\n"
    );

    fprintf(
        file,
        "============================================\n"
    );


    fprintf(
        file,
        "Total Students : %d\n",
        studentCount
    );

    fprintf(
        file,
        "Total Centers  : %d\n",
        centerCount
    );


    fprintf(
        file,
        "\nCENTERS:\n"
    );


    for(i = 0; i < centerCount; i++)
    {
        fprintf(
            file,
            "%d. %s\n",
            i + 1,
            centers[i]
        );
    }


    fprintf(
        file,
        "\nSTUDENT ALLOCATIONS:\n"
    );


    for(i = 0; i < studentCount; i++)
    {
        fprintf(
            file,
            "\nCandidate ID : %s\n",
            students[i].id
        );

        fprintf(
            file,
            "Name         : %s\n",
            students[i].name
        );

        fprintf(
            file,
            "Branch       : %s\n",
            students[i].branch
        );

        fprintf(
            file,
            "Center       : %s\n",
            students[i].allocatedCenter
        );

        fprintf(
            file,
            "Hall         : %s\n",
            students[i].hall
        );

        fprintf(
            file,
            "Seat         : %s\n",
            students[i].seat
        );


        fprintf(
            file,
            "History:\n"
        );


        for(j = 0; j < historyCount[i]; j++)
        {
            fprintf(
                file,
                "  - %s\n",
                history[i][j].action
            );
        }
    }


    fprintf(
        file,
        "\n============================================\n"
    );


    fclose(file);


    printf("\nPrevious examination archived successfully.\n");
}


/* ================================================= */
/*                  RESET EXAM                        */
/* ================================================= */


void archiveAndReset()
{
    char confirmation[10];


    if(studentCount == 0 && centerCount == 0)
    {
        printf("\nThere is no examination data to reset.\n");
        return;
    }


    printf("\n========== ARCHIVE & RESET ==========\n");

    printf("This will:\n");

    printf("1. Archive the current examination.\n");
    printf("2. Remove current students from active memory.\n");
    printf("3. Remove current centers.\n");
    printf("4. Reset allocation status.\n");
    printf("5. Prepare the system for a NEW exam.\n");


    printf("\nType YES to continue: ");

    scanf("%9s", confirmation);


    if(strcmp(confirmation, "YES") != 0)
    {
        printf("\nReset cancelled.\n");
        return;
    }


    /*
       Preserve old examination first.
    */

    archiveCurrentExam();


    /*
       Reset all current data.
    */

    initializeSystem();


    initializeCenterAllocation();


    printf("\n========================================\n");
    printf(" CURRENT EXAMINATION RESET SUCCESSFULLY\n");
    printf("========================================\n");

    printf("Old records remain stored in:\n");
    printf("exam_archive.txt\n");

    printf("\nSystem is ready for a NEW examination.\n");
}


/* ================================================= */
/*                 MAIN MENU                          */
/* ================================================= */


void mainMenu()
{
    int choice;


    do
    {
        printf("\n\n");
        printf("============================================\n");
        printf("       ONLINE EXAM SEATING SYSTEM\n");
        printf("============================================\n");

        printf("1. Student Registration\n");
        printf("2. Student Login\n");
        printf("3. Authority Login\n");
        printf("4. Exit\n");

        printf("============================================\n");

        printf("Enter choice: ");


        if(scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input.\n");

            while(getchar() != '\n');

            continue;
        }


        switch(choice)
        {
            case 1:

                studentRegistration();

                break;


            case 2:

                studentPortal();

                break;


            case 3:

                authorityPortal();

                break;


            case 4:

                printf("\nThank you for using the system.\n");

                break;


            default:

                printf("\nInvalid choice.\n");
        }


    } while(choice != 4);
}


/* ================================================= */
/*                    MAIN                            */
/* ================================================= */


int main()
{
    initializeSystem();

    initializeCenterAllocation();

    mainMenu();

    return 0;
}