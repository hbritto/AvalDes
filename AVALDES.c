/* -----------------------------------------------------------------------
C�digo gerado com o ASDA - Ambiente de Simula��o Distribu�da Autom�tico
--------------------------------------------------------------------------*/
#ifndef AVALDES
#define AVALDES

// #define MAX_CLIENTS 100

#include "smpl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum reqType { STATIC, DYNAMIC };
typedef enum reqType req_t;

typedef struct request_t {
    req_t type;
    short server;
    short CPU_check;
    float ts_start;
    float ts_end;
} request_t;

void help() {
    printf("Usage:\n");
    printf("./avaldes seed max_clients disk_static_time disk_dynamic_time cpu_static_time cpu_dynamic_time arrival_time use_case\n");
}



int main(int argc, char *argv[]) {
    if(argc <= 8) {
        help();
        return 0;
    }

    /* definicoes */
    unsigned long curr_clients = 0, max_clients = strtoul(argv[2], NULL, 10); //100
    int event, next_client = 1, aleatorio;
    double arrival_time = strtod(argv[7], NULL); //10;
    double disk_service_time[] = {strtod(argv[3], NULL), strtod(argv[4], NULL)}; //{20, 30};
    double CPU_service_time[] = {strtod(argv[5], NULL), strtod(argv[6], NULL)}; //{10, 15};

    request_t *client = (request_t *) malloc(sizeof(request_t) * max_clients);

    /* escalona a chegada do primeiro cliente */
    int seed = atoi(argv[1]);
    stream(seed);

    FILE *p, *saida;
    char filename[100];
    sprintf(filename, "AVALDES_%s_%s.out", argv[1], argv[8]);
    saida = fopen(filename, "w");

    if ((p = sendto(saida)) == NULL)
        printf("Erro na saida \n");

    /* prepara o sistema de simulacao e da nome ao modelo */
    smpl(0, " AVALDES");

    /* cria e da nome às instalações */
    facility("Front", 1);
    facility("CPU1", 1);
    facility("Disk1", 1);
    facility("CPU2", 1);
    facility("Disk2", 1);
    facility("CPU3", 1);
    facility("Disk3", 1);
    facility("CPU4", 1);
    facility("Disk4", 1);

    int i = 0;
    schedule(1, 0.0, i);
    while (curr_clients < max_clients) {
        cause(&event, &i);

        switch (event) {
        case 1:
            aleatorio = randompar(1, 10000);
            if (aleatorio <= 8000)
                client[i].type = STATIC;
            else
                client[i].type = DYNAMIC;

            schedule(2, 0.0, i);

            if (next_client < max_clients)
                schedule(1, 0.0, next_client++);
            break;

        /*  centro de serviço = Front */
        case 2:
            if (request("Front", event, i, 0) == 0)
                schedule(3, expntl(arrival_time), i);
            break;
        case 3:
            release("Front", i);
            aleatorio = randompar(1, 10000);
            if ((1 <= aleatorio) && (aleatorio <= 2500))
                schedule(4, 0.0, i);
            if ((2501 <= aleatorio) && (aleatorio <= 5000))
                schedule(8, 0.0, i);
            if ((5001 <= aleatorio) && (aleatorio <= 7500))
                schedule(12, 0.0, i);
            if ((7501 <= aleatorio) && (aleatorio <= 10000))
                schedule(16, 0.0, i);
            client[i].ts_end = time();
            break;

        /*  centro de serviço = CPU1 */
        case 4:
            if (request("CPU1", event, i, 0) == 0)
                schedule(5, expntl(CPU_service_time[client[i].type]), i);
            break;
        case 5:
            release("CPU1", i);
            aleatorio = randompar(1, 10000);
            if ((1 <= aleatorio) && (aleatorio <= 8000))
                schedule(6, 0.0, i);
            if ((8001 <= aleatorio) && (aleatorio <= 10000))
                curr_clients++;
            client[i].server = 0;
            break;

        /*  centro de serviço = CPU2 */
        case 8:
            if (request("CPU2", event, i, 0) == 0)
                schedule(9, expntl(CPU_service_time[client[i].type]), i);
            break;
        case 9:
            release("CPU2", i);
            aleatorio = randompar(1, 10000);
            if ((1 <= aleatorio) && (aleatorio <= 8000))
                schedule(10, 0.0, i);
            if ((8001 <= aleatorio) && (aleatorio <= 10000))
                curr_clients++;
            client[i].server = 1;
            break;

        /*  centro de serviço = CPU3 */
        case 12:
            if (request("CPU3", event, i, 0) == 0)
                schedule(13, expntl(CPU_service_time[client[i].type]), i);
            break;
        case 13:
            release("CPU3", i);
            aleatorio = randompar(1, 10000);
            if ((1 <= aleatorio) && (aleatorio <= 8000))
                schedule(14, 0.0, i);
            if ((8001 <= aleatorio) && (aleatorio <= 10000))
                curr_clients++;
            client[i].server = 2;
            break;

        /*  centro de serviço = CPU4 */
        case 16:
            if (request("CPU4", event, i, 0) == 0)
                schedule(17, expntl(CPU_service_time[client[i].type]), i);
            break;
        case 17:
            release("CPU4", i);
            aleatorio = randompar(1, 10000);
            if ((1 <= aleatorio) && (aleatorio <= 8000))
                schedule(18, 0.0, i);
            if ((8001 <= aleatorio) && (aleatorio <= 10000))
                curr_clients++;
            client[i].server = 3;
            break;

        /*  centro de serviço = Disk1 */
        case 6:
            if (request("Disk1", event, i, 0) == 0)
                schedule(7, expntl(disk_service_time[client[i].type]), i);
            break;
        case 7:
            release("Disk1", i);
            schedule(4, 0.0, i);
            break;

        /*  centro de serviço = Disk2 */
        case 10:
            if (request("Disk2", event, i, 0) == 0)
                schedule(11, expntl(disk_service_time[client[i].type]), i);
            break;
        case 11:
            release("Disk2", i);
            schedule(8, 0.0, i);
            break;

        /*  centro de serviço = Disk3 */
        case 14:
            if (request("Disk3", event, i, 0) == 0)
                schedule(15, expntl(disk_service_time[client[i].type]), i);
            break;
        case 15:
            release("Disk3", i);
            schedule(12, 0.0, i);
            break;

        /*  centro de serviço = Disk4 */
        case 18:
            if (request("Disk4", event, i, 0) == 0)
                schedule(19, expntl(disk_service_time[client[i].type]), i);
            break;
        case 19:
            release("Disk4", i);
            schedule(16, 0.0, i);
            break;
        }
    }

    // free(client);

    /* gera o relatorio da simulacao */
    fprintf(saida, "TempoSimulado: %f\n", time());

    fprintf(saida, "Utilizacao (\"Front\") = %g\n",
            utilizacao_recurso("Front"));
    fprintf(saida, "Comprimento medio fila (\"Front\") = %g\n",
            comprimento_medio_fila("Front"));
    fprintf(saida, "Periodo medio ocupado (\"Front\") = %g\n\n",
            periodo_medio_ocupado("Front"));
    fprintf(saida, "Utilizacao (\"CPU1\") = %g\n", utilizacao_recurso("CPU1"));
    fprintf(saida, "Comprimento medio fila (\"CPU1\") = %g\n",
            comprimento_medio_fila("CPU1"));
    fprintf(saida, "Periodo medio ocupado (\"CPU1\") = %g\n\n",
            periodo_medio_ocupado("CPU1"));
    fprintf(saida, "Utilizacao (\"Disk1\") = %g\n",
            utilizacao_recurso("Disk1"));
    fprintf(saida, "Comprimento medio fila (\"Disk1\") = %g\n",
            comprimento_medio_fila("Disk1"));
    fprintf(saida, "Periodo medio ocupado (\"Disk1\") = %g\n\n",
            periodo_medio_ocupado("Disk1"));
    fprintf(saida, "Utilizacao (\"CPU2\") = %g\n", utilizacao_recurso("CPU2"));
    fprintf(saida, "Comprimento medio fila (\"CPU2\") = %g\n",
            comprimento_medio_fila("CPU2"));
    fprintf(saida, "Periodo medio ocupado (\"CPU2\") = %g\n\n",
            periodo_medio_ocupado("CPU2"));
    fprintf(saida, "Utilizacao (\"Disk2\") = %g\n",
            utilizacao_recurso("Disk2"));
    fprintf(saida, "Comprimento medio fila (\"Disk2\") = %g\n",
            comprimento_medio_fila("Disk2"));
    fprintf(saida, "Periodo medio ocupado (\"Disk2\") = %g\n\n",
            periodo_medio_ocupado("Disk2"));
    fprintf(saida, "Utilizacao (\"CPU3\") = %g\n", utilizacao_recurso("CPU3"));
    fprintf(saida, "Comprimento medio fila (\"CPU3\") = %g\n",
            comprimento_medio_fila("CPU3"));
    fprintf(saida, "Periodo medio ocupado (\"CPU3\") = %g\n\n",
            periodo_medio_ocupado("CPU3"));
    fprintf(saida, "Utilizacao (\"Disk3\") = %g\n",
            utilizacao_recurso("Disk3"));
    fprintf(saida, "Comprimento medio fila (\"Disk3\") = %g\n",
            comprimento_medio_fila("Disk3"));
    fprintf(saida, "Periodo medio ocupado (\"Disk3\") = %g\n\n",
            periodo_medio_ocupado("Disk3"));
    fprintf(saida, "Utilizacao (\"CPU4\") = %g\n", utilizacao_recurso("CPU4"));
    fprintf(saida, "Comprimento medio fila (\"CPU4\") = %g\n",
            comprimento_medio_fila("CPU4"));
    fprintf(saida, "Periodo medio ocupado (\"CPU4\") = %g\n\n",
            periodo_medio_ocupado("CPU4"));
    fprintf(saida, "Utilizacao (\"Disk4\") = %g\n",
            utilizacao_recurso("Disk4"));
    fprintf(saida, "Comprimento medio fila (\"Disk4\") = %g\n",
            comprimento_medio_fila("Disk4"));
    fprintf(saida, "Periodo medio ocupado (\"Disk4\") = %g\n",
            periodo_medio_ocupado("Disk4"));

    fclose(saida);

    return 0;
}

#endif
/* ----------------------------------------------------------------------- */
