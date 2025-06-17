#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define M 128
#define N 10000

typedef struct {
    long t;
    char v[64];
} E;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <sensor> <timestamp>\n", argv[0]);
        return 1;
    }

    char *p;
    long a = strtol(argv[2], &p, 10);
    if (*p != '\0' || a <= 0) {
        fprintf(stderr, "Erro: timestamp inválido.\n");
        return 1;
    }

    char f[64];
    snprintf(f, sizeof(f), "%s.dat", argv[1]);

    FILE *fp = fopen(f, "r");
    if (!fp) {
        fprintf(stderr, "Erro: sensor '%s' não encontrado.\n", argv[1]);
        return 1;
    }

    E *d = malloc(sizeof(E) * N);
    if (!d) {
        perror("Erro ao alocar memória");
        fclose(fp);
        return 1;
    }

    int c = 0;
    char l[M];
    int ln = 0;

    while (fgets(l, sizeof(l), fp)) {
        ln++;
        if (c >= N) {
            fprintf(stderr, "Limite de %d entradas excedido. Interrompendo leitura.\n", N);
            break;
        }

        char *tok = strtok(l, " ");
        if (!tok) {
            fprintf(stderr, "Linha %d inválida: faltando timestamp\n", ln);
            continue;
        }

        char *end;
        long ts = strtol(tok, &end, 10);
        if (*end != '\0' || ts <= 0) {
            fprintf(stderr, "Linha %d inválida: timestamp malformado\n", ln);
            continue;
        }

        tok = strtok(NULL, " ");
        if (!tok) {
            fprintf(stderr, "Linha %d inválida: faltando nome do sensor\n", ln);
            continue;
        }

        tok = strtok(NULL, "\n");
        if (!tok) {
            fprintf(stderr, "Linha %d inválida: faltando valor do sensor\n", ln);
            continue;
        }

        d[c].t = ts;
        strncpy(d[c].v, tok, sizeof(d[c].v));
        d[c].v[sizeof(d[c].v) - 1] = '\0';
        c++;
    }
    fclose(fp);

    if (c == 0) {
        fprintf(stderr, "Nenhuma leitura válida encontrada no arquivo.\n");
        free(d);
        return 1;
    }

    int i = 0, j = c - 1, k = -1;
    long md = LONG_MAX;
    while (i <= j) {
        int m = (i + j) / 2;
        long diff = labs(d[m].t - a);

        if (diff < md) {
            md = diff;
            k = m;
        }

        if (d[m].t < a) {
            j = m - 1;
        } else {
            i = m + 1;
        }
    }

    if (k != -1) {
        printf("Leitura mais próxima: %ld %s\n", d[k].t, d[k].v);
    } else {
        printf("Nenhuma leitura válida encontrada.\n");
    }

    free(d);
    return 0;
}