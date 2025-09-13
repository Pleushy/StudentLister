#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

int studentCount;
Student *students;
const int COMMAND_COUNT = 7;
const Command COMMANDS[] = {
    {"cmds", "- Shows list of commands", 0},
    {"exit", "- Exits", 0},
    {"list", "- Shows a list of students", 0},
    {"add_student", "[student] - Adds a student", 1},
    {"remove_student", "[student] - Removes a student", 1},
    {"add_grade", "[student] [grade] - Adds a grade to a student", 2},
    {"remove_grade", "[student] [grade] - Removes a grade from a student", 2}
};



void print_cmds() {
    for (int i = 0; i < COMMAND_COUNT; i++) {
        printf("- %s %s\n", COMMANDS[i].name, COMMANDS[i].info);
    }
}

void list() {
    for (int i = 0; i < studentCount; i++) {
        printf("Name: %s | Grades: ", students[i].name);

        int gradeCount = students[i].gradeCount;
        float average = 0;
        if (gradeCount > 0) {
            for (int j = 0; j < gradeCount; j++) {
                int currentGrade = students[i].grades[j];
                average += currentGrade;
                printf("%d", currentGrade);
                if (j != gradeCount-1) {
                    printf(", ");
                }
            }
            average /= gradeCount;
        }
        printf(" | Average: %f\n", average);
    }
}

int student_exists(char *name) {
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(name, students[i].name) == 0) {
            return 1;
        }
    }
    return 0;
}

void add_student(char *name) {
    if (student_exists(name)) {
        printf("Student already exists.\n");
        return;
    }
    int bytes = sizeof(Student)*studentCount;
    Student *tmp = malloc(bytes);
    memcpy(tmp, students, bytes);
    students = malloc(bytes+sizeof(Student));
    memcpy(students, tmp, bytes);
    free(tmp);

    Student student = {name};
    students[studentCount] = student;
    studentCount++;
}

void remove_student(char *name) {
    if (!student_exists(name)) {
        printf("Student doesn't exist.\n");
        return;
    }
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(name, students[i].name) == 0) {
            int bytes = sizeof(Student)*studentCount;
            Student *tmp = malloc(bytes);
            memcpy(tmp, students, bytes);
            Student *_ = realloc(students, bytes-sizeof(Student));
            int found = 0;
            for (int j = 0; j < studentCount; j++) {
                if (j == i) {
                    found++;
                    continue;
                }
                students[j-found] = tmp[j];
            }
            free(tmp);
            studentCount--;
            break;
        }
    }
}

void add_grade(char *name, int grade) {
    if (!student_exists(name)) {
        printf("Student doesn't exist.\n");
        return;
    }
    if (grade < 1 || grade > 5) {
        printf("Invalid grade.\n");
        return;
    }
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(name, students[i].name) == 0) {
            int bytes = sizeof(int)*students[i].gradeCount;
            int *tmp = malloc(bytes);
            memcpy(tmp, students[i].grades, bytes);
            students[i].grades = malloc(bytes+sizeof(int));
            memcpy(students[i].grades, tmp, bytes);
            free(tmp);

            students[i].grades[students[i].gradeCount] = grade;
            students[i].gradeCount++;
            break;
        }
    }
}

void remove_grade(char *name, int grade) {
    if (!student_exists(name)) {
        printf("Student doesn't exist.\n");
        return;
    }
    if (grade < 1 || grade > 5) {
        printf("Invalid grade.\n");
        return;
    }
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(name, students[i].name) == 0) {
            int exists = 0;
            for (int j = 0; j < students[i].gradeCount; j++) {
                if (students[i].grades[j] == grade) {
                    exists = 1;
                }
            }
            if (!exists) {
                printf("Grade not found.\n");
                return;
            }
            // call it student_grade_size
            int bytes = sizeof(int)*students[i].gradeCount;
            int *tmp = malloc(bytes);
            memcpy(tmp, students[i].grades, bytes);
            int *_ = realloc(students[i].grades, bytes-sizeof(int));
            int found = 0;
            for (int j = 0; j < students[i].gradeCount; j++) {
                if (students[i].grades[j] == grade && !found) {
                    found = 1;
                    continue;
                }
                students[i].grades[j-found] = tmp[j];
            }
            free(tmp);
            students[i].gradeCount--;
            break;
        }
    }
}
// This is essentially not wrong but I would create Enum of commands
// so this can return int
//by this way it would be much simpler to find the right operation for the command
void get_cmd(char *command, char *student, int *grade) {
    // just parse the command here next time, other logic make in command handling
    scanf("%s", command);

    int found = 0;
    Command currect_command = {};
    for (int i = 0; i < COMMAND_COUNT; i++) {
        if (strcmp(command, COMMANDS[i].name) == 0) {
            currect_command = COMMANDS[i];
            found = 1;
            break;
        };
    }
    if (!found) {
        printf("\nDidn't find command, please try again.\n");
        get_cmd(command, student, grade);
    }


    for (int i = 0; i < currect_command.args; i++) {
        switch (i) {
            case 0:
                printf("\nWho would you like to affect?\n");
                scanf("%s", student);
                break;
            case 1:
                printf("\nWhat grade would you like to add or remove?\n");
                scanf("%d", grade);
                // maybe some printf that would confirm to the user that it works 
                break;
            default:
                break; 
        }
    }
}

void handle_command() {
    char command[20];
    // static 3:
    char student[20];
    int grade = -1;
    get_cmd(command, student, &grade);
    
    if (strcmp(command, "list") == 0) {
        list();
    } else if (strcmp(command, "cmds") == 0) {
        print_cmds();
    } else if (strcmp(command, "exit") == 0) {
        return;
    } else if (strcmp(command, "add_student") == 0) {
        add_student(student);
    } else if (strcmp(command, "remove_student") == 0) {
        remove_student(student);
    } else if (strcmp(command, "add_grade") == 0) {
        add_grade(student, grade);
    } else if (strcmp(command, "remove_grade") == 0) {
        remove_grade(student, grade);
    }
    printf("\n");
    // might not be essentially wrong, but stack overflow could happen
    handle_command();
}

int main() {
    printf("What would you like to do? (Type 'cmds' for a list of commands)\n\n");
    handle_command();
    return 0;
}