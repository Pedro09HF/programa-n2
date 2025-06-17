#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

#define MX_SNS 100
#define MX_NM 32
#define MX_LN 128

typedef enum { CONJ_Z, CONJ_Q, TEXTO, BINARIO, INV } TD;

typedef struct {
    char nm[MX_NM];
    TD tp;
} SNS;

long parse_dt(const char *s) {
    struct tm t = {0};
    if (sscanf(s, "%d-%d-%dT%d:%d:%d",
               &t.tm_year, &t.tm_mon, &t.tm_mday,
               &t.tm_hour, &t.tm_min, &t.tm_sec) != 6) {
        puts("Formato de data inválido. Use YYYY-MM-DDThh:mm:ss");
        exit(1);
    }
    t.tm_year -= 1900;
    t.tm_mon -= 1;
    time_t ts = mktime(&t);
    if (ts == -1) {
        puts("Data inválida");
        exit(1);
    }
    return ts;
}

TD get_tp(const char *s) {
    const char *tp[] = {"CONJ_Z", "CONJ_Q", "TEXTO", "BINARIO"};
    for (int i = 0; i < 4; i++) {
        if (strcmp(s, tp[i]) == 0) return (TD)i;
    }
    return INV;
}

void gen_vl(char *b, TD t) {
    switch (t) {
        case CONJ_Z:
            sprintf(b, "%d", rand() % 1000);
            break;
        case CONJ_Q:
            sprintf(b, "%.2f", (float)(rand() % 10000) / 100.0f);
            break;
        case TEXTO: {
            int l = 5 + rand() % 11;
            for (int i = 0; i < l; i++) b[i] = 'a' + rand() % 26;
            b[l] = '\0';
            break;
        }
        case BINARIO:
            strcpy(b, rand() % 2 ? "true" : "false");
            break;
        default:
            strcpy(b, "ERRO");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 5 || (argc - 3) % 2 != 0) {
        puts("Uso: ./programa <data_inicio> <data_fim> <sensor1> <tipo1> ...");
        return 1;
    }

    srand(time(NULL));

    long inicio = parse_dt(argv[1]);
    long fim = parse_dt(argv[2]);
    if (fim <= inicio) {
        puts("Erro: data_fim deve ser posterior a data_inicio");
        return 1;
    }

    int n_sns = (argc - 3) / 2;
    if (n_sns > MX_SNS) {
        printf("Erro: máximo de %d sensores suportados\n", MX_SNS);
        return 1;
    }

    SNS sns[MX_SNS];
    for (int i = 0; i < n_sns; i++) {
        if (strlen(argv[3 + i * 2]) >= MX_NM) {
            printf("Erro: nome do sensor '%s' muito longo\n", argv[3 + i * 2]);
            return 1;
        }
        if (strchr(argv[3 + i * 2], ' ') != NULL) {
            puts("Erro: nome do sensor não pode conter espaços");
            return 1;
        }
        strcpy(sns[i].nm, argv[3 + i * 2]);
        sns[i].tp = get_tp(argv[4 + i * 2]);
        if (sns[i].tp == INV) {
            printf("Erro: tipo inválido '%s'\n", argv[4 + i * 2]);
            return 1;
        }
    }

    FILE *out = fopen("dados_gerados.txt", "w");
    if (!out) {
        perror("Erro");
        return 1;
    }

    for (int i = 0; i < n_sns; i++) {
        for (int j = 0; j < 2000; j++) {
            long ts = inicio + (rand() % (fim - inicio + 1));
            char vl[64];
            gen_vl(vl, sns[i].tp);
            fprintf(out, "%ld %s %s\n", ts, sns[i].nm, vl);
        }
    }

    fclose(out);
    printf("Arquivo gerado: 'dados_gerados.txt' (%d sensores)\n", n_sns);
    return 0;
}