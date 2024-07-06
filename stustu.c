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
    fprintf(quarterly_marks_fp, "+-----------------------+------------+\n");
    fprintf(quarterly_marks_fp, "| Subject               | Marks      |\n");
    fprintf(quarterly_marks_fp, "+-----------------------+------------+\n");
    for (int i = 0; i < num_subjects; ++i) {
        fprintf(quarterly_marks_fp, "| %-21s| %-11.2f|\n", subjects[i], newStudent.marks.quarterly_marks[i]);
    }
    fprintf(quarterly_marks_fp, "+-----------------------+------------+\n");
    fprintf(quarterly_marks_fp, "| Total                 | %-11.2f|\n", newStudent.marks.quarterly_total);
    fprintf(quarterly_marks_fp, "| Percentage            | %-11.2f|\n", newStudent.marks.quarterly_percentage);
    fprintf(quarterly_marks_fp, "| Grade                 | %-11c|\n", newStudent.marks.quarterly_grade);
    fprintf(quarterly_marks_fp, "+-----------------------+------------+\n");
    fclose(quarterly_marks_fp);

    // Half Yearly marks details
    sprintf(marks_filename, "student_database/halfyearly_marks/%d.txt", newStudent.personal_info.roll_number);
    FILE *halfyearly_marks_fp = fopen(marks_filename, "w");
    if (halfyearly_marks_fp == NULL) {
        printf("Error creating half yearly marks details file.\n");
        return;
    }
    fprintf(halfyearly_marks_fp, "Half Yearly Exam Details:\n");
    fprintf(halfyearly_marks_fp, "+-----------------------+------------+\n");
    fprintf(halfyearly_marks_fp, "| Subject               | Marks      |\n");
    fprintf(halfyearly_marks_fp, "+-----------------------+------------+\n");
    for (int i = 0; i < num_subjects; ++i) {
        fprintf(halfyearly_marks_fp, "| %-21s| %-11.2f|\n", subjects[i], newStudent.marks.halfyearly_marks[i]);
    }
    fprintf(halfyearly_marks_fp, "+-----------------------+------------+\n");
    fprintf(halfyearly_marks_fp, "| Total                 | %-11.2f|\n", newStudent.marks.halfyearly_total);
    fprintf(halfyearly_marks_fp, "| Percentage            | %-11.2f|\n", newStudent.marks.halfyearly_percentage);
    fprintf(halfyearly_marks_fp, "| Grade                 | %-11c|\n", newStudent.marks.halfyearly_grade);
    fprintf(halfyearly_marks_fp, "+-----------------------+------------+\n");
    fclose(halfyearly_marks_fp);

    // Annual marks details
    sprintf(marks_filename, "student_database/annual_marks/%d.txt", newStudent.personal_info.roll_number);
    FILE *annual_marks_fp = fopen(marks_filename, "w");
    if (annual_marks_fp == NULL) {
        printf("Error creating annual marks details file.\n");
        return;
    }
    fprintf(annual_marks_fp, "Annual Exam Details:\n");
    fprintf(annual_marks_fp, "+-----------------------+------------+\n");
    fprintf(annual_marks_fp, "| Subject               | Marks      |\n");
    fprintf(annual_marks_fp, "+-----------------------+------------+\n");
    for (int i = 0; i < num_subjects; ++i) {
        fprintf(annual_marks_fp, "| %-21s| %-11.2f|\n", subjects[i], newStudent.marks.annual_marks[i]);
    }
    fprintf(annual_marks_fp, "+-----------------------+------------+\n");
    fprintf(annual_marks_fp, "| Total                 | %-11.2f|\n", newStudent.marks.annual_total);
    fprintf(annual_marks_fp, "| Percentage            | %-11.2f|\n", newStudent.marks.annual_percentage);
    fprintf(annual_marks_fp, "| Grade                 | %-11c|\n", newStudent.marks.annual_grade);
    fprintf(annual_marks_fp, "+-----------------------+------------+\n");
    fclose(annual_marks_fp);

    printf("\nNew student record created successfully for roll number %d.\n", newStudent.personal_info.roll_number);
}

void viewOrEditDatabase() {
    int roll_number;
    char choice;
    do {
        printf("\nView or Edit Existing Database:\n");
        printf("1. View student details\n");
        printf("2. Edit student details\n");
        printf("3. Delete student record\n");
        printf("4. Return to main menu\n");
        printf("Enter your choice (1/2/3/4): ");
        scanf(" %c", &choice);

        switch(choice) {
            case '1':
                printf("Enter roll number of the student: ");
                scanf("%d", &roll_number);
                viewStudentDetails(roll_number);
                break;
            case '2':
                printf("Enter roll number of the student: ");
                scanf("%d", &roll_number);
                editStudentDetails(roll_number);
                break;
            case '3':
                printf("Enter roll number of the student to delete: ");
                scanf("%d", &roll_number);
                deleteStudentRecord(roll_number);
                break;
            case '4':
                printf("Returning to main menu.\n");
                break;
            default:
                printf("Invalid choice. Please enter 1, 2, 3, or 4.\n");
        }
    } while(choice != '4');
}

void viewStudentDetails(int roll_number) {
    char filename[100];
    FILE *fp;

    // Personal info file
    sprintf(filename, "student_database/personal_info/%d.txt", roll_number);
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Student record not found for roll number %d.\n", roll_number);
        return;
    }
    printf("\nPersonal Details:\n");
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);

    // Quarterly marks file
    sprintf(filename, "student_database/quarterly_marks/%d.txt", roll_number);
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Quarterly marks record not found for roll number %d.\n", roll_number);
        return;
    }
    printf("\nQuarterly Exam Details:\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);

    // Half Yearly marks file
    sprintf(filename, "student_database/halfyearly_marks/%d.txt", roll_number);
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Half yearly marks record not found for roll number %d.\n", roll_number);
        return;
    }
    printf("\nHalf Yearly Exam Details:\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);

    // Annual marks file
    sprintf(filename, "student_database/annual_marks/%d.txt", roll_number);
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Annual marks record not found for roll number %d.\n", roll_number);
        return;
    }
    printf("\nAnnual Exam Details:\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

void viewAllStudentDetails() {
    DIR *dir;
    struct dirent *ent;
    const char *folder_path[] = {"student_database/personal_info", "student_database/quarterly_marks", "student_database/halfyearly_marks", "student_database/annual_marks"};

    // Open personal_info directory to get list of all students
    dir = opendir(folder_path[0]);
    if (dir != NULL) {
        printf("\nList of all students:\n");
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                int roll_number;
                sscanf(ent->d_name, "%d.txt", &roll_number);
                printf("- Roll Number: %d\n", roll_number);
            }
        }
        closedir(dir);
    } else {
        printf("Error opening directory: %s\n", folder_path[0]);
    }
}

void editStudentDetails(int roll_number) {
    char filename[100];
    FILE *fp;
    char line[100];
    int choice;

    // Personal info file
    sprintf(filename, "student_database/personal_info/%d.txt", roll_number);
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Student record not found for roll number %d.\n", roll_number);
        return;
    }
    printf("\nCurrent Personal Details:\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);

    // Ask user which part to edit
    printf("\nEdit:\n");
    printf("1. Personal Details\n");
    printf("2. Quarterly Exam Details\n");
    printf("3. Half Yearly Exam Details\n");
    printf("4. Annual Exam Details\n");
    printf("Enter your choice (1/2/3/4): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter new details:\n");
            createNewDatabase(); // Reuse createNewDatabase to take input and overwrite existing file
            break;
        case 2:
            printf("Enter new Quarterly Exam Details:\n");
            // Reuse createNewDatabase to take input and overwrite existing file
            break;
        case 3:
            printf("Enter new Half Yearly Exam Details:\n");
            // Reuse createNewDatabase to take input and overwrite existing file
            break;
        case 4:
            printf("Enter new Annual Exam Details:\n");
            // Reuse createNewDatabase to take input and overwrite existing file
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }
}

void deleteStudentRecord(int roll_number) {
    char filename[100];
    sprintf(filename, "student_database/personal_info/%d.txt", roll_number);
    if (remove(filename) != 0) {
        printf("Error deleting personal info file for roll number %d.\n", roll_number);
    }

    sprintf(filename, "student_database/quarterly_marks/%d.txt", roll_number);
    if (remove(filename) != 0) {
        printf("Error deleting quarterly marks file for roll number %d.\n", roll_number);
    }

    sprintf(filename, "student_database/halfyearly_marks/%d.txt", roll_number);
    if (remove(filename) != 0) {
        printf("Error deleting half yearly marks file for roll number %d.\n", roll_number);
    }

    sprintf(filename, "student_database/annual_marks/%d.txt", roll_number);
    if (remove(filename) != 0) {
        printf("Error deleting annual marks file for roll number %d.\n", roll_number);
    }

    printf("Student record for roll number %d deleted successfully.\n", roll_number);

    // Check and remove empty directories
    remove_directory_if_empty("student_database/personal_info");
    remove_directory_if_empty("student_database/quarterly_marks");
    remove_directory_if_empty("student_database/halfyearly_marks");
    remove_directory_if_empty("student_database/annual_marks");
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
    } else if (percentage >= 80) {
        return 'B';
    } else if (percentage >= 70) {
        return 'C';
    } else if (percentage >= 60) {
        return 'D';
    } else {
        return 'F';
    }
}

void remove_directory_if_empty(const char *dirname) {
    struct dirent *dir;
    DIR *d = opendir(dirname);

    if (d == NULL) {
        return;
    }

    // Check if directory is empty
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
            closedir(d);
            return;
        }
    }

    closedir(d);
    // Remove the directory
    #ifdef _WIN32
    _rmdir(dirname);
    #else
    rmdir(dirname);
    #endif
    printf("Directory %s removed successfully.\n", dirname);
}
