### Documentação do Simulador de Elevadores

1. Introdução
Este documento descreve a implementação de um simulador de elevadores utilizando o conceito de multithreading em C. O simulador foi desenvolvido para atender a critérios específicos de um projeto de sistemas operacionais, envolvendo o uso de threads, sincronização e controle de concorrência para simular o comportamento de elevadores em um prédio de 25 andares.

O sistema gerencia 3 elevadores independentes que respondem a pedidos feitos manualmente pelo usuário. A implementação foca em resolver problemas de concorrência, evitar condições de corrida (race conditions) e garantir a execução correta e sincronizada das threads.

2. Descrição do Projeto
2.1. Estrutura Geral
O simulador utiliza três componentes principais:

Elevadores (Threads de Elevador): Representam os elevadores no prédio. Cada elevador é uma thread que gerencia a movimentação entre os andares e atende aos pedidos feitos pelo usuário.
Fila de Pedidos (Array Compartilhado): Armazena os pedidos feitos pelo usuário. Cada posição do array indica o número de pedidos pendentes para um andar específico.
Monitor de Pedidos (Thread de Monitoramento): Responsável por receber as entradas do usuário, criando novos pedidos e atualizando a fila de pedidos.
2.2. Comunicação entre Threads
As threads dos elevadores e a thread de monitoramento de pedidos utilizam variáveis de condição e mutexes para se comunicarem e evitar conflitos de acesso a recursos compartilhados (como a fila de pedidos). Um mutex é utilizado para proteger o acesso à fila de pedidos e garantir que apenas uma thread possa modificar o array de pedidos por vez.

3. Lógica de Concorrência
3.1. Controle de Concorrência
Para evitar condições de corrida, a fila de pedidos e o estado dos elevadores são protegidos por um mutex (pthread_mutex_t mutex_pedidos). Este mutex é utilizado por qualquer thread que deseja acessar ou modificar a fila de pedidos. Além disso, uma variável de condição (pthread_cond_t cond_pedido) é usada para notificar os elevadores quando um novo pedido é adicionado à fila.

Quando um novo pedido é criado, a thread de monitoramento sinaliza (pthread_cond_signal) a variável de condição para informar às threads dos elevadores que há novos pedidos para serem atendidos. Cada elevador verifica a fila de pedidos e atende ao primeiro pedido disponível, movendo-se para o andar solicitado.

3.2. Evitando Deadlocks e Race Conditions
O simulador utiliza as seguintes estratégias para evitar problemas de concorrência:

Uso de Mutexes: O mutex mutex_pedidos protege a fila de pedidos e o estado dos elevadores para garantir que somente uma thread por vez possa acessar ou modificar esses recursos. Isso evita race conditions, onde múltiplas threads tentam modificar os mesmos recursos simultaneamente.
Variáveis de Condição: A variável cond_pedido é utilizada para notificar os elevadores sobre a presença de novos pedidos, evitando que os elevadores entrem em um loop de verificação desnecessário (busy waiting).
Organização e Liberação de Mutexes: Todos os mutexes são liberados logo após a modificação dos recursos, minimizando o tempo de bloqueio de threads.
4. Decisões de Design
4.1. Estrutura de Dados Utilizada
A fila de pedidos é implementada como um array int pedidos[NUM_ANDARES], onde NUM_ANDARES é o número total de andares (25 no caso). Cada posição do array representa o número de pedidos pendentes para o respectivo andar.

O estado de cada elevador é armazenado na estrutura Elevador, que contém as seguintes informações:

id: Identificador único do elevador (0, 1, 2).
andar_atual: Andar em que o elevador se encontra atualmente.
destino: Andar de destino que o elevador deve alcançar para atender a um pedido.
ocupado: Indica se o elevador está ocupado (1) ou livre (0).
4.2. Distribuição de Pedidos entre Elevadores
A lógica de distribuição de pedidos é simples: cada elevador verifica a fila de pedidos e atende ao primeiro pedido disponível. O sistema não implementa otimização para a escolha do elevador mais próximo, mas utiliza a sincronização para evitar que dois elevadores atendam o mesmo pedido ao mesmo tempo.

4.3. Movimentação dos Elevadores
Cada elevador se movimenta para o andar de destino utilizando um sleep(abs(elevador->andar_atual - elevador->destino)) para simular o tempo de deslocamento entre andares. Após chegar ao destino, o elevador libera o recurso e se torna disponível para novos pedidos.

5. Comportamento do Sistema durante a Execução
Durante a execução, o simulador segue a seguinte sequência de ações:

Inicialização:

Os três elevadores são inicializados e posicionados no andar 0.
A thread de monitoramento de pedidos é criada para permitir que o usuário insira manualmente os pedidos.
Recebimento de Pedidos:

O usuário insere o andar de destino (0 a 24) para criar um novo pedido.
A thread de monitoramento adiciona o pedido à fila e sinaliza a presença de um novo pedido usando pthread_cond_signal.
Movimentação dos Elevadores:

Cada elevador verifica a fila de pedidos e, se houver um pedido pendente, se move até o andar de destino para atendê-lo.
Logs são gerados para registrar a movimentação do elevador e o atendimento do pedido.
Logs do Sistema:

Os logs são gerados para cada ação do elevador, como:
Recebimento de novos pedidos.
Movimentação entre andares.
Atendimento de pedidos.
Estado do elevador (ocupado ou livre).
Finalização:

Como o simulador é um loop infinito, ele não termina automaticamente. A finalização do programa deve ser feita manualmente (por exemplo, interrompendo o processo com Ctrl + C).
6. Conclusão
O simulador de elevadores implementa corretamente o uso de threads, mutexes e variáveis de condição para gerenciar a concorrência e a comunicação entre elevadores e pedidos. A lógica é simples, mas efetiva para simular um sistema de elevadores, e pode ser facilmente expandida para incluir funcionalidades mais avançadas, como otimização de alocação de pedidos e controle de capacidade dos elevadores.

Com a documentação e os logs gerados durante a execução, é possível acompanhar o comportamento do sistema e entender como a concorrência e a sincronização foram implementadas para evitar problemas comuns como race conditions e deadlocks.


### Instruções para  Execução do Simulador de Elevadores

Quando o simulador estiver rodando, você pode interagir com ele inserindo manualmente os pedidos para os elevadores:

O programa solicitará que você digite um andar de destino para criar um pedido.
Insira um número entre 0 e 24 (representando os andares do prédio).
Pressione Enter para adicionar o pedido.
O simulador exibirá mensagens no console indicando qual elevador aceitou o pedido, a movimentação entre os andares e a conclusão do atendimento.

#Exemplo de interação:

Digite o andar de destino (0 a 24) para criar um pedido: 5
Pedido registrado no andar 5
Elevador 0 aceitou pedido para o andar 5
Elevador 0 movendo-se do andar 0 para o andar 5
Elevador 0 chegou ao andar 5






