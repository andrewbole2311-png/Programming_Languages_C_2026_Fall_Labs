// week5_task3_student_management_system.c
// Name: BOLE  Surname: ANDREA  Student ID: 260ADB177
//
// Task 3: Mini-project – Student management system with file persistence
// Week 5 – Files & Modular Programming
// TODO: Implement the functions to load, save, add, and list students.
//
// Build: make lab5   Run: ./bin/week5_task3_student_management_system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define LINE_LEN 256
#define DATA_FILE "students.txt"

typedef struct Student {
    char name[NAME_LEN];
    int id;
    float gpa;
} Student;

// Function prototypes (do not change these signatures – the autograder calls them directly)
int  load_students(Student arr[], int max);          // returns number of records loaded
int  save_students(const Student arr[], int count);  // 0 on success, -1 on error
void add_student(Student arr[], int *count);
void list_students(const Student arr[], int count);
int  read_line(const char *prompt, char *buf, size_t size);

int main(void) {
    Student students[MAX_STUDENTS];
    int count = 0;
    char input[LINE_LEN];
    int running = 1;

    // Load stuff at start
    count = load_students(students, MAX_STUDENTS);
    printf("Loaded %d student(s) from %s\n", count, DATA_FILE);

    while (running) {
        printf("\n=== Student Management System ===\n");
        printf("1. List students\n");
        printf("2. Add student\n");
        printf("3. Save and Exit\n");

        int choice;
        if (read_line("Select an option: ", input, sizeof input) != 0) {
            choice = 3;                         // end of input: save and exit instead of looping forever
        } else {
            char *end;
            long value = strtol(input, &end, 10);
            choice = (end != input && *end == '\0') ? (int)value : 0;   // non-numbers become invalid
        }

        switch (choice) {
            case 1:
                list_students(students, count);
                break;
            case 2:
                add_student(students, &count);
                break;
            case 3:
                if (save_students(students, count) != 0) {
                    perror("Failed to save data");
                }
                printf("Data saved. Goodbye!\n");
                running = 0;
                break;
            default:
                printf("Invalid option. Try again.\n");
        }
    }

    return EXIT_SUCCESS;
}

// Provided helper – prints a prompt and reads one line from stdin into buf.
int read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buf, (int)size, stdin) == NULL) {
        return -1;
    }
    size_t len = strcspn(buf, "\r\n");
    if (buf[len] == '\0' && len == size - 1) {
        // Line was longer than the buffer: discard the rest of it.
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    buf[len] = '\0';
    return 0;
}

// Read lines from file and store valid students
int load_students(Student arr[], int max) {
    FILE *fp = fopen(DATA_FILE, "r");
    if (fp == NULL) {
        // file doesn't exist yet, nothing to do
        return 0;
    }

    char line[LINE_LEN];
    int loaded = 0;

    // read line by line until end or array full
    while (loaded < max && fgets(line, sizeof(line), fp) != NULL) {
        char temp_name[NAME_LEN];
        int temp_id;
        float temp_gpa;

        // format is name,id,gpa
        if (sscanf(line, "%49[^,],%d,%f", temp_name, &temp_id, &temp_gpa) == 3) {
            strncpy(arr[loaded].name, temp_name, NAME_LEN - 1);
            arr[loaded].name[NAME_LEN - 1] = '\0';
            arr[loaded].id = temp_id;
            arr[loaded].gpa = temp_gpa;
            loaded++;
        }
    }

    fclose(fp);
    return loaded;
}

// Dump all students to the file
int save_students(const Student arr[], int count) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        return -1;
    }

    // write all entries in CSV style
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%d,%.2f\n", arr[i].name, arr[i].id, arr[i].gpa);
    }

    fclose(fp);
    return 0;
}

// Add a single student with basic checks
void add_student(Student arr[], int *count) {
    if (*count >= MAX_STUDENTS) {
        printf("Cannot add more students (limit reached).\n");
        return;
    }

    char name_buf[LINE_LEN];
    char id_buf[LINE_LEN];
    char gpa_buf[LINE_LEN];

    // get inputs
    if (read_line("Enter name: ", name_buf, sizeof(name_buf)) != 0) return;
    if (read_line("Enter ID: ", id_buf, sizeof(id_buf)) != 0) return;
    if (read_line("Enter GPA: ", gpa_buf, sizeof(gpa_buf)) != 0) return;

    // check name
    if (name_buf[0] == '\0' || strchr(name_buf, ',') != NULL) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    // check ID (must be pure int)
    char *end_id;
    long parsed_id = strtol(id_buf, &end_id, 10);
    if (end_id == id_buf || *end_id != '\0') {
        printf("Invalid input. Student not added.\n");
        return;
    }

    // check GPA (must be float between 0.0 and 4.0)
    char *end_gpa;
    float parsed_gpa = strtof(gpa_buf, &end_gpa);
    if (end_gpa == gpa_buf || *end_gpa != '\0' || parsed_gpa < 0.0f || parsed_gpa > 4.0f) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    // store student
    strncpy(arr[*count].name, name_buf, NAME_LEN - 1);
    arr[*count].name[NAME_LEN - 1] = '\0';
    arr[*count].id = (int)parsed_id;
    arr[*count].gpa = parsed_gpa;

    (*count)++;
    printf("Student added successfully!\n");
}

// Print all loaded students
void list_students(const Student arr[], int count) {
    if (count == 0) {
        printf("No students found.\n");
        return;
    }

    // matching example column layout
    printf("ID    Name                 GPA\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
    }
}
