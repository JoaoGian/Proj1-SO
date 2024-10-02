# Documentação do Simulador de Elevadores

### Introdução
Este documento apresenta um simulador de elevadores desenvolvido em linguagem C, utilizando multithreading. O objetivo é simular o funcionamento de elevadores em um prédio de 25 andares, atendendo a pedidos feitos pelos usuários. Para isso, o simulador utiliza threads, sincronização e controle de concorrência, garantindo que os elevadores operem de forma correta e eficiente.

O sistema controla três elevadores independentes que respondem aos pedidos inseridos manualmente pelos usuários. A implementação foca em resolver problemas de concorrência, evitar condições de corrida (race conditions) e assegurar que todas as threads funcionem de maneira sincronizada.

### Descrição do Projeto
Estrutura Geral
O simulador é composto por três componentes principais:

Elevadores (Threads de Elevador): Representam os elevadores no prédio. Cada elevador é controlado por uma thread que gerencia sua movimentação entre os andares e atende aos pedidos dos usuários.

Fila de Pedidos (Array Compartilhado): Armazena os pedidos feitos pelos usuários. Cada posição nesse array corresponde a um andar específico e indica quantos pedidos estão pendentes para aquele andar.

Monitor de Pedidos (Thread de Monitoramento): Responsável por receber as entradas dos usuários, criar novos pedidos e atualizar a fila de pedidos.

### Comunicação entre Threads
Para que as threads dos elevadores e a thread de monitoramento possam se comunicar sem conflitos, utilizamos variáveis de condição e mutexes (um tipo de trava). O mutex garante que apenas uma thread por vez possa acessar ou modificar a fila de pedidos, evitando que duas threads tentem alterar os mesmos dados simultaneamente.

### Lógica de Concorrência
Controle de Concorrência
Para prevenir condições de corrida, tanto a fila de pedidos quanto o estado dos elevadores são protegidos por um mutex chamado mutex_pedidos. Sempre que uma thread precisa acessar ou modificar a fila de pedidos, ela deve primeiro adquirir esse mutex. Além disso, usamos uma variável de condição chamada cond_pedido para notificar os elevadores quando um novo pedido é adicionado à fila.

Quando um usuário faz um novo pedido, a thread de monitoramento sinaliza (pthread_cond_signal) essa variável de condição, avisando aos elevadores que há um pedido a ser atendido. Cada elevador verifica a fila de pedidos e atende ao primeiro pedido disponível, deslocando-se até o andar solicitado.

### Evitando Problemas de Concorrência
Para garantir que o sistema funcione sem conflitos, adotamos as seguintes estratégias:

Uso de Mutexes: O mutex mutex_pedidos protege a fila de pedidos e o estado dos elevadores, garantindo que apenas uma thread por vez possa acessar esses recursos. Isso evita que múltiplas threads tentem modificar os mesmos dados ao mesmo tempo.

Variáveis de Condição: A variável cond_pedido é usada para notificar os elevadores sobre novos pedidos, evitando que eles fiquem verificando a fila constantemente sem necessidade (o que seria ineficiente).

Liberação Rápida de Mutexes: Os mutexes são liberados imediatamente após as modificações necessárias, reduzindo o tempo em que as threads ficam bloqueadas e melhorando a eficiência do sistema.

### Decisões de Design
Estrutura de Dados Utilizada
Fila de Pedidos: Implementada como um array int pedidos[NUM_ANDARES], onde NUM_ANDARES é o número total de andares (25 no caso). Cada posição do array representa o número de pedidos pendentes para aquele andar.

Estrutura do Elevador: Cada elevador tem uma estrutura que armazena:

id: Identificador único do elevador (0, 1 ou 2).
andar_atual: Andar em que o elevador está no momento.
destino: Andar para onde o elevador deve ir para atender a um pedido.
ocupado: Indica se o elevador está ocupado (1) ou livre (0).
#Distribuição de Pedidos entre Elevadores
A lógica para determinar qual elevador atende a um pedido é simples: cada elevador verifica a fila de pedidos e atende ao primeiro pedido disponível. Não há otimização para escolher o elevador mais próximo, mas a sincronização impede que dois elevadores atendam ao mesmo pedido ao mesmo tempo.

### Movimentação dos Elevadores
Para simular o tempo de deslocamento entre os andares, usamos a função sleep(abs(elevador->andar_atual - elevador->destino)). Isso faz com que o elevador "espere" por um tempo proporcional à distância que precisa percorrer. Após chegar ao destino, o elevador fica disponível para novos pedidos.

### Comportamento do Sistema durante a Execução
--Inicialização

Os três elevadores são iniciados e posicionados no andar 0.
A thread de monitoramento de pedidos é iniciada para permitir que os usuários façam pedidos.
--Recebimento de Pedidos

O usuário insere o andar de destino (entre 0 e 24) para criar um novo pedido.
A thread de monitoramento adiciona o pedido à fila e notifica os elevadores sobre a nova solicitação.
--Movimentação dos Elevadores

Cada elevador verifica a fila de pedidos.
Se houver um pedido pendente, o elevador se move até o andar solicitado para atendê-lo.
O sistema exibe mensagens no console para informar sobre a movimentação e o atendimento do pedido.
--Logs do Sistema

O simulador gera logs para acompanhar as ações dos elevadores, como:

Recebimento de novos pedidos.
Movimentação entre andares.
Atendimento de pedidos.
Mudanças no estado do elevador (ocupado ou livre).
--Finalização

O simulador é executado em um loop infinito e não termina automaticamente.
Para encerrar o programa, é necessário interrompê-lo manualmente (por exemplo, usando Ctrl + C).
#Conclusão
O simulador de elevadores implementa de forma eficaz o uso de threads, mutexes e variáveis de condição para gerenciar a concorrência e a comunicação entre os elevadores e os pedidos. Embora a lógica seja simples, ela é suficiente para simular um sistema real de elevadores e pode ser facilmente expandida para incluir funcionalidades mais avançadas, como otimização na distribuição de pedidos e controle de capacidade dos elevadores.

Com a documentação e os logs gerados, é possível entender claramente como o sistema funciona e como os problemas comuns de concorrência, como condições de corrida e deadlocks, foram evitados.

### Instruções para Executar o Simulador de Elevadores
Ao executar o simulador, você pode interagir com ele inserindo pedidos para os elevadores:

--Inserir Pedido:

O programa solicitará que você digite um andar de destino.
Insira um número entre 0 e 24 (os andares do prédio).
Pressione Enter para adicionar o pedido.
Acompanhar a Execução:

O simulador exibirá mensagens indicando:
Qual elevador aceitou o pedido.
A movimentação do elevador entre os andares.
A conclusão do atendimento.

### Exemplo de Interação

Digite o andar de destino (0 a 24) para criar um pedido: 5
Pedido registrado para o andar 5
Elevador 0 aceitou o pedido para o andar 5
Elevador 0 movendo-se do andar 0 para o andar 5
Elevador 0 chegou ao andar 5




