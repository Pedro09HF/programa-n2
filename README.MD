README - Sistema de Processamento de Dados de Sensores

 Descrição
Este projeto consiste em 3 programas inter-relacionados para:
1. Coletar e classificar leituras de sensores (Programa 1)
2. Buscar leituras específicas (Programa 2)
3. Gerar dados simulados (Programa 3)

Todos compartilham o mesmo formato de dados e são totalmente compatíveis.

 Programas

1. coleta_sensores (Programa 1)
Função: 
Processa arquivos brutos de sensores e organiza por tipo de dado.

Uso:  
```bash
./coleta_sensores arquivo_entrada.txt
Entrada:
Arquivo com linhas no formato:
timestamp nome_sensor valor

Saída:
Arquivos .dat separados por sensor, ordenados por timestamp.

2. busca_sensor (Programa 2)
Função:
Busca a leitura mais próxima de um timestamp específico.

Uso:

bash
./busca_sensor nome_sensor timestamp
Exemplo:

bash
./busca_sensor TEMP_001 1654789200
3. gerador_dados (Programa 3)
Função:
Gera dados simulados para teste.

Uso:

bash
./gerador_dados YYYY-MM-DDThh:mm:ss YYYY-MM-DDThh:mm:ss sensor1 tipo1 [sensor2 tipo2...]
Tipos válidos:

CONJ_Z: Inteiros

CONJ_Q: Decimais

TEXTO: Strings

BINARIO: true/false

Exemplo:

bash
./gerador_dados 2023-01-01T00:00:00 2023-01-02T00:00:00 TEMP_001 CONJ_Q PRESSAO CONJ_Z
 Formatos de Dados
Arquivos de Entrada/Saída
plaintext
timestamp nome_sensor valor
Exemplo:
1654789123 TEMP_001 23.45

Tipos de Dados
Tipo	Descrição	Exemplo
CONJ_Z	Números inteiros	42
CONJ_Q	Números decimais	23.45
TEXTO	Texto sem espaços	status
BINARIO	Valores lógicos	true
 Estrutura de Arquivos
plaintext
sistema_sensores/
├── coleta_sensores    # Programa 1
├── busca_sensor       # Programa 2
├── gerador_dados      # Programa 3
├── dados_gerados.txt  # Exemplo de saída
└── sensores/          # Pasta com arquivos .dat
 Como Executar
Compile todos programas:

bash
gcc -o coleta_sensores programa1.c
gcc -o busca_sensor programa2.c
gcc -o gerador_dados programa3.c
Gere dados de teste:

bash
./gerador_dados 2023-01-01T00:00:00 2023-01-05T00:00:00 TEMP CONJ_Q PRESSAO CONJ_Z
Processe os dados:

bash
./coleta_sensores dados_gerados.txt
Consulte os dados:

bash
./busca_sensor TEMP 1654789200
 Observações
Todos os programas validam os formatos de entrada

Timestamps devem ser positivos

Nomes de sensores não podem conter espaços

Arquivos de saída são sobrescritos sem aviso

text

Este README está otimizado para:
- Compatibilidade entre os programas
- Clareza nas instruções
- Exemplos práticos de uso
- Descrição precisa dos formatos de dados