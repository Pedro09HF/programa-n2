#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>

#define LIMITE_TAMANHO_STRING 16

typedef struct {
    long instante;
    char id_sensor[50];
    char valor[100];
} RegistroSensor;

bool is_null_value(const char *valor) {
    return valor == NULL || valor[0] == '\0' || strcmp(valor, "NULL") == 0;
}

int busca_binaria(FILE *arquivo, long alvo, int total_registros) {
    int esq = 0, dir = total_registros - 1;
    long menor_diff = LONG_MAX;
    int idx_proximo = -1;
    RegistroSensor registro;

    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        fseek(arquivo, meio * sizeof(RegistroSensor), SEEK_SET);
        
        if (fread(&registro, sizeof(RegistroSensor), 1, arquivo) != 1) {
            return -1;
        }

        if (is_null_value(registro.valor)) {
            int original_meio = meio;
            while (meio < dir) {
                meio++;
                fseek(arquivo, meio * sizeof(RegistroSensor), SEEK_SET);
                if (fread(&registro, sizeof(RegistroSensor), 1, arquivo) != 1) break;
                if (!is_null_value(registro.valor)) break;
            }
            if (is_null_value(registro.valor)) {
                meio = original_meio;
                while (meio > esq) {
                    meio--;
                    fseek(arquivo, meio * sizeof(RegistroSensor), SEEK_SET);
                    if (fread(&registro, sizeof(RegistroSensor), 1, arquivo) != 1) break;
                    if (!is_null_value(registro.valor)) break;
                }
            }
            if (is_null_value(registro.valor)) {
                if (registro.instante < alvo) esq = original_meio + 1;
                else dir = original_meio - 1;
                continue;
            }
        }

        long diff = labs(registro.instante - alvo);
        if (diff < menor_diff) {
            menor_diff = diff;
            idx_proximo = meio;
        }

        if (registro.instante < alvo) esq = meio + 1;
        else dir = meio - 1;
    }
    return idx_proximo;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s sensor timestamp\n", argv[0]);
        return 1;
    }

    char nome_arquivo[100];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.dat", argv[1]);
    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        fprintf(stderr, "Erro ao abrir arquivo %s: %s\n", nome_arquivo, strerror(errno));
        return 1;
    }

    fseek(arquivo, 0, SEEK_END);
    long file_size = ftell(arquivo);
    if (file_size == -1) {
        perror("Erro ao obter tamanho do arquivo");
        fclose(arquivo);
        return 1;
    }
    int total_registros = file_size / sizeof(RegistroSensor);
    fseek(arquivo, 0, SEEK_SET);

    if (total_registros == 0) {
        fprintf(stderr, "Arquivo vazio ou formato inválido\n");
        fclose(arquivo);
        return 1;
    }

    long timestamp_alvo;
    char *endptr;
    timestamp_alvo = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || errno == ERANGE) {
        fprintf(stderr, "Timestamp inválido: %s\n", argv[2]);
        fclose(arquivo);
        return 1;
    }

    int idx = busca_binaria(arquivo, timestamp_alvo, total_registros);
    if (idx != -1) {
        RegistroSensor registro;
        fseek(arquivo, idx * sizeof(RegistroSensor), SEEK_SET);
        if (fread(&registro, sizeof(RegistroSensor), 1, arquivo) == 1) {
            if (!is_null_value(registro.valor)) {
                printf("Leitura mais próxima: %ld %s %s\n", registro.instante, registro.id_sensor, registro.valor);
            } else {
                printf("Aviso: Registro encontrado contém valor nulo (timestamp: %ld)\n", registro.instante);
            }
        } else {
            perror("Erro ao ler registro encontrado");
        }
    } else {
        puts("Nenhum registro válido encontrado.");
    }

    fclose(arquivo);
    return 0;
}