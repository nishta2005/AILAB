#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 100

// Structure to hold a substitution pair
typedef struct {
    char var[20];
    char value[20];
} Substitution;

Substitution subs[MAX];
int subs_count = 0;

// Check if the string is a variable (lowercase)
int is_variable(char *s) {
    return islower(s[0]);
}

// Apply substitution to a term
void apply_subs(char *result, char *term) {
    for (int i = 0; i < subs_count; i++) {
        if (strcmp(term, subs[i].var) == 0) {
            strcpy(result, subs[i].value);
            return;
        }
    }
    strcpy(result, term);
}

// Occurs check (var in term?)
int occurs_check(char *var, char *term) {
    if (strcmp(var, term) == 0)
        return 1;

    char temp[50];
    strcpy(temp, term);

    // If function f(x,y) check inside arguments
    char *p = strchr(temp, '(');
    if (p) {
        p++;
        char *q = strtok(p, ",()");
        while (q) {
            if (occurs_check(var, q))
                return 1;
            q = strtok(NULL, ",()");
        }
    }
    return 0;
}

// Add substitution
int add_sub(char *var, char *value) {
    if (occurs_check(var, value))
        return 0;

    strcpy(subs[subs_count].var, var);
    strcpy(subs[subs_count].value, value);
    subs_count++;
    return 1;
}

// Unify two terms
int unify(char *x, char *y) {
    char tx[50], ty[50];
    apply_subs(tx, x);
    apply_subs(ty, y);

    if (strcmp(tx, ty) == 0)
        return 1; // Already equal

    // Case 1: x is variable
    if (is_variable(tx))
        return add_sub(tx, ty);

    // Case 2: y is variable
    if (is_variable(ty))
        return add_sub(ty, tx);

    // Case 3: both are functions
    char fx[20], fy[20];
    char argsx[50], argsy[50];

    char *px = strchr(tx, '(');
    char *py = strchr(ty, '(');

    if (!px || !py) return 0; // constants mismatch

    // Extract function name and arguments
    strncpy(fx, tx, px - tx);
    fx[px - tx] = '\0';
    strcpy(argsx, px + 1);
    argsx[strlen(argsx) - 1] = '\0'; // remove ')'

    strncpy(fy, ty, py - ty);
    fy[py - ty] = '\0';
    strcpy(argsy, py + 1);
    argsy[strlen(argsy) - 1] = '\0';

    // Function names must match
    if (strcmp(fx, fy) != 0)
        return 0;

    // Split arguments and unify pairwise
    char *ax = strtok(argsx, ",");
    char *ay = strtok(argsy, ",");

    while (ax && ay) {
        if (!unify(ax, ay))
            return 0;
        ax = strtok(NULL, ",");
        ay = strtok(NULL, ",");
    }

    return 1;
}

int main() {
    char t1[100], t2[100];

    printf("Enter term 1: ");
    scanf("%s", t1);

    printf("Enter term 2: ");
    scanf("%s", t2);

    if (unify(t1, t2)) {
        printf("\nUnifier found:\n");
        for (int i = 0; i < subs_count; i++) {
            printf("%s -> %s\n", subs[i].var, subs[i].value);
        }
    } else {
        printf("\nUnification failed.\n");
    }

    return 0;
}
