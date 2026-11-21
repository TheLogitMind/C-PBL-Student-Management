
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 4

typedef struct
{
    int courseID;
    char courseName[20];
    float fees;
} COURSE;

typedef struct students
{
    int id;
    char name[50];
    float totalFees;
    float paidFees;
    int courseID;
    char result[20];
    struct students *next;
} STD;

COURSE courses[MAX] = {
    {1, "DS", 8000},
    {2, "AI", 12000},
    {3, "ML", 10000},
    {4, "Web", 7000}
};

STD *head = NULL;

// Function declarations
void registerStudent();
void selectCourse();
void payFees();
void enterResult();
void displayStudents();
void searchStudent();
void manageFees();
void loadData();
void saveData();
STD *findStudent(int id);

// Find student
STD *findStudent(int id)
{
    STD *temp = head;

    while (temp != NULL)
    {
        if (temp->id == id)
            return temp;

        temp = temp->next;
    }

    return NULL;
}

// Register student
void registerStudent()
{
    STD *newNode, *temp;
    int id;

    newNode = (STD *)malloc(sizeof(STD));

    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter Student ID: ");
    scanf("%d", &id);

    if (id <= 0 || findStudent(id) != NULL)
    {
        printf("Invalid or duplicate ID!\n");
        free(newNode);
        return;
    }

    newNode->id = id;

    printf("Enter Student Name: ");
    scanf(" %49[^\n]", newNode->name);

    newNode->totalFees = 0;
    newNode->paidFees = 0;
    newNode->courseID = 0;
    strcpy(newNode->result, "Pending");
    newNode->next = NULL;

    if (head == NULL)
        head = newNode;
    else
    {
        temp = head;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newNode;
    }

    saveData();
    printf("Student Registered Successfully!\n");
}

// Select course
void selectCourse()
{
    STD *student;
    int id, choice, i;

    printf("Enter Student ID: ");
    scanf("%d", &id);

    student = findStudent(id);

    if (student == NULL)
    {
        printf("Student Not Found!\n");
        return;
    }

    if (student->courseID != 0 || student->paidFees > 0)
    {
        printf("Course already selected or payment exists!\n");
        return;
    }

    printf("\nAvailable Courses:\n");

    for (i = 0; i < MAX; i++)
        printf("%d. %s - Rs. %.2f\n",
               courses[i].courseID,
               courses[i].courseName,
               courses[i].fees);

    printf("Select Course ID: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > MAX)
    {
        printf("Invalid Course!\n");
        return;
    }

    student->courseID = choice;
    student->totalFees = courses[choice - 1].fees;

    saveData();

    printf("Course Selected Successfully!\n");
    printf("Total Fees: Rs. %.2f\n", student->totalFees);
}

// Pay fees
void payFees()
{
    STD *student;
    int id;
    float amount, due;

    printf("Enter Student ID: ");
    scanf("%d", &id);

    student = findStudent(id);

    if (student == NULL)
    {
        printf("Student Not Found!\n");
        return;
    }

    if (student->courseID == 0)
    {
        printf("Select a Course First!\n");
        return;
    }

    due = student->totalFees - student->paidFees;

    printf("Total Fees: Rs. %.2f\n", student->totalFees);
    printf("Paid Fees: Rs. %.2f\n", student->paidFees);
    printf("Due Fees: Rs. %.2f\n", due);

    if (due <= 0)
    {
        printf("Fees Already Paid!\n");
        return;
    }

    printf("Enter Payment Amount: ");
    scanf("%f", &amount);

    if (amount <= 0 || amount > due)
    {
        printf("Invalid Payment Amount!\n");
        return;
    }

    student->paidFees += amount;

    saveData();

    printf("Payment Successful!\n");

    if (student->paidFees >= student->totalFees)
        printf("Course Allocation Confirmed!\n");
    else
        printf("Remaining Due: Rs. %.2f\n",
               student->totalFees - student->paidFees);
}

// Enter result
void enterResult()
{
    STD *student;
    int id, choice;

    printf("Enter Student ID: ");
    scanf("%d", &id);

    student = findStudent(id);

    if (student == NULL)
    {
        printf("Student Not Found!\n");
        return;
    }

    if (student->courseID == 0 ||
        student->paidFees < student->totalFees)
    {
        printf("Complete Course Fee Payment First!\n");
        return;
    }

    printf("\n1. Excellent\n2. Good\n3. Fail\n");
    printf("Enter Result: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            strcpy(student->result, "Excellent");
            break;

        case 2:
            strcpy(student->result, "Good");
            break;

        case 3:
            strcpy(student->result, "Fail");
            break;

        default:
            printf("Invalid Choice!\n");
            return;
    }

    saveData();
    printf("Result Updated Successfully!\n");
}

// Display all students
void displayStudents()
{
    STD *temp = head;

    if (temp == NULL)
    {
        printf("No Student Records Found!\n");
        return;
    }

    while (temp != NULL)
    {
        printf("\nID: %d", temp->id);
        printf("\nName: %s", temp->name);

        if (temp->courseID > 0)
            printf("\nCourse: %s",
                   courses[temp->courseID - 1].courseName);
        else
            printf("\nCourse: Not Selected");

        printf("\nTotal Fees: %.2f", temp->totalFees);
        printf("\nPaid Fees: %.2f", temp->paidFees);
        printf("\nDue Fees: %.2f",
               temp->totalFees - temp->paidFees);
        printf("\nResult: %s\n", temp->result);

        temp = temp->next;
    }
}

// Search student
void searchStudent()
{
    STD *student;
    int id;

    printf("Enter Student ID: ");
    scanf("%d", &id);

    student = findStudent(id);

    if (student == NULL)
    {
        printf("Student Not Found!\n");
        return;
    }

    printf("\nID: %d\n", student->id);
    printf("Name: %s\n", student->name);

    if (student->courseID > 0)
        printf("Course: %s\n",
               courses[student->courseID - 1].courseName);
    else
        printf("Course: Not Selected\n");

    printf("Total Fees: %.2f\n", student->totalFees);
    printf("Paid Fees: %.2f\n", student->paidFees);
    printf("Due Fees: %.2f\n",
           student->totalFees - student->paidFees);
    printf("Result: %s\n", student->result);
}

// Admin updates course fees
void manageFees()
{
    int choice, i;
    float newFees;

    for (i = 0; i < MAX; i++)
        printf("%d. %s - Rs. %.2f\n",
               courses[i].courseID,
               courses[i].courseName,
               courses[i].fees);

    printf("Enter Course ID: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > MAX)
    {
        printf("Invalid Course ID!\n");
        return;
    }

    printf("Enter New Course Fee: ");
    scanf("%f", &newFees);

    if (newFees <= 0)
    {
        printf("Invalid Fee!\n");
        return;
    }

    courses[choice - 1].fees = newFees;

    FILE *fp = fopen("courses.txt", "w");

    if (fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    for (i = 0; i < MAX; i++)
        fprintf(fp, "%d %s %.2f\n",
                courses[i].courseID,
                courses[i].courseName,
                courses[i].fees);

    fclose(fp);
    printf("Course Fee Updated!\n");
}

// Save student records
void saveData()
{
    FILE *fp;
    STD *temp = head;

    fp = fopen("students.txt", "w");

    if (fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    while (temp != NULL)
    {
        fprintf(fp, "%d|%s|%.2f|%.2f|%d|%s\n",
                temp->id, temp->name,
                temp->totalFees, temp->paidFees,
                temp->courseID, temp->result);

        temp = temp->next;
    }

    fclose(fp);
}

// Load student records
void loadData()
{
    FILE *fp;
    STD *newNode, *temp;
    int id, courseID;
    char name[50], result[20];
    float totalFees, paidFees;

    fp = fopen("students.txt", "r");

    if (fp == NULL)
        return;

    while (fscanf(fp, "%d|%49[^|]|%f|%f|%d|%19[^\n]\n",
                  &id, name, &totalFees, &paidFees,
                  &courseID, result) == 6)
    {
        if (id <= 0 || findStudent(id) != NULL ||
            totalFees < 0 || paidFees < 0 ||
            paidFees > totalFees ||
            courseID < 0 || courseID > MAX ||
            (courseID == 0 && totalFees != 0) ||
            (courseID > 0 && totalFees <= 0))
            continue;

        newNode = (STD *)malloc(sizeof(STD));

        if (newNode == NULL)
            break;

        newNode->id = id;
        strcpy(newNode->name, name);
        newNode->totalFees = totalFees;
        newNode->paidFees = paidFees;
        newNode->courseID = courseID;
        strcpy(newNode->result, result);
        newNode->next = NULL;

        if (head == NULL)
            head = newNode;
        else
        {
            temp = head;

            while (temp->next != NULL)
                temp = temp->next;

            temp->next = newNode;
        }
    }

    fclose(fp);
}

int main()
{
    int choice;
    char username[30], password[30];

    // Admin Authentication
    printf("User ID: ");
    scanf("%29s", username);

    printf("Password: ");
    scanf("%29s", password);

    if (strcmp(username, "admin") != 0 ||
        strcmp(password, "1234") != 0)
    {
        printf("Invalid Login!\n");
        return 0;
    }

    // Load saved records
    {
        FILE *fp = fopen("courses.txt", "r");

        if (fp != NULL)
        {
            int id;
            char name[20];
            float fee;

            while (fscanf(fp, "%d %19s %f",
                          &id, name, &fee) == 3)
            {
                if (id >= 1 && id <= MAX &&
                    strcmp(name, courses[id - 1].courseName) == 0 &&
                    fee > 0)
                    courses[id - 1].fees = fee;
            }

            fclose(fp);
        }
    }

    loadData();

    do
    {
        printf("\n===== INSTITUTE MANAGEMENT SYSTEM =====\n");
        printf("1. Register Student\n");
        printf("2. Select Course\n");
        printf("3. Pay Fees\n");
        printf("4. Enter Result\n");
        printf("5. Display Students\n");
        printf("6. Search Student\n");
        printf("7. Manage Course Fees\n");
        printf("0. Exit\n");

        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1: registerStudent(); break;
            case 2: selectCourse(); break;
            case 3: payFees(); break;
            case 4: enterResult(); break;
            case 5: displayStudents(); break;
            case 6: searchStudent(); break;
            case 7: manageFees(); break;
            case 0: printf("Program Closed!\n"); break;
            default: printf("Invalid Choice!\n");
        }

    } while (choice != 0);

    // Free allocated memory
    while (head != NULL)
    {
        STD *temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
