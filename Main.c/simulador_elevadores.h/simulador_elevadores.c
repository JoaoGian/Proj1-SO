#include "simulador_elevadores.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

Elevador elevadores[NUM_ELEVADORES];  // Array que representa o estado de cada elevador
int pedidos[NUM_ANDARES] = {0};        // Fila de pedidos
pthread_mutex_t mutex_pedidos = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger a fila de pedidos
pthread_cond_t cond_pedido = PTHREAD_COND_INITIALIZER;     // Condição para indicar novos pedidos

// Função para gerenciar o comportamento de cada elevador
void* gerenciar_elevador(void* arg) {
    Elevador* elevador = (Elevador*) arg;
    while(1) {
        pthread_mutex_lock(&mutex_pedidos);
        
        // Verificar se há algum pedido para atender
        while (elevador->ocupado == 0) {
            int pedido_encontrado = 0;
            for (int i = 0; i < NUM_ANDARES; i++) {
                if (pedidos[i] > 0) { // Se há um pedido em um andar
                    printf("Elevador %ld aceitou pedido para o andar %d\n", (intptr_t)elevador, i);
                    elevador->destino = i;
                    elevador->ocupado = 1;
                    pedidos[i]--; // Remove o pedido da fila
                    pedido_encontrado = 1;
                    break;
                }
            }

            // Se não há pedidos, esperar até um novo pedido ser adicionado
            if (!pedido_encontrado) {
                printf("Elevador %ld aguardando novos pedidos...\n", (intptr_t)elevador);
                pthread_cond_wait(&cond_pedido, &mutex_pedidos);
            }
        }
        pthread_mutex_unlock(&mutex_pedidos);

        // Movimentação do elevador até o destino
        printf("Elevador %ld movendo-se do andar %d para o andar %d\n", (intptr_t)elevador, elevador->andar_atual, elevador->destino);
        sleep(abs(elevador->andar_atual - elevador->destino));  // Simula o tempo de movimentação entre andares
        elevador->andar_atual = elevador->destino;
        printf("Elevador %ld chegou ao andar %d\n", (intptr_t)elevador, elevador->andar_atual);

        // Liberar o elevador
        pthread_mutex_lock(&mutex_pedidos);
        elevador->ocupado = 0;
        pthread_mutex_unlock(&mutex_pedidos);
    }
    return NULL;
}

// Função para monitorar a entrada de pedidos do usuário
void* monitorar_pedidos(void* arg) {
    while(1) {
        int andar;
        printf("Digite o andar de destino (0 a %d) para criar um pedido: ", NUM_ANDARES - 1);
        scanf("%d", &andar);

        if (andar >= 0 && andar < NUM_ANDARES) {
            pthread_mutex_lock(&mutex_pedidos);
            pedidos[andar]++;  // Adicionar um pedido ao andar especificado pelo usuário
            printf("Pedido registrado no andar %d\n", andar);
            pthread_cond_signal(&cond_pedido);  // Sinalizar que há um novo pedido
            pthread_mutex_unlock(&mutex_pedidos);
        } else {
            printf("Andar inválido! Por favor, insira um valor entre 0 e %d.\n", NUM_ANDARES - 1);
        }
    }
    return NULL;
}
