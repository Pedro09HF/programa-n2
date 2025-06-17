Descrição dos programas
Este conjunto de programas tem como objetivo gerar, organizar e consultar dados de sensores em diferentes formatos e tipos.

gerador.c

Função:
Gera um arquivo de dados de sensores simulados, com registros contendo timestamp, nome do sensor e valor do sensor.
Os tipos suportados são:

CONJ_Z — dados do tipo inteiro

BINARIO — dados do tipo booleano

CONJ_Q — dados do tipo float

TEXTO — dados do tipo string

Inclui uma pequena probabilidade de gerar valores nulos ("NULL").

Uso:
./gerador inicio_unix fim_unix sensor1 tipo1 sensor2 tipo2 ...

inicio_unix e fim_unix são os timestamps unix que delimitam o intervalo de geração de dados.

Para cada sensor, passe o nome e o tipo (usando as strings: CONJ_Z, BINARIO, CONJ_Q, TEXTO).

Exemplo:
./gerador 1620000000 1620003600 sensorA CONJ_Z sensorB BINARIO sensorC CONJ_Q sensorD TEXTO

Gera o arquivo dados_teste.txt com os registros.

organizador.c

Função:
Lê um arquivo texto com registros de sensores (timestamp, sensor, valor), processa os dados e gera arquivos binários .dat para cada sensor.
Os registros são organizados em ordem decrescente pelo timestamp.

O programa pergunta o tipo do dado para cada sensor (0=inteiro, 1=booleano, 2=float, 3=string) na primeira ocorrência e valida os valores conforme o tipo.

Uso:
./organizador arquivo_entrada.txt

arquivo_entrada.txt deve conter registros no formato:
timestamp nome_sensor valor
por linha.

Gera arquivos .dat para cada sensor com os dados organizados.

consultor.c

Função:
Consulta um arquivo .dat gerado pelo organizador.c e busca um registro com timestamp mais próximo do informado.
O arquivo deve estar ordenado em ordem decrescente de timestamp para que a busca binária funcione corretamente.

Trata valores nulos ("NULL") e ignora registros inválidos.

Uso:
./consultor sensor timestamp

sensor: nome do sensor cujo arquivo sensor.dat será consultado.

timestamp: valor unix timestamp para busca.

Imprime o registro mais próximo encontrado.

Fluxo recomendado

Use o gerador.c para criar um arquivo de dados simulados.

Use o organizador.c para processar e gerar arquivos binários organizados.

Use o consultor.c para consultar os arquivos binários e buscar registros específicos.

Requisitos

Compilador C (gcc, clang, etc)

Sistema Unix/Linux ou Windows com suporte a compilação C

Compilação

gcc gerador.c -o gerador
gcc organizador.c -o organizador
gcc consultor.c -o consultor

