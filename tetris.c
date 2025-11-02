#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

char tiposPecas[] = {'I', 'O', 'T', 'L'};

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// Gera uma nova peça aleatória
Peca gerarPeca(int id) {
    Peca nova;
    nova.nome = tiposPecas[rand() % 4];
    nova.id = id;
    return nova;
}

// Inicializa a fila com peças aleatórias
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->tamanho = 0;
    for (int i = 0; i < TAM_FILA; i++) {
        fila->pecas[fila->tras] = gerarPeca(i);
        fila->tras = (fila->tras + 1) % TAM_FILA;
        fila->tamanho++;
    }
}

// Inicializa a pilha vazia
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

// Mostra o estado atual da fila
void mostrarFila(Fila *fila) {
    printf("Fila de peças:\n");
    int idx = fila->frente;
    for (int i = 0; i < fila->tamanho; i++) {
        printf("[%c %d] ", fila->pecas[idx].nome, fila->pecas[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }
    printf("\n");
}

// Mostra o estado atual da pilha
void mostrarPilha(Pilha *pilha) {
    printf("Pilha de reserva (Topo -> Base):\n");
    if (pilha->topo == -1) {
        printf("(vazia)\n");
        return;
    }
    for (int i = pilha->topo; i >= 0; i--) {
        printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
    }
    printf("\n");
}

// Remove a peça da frente da fila (jogar)
void jogarPeca(Fila *fila, int *proxId) {
    if (fila->tamanho == 0) {
        printf("A fila está vazia!\n\n");
        return;
    }
    printf("Jogando peça [%c %d]\n\n", fila->pecas[fila->frente].nome, fila->pecas[fila->frente].id);
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;
    // Adiciona nova peça para manter fila cheia
    fila->pecas[fila->tras] = gerarPeca((*proxId)++);
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->tamanho++;
}

// Move a peça da frente da fila para a pilha (reservar)
void reservarPeca(Fila *fila, Pilha *pilha, int *proxId) {
    if (fila->tamanho == 0) {
        printf("A fila está vazia!\n\n");
        return;
    }
    if (pilha->topo == TAM_PILHA - 1) {
        printf("A pilha está cheia!\n\n");
        return;
    }
    pilha->pecas[++(pilha->topo)] = fila->pecas[fila->frente];
    printf("Reservando peça [%c %d]\n\n", pilha->pecas[pilha->topo].nome, pilha->pecas[pilha->topo].id);
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;
    // Mantém fila cheia
    fila->pecas[fila->tras] = gerarPeca((*proxId)++);
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->tamanho++;
}

// Usa a peça do topo da pilha
void usarPecaReserva(Pilha *pilha) {
    if (pilha->topo == -1) {
        printf("A pilha está vazia!\n\n");
        return;
    }
    printf("Usando peça reservada [%c %d]\n\n", pilha->pecas[pilha->topo].nome, pilha->pecas[pilha->topo].id);
    pilha->topo--;
}

// Troca a peça da frente da fila com o topo da pilha
void trocarFrenteTopo(Fila *fila, Pilha *pilha) {
    if (fila->tamanho == 0 || pilha->topo == -1) {
        printf("Não há peças suficientes para trocar!\n\n");
        return;
    }
    Peca temp = fila->pecas[fila->frente];
    fila->pecas[fila->frente] = pilha->pecas[pilha->topo];
    pilha->pecas[pilha->topo] = temp;
    printf("Troca realizada entre frente da fila e topo da pilha.\n\n");
}

// Troca os 3 primeiros da fila com as 3 da pilha
void trocarTres(Fila *fila, Pilha *pilha) {
    if (fila->tamanho < 3 || pilha->topo < 2) {
        printf("Não há peças suficientes para troca múltipla!\n\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int idxFila = (fila->frente + i) % TAM_FILA;
        Peca temp = fila->pecas[idxFila];
        fila->pecas[idxFila] = pilha->pecas[pilha->topo - i];
        pilha->pecas[pilha->topo - i] = temp;
    }
    printf("Troca múltipla realizada entre os 3 primeiros da fila e 3 da pilha.\n\n");
}

int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    int proxId = TAM_FILA;
    int opcao;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    do {
        mostrarFila(&fila);
        mostrarPilha(&pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\nEscolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: jogarPeca(&fila, &proxId); break;
            case 2: reservarPeca(&fila, &pilha, &proxId); break;
            case 3: usarPecaReserva(&pilha); break;
            case 4: trocarFrenteTopo(&fila, &pilha); break;
            case 5: trocarTres(&fila, &pilha); break;
            case 0: printf("Saindo do programa.\n"); break;
            default: printf("Opção inválida! Tente novamente.\n\n");
        }
        printf("\n");
    } while(opcao != 0);

    return 0;
}
