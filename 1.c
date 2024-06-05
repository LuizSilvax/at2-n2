#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Musica {  
    char artista[100];
    char nome[100];
    struct Musica* prev;
    struct Musica* next;
} Musica;

typedef struct {
    Musica* head;
} Playlist;

Musica* criarMusica(char* artista, char* nome) {
    Musica* novaMusica = (Musica*)malloc(sizeof(Musica));
    strcpy(novaMusica->artista, artista);
    strcpy(novaMusica->nome, nome);
    novaMusica->prev = novaMusica;
    novaMusica->next = novaMusica;
    return novaMusica;
}

void inicializarPlaylist(Playlist* playlist) {
    playlist->head = NULL;
}

void adicionarMusica(Playlist* playlist, char* artista, char* nome) {
    Musica* novaMusica = criarMusica(artista, nome);
    if (playlist->head == NULL) {
        playlist->head = novaMusica;
    } else {
        Musica* tail = playlist->head->prev;
        tail->next = novaMusica;
        novaMusica->prev = tail;
        novaMusica->next = playlist->head;
        playlist->head->prev = novaMusica;
    }
}

void exibirPlaylist(Playlist* playlist) {
    if (playlist->head == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }
    Musica* atual = playlist->head;
    do {
        printf("Artista: %s, Musica: %s\n", atual->artista, atual->nome);
        atual = atual->next;
    } while (atual != playlist->head);
}

void carregarPlaylist(Playlist* playlist, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }
    char linha[200];
    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = '\0';  
        char* artista = strtok(linha, ";");
        char* nome = strtok(NULL, ";");
        if (artista && nome) {
            adicionarMusica(playlist, artista, nome);
        }
    }
    fclose(arquivo);
}

void salvarPlaylist(Playlist* playlist, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }
    if (playlist->head == NULL) {
        fclose(arquivo);
        return;
    }
    Musica* atual = playlist->head;
    do {
        fprintf(arquivo, "%s;%s\n", atual->artista, atual->nome);
        atual = atual->next;
    } while (atual != playlist->head);
    fclose(arquivo);
}

void inserirMusica(Playlist* playlist, const char* nomeArquivo) {
    char artista[100];
    char nome[100];
    printf("Digite o nome do artista: ");
    scanf(" %[^\n]s", artista);
    printf("Digite o nome da música: ");
    scanf(" %[^\n]s", nome);
    adicionarMusica(playlist, artista, nome);
    salvarPlaylist(playlist, nomeArquivo);
}

void removerMusica(Playlist* playlist, const char* nomeArquivo) {
    if (playlist->head == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }
    char nome[100];
    printf("Digite o nome da musica a ser removida: ");
    scanf(" %[^\n]s", nome);
    Musica* atual = playlist->head;
    do {
        if (strcmp(atual->nome, nome) == 0) {
            if (atual->prev == atual && atual->next == atual) {
                playlist->head = NULL;
            } else {
                atual->prev->next = atual->next;
                atual->next->prev = atual->prev;
                if (atual == playlist->head) {
                    playlist->head = atual->next;
                }
            }
            free(atual);
            salvarPlaylist(playlist, nomeArquivo);
            printf("Musica removida com sucesso.\n");
            return;
        }
        atual = atual->next;
    } while (atual != playlist->head);
    printf("Musica não encontrada.\n");
}

void buscarMusica(Playlist* playlist) {
    if (playlist->head == NULL) {
        printf("A playlist está vazia.\n");
        return;
    }
    char nome[100];
    printf("Digite o nome da musica a ser buscada: ");
    scanf(" %[^\n]s", nome);
    Musica* atual = playlist->head;
    do {
        if (strcmp(atual->nome, nome) == 0) {
            printf("Artista: %s, Musica: %s\n", atual->artista, atual->nome);
            return;
        }
        atual = atual->next;
    } while (atual != playlist->head);
    printf("Musica não encontrada.\n");
}

void menu(Playlist* playlist, const char* nomeArquivo) {
    int opcao;
    Musica* atual = playlist->head;
    do {
        printf("\nMenu:\n");
        printf("1. Exibir playlist\n");
        printf("2. Inserir nova musica\n");
        printf("3. Remover uma musica\n");
        printf("4. Buscar uma musica\n");
        printf("5. Avancar para proxima musica\n");
        printf("6. Retornar a musica anterior\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                exibirPlaylist(playlist);
                break;
            case 2:
                inserirMusica(playlist, nomeArquivo);
                break;
            case 3:
                removerMusica(playlist, nomeArquivo);
                break;
            case 4:
                buscarMusica(playlist);
                break;
            case 5:
                if (atual != NULL) {
                    atual = atual->next;
                    printf("Musica atual: Artista: %s, Musica: %s\n", atual->artista, atual->nome);
                } else {
                    printf("A playlist está vazia.\n");
                }
                break;
            case 6:
                if (atual != NULL) {
                    atual = atual->prev;
                    printf("Musica atual: Artista: %s, Musica: %s\n", atual->artista, atual->nome);
                } else {
                    printf("A playlist está vazia.\n");
                }
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao inválida.\n");
        }
    } while (opcao != 0);
}

int main() {
    Playlist playlist;
    inicializarPlaylist(&playlist);
    carregarPlaylist(&playlist, "musicas.txt");
    menu(&playlist, "musicas.txt");
    return 0;
}
