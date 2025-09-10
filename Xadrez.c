/* 
 * Tema 4 – Movimentação de Peças de Xadrez
 * Autor: (coloque seu nome)
 * Linguagem: C
 * 
 * Objetivos cobertos:
 * - Estruturas de repetição (for, while, do-while) -> Torre, Bispo, Dama
 * - Loops aninhados -> Cavalo
 * - Recursividade + múltiplas condições -> varredura em raios (Torre/Bispo/Dama)
 */

#include <stdio.h>
#include <ctype.h>

#define N 8

/* ---------- Utilidades de tabuleiro ---------- */

void limpar_tab(char t[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            t[i][j] = '.';
}

void imprimir_tab(char t[N][N]) {
    puts("\n   a b c d e f g h");
    puts("  -----------------");
    for (int i = 0; i < N; i++) {
        printf("%d| ", 8 - i);
        for (int j = 0; j < N; j++) {
            printf("%c ", t[i][j]);
        }
        printf("|%d\n", 8 - i);
    }
    puts("  -----------------");
    puts("   a b c d e f g h\n");
}

int dentro(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

/* Marca uma casa segura (dentro do tabuleiro) */
void marca(char t[N][N], int r, int c) {
    if (dentro(r, c) && t[r][c] == '.')
        t[r][c] = '*';
}

/* ---------- Parte “Mestre”: Recursividade para varrer raios ---------- */
/* Caminha na direção (dr,dc) a partir de (r,c), marcando até bater na borda. */
void varrer_recursivo(char t[N][N], int r, int c, int dr, int dc) {
    int nr = r + dr, nc = c + dc;
    if (!dentro(nr, nc)) return;
    marca(t, nr, nc);
    varrer_recursivo(t, nr, nc, dr, dc);
}

/* ---------- Parte “Novato”: versões iterativas (for/while/do-while) ---------- */

/* Torre (horizontais e verticais) – mistura for, while e do-while */
void torre_iterativa(char t[N][N], int r, int c) {
    /* Direita (for) */
    for (int j = c + 1; j < N; j++) marca(t, r, j);

    /* Esquerda (while) */
    int j = c - 1;
    while (j >= 0) { marca(t, r, j); j--; }

    /* Cima (do-while) */
    int i = r - 1;
    if (i >= 0) {
        do { marca(t, i, c); i--; } while (i >= 0);
    }

    /* Baixo (for) */
    for (int i2 = r + 1; i2 < N; i2++) marca(t, i2, c);
}

/* Bispo (diagonais) – apenas for para simplicidade */
void bispo_iterativo(char t[N][N], int r, int c) {
    for (int k = 1; dentro(r - k, c - k); k++) marca(t, r - k, c - k);
    for (int k = 1; dentro(r - k, c + k); k++) marca(t, r - k, c + k);
    for (int k = 1; dentro(r + k, c - k); k++) marca(t, r + k, c - k);
    for (int k = 1; dentro(r + k, c + k); k++) marca(t, r + k, c + k);
}

/* Dama = Torre + Bispo (iterativo) */
void dama_iterativa(char t[N][N], int r, int c) {
    torre_iterativa(t, r, c);
    bispo_iterativo(t, r, c);
}

/* ---------- Parte “Aventureiro”: loops aninhados para o Cavalo ---------- */
/* offsets de L: (±2,±1) e (±1,±2) – gerados por dois loops aninhados */
void cavalo_loops_aninhados(char t[N][N], int r, int c) {
    int d[2] = {1, 2};
    for (int i = 0; i < 2; i++) {
        for (int s1 = -1; s1 <= 1; s1 += 2) {
            for (int s2 = -1; s2 <= 1; s2 += 2) {
                int a = d[i], b = d[1 - i];          // gera (1,2) e (2,1)
                int nr = r + s1 * a, nc = c + s2 * b;
                marca(t, nr, nc);
            }
        }
    }
}

/* ---------- Versões recursivas para Torre/Bispo/Dama ---------- */

void torre_recursiva(char t[N][N], int r, int c) {
    varrer_recursivo(t, r, c,  0,  1); // direita
    varrer_recursivo(t, r, c,  0, -1); // esquerda
    varrer_recursivo(t, r, c, -1,  0); // cima
    varrer_recursivo(t, r, c,  1,  0); // baixo
}

void bispo_recursivo(char t[N][N], int r, int c) {
    varrer_recursivo(t, r, c, -1, -1);
    varrer_recursivo(t, r, c, -1,  1);
    varrer_recursivo(t, r, c,  1, -1);
    varrer_recursivo(t, r, c,  1,  1);
}

void dama_recursiva(char t[N][N], int r, int c) {
    torre_recursiva(t, r, c);
    bispo_recursivo(t, r, c);
}

/* ---------- Entrada e orquestração ---------- */

int main(void) {
    char tab[N][N];
    limpar_tab(tab);

    char peca;
    int linha, coluna;
    char usaRec;

    puts("=== Movimentacao de Pecas de Xadrez (C) ===");
    puts("Pecas: T=TORRE, B=BISPO, D=DAMA, C=CAVALO");
    printf("Informe a peca: ");
    if (scanf(" %c", &peca) != 1) return 0;

    printf("Informe linha e coluna (1..8 1..8): ");
    if (scanf("%d %d", &linha, &coluna) != 2) return 0;

    printf("Usar recursividade? (S/N): ");
    if (scanf(" %c", &usaRec) != 1) return 0;

    /* Convertendo para índices 0..7 (0 = linha 8, 7 = linha 1 para ficar como xadrez) */
    int r = N - linha;   // linha 8 em cima
    int c = coluna - 1;

    if (!dentro(r, c)) {
        puts("Posicao invalida.");
        return 0;
    }

    tab[r][c] = 'P'; /* marca posicao da peca */

    peca = toupper((unsigned char)peca);
    usaRec = toupper((unsigned char)usaRec);

    if (peca == 'C') {
        /* Cavalo sempre com loops aninhados (módulo aventureiro) */
        cavalo_loops_aninhados(tab, r, c);
    } else {
        int usar_rec = (usaRec == 'S');

        if (peca == 'T') {
            if (usar_rec) torre_recursiva(tab, r, c);
            else          torre_iterativa(tab, r, c);
        } else if (peca == 'B') {
            if (usar_rec) bispo_recursivo(tab, r, c);
            else          bispo_iterativo(tab, r, c);
        } else if (peca == 'D') {
            if (usar_rec) dama_recursiva(tab, r, c);
            else          dama_iterativa(tab, r, c);
        } else {
            puts("Peca invalida.");
            return 0;
        }
    }

    imprimir_tab(tab);

    puts("Legenda: P = peca | * = movimentos possiveis | . = vazio");
    puts("Estruturas usadas: for, while, do-while, loops aninhados e recursividade.");
    return 0;
}