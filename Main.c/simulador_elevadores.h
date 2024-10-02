#ifndef SIMULADOR_ELEVADORES_H
#define SIMULADOR_ELEVADORES_H

#include <pthread.h>

#define NUM_ELEVADORES 3  // Número de elevadores no prédio
#define NUM_ANDARES 25    // Número de andares no prédio

// Estrutura do elevador
typedef struct {
    int id;              // Identificador único para o elevador
    int andar_atual;
    int destino;
    int ocupado;         // 1 se o elevador está ocupado, 0 se está livre
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
