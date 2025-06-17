#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define LIMITE_SENSORES         2000
#define LIMITE_REGISTROS        100000
#define LIMITE_TAMANHO_STRING   16

typedef struct {
    long instante;
    char id_sensor[50];
    union {
        int inteiro;
        bool booleano;
        float racional;
        char texto[LIMITE_TAMANHO_STRING + 1];
    } valor;
    int tipo_dado;
} RegistroSensor;

int comparar_instantes(const void *a, const void *b) {
    RegistroSensor *ra = (RegistroSensor *)a;
    RegistroSensor *rb = (RegistroSensor *)b;
    if (ra->instante < rb->instante) return 1;  
    else if (ra->instante > rb->instante) return -1;
    else return 0;
}

bool string_vazia(const char *str) {
    return str == NULL || str[0] == '\0';
}

bool eh_numero_valido(const char *str) {
    if (string_vazia(str)) return false;
    char *endptr;
    strtod(str, &endptr);
    return endptr != str && *endptr == '\0';
}

void processar_arquivo(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    RegistroSensor *registros = malloc(LIMITE_REGISTROS * sizeof(RegistroSensor));
    if (!registros) {
        perror("Erro ao alocar memória");
        fclose(arquivo);
        exit(1);
    }

    char sensores[LIMITE_SENSORES][50];
    int tipos_sensor[LIMITE_SENSORES];
    int total_registros = 0;
    int total_sensores = 0;

    while (fscanf(arquivo, "%ld %49s", &registros[total_registros].instante, 
                                     registros[total_registros].id_sensor) == 2) {
        char valor_str[100];
        if (fscanf(arquivo, "%99s", valor_str) != 1) {
            printf("Aviso: Valor ausente para o sensor '%s' no instante %ld. Ignorando registro.\n",
                   registros[total_registros].id_sensor, registros[total_registros].instante);
            continue;
        }

        if (string_vazia(valor_str)) {
            printf("Aviso: Valor vazio para o sensor '%s' no instante %ld. Ignorando registro.\n",
                   registros[total_registros].id_sensor, registros[total_registros].instante);
            continue;
        }

        int idx_sensor = -1;
        for (int i = 0; i < total_sensores; i++) {
            if (strcmp(sensores[i], registros[total_registros].id_sensor) == 0) {
                idx_sensor = i;
                break;
            }
        }

        if (idx_sensor == -1) {
            if (total_sensores >= LIMITE_SENSORES) {
                puts("Aviso: Limite de sensores atingido. Ignorando novos sensores.");
                continue;
            }
            strcpy(sensores[total_sensores], registros[total_registros].id_sensor);
            puts("Defina o tipo de dado para o sensor:");
            puts("0 = Inteiro | 1 = Booleano | 2 = Float | 3 = String");
            printf("Sensor '%s': ", registros[total_registros].id_sensor);
            scanf("%d", &tipos_sensor[total_sensores]);
            total_sensores++;
            idx_sensor = total_sensores - 1;
        }

        registros[total_registros].tipo_dado = tipos_sensor[idx_sensor];
        switch (registros[total_registros].tipo_dado) {
            case 0:
                if (!eh_numero_valido(valor_str)) {
                    printf("Aviso: Valor '%s' inválido para inteiro (sensor '%s'). Usando 0 como padrão.\n",
                           valor_str, registros[total_registros].id_sensor);
                    registros[total_registros].valor.inteiro = 0;
                } else {
                    registros[total_registros].valor.inteiro = atoi(valor_str);
                }
                break;
            case 1:
                if (strcmp(valor_str, "verdadeiro") == 0) {
                    registros[total_registros].valor.booleano = true;
                } else if (strcmp(valor_str, "falso") == 0) {
                    registros[total_registros].valor.booleano = false;
                } else {
                    printf("Aviso: Valor '%s' inválido para booleano (sensor '%s'). Usando 'falso' como padrão.\n",
                           valor_str, registros[total_registros].id_sensor);
                    registros[total_registros].valor.booleano = false;
                }
                break;
            case 2:
                if (!eh_numero_valido(valor_str)) {
                    printf("Aviso: Valor '%s' inválido para float (sensor '%s'). Usando 0.0 como padrão.\n",
                           valor_str, registros[total_registros].id_sensor);
                    registros[total_registros].valor.racional = 0.0f;
                } else {
                    registros[total_registros].valor.racional = atof(valor_str);
                }
                break;
            case 3:
                if (string_vazia(valor_str)) {
                    printf("Aviso: String vazia para o sensor '%s'. Usando 'NULL' como padrão.\n",
                           registros[total_registros].id_sensor);
                    strcpy(registros[total_registros].valor.texto, "NULL");
                } else {
                    strncpy(registros[total_registros].valor.texto, valor_str, LIMITE_TAMANHO_STRING);
                    registros[total_registros].valor.texto[LIMITE_TAMANHO_STRING] = '\0';
                }
                break;
        }
        total_registros++;
    }
    fclose(arquivo);

    qsort(registros, total_registros, sizeof(RegistroSensor), comparar_instantes);

    for (int i = 0; i < total_sensores; i++) {
        char nome_saida[100];
        sprintf(nome_saida, "%s.dat", sensores[i]);
        FILE *saida = fopen(nome_saida, "w");
        if (!saida) {
            perror("Erro ao criar arquivo");
            continue;
        }

        for (int j = 0; j < total_registros; j++) {
            if (strcmp(registros[j].id_sensor, sensores[i]) == 0) {
                fprintf(saida, "%ld %s ", registros[j].instante, registros[j].id_sensor);
                switch (registros[j].tipo_dado) {
                    case 0: fprintf(saida, "%d\n", registros[j].valor.inteiro); break;
                    case 1: fprintf(saida, "%s\n", registros[j].valor.booleano ? "verdadeiro" : "falso"); break;
                    case 2: fprintf(saida, "%.2f\n", registros[j].valor.racional); break;
                    case 3: fprintf(saida, "%s\n", registros[j].valor.texto); break;
                }
            }
        }
        fclose(saida);
    }

    free(registros);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("Uso: ./organiza_dados arquivo_entrada.txt");
        return 1;
    }
    processar_arquivo(argv[1]);
    return 0;
}