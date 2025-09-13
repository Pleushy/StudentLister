#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char *name;
    int grade_count;
    int *grades;
} Student;

typedef struct {
    char *name;
    char *info;
    int args;
} Command;

int student_count;
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
    for (int i = 0; i < student_count; i++) {
        printf("Name: %s | Grades: ", students[i].name);

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
    int bytes = sizeof(Student)*student_count;
    Student *tmp = malloc(bytes);
    memcpy(tmp, students, bytes);
    students = malloc(bytes+sizeof(Student));
    memcpy(students, tmp, bytes);
    free(tmp);

    Student student = {name};
    students[student_count] = student;
    student_count++;
}

void remove_student(char *name) {
    if (!student_exists(name)) {
        printf("Student doesn't exist.\n");
        return;
    }
    for (int i = 0; i < student_count; i++) {
        if (strcmp(name, students[i].name) == 0) {
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
    for (int i = 0; i < student_count; i++) {
        if (strcmp(name, students[i].name) == 0) {
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
    for (int i = 0; i < student_count; i++) {
        if (strcmp(name, students[i].name) == 0) {
            int exists = 0;
            for (int j = 0; j < students[i].grade_count; j++) {
                if (students[i].grades[j] == grade) {
                    exists = 1;
                }
            }
            if (!exists) {
                printf("Grade not found.\n");
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
}

void get_command(char *command, char *student, int *grade) {
    scanf("%s", command);

    int found = 0;
    Command current_command = {};
    for (int i = 0; i < sizeof(COMMANDS)/sizeof(Command); i++) {
        if (strcmp(command, COMMANDS[i].name) == 0) {
            current_command = COMMANDS[i];
            found = 1;
            break;
        };
    }
    if (!found) {
        printf("\nDidn't find command, please try again.\n");
        get_command(command, student, grade);
    }

    for (int i = 0; i < current_command.args; i++) {
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