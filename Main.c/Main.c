#include "simulador_elevadores.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pthread_t threads_elevadores[NUM_ELEVADORES];
    pthread_t thread_monitor_pedidos;

    // Inicializar elevadores com identificadores únicos
    for (int i = 0; i < NUM_ELEVADORES; i++) {
        elevadores[i].id = i;  // Identificador único para cada elevador
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
