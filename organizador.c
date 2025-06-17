#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define A 256
#define B 32
#define C 64
#define D 10000
#define E 256

typedef struct {
    long x;
    char y[B];
    char z[C];
} W;

bool func1(const char *s) {
    if (*s == '-' || *s == '+') s++;
    while (*s) {
        if (!isdigit(*s)) return false;
        s++;
    }
    return true;
}

bool func2(const char *s) {
    int p = 0;
    if (*s == '-' || *s == '+') s++;
    while (*s) {
        if (*s == '.') {
            if (++p > 1) return false;
        } else if (!isdigit(*s)) {
            return false;
        }
        s++;
    }
    return true;
}

bool func3(const char *s) {
    return (strcasecmp(s, "true") == 0 || strcasecmp(s, "false") == 0);
}

bool func4(const char *s) {
    while (*s) {
        if (isspace(*s)) return false;
        s++;
    }
    return true;
}

int func5(const void* a, const void* b) {
    return ((W*)b)->x - ((W*)a)->x;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    FILE *f1 = fopen(argv[1], "r");
    if (!f1) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    W w1[D];
    int t1 = 0;
    char l1[A];
    int n1 = 0;

    while (fgets(l1, sizeof(l1), f1)) {
        n1++;
        char *t = strtok(l1, " \n");
        if (!t) {
            printf("Linha %d inválida: faltando timestamp\n", n1);
            continue;
        }

        if (!func1(t)) {
            printf("Linha %d: timestamp inválido\n", n1);
            continue;
        }

        long ts = atol(t);
        if (ts <= 0) {
            printf("Linha %d: timestamp deve ser positivo\n", n1);
            continue;
        }

        t = strtok(NULL, " \n");
        if (!t) {
            printf("Linha %d: nome do sensor faltando\n", n1);
            continue;
        }

        if (strlen(t) >= B || !func4(t)) {
            printf("Linha %d: nome de sensor inválido\n", n1);
            continue;
        }

        char s1[B];
        strncpy(s1, t, B);

        t = strtok(NULL, "\n");
        if (!t) {
            printf("Linha %d: valor faltando\n", n1);
            continue;
        }

        if (strlen(t) >= C) {
            printf("Linha %d: valor muito longo\n", n1);
            continue;
        }

        if (!(func1(t) || func2(t) || func3(t) || func4(t))) {
            printf("Linha %d: valor inválido\n", n1);
            continue;
        }

        if (t1 >= D) {
            printf("Limite máximo de leituras atingido (%d)\n", D);
            break;
        }

        w1[t1].x = ts;
        strncpy(w1[t1].y, s1, B);
        strncpy(w1[t1].z, t, C);
        t1++;
    }
    fclose(f1);

    qsort(w1, t1, sizeof(W), func5);

    char s2[E][B];
    int t2 = 0;

    for (int i = 0; i < t1; i++) {
        bool f = false;
        for (int j = 0; j < t2; j++) {
            if (strcmp(s2[j], w1[i].y) == 0) {
                f = true;
                break;
            }
        }
        if (!f && t2 < E) {
            strncpy(s2[t2], w1[i].y, B);
            t2++;
        }
    }

    FILE *f2[E];
    for (int i = 0; i < t2; i++) {
        char n2[B + 5];
        snprintf(n2, sizeof(n2), "%s.dat", s2[i]);
        f2[i] = fopen(n2, "w");
        if (!f2[i]) {
            perror("Erro ao criar arquivo de saída");
            return 1;
        }
    }

    for (int i = 0; i < t1; i++) {
        for (int j = 0; j < t2; j++) {
            if (strcmp(w1[i].y, s2[j]) == 0) {
                fprintf(f2[j], "%ld %s %s\n", w1[i].x, w1[i].y, w1[i].z);
                break;
            }
        }
    }

    for (int i = 0; i < t2; i++) {
        fclose(f2[i]);
    }

    puts("Separação e ordenação concluída.");
    printf("%d leituras processadas.\n", t1);
    return 0;
}