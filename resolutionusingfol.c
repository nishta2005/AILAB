#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLAUSES 100
#define MAX_LITS 20
#define MAX_LEN 50

typedef struct {
    char lits[MAX_LITS][MAX_LEN];
    int count;
} Clause;

Clause KB[MAX_CLAUSES];
int kb_size = 0;

// Check if literal is complement of another
int is_complement(char *a, char *b) {
    if (a[0] == '~' && strcmp(a + 1, b) == 0) return 1;
    if (b[0] == '~' && strcmp(b + 1, a) == 0) return 1;
    return 0;
}

// Check if clause already exists
int clause_exists(Clause c) {
    for (int i = 0; i < kb_size; i++) {
        if (KB[i].count == c.count) {
            int match = 1;
            for (int j = 0; j < c.count; j++) {
                if (strcmp(KB[i].lits[j], c.lits[j]) != 0) {
                    match = 0; break;
                }
            }
            if (match) return 1;
        }
    }
    return 0;
}

// Resolution rule
Clause resolve_clauses(Clause A, Clause B, int *valid) {
    Clause result;
    result.count = 0;
    *valid = 0;

    for (int i = 0; i < A.count; i++) {
        for (int j = 0; j < B.count; j++) {
            if (is_complement(A.lits[i], B.lits[j])) {
                *valid = 1;

                // Add remaining literals from A
                for (int x = 0; x < A.count; x++) {
                    if (x == i) continue;
                    strcpy(result.lits[result.count++], A.lits[x]);
                }

                // Add remaining literals from B
                for (int y = 0; y < B.count; y++) {
                    if (y == j) continue;
                    strcpy(result.lits[result.count++], B.lits[y]);
                }

                return result;
            }
        }
    }

    return result;
}

void print_clause(Clause c) {
    if (c.count == 0) {
        printf("EMPTY\n");
        return;
    }
    for (int i = 0; i < c.count; i++) {
        printf("%s ", c.lits[i]);
    }
    printf("\n");
}

int main() {
    int n;
    printf("Enter number of clauses: ");
    scanf("%d", &n);

    printf("Enter clauses (space-separated literals):\n");

    for (int i = 0; i < n; i++) {
        KB[i].count = 0;
        char line[200];
        getchar(); 
        fgets(line, 200, stdin);

        char *tok = strtok(line, " \n");
        while (tok != NULL) {
            strcpy(KB[i].lits[KB[i].count++], tok);
            tok = strtok(NULL, " \n");
        }
    }

    kb_size = n;

    for (int i = 0; i < kb_size; i++) {
        for (int j = i + 1; j < kb_size; j++) {
            int valid;
            Clause res = resolve_clauses(KB[i], KB[j], &valid);

            if (valid) {
                printf("Resolving: ");
                print_clause(KB[i]);
                printf("AND ");
                print_clause(KB[j]);
                printf("=> ");
                print_clause(res);
                printf("\n");

                if (res.count == 0) {
                    printf("❗ CONTRADICTION FOUND: Empty clause derived.\n");
                    return 0;
                }

                if (!clause_exists(res)) {
                    KB[kb_size++] = res;
                }
            }
        }
    }

    printf("NO contradiction found. KB is satisfiable.\n");
    return 0;
}
