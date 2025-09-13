#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char *name;
    int gradeCount;
    int *grades;
} Student;

typedef struct {
    char *name;
    char *info;
    int args;
} Command;

int studentCount;
Student *students;
const Command commands[7] = {
    {"cmds", "- Shows list of commands", 0},
    {"exit", "- Exits", 0},
    {"list", "- Shows a list of students", 0},
    {"add_student", "[student] - Adds a student", 1},
    {"remove_student", "[student] - Removes a student", 1},
    {"add_grade", "[student] [grade] - Adds a grade to a student", 2},
    {"remove_grade", "[student] [grade] - Removes a grade from a student", 2}
};
const __uint64_t commandAmount = sizeof(commands)/sizeof(Command);

void printCommands() {
    for (int i = 0; i < commandAmount; i++) {
        printf("- %s %s\n", commands[i].name, commands[i].info);
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

int studentExists(char *name) {
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(name, students[i].name) == 0) {
            return 1;
        }
    }
    return 0;
}

void addStudent(char *name) {
    if (studentExists(name)) {
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

void removeStudent(char *name) {
    if (!studentExists(name)) {
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

void addGrade(char *name, int grade) {
    if (!studentExists(name)) {
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

void removeGrade(char *name, int grade) {
    if (!studentExists(name)) {
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

void getCommand(char *command, char *student, int *grade) {
    scanf("%s", command);

    int found = 0;
    Command currentCommand = {};
    for (int i = 0; i < commandAmount; i++) {
        if (strcmp(command, commands[i].name) == 0) {
            currentCommand = commands[i];
            found = 1;
            break;
        };
    }
    if (!found) {
        printf("\nDidn't find command, please try again.\n");
        getCommand(command, student, grade);
    }

    for (int i = 0; i < currentCommand.args; i++) {
        switch (i) {
            case 0:
                printf("\nWho would you like to affect?\n");
                scanf("%s", student);
                break;
            case 1:
                printf("\nWhat grade would you like to add or remove?\n");
                scanf("%d", grade);
                break;
            default:
                break; 
        }
    }
}

void handleCommand() {
    char command[20];
    char student[20];
    int grade = -1;
    getCommand(command, student, &grade);
    
    if (strcmp(command, "list") == 0) {
        list();
    } else if (strcmp(command, "cmds") == 0) {
        printCommands();
    } else if (strcmp(command, "exit") == 0) {
        return;
    } else if (strcmp(command, "add_student") == 0) {
        addStudent(student);
    } else if (strcmp(command, "remove_student") == 0) {
        removeStudent(student);
    } else if (strcmp(command, "add_grade") == 0) {
        addGrade(student, grade);
    } else if (strcmp(command, "remove_grade") == 0) {
        removeGrade(student, grade);
    }
    printf("\n");
    handleCommand();
}

int main() {
    printf("What would you like to do? (Type 'cmds' for a list of commands)\n\n");
    handleCommand();
    return 0;
}