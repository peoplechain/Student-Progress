#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

// Structure definition for student personal details
struct PersonalInfo {
    char name[50];
    int roll_number;
    char address[100];
    char class_name[20];
    int age;
};

// Structure definition for student marks
struct Marks {
    float quarterly_marks[5];
    float halfyearly_marks[5];
    float annual_marks[5];
    float quarterly_total; // Total marks out of 500 for Quarterly Exam
    float halfyearly_total; // Total marks out of 500 for Half Yearly Exam
    float annual_total; // Total marks out of 500 for Annual Exam
    float quarterly_percentage;
    char quarterly_grade;
    float halfyearly_percentage;
    char halfyearly_grade;
    float annual_percentage;
    char annual_grade;
};

// Structure definition for student
struct Student {
    struct PersonalInfo personal_info;
    struct Marks marks;
};

void createNewDatabase();
void viewOrEditDatabase();
void viewStudentDetails(int roll_number);
void viewAllStudentDetails();
void editStudentDetails(int roll_number);
void deleteStudentRecord(int roll_number);
float calculateTotalMarks(float marks[]);
float calculatePercentage(float total_marks, float max_marks);
char calculateGrade(float percentage);
void remove_directory_if_empty(const char *dir_name);

int main() {
    char choice;

    do {
        printf("\nMain Menu:\n");
        printf("1. Create new student database\n");
        printf("2. View, edit, or delete an existing student database\n");
        printf("3. View all students details\n");
        printf("4. Exit\n");
        printf("Enter your choice (1/2/3/4): ");
        scanf(" %c", &choice);

        switch(choice) {
            case '1':
                createNewDatabase();
                break;
            case '2':
                viewOrEditDatabase();
                break;
            case '3':
                viewAllStudentDetails();
                break;
            case '4':
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please enter 1, 2, 3, or 4.\n");
        }
    } while(choice != '4');

    return 0;
}

void createNewDatabase() {
    struct Student newStudent;
    const char *subjects[] = {"English", "Tamil", "Maths", "Science", "Social Science"};
    int num_subjects = sizeof(subjects) / sizeof(subjects[0]);

    // Input details from user
    printf("\nEnter details of the new student:\n");
    printf("Name: ");
    scanf(" %[^\n]", newStudent.personal_info.name);
    printf("Roll Number: ");
    scanf("%d", &newStudent.personal_info.roll_number);
    printf("Address: ");
    scanf(" %[^\n]", newStudent.personal_info.address);
    printf("Class: ");
    scanf(" %[^\n]", newStudent.personal_info.class_name);
    printf("Age: ");
    scanf("%d", &newStudent.personal_info.age);

    // Input marks for each subject for Quarterly Exam
    printf("\nEnter marks for Quarterly Exam (out of 100):\n");
    for (int i = 0; i < num_subjects; ++i) {
        printf("%s: ", subjects[i]);
        scanf("%f", &newStudent.marks.quarterly_marks[i]);
    }

    // Calculate total marks for Quarterly Exam (out of 500)
    newStudent.marks.quarterly_total = calculateTotalMarks(newStudent.marks.quarterly_marks);

    // Calculate percentage and grade for Quarterly Exam
    newStudent.marks.quarterly_percentage = calculatePercentage(newStudent.marks.quarterly_total, 500);
    newStudent.marks.quarterly_grade = calculateGrade(newStudent.marks.quarterly_percentage);

    // Input marks for each subject for Half Yearly Exam
    printf("\nEnter marks for Half Yearly Exam (out of 100):\n");
    for (int i = 0; i < num_subjects; ++i) {
        printf("%s: ", subjects[i]);
        scanf("%f", &newStudent.marks.halfyearly_marks[i]);
    }

    // Calculate total marks for Half Yearly Exam (out of 500)
    newStudent.marks.halfyearly_total = calculateTotalMarks(newStudent.marks.halfyearly_marks);

    // Calculate percentage and grade for Half Yearly Exam
    newStudent.marks.halfyearly_percentage = calculatePercentage(newStudent.marks.halfyearly_total, 500);
    newStudent.marks.halfyearly_grade = calculateGrade(newStudent.marks.halfyearly_percentage);

    // Input marks for each subject for Annual Exam
    printf("\nEnter marks for Annual Exam (out of 100):\n");
    for (int i = 0; i < num_subjects; ++i) {
        printf("%s: ", subjects[i]);
        scanf("%f", &newStudent.marks.annual_marks[i]);
    }

    // Calculate total marks for Annual Exam (out of 500)
    newStudent.marks.annual_total = calculateTotalMarks(newStudent.marks.annual_marks);

    // Calculate percentage and grade for Annual Exam
    newStudent.marks.annual_percentage = calculatePercentage(newStudent.marks.annual_total, 500);
    newStudent.marks.annual_grade = calculateGrade(newStudent.marks.annual_percentage);

    // Create folders if they don't exist
    #ifdef _WIN32
    _mkdir("student_database");                // Main folder to contain all student data
    _mkdir("student_database/personal_info");  // Folder for personal info
    _mkdir("student_database/quarterly_marks");// Folder for quarterly marks info
    _mkdir("student_database/halfyearly_marks");// Folder for half yearly marks info
    _mkdir("student_database/annual_marks");   // Folder for annual marks info
    #else
    mkdir("student_database", 0777);                // Main folder to contain all student data
    mkdir("student_database/personal_info", 0777);  // Folder for personal info
    mkdir("student_database/quarterly_marks", 0777);// Folder for quarterly marks info
    mkdir("student_database/halfyearly_marks", 0777);// Folder for half yearly marks info
    mkdir("student_database/annual_marks", 0777);   // Folder for annual marks info
    #endif

    // Save personal details to file in 'personal_info' folder
    char personal_filename[100];
    sprintf(personal_filename, "student_database/personal_info/%d.txt", newStudent.personal_info.roll_number);
    FILE *personal_fp = fopen(personal_filename, "w");
    if (personal_fp == NULL) {
        printf("Error creating personal details file.\n");
        return;
    }
    fprintf(personal_fp, "Name: %s\n", newStudent.personal_info.name);
    fprintf(personal_fp, "Roll Number: %d\n", newStudent.personal_info.roll_number);
    fprintf(personal_fp, "Address: %s\n", newStudent.personal_info.address);
    fprintf(personal_fp, "Class: %s\n", newStudent.personal_info.class_name);
    fprintf(personal_fp, "Age: %d\n", newStudent.personal_info.age);
    fclose(personal_fp);

    // Save marks details to file in respective folders
    char marks_filename[100];

    // Quarterly marks details
    sprintf(marks_filename, "student_database/quarterly_marks/%d.txt", newStudent.personal_info.roll_number);
    FILE *quarterly_marks_fp = fopen(marks_filename, "w");
    if (quarterly_marks_fp == NULL) {
        printf("Error creating quarterly marks details file.\n");
        return;
    }
    fprintf(quarterly_marks_fp, "Quarterly Exam Details:\n");
    for (int i = 0; i < num_subjects; ++i) {
        fprintf(quarterly_marks_fp, "%s: %.2f\n", subjects[i], newStudent.marks.quarterly_marks[i]);
    }
    fprintf(quarterly_marks_fp, "Percentage: %.2f\n", newStudent.marks.quarterly_percentage);
    fprintf(quarterly_marks_fp, "Grade: %c\n", newStudent.marks.quarterly_grade);
    fclose(quarterly_marks_fp);

    // Half Yearly marks details
    sprintf(marks_filename, "student_database/halfyearly_marks/%d.txt", newStudent.personal_info.roll_number);
    FILE *halfyearly_marks_fp = fopen(marks_filename, "w");
    if (halfyearly_marks_fp == NULL) {
        printf("Error creating half yearly marks details file.\n");
        return;
    }
    fprintf(halfyearly_marks_fp, "Half Yearly Exam Details:\n");
    for (int i = 0; i < num_subjects; ++i) {
        fprintf(halfyearly_marks_fp, "%s: %.2f\n", subjects[i], newStudent.marks.halfyearly_marks[i]);
    }
    fprintf(halfyearly_marks_fp, "Percentage: %.2f\n", newStudent.marks.halfyearly_percentage);
    fprintf(halfyearly_marks_fp, "Grade: %c\n", newStudent.marks.halfyearly_grade);
    fclose(halfyearly_marks_fp);

    // Annual marks details
    sprintf(marks_filename, "student_database/annual_marks/%d.txt", newStudent.personal_info.roll_number);
    FILE *annual_marks_fp = fopen(marks_filename, "w");
    if (annual_marks_fp == NULL) {
        printf("Error creating annual marks details file.\n");
        return;
    }
    fprintf(annual_marks_fp, "Annual Exam Details:\n");
    for (int i = 0; i < num_subjects; ++i) {
        fprintf(annual_marks_fp, "%s: %.2f\n", subjects[i], newStudent.marks.annual_marks[i]);
    }
    fprintf(annual_marks_fp, "Percentage: %.2f\n", newStudent.marks.annual_percentage);
    fprintf(annual_marks_fp, "Grade: %c\n", newStudent.marks.annual_grade);
    fclose(annual_marks_fp);

    printf("\nNew student details added successfully.\n");
}

void viewOrEditDatabase() {
    char choice;
    int roll_number;

    printf("\nView or Edit Student Database:\n");
    printf("1. View student details\n");
    printf("2. Edit student details\n");
    printf("3. Delete student record\n");
    printf("4. Go back to main menu\n");
    printf("Enter your choice (1/2/3/4): ");
    scanf(" %c", &choice);

    switch(choice) {
        case '1':
            printf("Enter roll number of student to view details: ");
            scanf("%d", &roll_number);
            viewStudentDetails(roll_number);
            break;
        case '2':
            printf("Enter roll number of student to edit details: ");
            scanf("%d", &roll_number);
            editStudentDetails(roll_number);
            break;
        case '3':
            printf("Enter roll number of student to delete record: ");
            scanf("%d", &roll_number);
            deleteStudentRecord(roll_number);
            break;
        case '4':
            printf("Returning to main menu.\n");
            break;
        default:
            printf("Invalid choice. Please enter 1, 2, 3, or 4.\n");
    }
}

void viewStudentDetails(int roll_number) {
    char personal_filename[100];
    char marks_filename[100];
    char buffer[256];

    printf("\nStudent Details (Roll Number: %d):\n", roll_number);

    // View personal details
    sprintf(personal_filename, "student_database/personal_info/%d.txt", roll_number);
    FILE *personal_fp = fopen(personal_filename, "r");
    if (personal_fp == NULL) {
        printf("Error: Student with roll number %d not found.\n", roll_number);
        return;
    }
    while (fgets(buffer, sizeof(buffer), personal_fp) != NULL) {
        printf("%s", buffer);
    }
    fclose(personal_fp);

    // View quarterly marks details
    sprintf(marks_filename, "student_database/quarterly_marks/%d.txt", roll_number);
    FILE *quarterly_marks_fp = fopen(marks_filename, "r");
    if (quarterly_marks_fp != NULL) {
        printf("\nQuarterly Exam Details:\n");
        while (fgets(buffer, sizeof(buffer), quarterly_marks_fp) != NULL) {
            printf("%s", buffer);
        }
        fclose(quarterly_marks_fp);
    }

    // View half yearly marks details
    sprintf(marks_filename, "student_database/halfyearly_marks/%d.txt", roll_number);
    FILE *halfyearly_marks_fp = fopen(marks_filename, "r");
    if (halfyearly_marks_fp != NULL) {
        printf("\nHalf Yearly Exam Details:\n");
        while (fgets(buffer, sizeof(buffer), halfyearly_marks_fp) != NULL) {
            printf("%s", buffer);
        }
        fclose(halfyearly_marks_fp);
    }

    // View annual marks details
    sprintf(marks_filename, "student_database/annual_marks/%d.txt", roll_number);
    FILE *annual_marks_fp = fopen(marks_filename, "r");
    if (annual_marks_fp != NULL) {
        printf("\nAnnual Exam Details:\n");
        while (fgets(buffer, sizeof(buffer), annual_marks_fp) != NULL) {
            printf("%s", buffer);
        }
        fclose(annual_marks_fp);
    }
}

void viewAllStudentDetails() {
    struct dirent *entry;
    DIR *dp;
    char buffer[256];

    printf("\nAll Students Details:\n");

    // View all personal details
    dp = opendir("student_database/personal_info");
    if (dp == NULL) {
        printf("Error: No student records found.\n");
        return;
    }
    printf("%-10s %-20s %-10s %-30s %-10s %-10s\n", "Roll No", "Name", "Age", "Address", "Class", "Grade");
    printf("-----------------------------------------------------------------------------------------\n");
    while ((entry = readdir(dp))) {
        if (entry->d_name[0] == '.') continue;
        char personal_filename[100];
        sprintf(personal_filename, "student_database/personal_info/%s", entry->d_name);
        FILE *personal_fp = fopen(personal_filename, "r");
        if (personal_fp != NULL) {
            int roll_number;
            char name[50], address[100], class_name[20];
            int age;
            while (fgets(buffer, sizeof(buffer), personal_fp) != NULL) {
                sscanf(buffer, "Roll Number: %d", &roll_number);
                sscanf(buffer, "Name: %[^\n]", name);
                sscanf(buffer, "Address: %[^\n]", address);
                sscanf(buffer, "Class: %[^\n]", class_name);
                sscanf(buffer, "Age: %d", &age);
            }
            printf("%-10d %-20s %-10d %-30s %-10s\n", roll_number, name, age, address, class_name);
            fclose(personal_fp);
        }
    }
    closedir(dp);
}

void editStudentDetails(int roll_number) {
    printf("\nEditing details for student with roll number %d is not yet implemented.\n", roll_number);
}

void deleteStudentRecord(int roll_number) {
    char personal_filename[100];
    char quarterly_marks_filename[100];
    char halfyearly_marks_filename[100];
    char annual_marks_filename[100];

    sprintf(personal_filename, "student_database/personal_info/%d.txt", roll_number);
    sprintf(quarterly_marks_filename, "student_database/quarterly_marks/%d.txt", roll_number);
    sprintf(halfyearly_marks_filename, "student_database/halfyearly_marks/%d.txt", roll_number);
    sprintf(annual_marks_filename, "student_database/annual_marks/%d.txt", roll_number);

    // Delete personal details file
    if (remove(personal_filename) == 0) {
        printf("Deleted personal details for roll number %d.\n", roll_number);
    } else {
        printf("Error deleting personal details for roll number %d.\n", roll_number);
    }

    // Delete quarterly marks details file
    if (remove(quarterly_marks_filename) == 0) {
        printf("Deleted quarterly marks details for roll number %d.\n", roll_number);
    } else {
        printf("Error deleting quarterly marks details for roll number %d.\n", roll_number);
    }

    // Delete half yearly marks details file
    if (remove(halfyearly_marks_filename) == 0) {
        printf("Deleted half yearly marks details for roll number %d.\n", roll_number);
    } else {
        printf("Error deleting half yearly marks details for roll number %d.\n", roll_number);
    }

    // Delete annual marks details file
    if (remove(annual_marks_filename) == 0) {
        printf("Deleted annual marks details for roll number %d.\n", roll_number);
    } else {
        printf("Error deleting annual marks details for roll number %d.\n", roll_number);
    }

    // Try to remove directories if empty
    remove_directory_if_empty("student_database/annual_marks");
    remove_directory_if_empty("student_database/halfyearly_marks");
    remove_directory_if_empty("student_database/quarterly_marks");
    remove_directory_if_empty("student_database/personal_info");
    remove_directory_if_empty("student_database");
}

float calculateTotalMarks(float marks[]) {
    float total = 0;
    for (int i = 0; i < 5; ++i) {
        total += marks[i];
    }
    return total;
}

float calculatePercentage(float total_marks, float max_marks) {
    return (total_marks / max_marks) * 100;
}

char calculateGrade(float percentage) {
    if (percentage >= 90) {
        return 'A';
    } else if (percentage >= 75) {
        return 'B';
    } else if (percentage >= 50) {
        return 'C';
    } else if (percentage >= 35) {
        return 'D';
    } else {
        return 'F';
    }
}

void remove_directory_if_empty(const char *dir_name) {
    struct dirent *entry;
    DIR *dp = opendir(dir_name);
    if (dp == NULL) {
        return;
    }

    int is_empty = 1;
    while ((entry = readdir(dp))) {
        if (entry->d_name[0] == '.') continue;
        is_empty = 0;
        break;
    }
    closedir(dp);

    if (is_empty) {
        rmdir(dir_name);
    }
}
