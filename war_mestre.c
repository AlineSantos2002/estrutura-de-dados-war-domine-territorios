#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define NUM_TERRITORIOS 5
#define NUM_MISSOES 2
#define TAM_NOME 30
#define TAM_COR 15

typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa);
void exibirMissao(int idMissao);
void faseDeAtaque(Territorio* mapa);
void simularAtaque(Territorio* origem, Territorio* destino);
int sortearMissao();
int verificarVitoria(const Territorio* mapa, int missaoAtual);
void limparBufferEntrada();

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));
    
    Territorio* mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro: Não foi possível alocar memória para o mapa!\n");
        return 1;
    }
    
    inicializarTerritorios(mapa);
    int missaoAtual = sortearMissao();
    
    printf("=== WAR ESTRUTURADO - NÍVEL MESTRE ===\n");
    printf("Bem-vindo, comandante Azul!\n");
    
    int opcao;
    int vitoria = 0;
    
    do {
        printf("\n========================================\n");
        exibirMapa(mapa);
        printf("\n");
        exibirMissao(missaoAtual);
        printf("\n");
        exibirMenuPrincipal();
        
        printf("Escolha sua ação: ");
        scanf("%d", &opcao);
        limparBufferEntrada();
        
        switch(opcao) {
            case 1:
                faseDeAtaque(mapa);
                break;
            case 2:
                vitoria = verificarVitoria(mapa, missaoAtual);
                if (vitoria) {
                    printf("\n*** PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO E VENCEU O JOGO! ***\n");
                } else {
                    printf("\nMissão ainda não cumprida. Continue lutando!\n");
                }
                break;
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
        
        if (vitoria) {
            break;
        }
        
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
        
    } while(opcao != 0);
    
    liberarMemoria(mapa);
    printf("Jogo finalizado. Obrigado por jogar!\n");
    
    return 0;
}

Territorio* alocarMapa() {
    Territorio* mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    return mapa;
}

void inicializarTerritorios(Territorio* mapa) {
    strcpy(mapa[0].nome, "América");
    strcpy(mapa[0].cor, "Verde");
    mapa[0].tropas = 5;
    
    strcpy(mapa[1].nome, "Europa");
    strcpy(mapa[1].cor, "Azul");
    mapa[1].tropas = 3;
    
    strcpy(mapa[2].nome, "Ásia");
    strcpy(mapa[2].cor, "Vermelho");
    mapa[2].tropas = 2;
    
    strcpy(mapa[3].nome, "África");
    strcpy(mapa[3].cor, "Amarelo");
    mapa[3].tropas = 4;
    
    strcpy(mapa[4].nome, "Oceania");
    strcpy(mapa[4].cor, "Branco");
    mapa[4].tropas = 1;
}

void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
    }
}

void exibirMenuPrincipal() {
    printf("=== MENU DE AÇÕES ===\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
}

void exibirMapa(const Territorio* mapa) {
    printf("=== MAPA DO MUNDO ===\n");
    printf("%-2s | %-15s | %-10s | %s\n", "ID", "Território", "Exército", "Tropas");
    printf("---|-----------------|------------|--------\n");
    
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-2d | %-15s | %-10s | %d\n", 
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void exibirMissao(int idMissao) {
    printf("=== SUA MISSÃO ===\n");
    switch(idMissao) {
        case 0:
            printf("Destruir o exército Verde\n");
            break;
        case 1:
            printf("Conquistar 3 territórios\n");
            break;
    }
}

void faseDeAtaque(Territorio* mapa) {
    int origem, destino;
    
    printf("\n=== FASE DE ATAQUE ===\n");
    printf("Escolha o território de ORIGEM (1-5): ");
    scanf("%d", &origem);
    printf("Escolha o território de DESTINO (1-5): ");
    scanf("%d", &destino);
    limparBufferEntrada();
    
    if (origem < 1 || origem > 5 || destino < 1 || destino > 5) {
        printf("Erro: Territórios inválidos!\n");
        return;
    }
    
    if (origem == destino) {
        printf("Erro: Não pode atacar o próprio território!\n");
        return;
    }
    
    Territorio* terrOrigem = &mapa[origem - 1];
    Territorio* terrDestino = &mapa[destino - 1];
    
    if (strcmp(terrOrigem->cor, "Azul") != 0) {
        printf("Erro: Só pode atacar de territórios sob seu controle!\n");
        return;
    }
    
    if (strcmp(terrDestino->cor, "Azul") == 0) {
        printf("Erro: Só pode atacar territórios inimigos!\n");
        return;
    }
    
    if (terrOrigem->tropas <= 1) {
        printf("Erro: Território de origem precisa ter pelo menos 2 tropas!\n");
        return;
    }
    
    simularAtaque(terrOrigem, terrDestino);
}

void simularAtaque(Territorio* origem, Territorio* destino) {
    printf("\n=== BATALHA: %s vs %s ===\n", origem->nome, destino->nome);
    
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;
    
    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);
    
    if (dadoAtacante > dadoDefensor) {
        destino->tropas--;
        printf("VITÓRIA! %s perde 1 tropa.\n", destino->nome);
        
        if (destino->tropas <= 0) {
            printf("CONQUISTA! %s agora é Azul!\n", destino->nome);
            destino->tropas = 1;
            strcpy(destino->cor, "Azul");
            origem->tropas--;
        }
    } else {
        origem->tropas--;
        printf("DERROTA! %s perde 1 tropa.\n", origem->nome);
    }
}

int sortearMissao() {
    return rand() % NUM_MISSOES;
}

int verificarVitoria(const Territorio* mapa, int missaoAtual) {
    switch(missaoAtual) {
        case 0:
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor, "Verde") == 0 && mapa[i].tropas > 0) {
                    return 0;
                }
            }
            return 1;
            
        case 1:
            int territoriosAzuis = 0;
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor, "Azul") == 0) {
                    territoriosAzuis++;
                }
            }
            return territoriosAzuis >= 3;
            
        default:
            return 0;
    }
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}