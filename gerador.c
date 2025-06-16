#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define LIMITE_SENSORES 2000
#define LIMITE_TAMANHO_STRING 16
#define PROBABILIDADE_NULO 0.05

typedef struct {
    char nome[50];
    int tipo;
} Sensor;

void gerar_valor(int tipo, char *saida) {
    if ((float)rand() / RAND_MAX < PROBABILIDADE_NULO) {
        strcpy(saida, "NULL");
        return;
    }

    switch (tipo) {
        case 0: sprintf(saida, "%d", rand() % 1000); break;
        case 1: sprintf(saida, "%s", rand() % 2 ? "verdadeiro" : "falso"); break;
        case 2: sprintf(saida, "%.2f", (float)(rand() % 1000) / 10.0f); break;
        case 3: {
            char tmp[LIMITE_TAMANHO_STRING + 1] = {0};
            int length = 1 + rand() % LIMITE_TAMANHO_STRING;
            for (int i = 0; i < length; i++)
                tmp[i] = 'a' + rand() % 26;
            sprintf(saida, "%s", tmp);
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 6 || (argc - 2) % 2 != 0) {
        printf("Uso: %s inicio_unix fim_unix sensor1 tipo1 sensor2 tipo2 ...\n", argv[0]);
        printf("Tipos: 0=int, 1=bool, 2=float, 3=string\n");
        return 1;
    }

    long inicio = atol(argv[1]);
    long fim = atol(argv[2]);
    if (inicio >= fim) {
        printf("Erro: inicio (%ld) deve ser menor que fim (%ld)\n", inicio, fim);
        return 1;
    }

    Sensor sensores[LIMITE_SENSORES];
    int total_sensores = (argc - 3) / 2;

    if (total_sensores > LIMITE_SENSORES) {
        printf("Erro: Máximo de %d sensores suportados\n", LIMITE_SENSORES);
        return 1;
    }

    for (int i = 0; i < total_sensores; i++) {
        strncpy(sensores[i].nome, argv[3 + 2 * i], 49);
        sensores[i].nome[49] = '\0';
        sensores[i].tipo = atoi(argv[4 + 2 * i]);
        if (sensores[i].tipo < 0 || sensores[i].tipo > 3) {
            printf("Erro: Tipo inválido para sensor %s (%d)\n", sensores[i].nome, sensores[i].tipo);
            return 1;
        }
    }

    srand(time(NULL));
    FILE *saida = fopen("dados_teste.txt", "w");
    if (!saida) {
        perror("Erro ao criar arquivo");
        return 1;
    }

    for (int i = 0; i < total_sensores; i++) {
        for (int j = 0; j < 2000; j++) {
            long instante = inicio + rand() % (fim - inicio + 1);
            char valor[100];
            gerar_valor(sensores[i].tipo, valor);
            fprintf(saida, "%ld %s %s\n", instante, sensores[i].nome, valor);
        }
    }

    fclose(saida);
    printf("Arquivo gerado com %d sensores\n", total_sensores);
    return 0;
}