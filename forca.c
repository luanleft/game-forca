#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "forca.h"

char palavrasecreta[TAMANHO_PALAVRA];
char chutes[26];
int chutesDados = 0;

void abertura() {
    printf("*********************\n");
    printf("**  Jogo De Forca  **\n");
    printf("*********************\n\n");
}

void chuta() {
    char chute;
    scanf(" %c", &chute);

    chutes[chutesDados] = chute;
    chutesDados++;
}

int jaChutou(char letra) {
    for (int j = 0; j < chutesDados; j++) {
        if (chutes[j] == letra) {
            return 1;
        }
    }
    return 0;
}

void desenhaForca() {
    for (int i = 0; i < strlen(palavrasecreta); i++) {
        int achou = jaChutou(palavrasecreta[i]);
        if (achou) {
            printf("%c", palavrasecreta[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

void adicionaPalavra() {
    char quer;
    printf("Voce deseja adicionar uma nova palavra? (S/N): ");
    scanf(" %c", &quer);

    if (quer == 'S') {
        char novaPalavra[TAMANHO_PALAVRA];
        printf("Qual a nova palavra? ");
        scanf("%s", novaPalavra);

        FILE *f = fopen("palavras.txt", "a");
        if (f == NULL) {
            printf("Desculpe, banco de dados não disponível\n");
            exit(1);
        }

        int qtd;
        fseek(f, 0, SEEK_SET);
        if (fscanf(f, "%d", &qtd) != 1) {
            qtd = 0; 
        }
        qtd++;

        fseek(f, 0, SEEK_SET);
        fprintf(f, "%d\n", qtd);

        fseek(f, 0, SEEK_END);
        fprintf(f, "%s\n", novaPalavra);

        fclose(f);
    }
}

void escolhePalavra() {
    FILE *f = fopen("palavras.txt", "r");
    if (f == NULL) {
        printf("Desculpe, banco de dados nao disponível!\n");
        exit(1);
    }

    int qtddepalavras;
    if (fscanf(f, "%d", &qtddepalavras) != 1) {
        printf("Erro na leitura do número de palavras\n");
        fclose(f);
        exit(1);
    }

    srand(time(0));
    int randomico = rand() % qtddepalavras;

    fseek(f, 0, SEEK_SET); // Reset the file pointer to read the words
    fscanf(f, "%d", &qtddepalavras); // Skip the number of words

    for (int i = 0; i <= randomico; i++) {
        if (fscanf(f, "%s", palavrasecreta) != 1) {
            printf("Erro ao ler palavra\n");
            fclose(f);
            exit(1);
        }
    }

    fclose(f);
}

int enforcou() {
    int erros = 0;
    for (int i = 0; i < chutesDados; i++) {
        int existe = 0;
        for (int j = 0; j < strlen(palavrasecreta); j++) {
            if (chutes[i] == palavrasecreta[j]) {
                existe = 1;
                break;
            }
        }
        if (!existe) {
            erros++;
        }
    }
    return erros >= 5;
}

int acertou() {
    for (int i = 0; i < strlen(palavrasecreta); i++) {
        if (!jaChutou(palavrasecreta[i])) {
            return 0;
        }
    }
    return 1;
}

int main() {
    escolhePalavra();
    abertura();

    do {
        desenhaForca();
        chuta();
    } while (!acertou() && !enforcou());

    if (acertou()) {
        printf("Parabens, voce acertou a palavra!\n");
    } else {
        printf("Voce foi enforcado! A palavra era: %s\n", palavrasecreta);
        adicionaPalavra();

    }

}
