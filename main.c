#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <SDL.h>
#include <SDL_image.h>

#define WIDTH 640
#define HEIGHT 480

SDL_Window * window;
SDL_Renderer * render;
SDL_Event evento;

/*ESTE TABULEIRO ESTÁ REPRESENTADO POR NÚMEROS (8×8).
?NUMEROS PARES(BRANCAS), NUMEROS ÍMPARES(PRETAS).
10(peão) 20(bispo) 30(cavalo) 40(torre) 50(rainha) 60(rei)*/


int xadrez [8][8] = {                                  //Peças do xadrez
    41, 31, 21, 51, 61, 21, 31, 41,
    11, 11, 11, 11, 11, 11, 11, 11,
    00, 00, 00, 00, 00, 00, 00, 00,
    00, 00, 00, 00, 00, 00, 00, 00,
    00, 00, 00, 00, 00, 00, 00, 00,
    00, 00, 00, 00, 00, 00, 00, 00,
    10, 10, 10, 10, 10, 10, 10, 10,
    40, 30, 20, 50, 60, 20, 30, 40,
};


SDL_Texture * imagens[12];
char nomesImagens[][50] = {"BB.PNG", "BP.PNG", "CB.PNG", "CP.PNG",                  //Imagens das peças
                           "KB.PNG", "KP.PNG", "QB.PNG", "QP.PNG",
                           "PB.PNG", "PP.PNG", "TB.PNG", "TP.PNG"};
int pecas[] = {20, 21, 30, 31, 60, 61, 50, 51, 10, 11, 40, 41};

int linhaOrigem = -1;
int colunaOrigem = -1;

int init_graficos(){

	if (SDL_Init( SDL_INIT_EVERYTHING ) >= 0 ) {
            printf("\n Inicializou o SDL...");
        window = SDL_CreateWindow("X A D R E Z",
        50, 50, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

        if (window != NULL) {
            render = SDL_CreateRenderer(window, -1, 0);
            if (render != NULL) {
			}

    }
	}
}

int init_imagens() {
    if(IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG){                     //Imagens
        int i;
        printf(" Carregando imagens .... \n");
        for (i = 0; i<12; i++){
            char nomeImg[50] = "imagens/";
            strcat(nomeImg, nomesImagens[i]);
            printf(" Imagens : %s\n", nomeImg);
            SDL_Surface * surImage = IMG_Load(nomeImg);
            imagens[i] = SDL_CreateTextureFromSurface( render, surImage);
            }
        }
        else{
            printf("Nao foi possivel iniciar a imagem");
        }


}

 SDL_Texture * imagemFromPeca (char peca) {
    int i;
    for (i = 0; i < 12; i++){
        if (pecas[i] == peca){
            return imagens[i];
        }
    }
}

int pintarTela(){
    int w = WIDTH / 8;                                      //Pintura da tela
    int h = HEIGHT / 8;
    int lin, col;


    SDL_SetRenderDrawColor( render, 0, 0, 0, 255);
    SDL_RenderClear(render);

    for (lin = 0; lin < 8 ; lin++) {
        for(col=0; col < 8; col++) {
            if ((col + lin) % 2 == 0){
                SDL_SetRenderDrawColor( render, 100, 100, 100, 255);
            } else{
                SDL_SetRenderDrawColor( render, 255, 255, 0, 255);
            }
            int x = col * w;
            int y = lin * h;
            SDL_Rect r = {x, y, w, h};
            SDL_RenderFillRect(render, &r);
            char p = xadrez[lin][col];
            if (p != ' '){
                int wOri, hOri;
                SDL_Texture * img = imagemFromPeca( p );
                SDL_QueryTexture(img, NULL, NULL, &wOri, &hOri);
                SDL_Rect rOrig = {0, 0, wOri, hOri};
                SDL_RenderCopy(render, img, &rOrig, &r);
            }
            if(linhaOrigem == lin && colunaOrigem == col){
                SDL_SetRenderDrawColor( render, 0, 0, 255, 255);
                SDL_RenderDrawRect(render, &r);
            }
        }
    }

    SDL_RenderPresent(render);
}
int pintarTelaTexto() {

	system("cls");

	int lin, col, r;

	for (lin = 0; lin < 8 ; lin++) {
		for(r = 0; r < 3; r++) {
			for(col=0; col < 8; col++) {

				char peca = xadrez[lin][col];
				char letra = ( (lin + col) % 2 == 0) ? '\xB2': 00;

				letra = (r == 1 && peca != 00) ? peca : letra;       //Operador Ternario
				if ( (lin + col) % 2 == 0) {

					printf("\xB2\xB2\%c\xB2\xB2", letra);
				}
				else {
					printf ("  %c  ", letra);
				}

			}
			printf("\n");

		}
	}
}

int moverPeca ( int linOri, int colOri, int linDes, int colDes){
    int capturaEventos();
	int mover = 0;                                                              //Movimento de cada peça
	char peca;
	int dV = abs(linDes - linOri);
	int dH = abs(colDes - colOri);

		if ((linOri >=0 && linOri < 8 && colOri >=0 && colOri < 8) &&
			(linDes >=0 && linDes < 8 && colDes >=0 && colDes < 8) &&
			(dV + dH > 0)){

			peca = xadrez[linOri][colOri];

			if ((peca == 41 || peca == 40 ) && (dV == 0 || dH == 0) ) {
				mover = 1;
			}
			if ((peca == 21 || peca == 20 ) && (dV == dH) ) {
				mover = 1;
			}
			if ((peca == 31 || peca == 30 ) &&
			( (dV == 1 && dH == 2)  || (dV == 2 && dH == 1) )) {
				mover = 1;
			}
			if ((peca == 51 || peca == 50 ) &&
			((dV == dH) || (dV == 0) || dH == 0 ) ) {
				mover = 1;
			}
			if ((peca == 61 || peca == 60 ) &&
			((dV >= 0 && dV <= 1) && (dV >= 0 && dH <= 1)) ) {
				mover = 1;
			}
			if ((peca == 11) && ((linDes - linOri) == 1) && (dH == 0)) {
				mover = 1;
				if(xadrez[1]){
                    mover = 2;
				}
			}
			if ((peca == 10) && ((linOri - linDes) == 1) && (dH == 0)) {
				mover = 1;
			}
			if (peca % 2 == 0){     //Peças Brancas

			}
			if (peca % 2 != 0){     //Peças Pretas

			}

			if (mover) {
				xadrez[linDes][colDes] = xadrez[linOri][colOri];
				xadrez [linOri][colOri] = 00;
				return 1;	// Movimento realizado
			}
				else {
					return 9; // Peça nao pode ser movida deste jeito
				}
	}
	else {
		return 0;
	}
}

int capturaEventos(){
    int e = SDL_PollEvent(&evento);
    if (evento.type == SDL_QUIT){
        exit(0);
    }
    else if(evento.type == SDL_MOUSEMOTION){
        //printf(" X : %d, Y: %d \n", evento.motion.x, evento.motion.y);
    }
    else if(evento.type == SDL_MOUSEBUTTONDOWN){
        int w = WIDTH / 8;
        int h = HEIGHT / 8;
        //printf(" X : %d, Y: %d \n", evento.motion.x, evento.motion.y);
        int coluna = evento.motion.x / w;
        int linha = evento.motion.y / h;

        if(linhaOrigem == -1){
        linhaOrigem = linha;
        colunaOrigem = coluna;
        }
        else{
            moverPeca(linhaOrigem, colunaOrigem, linha, coluna);
            linhaOrigem = -1;
            colunaOrigem = -1;
        }
        printf("Linha :  %d   Coluna:   %d\n", linha, coluna);
    }
}

int main(int argc, char ** args){
    init_graficos();
    init_imagens();
	int linOri,linDes, colOri, colDes;
	while (1) {
        pintarTela();
        capturaEventos();
        SDL_Delay(20);

		/*pintarTelaTexto();
		printf("informe a linha e coluna de origem: ");
		scanf("%d %d", &linOri,&colOri);
		printf("\ninforme a linha e coluna de destino: ");
		scanf("%d %d", &linDes,&colDes);

		int resultado = moverPeca(linOri, colOri, linDes, colDes);
			if(resultado != 1){
				switch(resultado){
					case 9:
						printf("\nERRO: Peca nao pode ser movida desse jeito");
						getch();
						break;
					case 0:
						printf("\nERRO: Coordenadas invalidas");
						getch();
						break;
					case 1:
						break;

				}
			printf("\nERRO: Coordenadas invalidas!");
			getch();
		}*/
	}
    getch();
	return 0;
}
