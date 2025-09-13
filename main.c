#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

int student_count;
Student *students;

const int INPUT_BUFFER = 50;
const int COMMAND_COUNT = 7;
const Command COMMANDS[] = {
    {"cmds", "- Shows list of commands"},
    {"exit", "- Exits"},
    {"list", "- Shows a list of students"},
    {"add_student", "[student] - Adds a student"},
    {"remove_student", "[student] - Removes a student"},
    {"add_grade", "[student] [grade] - Adds a grade to a student"},
    {"remove_grade", "[student] [grade] - Removes a grade from a student"}
};

void print_cmds() {
    for (int i = 0; i < COMMAND_COUNT; i++) {
        printf("- %s %s\n", COMMANDS[i].name, COMMANDS[i].info);
    }
}

void list() {
    if (!student_count) {
        printf("# Student list is empty.\n");
        return;
    }
    for (int i = 0; i < student_count; i++) {
        printf("# Name: %s | Grades: ", students[i].name);

        int grade_count = students[i].grade_count;
        float average = 0;
        if (grade_count > 0) {
            for (int j = 0; j < grade_count; j++) {
                int current_grade = students[i].grades[j];
                average += current_grade;
                printf("%d", current_grade);
                if (j != grade_count-1) {
                    printf(", ");
                }
            }
            average /= grade_count;
        }
        printf(" | Average: %f\n", average);
    }
}

int student_exists(char *name) {
    for (int i = 0; i < student_count; i++) {
        if (!strcmp(name, students[i].name)) {
            return 1;
        }
    }
    return 0;
}

void add_student() {
    char *name = strtok(NULL, " ");
    if (name == NULL) {
        printf("# Name can not be null.\n");
        return;
    }

    if (student_exists(name)) {
        printf("# Student already exists.\n");
        return;
    }
    int bytes = sizeof(Student)*student_count;
    Student *tmp = malloc(bytes);
    memcpy(tmp, students, bytes);
    students = malloc(bytes+sizeof(Student));
    memcpy(students, tmp, bytes);
    free(tmp);

    Student student = {name};
    students[student_count] = student;
    student_count++;

    printf("# Added student.\n");
}

void remove_student() {
    char *name = strtok(NULL, " ");
    if (name == NULL) {
        printf("# Name can not be null.\n");
        return;
    }

    if (!student_exists(name)) {
        printf("# Student doesn't exist.\n");
        return;
    }
    for (int i = 0; i < student_count; i++) {
        if (!strcmp(name, students[i].name)) {
            int bytes = sizeof(Student)*student_count;
            Student *tmp = malloc(bytes);
            memcpy(tmp, students, bytes);
            Student *_ = realloc(students, bytes-sizeof(Student));
            int found = 0;
            for (int j = 0; j < student_count; j++) {
                if (j == i) {
                    found++;
                    continue;
                }
                students[j-found] = tmp[j];
            }
            free(tmp);
            student_count--;
            break;
        }
    }

    printf("# Removed student.\n");
}

void add_grade() {
    char *name = strtok(NULL, " ");
    if (name == NULL) {
        printf("# Name can not be null.\n");
        return;
    }
    char *char_grade = strtok(NULL, " ");
    if (char_grade == NULL) {
        printf("# Grade can not be null.\n");
        return;
    }
    int grade = char_grade[0] - '0';

    if (!student_exists(name)) {
        printf("# Student doesn't exist.\n");
        return;
    }
    if (grade < 1 || grade > 5) {
        printf("# Invalid grade.\n");
        return;
    }
    for (int i = 0; i < student_count; i++) {
        if (!strcmp(name, students[i].name)) {
            int bytes = sizeof(int)*students[i].grade_count;
            int *tmp = malloc(bytes);
            memcpy(tmp, students[i].grades, bytes);
            students[i].grades = malloc(bytes+sizeof(int));
            memcpy(students[i].grades, tmp, bytes);
            free(tmp);

            students[i].grades[students[i].grade_count] = grade;
            students[i].grade_count++;
            break;
        }
    }
    printf("# Added grade.\n");
}

void remove_grade() {
    char *name = strtok(NULL, " ");
    if (name == NULL) {
        printf("# Name can not be null.\n");
        return;
    }
    char *char_grade = strtok(NULL, " ");
    if (char_grade == NULL) {
        printf("# Grade can not be null.\n");
        return;
    }
    int grade = char_grade[0] - '0';

    if (!student_exists(name)) {
        printf("# Student doesn't exist.\n");
        return;
    }
    if (grade < 1 || grade > 5) {
        printf("# Invalid grade.\n");
        return;
    }
    for (int i = 0; i < student_count; i++) {
        if (!strcmp(name, students[i].name)) {
            int exists = 0;
            for (int j = 0; j < students[i].grade_count; j++) {
                if (students[i].grades[j] == grade) {
                    exists = 1;
                }
            }
            if (!exists) {
                printf("# Grade not found.\n");
                return;
            }


            int bytes = sizeof(int)*students[i].grade_count;
            int *tmp = malloc(bytes);
            memcpy(tmp, students[i].grades, bytes);
            int *_ = realloc(students[i].grades, bytes-sizeof(int));
            int found = 0;
            for (int j = 0; j < students[i].grade_count; j++) {
                if (students[i].grades[j] == grade && !found) {
                    found = 1;
                    continue;
                }
                students[i].grades[j-found] = tmp[j];
            }
            free(tmp);
            students[i].grade_count--;
            break;
        }
    }
    printf("# Removed grade.\n");
}

void get_command(char *command) {
    fgets(command, INPUT_BUFFER, stdin);
    for (int i = 0; i < INPUT_BUFFER; i++) {
        if ((command[i] == ' ' && !i ) || command[i] == '\n') {
            command[i] = 0;
            if (!i) {
                printf("\n");
                return;
            };
        }
    }

    command = strtok(command, " ");

    for (int i = 0; i < sizeof(COMMANDS)/sizeof(Command); i++) {
        if (!strcmp(command, COMMANDS[i].name)) {
            return;
        };
    }
    printf("# Didn't find command, please try again.\n\n");
    get_command(command);
}

void handle_command() {
    char command[INPUT_BUFFER];
    get_command(command);

    if (!strcmp(command, "exit")) {
        return;
    }
    
    for (int i = 0; i < (sizeof(function_map) / sizeof(function_map[0])); i++) {
        if (!strcmp(function_map[i].name, command)) {
            function_map[i].func();
            printf("\n");
        }
    }

    handle_command();
}

int main() {
    printf("# What would you like to do? (Type 'cmds' for a list of commands)\n\n");

    handle_command();
    return 0;
}