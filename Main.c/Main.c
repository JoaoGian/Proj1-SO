### Main

#include "simulador_elevadores.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pthread_t threads_elevadores[NUM_ELEVADORES];
    pthread_t thread_monitor_pedidos;

    // Inicializar elevadores
    for (int i = 0; i < NUM_ELEVADORES; i++) {
        elevadores[i].andar_atual = 0;  // Todos os elevadores começam no andar 0
        elevadores[i].destino = 0;
        elevadores[i].ocupado = 0;  // Elevadores começam livres
        pthread_create(&threads_elevadores[i], NULL, gerenciar_elevador, (void*)&elevadores[i]);
    }

    // Criar a thread para monitorar pedidos inseridos manualmente pelo usuário
    pthread_create(&thread_monitor_pedidos, NULL, monitorar_pedidos, NULL);

    // Esperar que todas as threads dos elevadores terminem (embora isso nunca ocorra, pois estão em loop infinito)
    for (int i = 0; i < NUM_ELEVADORES; i++) {
        pthread_join(threads_elevadores[i], NULL);
    }

    pthread_join(thread_monitor_pedidos, NULL);

    // Destruir mutexes e variáveis de condição
    pthread_mutex_destroy(&mutex_pedidos);
    pthread_cond_destroy(&cond_pedido);

    return 0;
}



### simulador_elevadores.h


#ifndef SIMULADOR_ELEVADORES_H
#define SIMULADOR_ELEVADORES_H

#include <pthread.h>

#define NUM_ELEVADORES 3  // Número de elevadores no prédio
#define NUM_ANDARES 25    // Número de andares no prédio (atualizado para 25)

// Estrutura do elevador
typedef struct {
    int andar_atual;
    int destino;
    int ocupado;  // 1 se o elevador está ocupado, 0 se está livre
} Elevador;

// Variáveis globais e mutexes
extern Elevador elevadores[NUM_ELEVADORES];
extern int pedidos[NUM_ANDARES];  // Array que representa a fila de pedidos para cada andar
extern pthread_mutex_t mutex_pedidos; // Mutex para a fila de pedidos
extern pthread_cond_t cond_pedido; // Condição para novo pedido

// Funções utilizadas
void* gerenciar_elevador(void* arg);
void* monitorar_pedidos(void* arg);

#endif // SIMULADOR_ELEVADORES_H


simulador_elevadores.c

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
