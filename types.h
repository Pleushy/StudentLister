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

void print_cmds(void);

void list(void);

void add_student(void);

void remove_student(void);

void add_grade(void);

void remove_grade(void);

const static struct {
    const char *name;
    void (*func)(void);
} function_map[] = {
    {"cmds", print_cmds},
    {"list", list},
    {"add_student", add_student},
    {"remove_student", remove_student},
    {"add_grade", add_grade},
    {"remove_grade", remove_grade}
};
