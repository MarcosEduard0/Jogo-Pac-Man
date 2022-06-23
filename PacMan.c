#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define true 1
#define false 0

//Definicoes, para facilitar o uso da funcao para desenhar o mapa
#define PAREDE 1
#define PORTA 2
#define PILULA 3
#define PILULA2 6
#define BARRAGEM 4
#define INICIO 5

//Desfinicoes da configuracao do Video
#define LARGURA 560 // Numero total de colunas*largura_da_parede, quantas colunas temos VEZES o tamanho em Pixel da parede
#define ALTURA  680 //Numero total de linhas*altura_da_parede, quantas linhas temos VEZES o tamanho em Pixel da parede
#define BPP       0 //A Flag SDL_ANYFORMAT se encaregara da resolucao

int carrega_imagens ();//Funcao para carregar as imagens no jogo
int carrega_musicas(); 
void desenha_mapa (); //Funcao para blitar as imagens no locais certo
void move_jogador (  ); //o nome da funcao ja fala oq faz kkkk
int iniciacao(); //Inicia e testa tudo que precisa para o jogo rodar
void movimenta_imagem(SDL_Event evento); //o nome ja diz oq faz
void Tela_Inicial();
void movimenta_fantasmaRed(  );
void movimenta_fantasmaBlue(  );
void movimenta_fantasmaOrange(  );
void movimenta_fantasmaPink(  );
void blitar_Local_Inicial( );
void Morte_PacMan( );
int teste_colisao(int );
void Liberar_Imagens(  );
void Liberar_Musicas(  );
void Ler_Nome_Jogador(  );
void carrega_mapas( );


SDL_Window* gJanela = NULL;
SDL_Surface *gTela = NULL, *gPiso = NULL, *gParede = NULL, *gPacFechado = NULL, *gPacEsquerda2 = NULL, *gUsuario = NULL;
SDL_Surface *gPacDireita = NULL, *gPacEsquerda = NULL, *gPacBaixo = NULL, *gPacCima = NULL, *gPacDireita2 = NULL;
SDL_Surface *gPacCima2 = NULL, *gPacBaixo2 = NULL, *gGameOver = NULL, *gTelaInicial = NULL, *gStartGame = NULL, *gRed1 = NULL, *gRed2 = NULL, *gPac = NULL, *gFantasmaRed, *gBlue1, *gBlue2, *gOrange1, *gOrange2, *gFantasmaBlue, *gFantasmaOrange, *gFantasmaPink;
SDL_Surface *gPac1 = NULL, *gPac2 = NULL, *gPilula = NULL, *gAbout = NULL, *gVida = NULL, *gLives = NULL, *gttf = NULL, *gPause = NULL, *gPilula2 = NULL, *gFantasmaFraco = NULL, *gTempFantRed, *gTempFantOrange, *gTempFantBlue, *gTempFantPink, *gBarragem = NULL;
SDL_Surface *gScore = NULL, *gHighScore = NULL, *gControls = NULL, *gPink1 = NULL, *gPink2 = NULL, *gMorrendo1 = NULL, *gMorrendo2 = NULL, *gMorrendo3 = NULL, *gMorrendo4 = NULL, *gMorrendo5 = NULL, *gMorrendo6 = NULL, *gMorrendo7 = NULL, *gMorrendo8 = NULL, *gMorrendo9 = NULL, *gGetReady = NULL, *gDigiteName = NULL, *gPonto200 = NULL, *gPonto400 = NULL, *gPonto800 = NULL, *gPonto1600 = NULL, *gLVL2 = NULL;
SDL_Rect gDestino, gDestFantasmaRed, gDestFantasmaBlue, gDestFantasmaOrange, gDestFantasmaPink, gDestGetReady = {215,340, 0 ,0}, gDestPause = {240,340, 0 ,0};// para blitar o jogador no local certo

TTF_Font *gFonte;
Mix_Chunk *gPastilha = NULL, *gMortePacMan = NULL, *gIntro = NULL, *gMorteFantasma  = NULL;//Ponteiros para armazenar as musicas
Mix_Music *gMusicFundo = NULL, *gMusicFundo2 = NULL;
FILE *gSomarPontos, *gPlacarFinal, *gMapa1;

SDL_Color gCor = {0, 0, 0}, gCorUserName = {255, 255, 255}; //Cor das fontes. 
SDL_Rect gDestFonte = {115, 635, 0, 0};//Cria um retângulo que irá indicar onde o texto da pontuação ficará na tela 


const int totalTimeToWait = 60 * 1; //5 sec * 60 frames/sec = 5*60 frames to wait. EM FAZE DE TESTES!!!
int mapa[31][28];
int gTelaPrincipal = true, gColuna_atual = 14, gLinha_atual = 23, gQuit = 0, gPontuacao = 0, gVidas = 3, movimentoRed = 3, gLinha_FantasmaRed = 13, gColuna_FantasmaRed = 13, movimentoBlue = 3, gLinha_FantasmaBlue = 13, gColuna_FantasmaBlue = 14, movimentoOrange = 3, gLinha_FantasmaOrange = 14, gColuna_FantasmaOrange = 13, movimentoPink = 3, gLinha_FantasmaPink = 14, gColuna_FantasmaPink = 14, gMusicControlador = true, gQuantFant = 0, gControlRed = false, gControlBlue = false, gControlPink = false, gControlOrange = false, gPontosVidas = 10000;//Variaveis de inicialização.

char gPontos[10000];//Armazenar pontos para guartar em um arquivo .txt
char gNomeUsuario[80];

typedef struct{
	char nome[80];
	int pontos;
}JOGADORES;

int main (void){
	
SDL_Event evento; // para os eventos que iremos ter

srand(time(NULL));

if( !iniciacao() ){
		printf( "Falha na iniciação!\n" );
}else{
	if (!carrega_imagens()){ // Se não carregou uma ou outra imagem irar da erro e nao abrir o jogo
		printf("Falha ao carregar as imagens!\n");
	}else{
		if(!carrega_musicas()){
			printf("Falha ao carregar as musicas!\n");
		}else{
			while (!gQuit){
				carrega_mapas( );
				gPontuacao = 0;
				gVidas = 3;
				if(gTelaPrincipal){
					Tela_Inicial(  );//MOSTRA A TELA INICIAL
					gTelaPrincipal = false;//FALA Q A TELA INICIAL ACABOU
					if(!gQuit){ //REOLHAR ESSA PARTE DO PROGRAMA
						SDL_FillRect(gTela, NULL, SDL_MapRGB(gTela->format, 255, 255, 255)); // LIMPAR OQ TINHA ANTERIORMETE NA TELA
						desenha_mapa();//DESENHANDO O MAPA
						blitar_Local_Inicial();//COLOCANDO OS PERSONAGEM NOS SEUS LUGARES
						SDL_BlitSurface(gGetReady, NULL, gTela, &gDestGetReady);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(4000);// ESPERANDO 4 SEGUNDOS ANTES DE COMEÇAR
					}
				}
				
				while(SDL_PollEvent(&evento)){
					switch(evento.type){
						case SDL_QUIT:
							gQuit = true;
							break;
						case SDL_KEYDOWN:
							move_jogador();
							break;
						default:
							break;
					}	
				}
			}
		}
	}
}
	// Finalizando o SDL
	fclose(gSomarPontos);
	Liberar_Imagens();
	Liberar_Musicas();
	SDL_DestroyWindow( gJanela );
	Mix_Quit();
	SDL_Quit();
	return 0;
}
int iniciacao(  ){

	//sinalizador de inicialização do programa
	int success = true;

	// Inicializa o SDL e o sistema de vídeo e verificando se foi com sucesso
	if( SDL_Init( SDL_INIT_VIDEO || SDL_INIT_AUDIO ) < 0 ){
		printf( "SDL não pode inicializar! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}else{	
		if(TTF_Init() < 0){ //Inicializando a TTF
			printf("Falha ao inicializar TTF!");
			success = false;
		}else{
			gFonte = TTF_OpenFont("fontes/formal.ttf", 28); //Pegando a fonte e colocando um tamanho
			if(!gFonte){ //Verificando se carregou a fonte 
				printf("Não foi carregada a fonte!");
				success = false;
			}else{
				TTF_SetFontStyle(gFonte, TTF_STYLE_BOLD); //Setando um estilo para a fonte
				
				gJanela = SDL_CreateWindow( "PAC MAN - UFRJ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, LARGURA, ALTURA, SDL_WINDOW_SHOWN); //Criar a Janela

				if(!gJanela){
					printf( "A janela não pode ser criada! SDL_Error: %s\n", SDL_GetError() );
						success = false;
				}else{
					gTela = SDL_GetWindowSurface( gJanela ); //Obetendo superficie da janela
				
					if(!gTela){
						printf("Erro ao criar superficie\n");
						success = false;
					}else{
						gSomarPontos = fopen("pontos.txt", "w+");
						if(!gSomarPontos){
							printf("Erro ao criar arquivo txt\n");
							success = false;
						}else{
							gPlacarFinal = fopen("HighScore.txt", "a+");
							if(!gPlacarFinal){
								printf("Erro ao criar arquivo txt\n");
								success = false;
							}else{
								if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
									printf("Nao foi possivel inicializar o audio: %s\n", Mix_GetError());
									success = false;
								}
							}
						}
					}
				}
			}
		}
	}

	return success;
}
void carrega_mapas( ){

	
	gMapa1 = fopen("mapas/mapa1.txt", "r");
	if(gMapa1 == NULL){
		SDL_Log("ERRO AO CARREGAR MAPA");
		gQuit = true;
	}
	int i, j;

	for(i = 0; i < 31 ; i++){
		for(j = 0; j < 28; j ++){
			fscanf(gMapa1, "%d", &mapa[i][j]);
		}
	}
	fclose(gMapa1);
}
int carrega_imagens (  ){

	int success = true;

	// Carrega as Imagens e verificando se ocorreu tudo certo.
	gParede = SDL_LoadBMP("imagens/20x20/parede.bmp");
	if (!gParede){
		printf("Erro ao carregar imagem parede.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPacFechado = SDL_LoadBMP("imagens/20x20/fechada.bmp");
	if (!gPacFechado){
		printf("Erro ao carregar imagem fachado.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPacDireita = SDL_LoadBMP("imagens/20x20/direita.bmp");
	if(!gPacDireita){
		printf("Erro ao carregar imagem direita.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPacEsquerda = SDL_LoadBMP("imagens/20x20/esquerda.bmp");
	if(!gPacEsquerda){
		printf("Erro ao carregar imagem esquerda.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPacCima = SDL_LoadBMP("imagens/20x20/cima.bmp");
	if(!gPacCima){
		printf("Erro ao carregar imagem cima.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPacBaixo = SDL_LoadBMP("imagens/20x20/baixo.bmp");
	if(!gPacBaixo){
		printf("Erro ao carregar imagem baixo.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPacDireita2 = SDL_LoadBMP("imagens/20x20/direita2.bmp");
	if(!gPacDireita2){
		printf("Erro ao carregar imagem direita2.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPacEsquerda2 = SDL_LoadBMP("imagens/20x20/esquerda2.bmp");
	if(!gPacEsquerda2){
		printf("Erro ao carregar imagem esquerda2.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPacCima2 = SDL_LoadBMP("imagens/20x20/cima2.bmp");
	if(!gPacCima2){
		printf("Erro ao carregar imagem cima2.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPacBaixo2 = SDL_LoadBMP("imagens/20x20/baixo2.bmp");
	if(!gPacBaixo2){
		printf("Erro ao carregar imagem baixo2.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gGameOver = SDL_LoadBMP("imagens/GameOver.bmp");
	if(!gGameOver){
		printf("Erro ao carregar imagem GameOver.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gTelaInicial = SDL_LoadBMP("imagens/TelaInicial.bmp");
	if(!gTelaInicial){
		printf("Erro ao carregar imagem TelaInicial.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gStartGame = SDL_LoadBMP("imagens/StartGame.bmp");
	if (!gStartGame){
		printf("Erro ao carregar imagem StartGame.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gRed1 = SDL_LoadBMP("imagens/Red1.bmp");
	if (!gRed1){
		printf("Erro ao carregar imagem gFantasmaRed1.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gRed2 = SDL_LoadBMP("imagens/Red2.bmp");
	if (!gRed2){
		printf("Erro ao carregar imagem gFantasmaRed2.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPac = SDL_LoadBMP("imagens/Pac.bmp");
	if (!gPac){
		printf("Erro ao carregar imagem Pac.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPac1 = SDL_LoadBMP("imagens/Pac1.bmp");
	if (!gPac1){
		printf("Erro ao carregar imagem Pac1.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPac2 = SDL_LoadBMP("imagens/Pac2.bmp");
	if (!gPac2){
		printf("Erro ao carregar imagem Pac2.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPilula = SDL_LoadBMP("imagens/pilula.bmp");
	if (!gPilula){
		printf("Erro ao carregar imagem pilula.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPilula2 = SDL_LoadBMP("imagens/pilula2.bmp");
	if (!gPilula2){
		printf("Erro ao carregar imagem pilula2.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gAbout = SDL_LoadBMP("imagens/about.bmp");
	if (!gAbout){
		printf("Erro ao carregar imagem about.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gVida = SDL_LoadBMP("imagens/vida.bmp");
	if (!gVida){
		printf("Erro ao carregar imagem vida.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gLives = SDL_LoadBMP("imagens/lives.bmp");
	if (!gLives){
		printf("Erro ao carregar imagem lives.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gFantasmaRed = SDL_LoadBMP("imagens/20x20/FantasmaRed.bmp");
	if (!gFantasmaRed){
		printf("Erro ao carregar imagem FantasmaRed.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gScore = SDL_LoadBMP("imagens/score.bmp");
	if (!gScore){
		printf("Erro ao carregar imagem score.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gControls = SDL_LoadBMP("imagens/controls.bmp");
	if (!gControls){
		printf("Erro ao carregar imagem controls.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gHighScore = SDL_LoadBMP("imagens/HighScore.bmp");
	if (!gHighScore){
		printf("Erro ao carregar imagem HighScore.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPink1 = SDL_LoadBMP("imagens/pink1.bmp");
	if (!gPink1){
		printf("Erro ao carregar imagem pink1.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPink2 = SDL_LoadBMP("imagens/pink2.bmp");
	if (!gPink2){
		printf("Erro ao carregar imagem pink2.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gBlue1 = SDL_LoadBMP("imagens/blue1.bmp");
	if (!gBlue1){
		printf("Erro ao carregar imagem blue1.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gBlue2 = SDL_LoadBMP("imagens/blue2.bmp");
	if (!gBlue2){
		printf("Erro ao carregar imagem blue2.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gOrange1 = SDL_LoadBMP("imagens/orange1.bmp");
	if (!gOrange1){
		printf("Erro ao carregar imagem orange1.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gOrange2 = SDL_LoadBMP("imagens/orange2.bmp");
	if (!gOrange2){
		printf("Erro ao carregar imagem orange2.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gFantasmaBlue = SDL_LoadBMP("imagens/20x20/FantasmaBlue.bmp");
	if (!gFantasmaBlue){
		printf("Erro ao carregar imagem FantasmaBlue.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gFantasmaOrange = SDL_LoadBMP("imagens/20x20/FantasmaOrange.bmp");
	if (!gFantasmaOrange){
		printf("Erro ao carregar imagem FantasmaOrange.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gFantasmaPink = SDL_LoadBMP("imagens/20x20/FantasmaPink.bmp");
	if (!gFantasmaPink){
		printf("Erro ao carregar imagem FantasmaPink.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gMorrendo1 = SDL_LoadBMP("imagens/morrendo/1.bmp");
	if (!gMorrendo1){
		printf("Erro ao carregar imagem 1.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gMorrendo2 = SDL_LoadBMP("imagens/morrendo/2.bmp");
	if (!gMorrendo2){
		printf("Erro ao carregar imagem 2.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gMorrendo3 = SDL_LoadBMP("imagens/morrendo/3.bmp");
	if (!gMorrendo3){
		printf("Erro ao carregar imagem 3.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gMorrendo4 = SDL_LoadBMP("imagens/morrendo/4.bmp");
	if (!gMorrendo4){
		printf("Erro ao carregar imagem 4.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gMorrendo5 = SDL_LoadBMP("imagens/morrendo/5.bmp");
	if (!gMorrendo5){
		printf("Erro ao carregar imagem 5.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gMorrendo6 = SDL_LoadBMP("imagens/morrendo/6.bmp");
	if (!gMorrendo6){
		printf("Erro ao carregar imagem 6.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gMorrendo7 = SDL_LoadBMP("imagens/morrendo/7.bmp");
	if (!gMorrendo7){
		printf("Erro ao carregar imagem 7.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gMorrendo8 = SDL_LoadBMP("imagens/morrendo/8.bmp");
	if (!gMorrendo8){
		printf("Erro ao carregar imagem 8.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gMorrendo9 = SDL_LoadBMP("imagens/morrendo/9.bmp");
	if (!gMorrendo9){
		printf("Erro ao carregar imagem 9.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gGetReady = SDL_LoadBMP("imagens/GetReady.bmp");
	if (!gMorrendo9){
		printf("Erro ao carregar imagem 9.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gDigiteName = SDL_LoadBMP("imagens/UserName.bmp");
	if (!gDigiteName){
		printf("Erro ao carregar imagem UserName.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPause = SDL_LoadBMP("imagens/pause.bmp");
	if (!gPause){
		printf("Erro ao carregar imagem pause.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gFantasmaFraco = SDL_LoadBMP("imagens/20x20/FantasmaFraco.bmp");
	if (!gFantasmaFraco){
		printf("Erro ao carregar imagem FantasmaFraco.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gBarragem = SDL_LoadBMP("imagens/20x20/barragem.bmp");
	if (!gFantasmaFraco){
		printf("Erro ao carregar imagem barragem.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPonto200 = SDL_LoadBMP("imagens/20x20/200.bmp");
	if (!gFantasmaFraco){
		printf("Erro ao carregar imagem 200.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPonto400 = SDL_LoadBMP("imagens/20x20/400.bmp");
	if (!gFantasmaFraco){
		printf("Erro ao carregar imagem 400.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPonto800 = SDL_LoadBMP("imagens/20x20/800.bmp");
	if (!gFantasmaFraco){
		printf("Erro ao carregar imagem 800.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gPonto1600 = SDL_LoadBMP("imagens/20x20/1600.bmp");
	if (!gFantasmaFraco){
		printf("Erro ao carregar imagem 1600.bmp, verifique se está no diretório correto\n");
		success = false;
	}
	gLVL2 = SDL_LoadBMP("imagens/lvl2.bmp");
	if (!gLVL2){
		printf("Erro ao carregar imagem lvl2.bmp, verifique se está no diretório correto\n");
		success = false;
	}

	gTempFantRed = gFantasmaRed;
	gTempFantBlue = gFantasmaBlue;
	gTempFantPink = gFantasmaPink;
	gTempFantOrange = gFantasmaOrange;
	
	return success;
}
int carrega_musicas(  ){

	int success = true;

	gPastilha = Mix_LoadWAV("music/PelletEat1.wav");
	if(!gPastilha){
		puts("Erro ao carregar o som gPastilha");
		success = false;
	}
	gMortePacMan = Mix_LoadWAV("music/PacEaten.wav");
	if(!gMortePacMan){
		puts("Erro ao carregar o som gMortePacMan");
		success = false;
	}
	gIntro = Mix_LoadWAV("music/intro.wav");
	if(!gIntro){
		puts("Erro ao carregar o som gIntro");
		success = false;
	}
	gMusicFundo = Mix_LoadMUS("music/Siren.wav");
	if(!gMusicFundo){
		puts("Erro ao carregar o som gMusicFundo");
		success = false;
	}
	gMorteFantasma = Mix_LoadWAV("music/GhostEaten.wav");
	if(!gIntro){
		puts("Erro ao carregar o som GhostEaten");
		success = false;
	}
	gMusicFundo2 = Mix_LoadMUS("music/Invincible.wav");
	if(!gMusicFundo2){
		puts("Erro ao carregar o som Invincible");
		success = false;
	}
	
	
	return success;
}
void desenha_mapa (  ){

SDL_Rect destino, destVida = {450, 630, 0, 0}; //Posição das vidas;
SDL_Rect destLives = {370, 630, 0, 0};//Posicao da imagem Lives
SDL_Rect destScore = {20, 636, 0, 0};//Posição da imagem Score
int linha, coluna;

	for (linha = 0; linha < 31; linha++){
		destino.y = linha * 20;
		for (coluna = 0; coluna < 28; coluna++){
			destino.x = coluna * 20;

			if (mapa[linha][coluna] == PAREDE){
			// pegue a imagem parede e jogue na tela em destino [linha][coluna]
			SDL_BlitSurface(gParede, NULL, gTela, &destino);
			}
			if (mapa[linha][coluna] == PILULA){
			// pegue a imagem fantasma e jogue na tela em destino [linha][coluna]
			SDL_BlitSurface(gPilula, NULL, gTela, &destino);
			}
			if (mapa[linha][coluna] == PILULA2){
			// pegue a imagem fantasma e jogue na tela em destino [linha][coluna]
			SDL_BlitSurface(gPilula2, NULL, gTela, &destino);
			}
			if (mapa[linha][coluna] == BARRAGEM){
			// pegue a imagem fantasma e jogue na tela em destino [linha][coluna]
			SDL_BlitSurface(gBarragem, NULL, gTela, &destino);
			}
		}
	}
	SDL_BlitSurface(gLives, NULL, gTela, &destLives);// DESTINO DA IMAGEM LIVES
	SDL_BlitSurface(gScore, NULL, gTela, &destScore);// DESTINO DA IMAGEM SCORE
	SDL_BlitSurface(gttf, NULL, gTela, &gDestFonte); //DESTINO DA PONTUAÇÃO
	for(int i = 1; i < gVidas; i++){
		SDL_BlitSurface(gVida, NULL, gTela, &destVida);
		destVida.x+=40;
	}
	
}
void move_jogador (  ){

Mix_PlayMusic(gMusicFundo, -1);//COMEÇANDO A TOCAR A MUSICA DE FUNDO
SDL_Rect Level = {230,340, 0 ,0};
int running = true, pause = true, musica = true, temporizador = false; //CONTROLE DE VERDADE
int QuantPilula = 0, TempoFantasma = true;
int remainingTime = totalTimeToWait;//CONTROLADOR DE MORTES DOS FANTASMAS;
SDL_Event EventoPac, TempTecla;

///QUERO COLOCAR O PACMAN PARA INICAR ANDANDO PARA ESQUERDA!!

while(running){

	SDL_PollEvent(&EventoPac);
	
	if(temporizador){
		remainingTime--;
		
		if(remainingTime == 0){
			remainingTime = totalTimeToWait;
			gFantasmaRed = gTempFantRed;
			gFantasmaBlue = gTempFantBlue;
			gFantasmaPink = gTempFantPink;
			gFantasmaOrange = gTempFantOrange;
			gQuantFant = 0;
			Mix_PauseMusic();
			Mix_PlayMusic(gMusicFundo, -1);
			temporizador = false;
			gControlRed = false;
			gControlBlue = false;
			gControlOrange = false;
			gControlPink = false;
		}
	}
	if(remainingTime >= 0 && remainingTime <= 15){//FANZENDO O FANTASMA PISCAR
		if(TempoFantasma){
			gFantasmaRed = gTempFantRed;
			gFantasmaBlue = gTempFantBlue;
			gFantasmaPink = gTempFantPink;
			gFantasmaOrange = gTempFantOrange;
			TempoFantasma = false;
		}
		else{
			gFantasmaRed = gFantasmaFraco;
			gFantasmaOrange = gFantasmaFraco;
			gFantasmaBlue = gFantasmaFraco;
			gFantasmaPink = gFantasmaFraco;
			TempoFantasma = true;
		}
		
	}
	if(EventoPac.type == SDL_QUIT || EventoPac.key.keysym.sym == SDLK_q ){
		gQuit = true;
		break;
	}
	if(EventoPac.key.keysym.sym == SDLK_p){ //PAUSANDO O JOGO
		pause = true;
		EventoPac.key.keysym.sym = SDLK_a;
		SDL_BlitSurface(gPause, NULL, gTela, &gDestPause);
		SDL_UpdateWindowSurface(gJanela);
		while(pause){
			SDL_PollEvent(&EventoPac);
			if(EventoPac.type == SDL_KEYUP){
				if(EventoPac.key.keysym.sym == SDLK_p){
					pause = false;
					EventoPac.key.keysym.sym = TempTecla.key.keysym.sym;
				}
				if(EventoPac.type == SDL_QUIT || EventoPac.key.keysym.sym == SDLK_q ){//CORRIGIR, POIS NAO QR SAR QUANDA TA NO PAUSE
					gQuit = true;
					running = false;
					break;
				}
			}
		}
	}
	
	if(EventoPac.key.keysym.sym == SDLK_m){ //CONTROLADOR DE SOM
		if(gMusicControlador){
			Mix_PauseMusic();
			musica = false;
			gMusicControlador = false;
		}else{
			musica = true;
			gMusicControlador = true;
			Mix_PlayChannel(-1, gPastilha, 0);
			Mix_ResumeMusic();
		}
	}

	switch (EventoPac.key.keysym.sym){
		
		case SDLK_UP:
				TempTecla.key.keysym.sym = SDLK_UP;
				gLinha_atual--;
				if(mapa[gLinha_atual][gColuna_atual] == PAREDE) gLinha_atual++;
				if(mapa[gLinha_atual][gColuna_atual] == PILULA || mapa[gLinha_atual][gColuna_atual] == PILULA2){
					if(mapa[gLinha_atual][gColuna_atual] == PILULA)gPontuacao+=10, QuantPilula++;
					if(mapa[gLinha_atual][gColuna_atual] == PILULA2){
						Mix_PauseMusic();
						Mix_PlayMusic(gMusicFundo2, -1);
						gPontuacao+=50;
						temporizador = true;
						gControlRed = true;
						gControlBlue = true;
						gControlOrange = true;
						gControlPink = true;
						gFantasmaOrange = gFantasmaFraco;
						gFantasmaBlue = gFantasmaFraco;
						gFantasmaPink = gFantasmaFraco;
						gFantasmaRed = gFantasmaFraco;
						QuantPilula++;
					}
					if(QuantPilula == 287){//RESETANDO AS PASTILHAS DO MAPA E PASSANDO DE LEVEL
						carrega_mapas();
						SDL_FillRect(gTela, NULL, SDL_MapRGB(gTela->format, 255, 255, 255));//LIMPANDO A TELA
						desenha_mapa();
						SDL_BlitSurface(gLVL2,NULL, gTela, &Level);
						gColuna_atual = 14, gLinha_atual = 23, movimentoRed = 3, gLinha_FantasmaRed = 13, gColuna_FantasmaRed = 13, movimentoBlue = 3, gLinha_FantasmaBlue = 13, gColuna_FantasmaBlue = 14, movimentoOrange = 3, gLinha_FantasmaOrange = 14, gColuna_FantasmaOrange = 13, movimentoPink = 3, gLinha_FantasmaPink = 14, gColuna_FantasmaPink = 14;
						blitar_Local_Inicial();
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(3000);
					}
					mapa[gLinha_atual][gColuna_atual] = 0;
					rewind(gSomarPontos);
					fprintf(gSomarPontos, "%d", gPontuacao);
					rewind(gSomarPontos);
					fgets(gPontos, 10000, gSomarPontos);
					gttf = TTF_RenderText_Blended(gFonte, gPontos, gCor); //COR, FONTE E TEXTO DA PONTUAÇÃO
					if(musica)Mix_PlayChannel(-1, gPastilha, 0);
					else Mix_Pause(-1);
				}
				running = teste_colisao(temporizador);//TESTANDO COLISAÇÃO, SE SIM, SAIA DO LOOP;
				if(!running)break;
				else{
					gDestino.y = gLinha_atual *20;
					SDL_FillRect(gTela, NULL, SDL_MapRGB(gTela->format, 255, 255, 255));
					desenha_mapa();
					movimenta_fantasmaRed();
					movimenta_fantasmaBlue();
					movimenta_fantasmaOrange();
					movimenta_fantasmaPink();
					movimenta_imagem(EventoPac);
					SDL_UpdateWindowSurface( gJanela );
					SDL_PollEvent(&EventoPac);
			//------------------------------------------------		COLOCAR EM UMA FUNÇÃO
					if(EventoPac.key.keysym.sym == SDLK_LEFT){
						gColuna_atual--;
						if (mapa[gLinha_atual][gColuna_atual] == PAREDE){
							gColuna_atual++;
							EventoPac.key.keysym.sym = SDLK_UP;
						 }
						else gColuna_atual++;
					}
					if(EventoPac.key.keysym.sym == SDLK_RIGHT){
						gColuna_atual++;
						if (mapa[gLinha_atual][gColuna_atual] == PAREDE){
							gColuna_atual--;
							EventoPac.key.keysym.sym = SDLK_UP;
						 }
						else gColuna_atual--;
					}
			//------------------------------------------------
				}
				break;

		case SDLK_DOWN:
				TempTecla.key.keysym.sym = SDLK_DOWN;
				if(gLinha_atual < 31) gLinha_atual++;
				if (mapa[gLinha_atual][gColuna_atual] == PAREDE) gLinha_atual--;
				if (mapa[gLinha_atual][gColuna_atual] == BARRAGEM) gLinha_atual--;
				if(mapa[gLinha_atual][gColuna_atual] == PILULA || mapa[gLinha_atual][gColuna_atual] == PILULA2){
					if(mapa[gLinha_atual][gColuna_atual] == PILULA)gPontuacao+=10, QuantPilula++;
					if(mapa[gLinha_atual][gColuna_atual] == PILULA2){
						Mix_PauseMusic();
						Mix_PlayMusic(gMusicFundo2, -1);
						gPontuacao+=50;
						temporizador = true;
						gControlRed = true;
						gControlBlue = true;
						gControlOrange = true;
						gControlPink = true;
						gFantasmaOrange = gFantasmaFraco;
						gFantasmaBlue = gFantasmaFraco;
						gFantasmaPink = gFantasmaFraco;
						gFantasmaRed = gFantasmaFraco;
						QuantPilula++;
					}
					if(QuantPilula == 287){//RESETANDO AS PASTILHAS DO MAPA E PASSANDO DE LEVEL
						carrega_mapas();
						SDL_FillRect(gTela, NULL, SDL_MapRGB(gTela->format, 255, 255, 255));//LIMPANDO A TELA
						desenha_mapa();
						SDL_BlitSurface(gLVL2,NULL, gTela, &Level);
						gColuna_atual = 14, gLinha_atual = 23, movimentoRed = 3, gLinha_FantasmaRed = 13, gColuna_FantasmaRed = 13, movimentoBlue = 3, gLinha_FantasmaBlue = 13, gColuna_FantasmaBlue = 14, movimentoOrange = 3, gLinha_FantasmaOrange = 14, gColuna_FantasmaOrange = 13, movimentoPink = 3, gLinha_FantasmaPink = 14, gColuna_FantasmaPink = 14;
						blitar_Local_Inicial();
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(3000);
					}
					mapa[gLinha_atual][gColuna_atual] = 0;
					rewind(gSomarPontos);
					fprintf(gSomarPontos, "%d", gPontuacao);
					rewind(gSomarPontos);
					fgets(gPontos, 10000, gSomarPontos);
					gttf = TTF_RenderText_Blended(gFonte, gPontos, gCor); //PONTUAÇÃO
					if(musica)Mix_PlayChannel(-1, gPastilha, 0);
					else Mix_Pause(-1);
				}
				running = teste_colisao(temporizador);
				if(!running)break;
				else{
					gDestino.y = gLinha_atual *20;
					SDL_FillRect(gTela, NULL, SDL_MapRGB(gTela->format, 255, 255, 255));
					desenha_mapa();
					movimenta_fantasmaRed();
					movimenta_fantasmaBlue();
					movimenta_fantasmaOrange();
					movimenta_fantasmaPink();
					movimenta_imagem(EventoPac);
					SDL_UpdateWindowSurface( gJanela );
					SDL_PollEvent(&EventoPac);
			//------------------------------------------------		
					if(EventoPac.key.keysym.sym == SDLK_LEFT){
						gColuna_atual--;
						if (mapa[gLinha_atual][gColuna_atual] == PAREDE){
							gColuna_atual++;
							EventoPac.key.keysym.sym = SDLK_DOWN;
						 }
						else gColuna_atual++;
					}
					if(EventoPac.key.keysym.sym == SDLK_RIGHT){
						gColuna_atual++;
						if (mapa[gLinha_atual][gColuna_atual] == PAREDE){
							gColuna_atual--;
							EventoPac.key.keysym.sym = SDLK_DOWN;
						 }
						else gColuna_atual--;
					}
			//------------------------------------------------
				}
			break;

		case SDLK_LEFT:
				TempTecla.key.keysym.sym = SDLK_LEFT;
				gColuna_atual--;
				if (mapa[gLinha_atual][gColuna_atual] == PAREDE) gColuna_atual++;
				if (mapa[gLinha_atual][gColuna_atual] == PORTA) gColuna_atual = 27; //transporta o pacman
				if(mapa[gLinha_atual][gColuna_atual] == PILULA || mapa[gLinha_atual][gColuna_atual] == PILULA2){
					if(mapa[gLinha_atual][gColuna_atual] == PILULA)gPontuacao+=10, QuantPilula++;
					if(mapa[gLinha_atual][gColuna_atual] == PILULA2){
						Mix_PauseMusic();
						Mix_PlayMusic(gMusicFundo2, -1);
						gPontuacao+=50;
						temporizador = true;
						gControlRed = true;
						gControlBlue = true;
						gControlOrange = true;
						gControlPink = true;
						gFantasmaOrange = gFantasmaFraco;
						gFantasmaBlue = gFantasmaFraco;
						gFantasmaPink = gFantasmaFraco;
						gFantasmaRed = gFantasmaFraco;
						QuantPilula++;
					}
					if(QuantPilula == 287){//RESETANDO AS PASTILHAS DO MAPA E PASSANDO DE LEVEL
						carrega_mapas();
						SDL_FillRect(gTela, NULL, SDL_MapRGB(gTela->format, 255, 255, 255));//LIMPANDO A TELA
						desenha_mapa();
						SDL_BlitSurface(gLVL2,NULL, gTela, &Level);
						gColuna_atual = 14, gLinha_atual = 23, movimentoRed = 3, gLinha_FantasmaRed = 13, gColuna_FantasmaRed = 13, movimentoBlue = 3, gLinha_FantasmaBlue = 13, gColuna_FantasmaBlue = 14, movimentoOrange = 3, gLinha_FantasmaOrange = 14, gColuna_FantasmaOrange = 13, movimentoPink = 3, gLinha_FantasmaPink = 14, gColuna_FantasmaPink = 14;
						blitar_Local_Inicial();
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(3000);
					}
					mapa[gLinha_atual][gColuna_atual] = 0;
					rewind(gSomarPontos);
					fprintf(gSomarPontos, "%d", gPontuacao);
					rewind(gSomarPontos);
					fgets(gPontos, 10000, gSomarPontos);
					gttf = TTF_RenderText_Blended(gFonte, gPontos, gCor); //PONTUAÇÃO
					if(musica)Mix_PlayChannel(-1, gPastilha, 0);
					else Mix_Pause(-1);
				}

				running = teste_colisao(temporizador);
				if(!running)break;
				else{
					gDestino.x = gColuna_atual *20;
					SDL_FillRect(gTela, NULL, SDL_MapRGB(gTela->format, 255, 255, 255));
					desenha_mapa();
					movimenta_fantasmaRed();
					movimenta_fantasmaBlue();
					movimenta_fantasmaOrange();
					movimenta_fantasmaPink();
					movimenta_imagem(EventoPac);
					SDL_UpdateWindowSurface( gJanela );
					SDL_PollEvent(&EventoPac);
			//------------------------------------------------		
					if(EventoPac.key.keysym.sym == SDLK_UP){
						gLinha_atual--;
						if(mapa[gLinha_atual][gColuna_atual] == PAREDE){
							gLinha_atual++;
							EventoPac.key.keysym.sym = SDLK_LEFT;
						 }
						else gLinha_atual++;
					}
					if(EventoPac.key.keysym.sym == SDLK_DOWN){
						gLinha_atual++;
						if(mapa[gLinha_atual][gColuna_atual] == PAREDE){
							gLinha_atual--;
							EventoPac.key.keysym.sym = SDLK_LEFT;
						 }
						else gLinha_atual--;
					}
			//------------------------------------------------
				}
			break;

			case SDLK_RIGHT:
			
					TempTecla.key.keysym.sym = SDLK_RIGHT;
					gColuna_atual++;
					if (mapa[gLinha_atual][gColuna_atual] == PAREDE) gColuna_atual--;
					if (mapa[gLinha_atual][gColuna_atual] == PORTA) gColuna_atual = 0; //transporta o pacman
					if(mapa[gLinha_atual][gColuna_atual] == PILULA || mapa[gLinha_atual][gColuna_atual] == PILULA2){
						if(mapa[gLinha_atual][gColuna_atual] == PILULA)gPontuacao+=10, QuantPilula++;
						if(mapa[gLinha_atual][gColuna_atual] == PILULA2){
							Mix_PauseMusic();
							Mix_PlayMusic(gMusicFundo2, -1);
							gPontuacao+=50;
							temporizador = true;
							gControlRed = true;
							gControlBlue = true;
							gControlOrange = true;
							gControlPink = true;
							gFantasmaOrange = gFantasmaFraco;
							gFantasmaBlue = gFantasmaFraco;
							gFantasmaPink = gFantasmaFraco;
							gFantasmaRed = gFantasmaFraco;
							QuantPilula++;
						}
						if(QuantPilula == 287){//RESETANDO AS PASTILHAS DO MAPA E PASSANDO DE LEVEL
							carrega_mapas();
							SDL_FillRect(gTela, NULL, SDL_MapRGB(gTela->format, 255, 255, 255));//LIMPANDO A TELA
							desenha_mapa();
							SDL_BlitSurface(gLVL2,NULL, gTela, &Level);
							gColuna_atual = 14, gLinha_atual = 23, movimentoRed = 3, gLinha_FantasmaRed = 13, gColuna_FantasmaRed = 13, movimentoBlue = 3, gLinha_FantasmaBlue = 13, gColuna_FantasmaBlue = 14, movimentoOrange = 3, gLinha_FantasmaOrange = 14, gColuna_FantasmaOrange = 13, movimentoPink = 3, gLinha_FantasmaPink = 14, gColuna_FantasmaPink = 14;
							blitar_Local_Inicial();
							SDL_UpdateWindowSurface(gJanela);
							SDL_Delay(3000);
						}
						mapa[gLinha_atual][gColuna_atual] = 0;
						rewind(gSomarPontos);
						fprintf(gSomarPontos, "%d", gPontuacao);
						rewind(gSomarPontos);
						fgets(gPontos, 10000, gSomarPontos);
						gttf = TTF_RenderText_Blended(gFonte, gPontos, gCor); //PONTUAÇÃO
						if(musica)Mix_PlayChannel(-1, gPastilha, 0);
						else Mix_Pause(-1);
					}

					running = teste_colisao(temporizador);
					if(!running)break;
					else{
						gDestino.x = gColuna_atual *20;
						SDL_FillRect(gTela, NULL, SDL_MapRGB(gTela->format, 255, 255, 255));
						desenha_mapa();
						movimenta_fantasmaRed();
						movimenta_fantasmaBlue();
						movimenta_fantasmaOrange();
						movimenta_fantasmaPink();
						movimenta_imagem(EventoPac);
						SDL_UpdateWindowSurface( gJanela );
						SDL_PollEvent(&EventoPac);
				//------------------------------------------------		VERIFICAÇÃO DE PAREDE
						if(EventoPac.key.keysym.sym == SDLK_UP){
							gLinha_atual--;
							if(mapa[gLinha_atual][gColuna_atual] == PAREDE){
								gLinha_atual++;
								EventoPac.key.keysym.sym = SDLK_RIGHT;
							 }
							else gLinha_atual++;
						}
						if(EventoPac.key.keysym.sym == SDLK_DOWN){
							gLinha_atual++;
							if(mapa[gLinha_atual][gColuna_atual] == PAREDE){
								gLinha_atual--;
								EventoPac.key.keysym.sym = SDLK_RIGHT;
							 }
							else gLinha_atual--;
						}
				//------------------------------------------------	
					}
				break;
				
			default:
				EventoPac.key.keysym.sym = TempTecla.key.keysym.sym;
				break;
		}
		
		if(gPontuacao >= gPontosVidas){ //GANHANDO MAIS UMA VIDA
			gVidas++;
			gPontosVidas+=10000;
		}
		
	}
}
void movimenta_imagem(SDL_Event evento){

	switch (evento.key.keysym.sym){

		case SDLK_RIGHT:
			SDL_BlitSurface(gPacDireita, NULL, gTela, &gDestino);
			SDL_UpdateWindowSurface( gJanela );
			SDL_Delay(50);
			SDL_BlitSurface(gPacDireita2, NULL, gTela, &gDestino);
			SDL_UpdateWindowSurface( gJanela );
			SDL_Delay(50);
			SDL_BlitSurface(gPacFechado, NULL, gTela, &gDestino);
			SDL_UpdateWindowSurface( gJanela );
			SDL_Delay(50);
			break;

		case SDLK_LEFT:
			SDL_BlitSurface(gPacEsquerda, NULL, gTela, &gDestino);
			SDL_UpdateWindowSurface( gJanela );
			SDL_Delay(50);
			SDL_BlitSurface(gPacEsquerda2, NULL, gTela, &gDestino);
			SDL_UpdateWindowSurface( gJanela );
			SDL_Delay(50);
			SDL_BlitSurface(gPacFechado, NULL, gTela, &gDestino);
			SDL_UpdateWindowSurface( gJanela );
			SDL_Delay(50);
			break;

		case SDLK_UP:
			SDL_BlitSurface(gPacCima, NULL, gTela, &gDestino);
			SDL_UpdateWindowSurface( gJanela );
			SDL_Delay(50);
			SDL_BlitSurface(gPacCima2, NULL, gTela, &gDestino);
			SDL_UpdateWindowSurface( gJanela );
			SDL_Delay(50);
			SDL_BlitSurface(gPacFechado, NULL, gTela, &gDestino);
			SDL_UpdateWindowSurface( gJanela );
			SDL_Delay(50);
			break;

		case SDLK_DOWN:
			SDL_BlitSurface(gPacBaixo, NULL, gTela, &gDestino);
			SDL_UpdateWindowSurface( gJanela );
			SDL_Delay(50);
			SDL_BlitSurface(gPacBaixo2, NULL, gTela, &gDestino);
			SDL_UpdateWindowSurface( gJanela );
			SDL_Delay(50);
			SDL_BlitSurface(gPacFechado, NULL, gTela, &gDestino);
			SDL_UpdateWindowSurface( gJanela );
			SDL_Delay(50);
			break;

		default:
			break;
	}

}
void Tela_Inicial(  ){

	SDL_Rect destStart = {185, 397, 197, 39}, destAbout = {240, 480, 80, 20}, destGoBack = {230, 560, 100, 30}; //X,Y,L,H
	SDL_Rect destControls = {220, 440, 120, 25}, destHighScore = {210, 515, 140, 20};
	SDL_Rect Pac = {160, 280, 0, 0};//POSIÇÃO DO PACMAN NA TELA
	SDL_Rect Red = {120, 280, 0, 0};//POSIÇÃO DO FANTASMA RED NA TELA
	SDL_Rect Pink = {80, 280, 0, 0};//POSIÇÃO DO FANTASMA PINK NA TELA
	SDL_Rect Blue = {40, 280, 0, 0};//POSIÇÃO DO FANTASMA BLUE NA TELA
	SDL_Rect Orange = {0, 280, 0, 0};//POSIÇÃO DO FANTASMA ORANGE NA TELA
	
	SDL_Event EventoTela; // para os eventos que iremos ter
	int running = true, quit;

	while(running){
		SDL_BlitSurface(gTelaInicial, NULL, gTela, NULL);
		quit = true;
		while(SDL_PollEvent(&EventoTela)){
			switch(EventoTela.type){
				case SDL_QUIT: //Caso o usuário peça para fechar o jogo, TENHO Q MANDAR SAIR E LIBERAR IMAGENS ARQUIVOS ETC
					gQuit = true;
					running = false;
					break;
				case SDL_MOUSEBUTTONDOWN: //Em caso de clique
				{
					SDL_Point mousePos = {EventoTela.button.x, EventoTela.button.y}; //Criamos um ponto com a posição do mouse
					if(EventoTela.button.button == SDL_BUTTON_LEFT && SDL_EnclosePoints(&mousePos, 1, &destStart, NULL) ){
						Mix_PlayChannel(-1, gIntro, 0);
						running = false;
					}
					if(EventoTela.button.button == SDL_BUTTON_LEFT && SDL_EnclosePoints(&mousePos, 1, &destAbout, NULL) ){//TELA ABOUT
						
						while(quit){
							while(SDL_PollEvent(&EventoTela)){							
								SDL_BlitSurface(gAbout, NULL, gTela, NULL);
								SDL_UpdateWindowSurface( gJanela );
								SDL_Point mousePos = {EventoTela.button.x, EventoTela.button.y};
								if(EventoTela.button.button == SDL_BUTTON_LEFT && SDL_EnclosePoints(&mousePos, 1, &destGoBack, NULL) ){
									quit = false;
								}
								if(EventoTela.type == SDL_QUIT)exit(1);//CORRIGIR SAIDA
							}
						}
					}
					if(EventoTela.button.button == SDL_BUTTON_LEFT && SDL_EnclosePoints(&mousePos, 1, &destControls, NULL) ){//TELA CONTROLS
						while(quit){
							while(SDL_PollEvent(&EventoTela)){
								SDL_BlitSurface(gControls, NULL, gTela, NULL);
								SDL_UpdateWindowSurface( gJanela );
								SDL_Point mousePos = {EventoTela.button.x, EventoTela.button.y};
								if(EventoTela.button.button == SDL_BUTTON_LEFT && SDL_EnclosePoints(&mousePos, 1, &destGoBack, NULL) ){
									quit = false;
								}
								if(EventoTela.type == SDL_QUIT)exit(1);//CORRIGIR SAIDA
							}
						}
					}
					if(EventoTela.button.button == SDL_BUTTON_LEFT && SDL_EnclosePoints(&mousePos, 1, &destHighScore, NULL) ){//TELA HIGHSCORE
						JOGADORES player[10];
						int i = 0, temp, j, k, m = 0;
						char tempChar[80], score[20];
						
						SDL_Rect destNome = {100, 147, 0, 0}, destPonto = {405, 147, 0, 0};
						SDL_Surface *ScoreNome, *ScorePonto;

						SDL_BlitSurface(gHighScore, NULL, gTela, NULL);
						rewind(gPlacarFinal);

						while(quit){
							while(SDL_PollEvent(&EventoTela)){
								while(!feof(gPlacarFinal)){ 	//LENDO NOME E PONTOS
									fscanf(gPlacarFinal, "%s", player[i].nome);
									fscanf(gPlacarFinal, "%d", &player[i].pontos);
									i++;
								}
								for(j = 0; j < i-1; j++){ 	//ORDENANDO OS PONTOS E NOMES
									for(k = j+1; k < i-1; k++){
										if(player[j].pontos < player[k].pontos){
											temp = player[k].pontos;
											strcpy(tempChar, player[k].nome);
											player[k].pontos = player[j].pontos;
											strcpy(player[k].nome, player[j].nome);
											player[j].pontos = temp;
											strcpy(player[j].nome, tempChar);
										}
									}
								}

								while(m < i-1){ //PRINTANDO NA TELA
									sprintf(score,"%d", player[m].pontos);//CONVERTENDO INT PARA STRING
									ScoreNome = TTF_RenderText_Blended(gFonte, player[m].nome, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
									ScorePonto = TTF_RenderText_Blended(gFonte, score, gCorUserName);//CORRIGIR PONTOS
									SDL_BlitSurface(ScoreNome, NULL, gTela, &destNome); 
									SDL_BlitSurface(ScorePonto, NULL, gTela, &destPonto);
									destNome.y+=32;
									destPonto.y+=32;
									SDL_UpdateWindowSurface( gJanela );
									m++;
								}
								SDL_Point mousePos = {EventoTela.button.x, EventoTela.button.y};
								if(EventoTela.button.button == SDL_BUTTON_LEFT && SDL_EnclosePoints(&mousePos, 1, &destGoBack, NULL) ){
									quit = false;
								}
								if(EventoTela.type == SDL_QUIT)exit(1); //CORRIGIR SAIDA
							}
						}
					}
					break;
				}
				case SDL_MOUSEMOTION:
				{
					SDL_Point mousePos = {EventoTela.button.x, EventoTela.button.y}; //PEGA A POSIÇÃO DE ONDE FOI O CLIK DO MOUSE
					if(SDL_EnclosePoints(&mousePos, 1, &destStart, NULL) ) // ponto criado estiver dentro do StarGame
					{
						SDL_BlitSurface(gStartGame, NULL, gTela, &destStart);
						SDL_UpdateWindowSurface( gJanela );
					}
				}
					break;
			}
		}
			//ANIMAÇÃO NA TELA PRINCIPAL COM ALGUNS BUGS rs
			SDL_BlitSurface(gRed1, NULL, gTela, &Red);
			SDL_BlitSurface(gPink1, NULL, gTela, &Pink);
			SDL_BlitSurface(gBlue1, NULL, gTela, &Blue);
			SDL_BlitSurface(gOrange1, NULL, gTela, &Orange);
			SDL_BlitSurface(gPac, NULL, gTela, &Pac);
			SDL_UpdateWindowSurface( gJanela );
			Orange.x++;
			Blue.x++;
			Pink.x++;
			Red.x++;
			Pac.x++;
			SDL_Delay(10);
			SDL_BlitSurface(gRed2, NULL, gTela, &Red);
			SDL_BlitSurface(gPink2, NULL, gTela, &Pink);
			SDL_BlitSurface(gBlue2, NULL, gTela, &Blue);
			SDL_BlitSurface(gOrange2, NULL, gTela, &Orange);
			SDL_BlitSurface(gPac2, NULL, gTela, &Pac);
			SDL_UpdateWindowSurface( gJanela );
			Orange.x++;
			Blue.x++;
			Pink.x++;
			Red.x++;
			Pac.x++;
			SDL_Delay(10);
			if(Red.x >= 560)Red.x = 0;
			if(Pink.x >= 560)Pink.x = 0;
			if(Blue.x >= 560)Blue.x = 0;
			if(Orange.x >= 560)Orange.x = 0;
			if(Pac.x >= 560)Pac.x = 0;
	}
}
void movimenta_fantasmaRed(  ){

	switch(movimentoRed){

		case 0:
			gColuna_FantasmaRed++;
			
			if(mapa[gLinha_FantasmaRed][gColuna_FantasmaRed] == PAREDE){
				gColuna_FantasmaRed--;
				movimentoRed = ((rand() % 2)+2);
			}
			gDestFantasmaRed.x = gColuna_FantasmaRed*20;
			gDestFantasmaRed.y = gLinha_FantasmaRed*20;
			SDL_BlitSurface(gFantasmaRed, NULL, gTela, &gDestFantasmaRed);
			break;
			
		case 1:
			gColuna_FantasmaRed--;
			
			if(mapa[gLinha_FantasmaRed][gColuna_FantasmaRed] == PAREDE){
				gColuna_FantasmaRed++;
				movimentoRed = ((rand() % 2)+2);
			}
			gDestFantasmaRed.x = gColuna_FantasmaRed*20;
			gDestFantasmaRed.y = gLinha_FantasmaRed*20;
			SDL_BlitSurface(gFantasmaRed, NULL, gTela, &gDestFantasmaRed);
			break;
		
		case 2:
			gLinha_FantasmaRed++;
			
			if(mapa[gLinha_FantasmaRed][gColuna_FantasmaRed] == PAREDE){
				gLinha_FantasmaRed--;
				movimentoRed = rand() % 2;
			}
			gDestFantasmaRed.x = gColuna_FantasmaRed*20;
			gDestFantasmaRed.y = gLinha_FantasmaRed*20;
			SDL_BlitSurface(gFantasmaRed, NULL, gTela, &gDestFantasmaRed);
			break;
		
		case 3:
			gLinha_FantasmaRed--;
			
			if(mapa[gLinha_FantasmaRed][gColuna_FantasmaRed] == PAREDE){
				gLinha_FantasmaRed++;
				movimentoRed = rand() % 2;	
			}
			gDestFantasmaRed.x = gColuna_FantasmaRed*20;
			gDestFantasmaRed.y = gLinha_FantasmaRed*20;
			SDL_BlitSurface(gFantasmaRed, NULL, gTela, &gDestFantasmaRed);
			break;
	}
}
void movimenta_fantasmaBlue(  ){

	switch(movimentoBlue){

		case 0:
			gColuna_FantasmaBlue++;
			
			if(mapa[gLinha_FantasmaBlue][gColuna_FantasmaBlue] == PAREDE){
				gColuna_FantasmaBlue--;
				movimentoBlue = ((rand() % 2)+2);
			}
			gDestFantasmaBlue.x = gColuna_FantasmaBlue*20;
			gDestFantasmaBlue.y = gLinha_FantasmaBlue*20;
			SDL_BlitSurface(gFantasmaBlue, NULL, gTela, &gDestFantasmaBlue);
			break;
			
		case 1:
			gColuna_FantasmaBlue--;
			
			if(mapa[gLinha_FantasmaBlue][gColuna_FantasmaBlue] == PAREDE){
				gColuna_FantasmaBlue++;
				movimentoBlue = ((rand() % 2)+2);
			}
			gDestFantasmaBlue.x = gColuna_FantasmaBlue*20;
			gDestFantasmaBlue.y = gLinha_FantasmaBlue*20;
			SDL_BlitSurface(gFantasmaBlue, NULL, gTela, &gDestFantasmaBlue);
			break;
		
		case 2:
			gLinha_FantasmaBlue++;
			
			if(mapa[gLinha_FantasmaBlue][gColuna_FantasmaBlue] == PAREDE){
				gLinha_FantasmaBlue--;
				movimentoBlue = rand() % 2;
			}
			gDestFantasmaBlue.x = gColuna_FantasmaBlue*20;
			gDestFantasmaBlue.y = gLinha_FantasmaBlue*20;
			SDL_BlitSurface(gFantasmaBlue, NULL, gTela, &gDestFantasmaBlue);
			break;
		
		case 3:
			gLinha_FantasmaBlue--;
			
			if(mapa[gLinha_FantasmaBlue][gColuna_FantasmaBlue] == PAREDE){
				gLinha_FantasmaBlue++;
				movimentoBlue = rand() % 2;	
			}
			gDestFantasmaBlue.x = gColuna_FantasmaBlue*20;
			gDestFantasmaBlue.y = gLinha_FantasmaBlue*20;
			SDL_BlitSurface(gFantasmaBlue, NULL, gTela, &gDestFantasmaBlue);
			break;
	}
}
void movimenta_fantasmaOrange(  ){

	switch(movimentoOrange){

		case 0:
			gColuna_FantasmaOrange++;
			
			if(mapa[gLinha_FantasmaOrange][gColuna_FantasmaOrange] == PAREDE){
				gColuna_FantasmaOrange--;
				movimentoOrange = ((rand() % 2)+2);
			}
			gDestFantasmaOrange.x = gColuna_FantasmaOrange*20;
			gDestFantasmaOrange.y = gLinha_FantasmaOrange*20;
			SDL_BlitSurface(gFantasmaOrange, NULL, gTela, &gDestFantasmaOrange);
			break;
			
		case 1:
			gColuna_FantasmaOrange--;
			
			if(mapa[gLinha_FantasmaOrange][gColuna_FantasmaOrange] == PAREDE){
				gColuna_FantasmaOrange++;
				movimentoOrange = ((rand() % 2)+2);
			}
			gDestFantasmaOrange.x = gColuna_FantasmaOrange*20;
			gDestFantasmaOrange.y = gLinha_FantasmaOrange*20;
			SDL_BlitSurface(gFantasmaOrange, NULL, gTela, &gDestFantasmaOrange);
			break;
		
		case 2:
			gLinha_FantasmaOrange++;
			
			if(mapa[gLinha_FantasmaOrange][gColuna_FantasmaOrange] == PAREDE){
				gLinha_FantasmaOrange--;
				movimentoOrange = rand() % 2;
			}
			gDestFantasmaOrange.x = gColuna_FantasmaOrange*20;
			gDestFantasmaOrange.y = gLinha_FantasmaOrange*20;
			SDL_BlitSurface(gFantasmaOrange, NULL, gTela, &gDestFantasmaOrange);
			break;
		
		case 3:
			gLinha_FantasmaOrange--;
			
			if(mapa[gLinha_FantasmaOrange][gColuna_FantasmaOrange] == PAREDE){
				gLinha_FantasmaOrange++;
				movimentoOrange = rand() % 2;	
			}
			gDestFantasmaOrange.x = gColuna_FantasmaOrange*20;
			gDestFantasmaOrange.y = gLinha_FantasmaOrange*20;
			SDL_BlitSurface(gFantasmaOrange, NULL, gTela, &gDestFantasmaOrange);
			break;
	}
}
void movimenta_fantasmaPink(  ){

	switch(movimentoPink){

		case 0:
			gColuna_FantasmaPink++;
			
			if(mapa[gLinha_FantasmaPink][gColuna_FantasmaPink] == PAREDE){
				gColuna_FantasmaPink--;
				movimentoPink = ((rand() % 2)+2);
			}
			gDestFantasmaPink.x = gColuna_FantasmaPink*20;
			gDestFantasmaPink.y = gLinha_FantasmaPink*20;
			SDL_BlitSurface(gFantasmaPink, NULL, gTela, &gDestFantasmaPink);
			break;
			
		case 1:
			gColuna_FantasmaPink--;
			
			if(mapa[gLinha_FantasmaPink][gColuna_FantasmaPink] == PAREDE){
				gColuna_FantasmaPink++;
				movimentoPink = ((rand() % 2)+2);
			}
			gDestFantasmaPink.x = gColuna_FantasmaPink*20;
			gDestFantasmaPink.y = gLinha_FantasmaPink*20;
			SDL_BlitSurface(gFantasmaPink, NULL, gTela, &gDestFantasmaPink);
			break;
		
		case 2:
			gLinha_FantasmaPink++;
			
			if(mapa[gLinha_FantasmaPink][gColuna_FantasmaPink] == PAREDE){
				gLinha_FantasmaPink--;
				movimentoPink = rand() % 2;
			}
			gDestFantasmaPink.x = gColuna_FantasmaPink*20;
			gDestFantasmaPink.y = gLinha_FantasmaPink*20;
			SDL_BlitSurface(gFantasmaPink, NULL, gTela, &gDestFantasmaPink);
			break;
		
		case 3:
			gLinha_FantasmaPink--;
			
			if(mapa[gLinha_FantasmaPink][gColuna_FantasmaPink] == PAREDE){
				gLinha_FantasmaPink++;
				movimentoPink = rand() % 2;	
			}
			gDestFantasmaPink.x = gColuna_FantasmaPink*20;
			gDestFantasmaPink.y = gLinha_FantasmaPink*20;
			SDL_BlitSurface(gFantasmaPink, NULL, gTela, &gDestFantasmaPink);
			break;
	}
}
int teste_colisao(int temporizador){

	if(gLinha_atual == gLinha_FantasmaRed){
		if(gColuna_atual == gColuna_FantasmaRed){//TESTANDO COLISAO COM VERMELHO
			if(gControlRed){
				if(temporizador){
					if(gMusicControlador) Mix_PlayChannel(-1, gMorteFantasma, 0);
					gLinha_FantasmaRed = 13; gColuna_FantasmaRed = 13; movimentoRed = 3;
					gQuantFant++;
					if(gQuantFant == 1){
						gPontuacao+=200;
						SDL_BlitSurface(gPonto200, NULL, gTela, &gDestFantasmaRed);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					if(gQuantFant == 2){
						gPontuacao+=400;
						SDL_BlitSurface(gPonto400, NULL, gTela, &gDestFantasmaRed);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					if(gQuantFant == 3){
						gPontuacao+=800;
						SDL_BlitSurface(gPonto800, NULL, gTela, &gDestFantasmaRed);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					if(gQuantFant == 4){
						gPontuacao+=1600;
						SDL_BlitSurface(gPonto1600, NULL, gTela, &gDestFantasmaRed);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					rewind(gSomarPontos);
					fprintf(gSomarPontos, "%d", gPontuacao);
					rewind(gSomarPontos);
					fgets(gPontos, 10000, gSomarPontos);
					gttf = TTF_RenderText_Blended(gFonte, gPontos, gCor); //PONTUAÇÃO
					gFantasmaRed = gTempFantRed; //VOLTANDO A SUA COR
					gControlRed= false;
				}
			}else{
				gColuna_atual = 14, gLinha_atual = 23, movimentoRed = 3, gLinha_FantasmaRed = 13, gColuna_FantasmaRed = 13, movimentoBlue = 3, gLinha_FantasmaBlue = 13, gColuna_FantasmaBlue = 14, movimentoOrange = 3, gLinha_FantasmaOrange = 14, gColuna_FantasmaOrange = 13, movimentoPink = 3, gLinha_FantasmaPink = 14, gColuna_FantasmaPink = 14;
				gVidas--;
				if(!gVidas){
					Mix_PauseMusic();
					Morte_PacMan();
					SDL_BlitSurface(gGameOver, NULL, gTela, &gDestGetReady);
					SDL_UpdateWindowSurface( gJanela );
					SDL_Delay(2000);//ESPERADO ANTES DE MOSTRAR TELA DE SCORES
					Ler_Nome_Jogador(  );
					return false;
				}
				Mix_PauseMusic();//PAUSANDO A MUSICA DE FUNDO PARA A MORTE DO PACMAN
				Morte_PacMan(); //ANIMAÇÃO DA MORTE DO PACMAN
				SDL_FillRect(gTela, NULL, SDL_MapRGB(gTela->format, 255, 255, 255));//LIMPANDO A TELA
				desenha_mapa(); //REDESENHANDO O MAPA
				blitar_Local_Inicial( );//RECOLOCANDO OS FANTASMAS E O PACMAN NO SEUS LOCAIS DE INICIO
				SDL_BlitSurface(gGetReady, NULL, gTela, &gDestGetReady);
				SDL_UpdateWindowSurface(gJanela);
				SDL_Delay(2000);//ESPERANDO ANTES DE COMEÇAR
				Mix_ResumeMusic();//RETORNANDO A MUSICA DO PACMAN
			}
		}
	}
	if(gLinha_atual == gLinha_FantasmaBlue){
		if(gColuna_atual == gColuna_FantasmaBlue){//TESTANDO COLISAO COM AZUL
			if(gControlBlue){
				if(temporizador){
					if(gMusicControlador) Mix_PlayChannel(-1, gMorteFantasma, 0);
					
					gLinha_FantasmaBlue = 13; gColuna_FantasmaBlue = 14; movimentoBlue = 3;
					gQuantFant++;
					if(gQuantFant == 1){
						gPontuacao+=200;
						SDL_BlitSurface(gPonto200, NULL, gTela, &gDestFantasmaBlue);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					if(gQuantFant == 2){
						gPontuacao+=400;
						SDL_BlitSurface(gPonto400, NULL, gTela, &gDestFantasmaBlue);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					if(gQuantFant == 3){
						gPontuacao+=800;
						SDL_BlitSurface(gPonto800, NULL, gTela, &gDestFantasmaBlue);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					if(gQuantFant == 4){
						gPontuacao+=1600;
						SDL_BlitSurface(gPonto1600, NULL, gTela, &gDestFantasmaBlue);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					rewind(gSomarPontos);
					fprintf(gSomarPontos, "%d", gPontuacao);
					rewind(gSomarPontos);
					fgets(gPontos, 10000, gSomarPontos);
					gttf = TTF_RenderText_Blended(gFonte, gPontos, gCor); //PONTUAÇÃO
					gFantasmaBlue = gTempFantBlue; //VOLTANDO A SUA COR
					gControlBlue = false;
				}
			}else{
				gColuna_atual = 14, gLinha_atual = 23, movimentoRed = 3, gLinha_FantasmaRed = 13, gColuna_FantasmaRed = 13, movimentoBlue = 3, gLinha_FantasmaBlue = 13, gColuna_FantasmaBlue = 14, movimentoOrange = 3, gLinha_FantasmaOrange = 14, gColuna_FantasmaOrange = 13, movimentoPink = 3, gLinha_FantasmaPink = 14, gColuna_FantasmaPink = 14;
				gVidas--;
				if(!gVidas){
					Mix_PauseMusic();
					Morte_PacMan();
					SDL_BlitSurface(gGameOver, NULL, gTela, &gDestGetReady);
					SDL_UpdateWindowSurface( gJanela );
					SDL_Delay(2000);
					Ler_Nome_Jogador(  );
					return false;
				}
				Mix_PauseMusic();//PAUSANDO A MUSICA DE FUNDO PARA A MORTE DO PACMAN
				Morte_PacMan(); //ANIMAÇÃO DA MORTE DO PACMAN
				SDL_FillRect(gTela, NULL, SDL_MapRGB(gTela->format, 255, 255, 255));//LIMPANDO A TELA
				desenha_mapa(); //REDESENHANDO O MAPA
				blitar_Local_Inicial( );//RECOLOCANDO OS FANTASMAS E O PACMAN NO SEUS LOCAIS DE INICIO
				SDL_BlitSurface(gGetReady, NULL, gTela, &gDestGetReady);
				SDL_UpdateWindowSurface(gJanela);
				SDL_Delay(2000);//ESPERANDO ANTES DE COMEÇAR
				Mix_ResumeMusic();//RETORNANDO A MUSICA DO PACMAN
			}
		}
	}
	if(gLinha_atual == gLinha_FantasmaOrange){
		if(gColuna_atual == gColuna_FantasmaOrange){//TESTANDO COLISAO COM LARANJA
			if(gControlOrange){
				if(temporizador){
					if(gMusicControlador) Mix_PlayChannel(-1, gMorteFantasma, 0);
					
					gLinha_FantasmaOrange = 14; gColuna_FantasmaOrange = 13; movimentoOrange = 3;
					gQuantFant++;
					if(gQuantFant == 1){
						gPontuacao+=200;
						SDL_BlitSurface(gPonto200, NULL, gTela, &gDestFantasmaOrange);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					if(gQuantFant == 2){
						gPontuacao+=400;
						SDL_BlitSurface(gPonto400, NULL, gTela, &gDestFantasmaOrange);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					if(gQuantFant == 3){
						gPontuacao+=800;
						SDL_BlitSurface(gPonto800, NULL, gTela, &gDestFantasmaOrange);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					if(gQuantFant == 4){
						gPontuacao+=1600;
						SDL_BlitSurface(gPonto1600, NULL, gTela, &gDestFantasmaOrange);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					rewind(gSomarPontos);
					fprintf(gSomarPontos, "%d", gPontuacao);
					rewind(gSomarPontos);
					fgets(gPontos, 10000, gSomarPontos);
					gttf = TTF_RenderText_Blended(gFonte, gPontos, gCor); //PONTUAÇÃO
					gFantasmaOrange = gTempFantOrange; //VOLTANDO A SUA COR
					gControlOrange = false;
				}
			}else{
				gColuna_atual = 14, gLinha_atual = 23, movimentoRed = 3, gLinha_FantasmaRed = 13, gColuna_FantasmaRed = 13, movimentoBlue = 3, gLinha_FantasmaBlue = 13, gColuna_FantasmaBlue = 14, movimentoOrange = 3, gLinha_FantasmaOrange = 14, gColuna_FantasmaOrange = 13, movimentoPink = 3, gLinha_FantasmaPink = 14, gColuna_FantasmaPink = 14;
				gVidas--;
				if(!gVidas){
					Mix_PauseMusic();
					Morte_PacMan();
					SDL_BlitSurface(gGameOver, NULL, gTela, &gDestGetReady);
					SDL_UpdateWindowSurface( gJanela );
					SDL_Delay(2000);
					Ler_Nome_Jogador(  );
					return false;
				}
				Mix_PauseMusic();//PAUSANDO A MUSICA DE FUNDO PARA A MORTE DO PACMAN
				Morte_PacMan(); //ANIMAÇÃO DA MORTE DO PACMAN
				SDL_FillRect(gTela, NULL, SDL_MapRGB(gTela->format, 255, 255, 255));//LIMPANDO A TELA
				desenha_mapa(); //REDESENHANDO O MAPA
				blitar_Local_Inicial( );//RECOLOCANDO OS FANTASMAS E O PACMAN NO SEUS LOCAIS DE INICIO
				SDL_BlitSurface(gGetReady, NULL, gTela, &gDestGetReady);
				SDL_UpdateWindowSurface(gJanela);
				SDL_Delay(2000);//ESPERANDO ANTES DE COMEÇAR
				Mix_ResumeMusic();//RETORNANDO A MUSICA DO PACMAN
			}
		}
	}
	if(gLinha_atual == gLinha_FantasmaPink){
		if(gColuna_atual == gColuna_FantasmaPink){//TESTANDO COLISAO COM ROSA
			if(gControlPink){
				if(temporizador){
					if(gMusicControlador) Mix_PlayChannel(-1, gMorteFantasma, 0);
					
					gLinha_FantasmaPink = 14; gColuna_FantasmaPink = 14; movimentoPink = 3;
					gQuantFant++;
					if(gQuantFant == 1){
						gPontuacao+=200;
						SDL_BlitSurface(gPonto200, NULL, gTela, &gDestFantasmaPink);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					if(gQuantFant == 2){
						gPontuacao+=400;
						SDL_BlitSurface(gPonto400, NULL, gTela, &gDestFantasmaPink);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					if(gQuantFant == 3){
						gPontuacao+=800;
						SDL_BlitSurface(gPonto800, NULL, gTela, &gDestFantasmaPink);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					if(gQuantFant == 4){
						gPontuacao+=1600;
						SDL_BlitSurface(gPonto1600, NULL, gTela, &gDestFantasmaPink);
						SDL_UpdateWindowSurface(gJanela);
						SDL_Delay(200);
					}
					rewind(gSomarPontos);
					fprintf(gSomarPontos, "%d", gPontuacao);
					rewind(gSomarPontos);
					fgets(gPontos, 10000, gSomarPontos);
					gttf = TTF_RenderText_Blended(gFonte, gPontos, gCor); //PONTUAÇÃO
					gFantasmaPink = gTempFantPink; //VOLTANDO A SUA COR
					gControlPink = false;
				}
			}else{
				gColuna_atual = 14, gLinha_atual = 23, movimentoRed = 3, gLinha_FantasmaRed = 13, gColuna_FantasmaRed = 13, movimentoBlue = 3, gLinha_FantasmaBlue = 13, gColuna_FantasmaBlue = 14, movimentoOrange = 3, gLinha_FantasmaOrange = 14, gColuna_FantasmaOrange = 13, movimentoPink = 3, gLinha_FantasmaPink = 14, gColuna_FantasmaPink = 14;
				gVidas--;
				if(!gVidas){
					Mix_PauseMusic();
					Morte_PacMan();
					SDL_BlitSurface(gGameOver, NULL, gTela, &gDestGetReady);
					SDL_UpdateWindowSurface( gJanela );
					SDL_Delay(2000);
					Ler_Nome_Jogador(  );
					return false;
				}
				Mix_PauseMusic();//PAUSANDO A MUSICA DE FUNDO PARA A MORTE DO PACMAN
				Morte_PacMan(); //ANIMAÇÃO DA MORTE DO PACMAN
				SDL_FillRect(gTela, NULL, SDL_MapRGB(gTela->format, 255, 255, 255));//LIMPANDO A TELA
				desenha_mapa(); //REDESENHANDO O MAPA
				blitar_Local_Inicial( );//RECOLOCANDO OS FANTASMAS E O PACMAN NO SEUS LOCAIS DE INICIO
				SDL_BlitSurface(gGetReady, NULL, gTela, &gDestGetReady);
				SDL_UpdateWindowSurface(gJanela);
				SDL_Delay(2000);//ESPERANDO ANTES DE COMEÇAR
				Mix_ResumeMusic();//RETORNANDO A MUSICA DO PACMAN
			}
		}
	}
	
	
	return true;
}
void Morte_PacMan( ){

	if(gMusicControlador){
		Mix_PlayChannel(-1, gMortePacMan, 0); //TOCANDO A TRISTE MUSICA DA MORTE DO PACMAN
	}else Mix_Pause(-1);

	SDL_FillRect(gTela, NULL, SDL_MapRGB(gTela->format, 255, 255, 255));//LIMPANDO A TELA
	desenha_mapa(); //REDESENHANDO O MAPA
	//ANIMAÇÃO DA MORTE DO PACMAN
	SDL_BlitSurface(gMorrendo1, NULL, gTela, &gDestino);
	SDL_UpdateWindowSurface( gJanela );
	SDL_Delay(130);
	SDL_BlitSurface(gMorrendo2, NULL, gTela, &gDestino);
	SDL_UpdateWindowSurface( gJanela );
	SDL_Delay(130);
	SDL_BlitSurface(gMorrendo3, NULL, gTela, &gDestino);
	SDL_UpdateWindowSurface( gJanela );
	SDL_Delay(130);
	SDL_BlitSurface(gMorrendo4, NULL, gTela, &gDestino);
	SDL_UpdateWindowSurface( gJanela );
	SDL_Delay(130);
	SDL_BlitSurface(gMorrendo5, NULL, gTela, &gDestino);
	SDL_UpdateWindowSurface( gJanela );
	SDL_Delay(130);
	SDL_BlitSurface(gMorrendo6, NULL, gTela, &gDestino);
	SDL_UpdateWindowSurface( gJanela );
	SDL_Delay(130);
	SDL_BlitSurface(gMorrendo7, NULL, gTela, &gDestino);
	SDL_UpdateWindowSurface( gJanela );
	SDL_Delay(130);
	SDL_BlitSurface(gMorrendo8, NULL, gTela, &gDestino);
	SDL_UpdateWindowSurface( gJanela );
	SDL_Delay(130);
	SDL_BlitSurface(gMorrendo9, NULL, gTela, &gDestino);
	SDL_UpdateWindowSurface( gJanela );
	SDL_Delay(130);
}
void blitar_Local_Inicial( ){
	gDestino.x = gColuna_atual *20; //POSIÇÃO INICIAL X DO PACMAN
	gDestino.y = gLinha_atual *20; //POSIÇÃO INICIAL Y DO PACMAN
	gDestFantasmaRed.x = gColuna_FantasmaRed*20;//POSIÇÃO INICIAL X DO FANTASMA VERMELHOR
	gDestFantasmaRed.y = gLinha_FantasmaRed*20;//POSIÇÃO INICIAL Y DO FANTASMA VERMELHO
	gDestFantasmaBlue.x = gColuna_FantasmaBlue*20;//POSIÇÃO INICIAL X DO FANTASMA AZUL
	gDestFantasmaBlue.y = gLinha_FantasmaBlue*20;//POSIÇÃO INICIAL Y DO FANTASMA AZUL
	gDestFantasmaOrange.x = gColuna_FantasmaOrange*20;//POSIÇÃO INICIAL X DO FANTASMA LARANJA
	gDestFantasmaOrange.y = gLinha_FantasmaOrange*20;//POSIÇÃO INICIAL Y DO FANTASMA LARANJA
	gDestFantasmaPink.x = gColuna_FantasmaPink*20;//POSIÇÃO INICIAL X DO FANTASMA ROSA
	gDestFantasmaPink.y = gLinha_FantasmaPink*20;//POSIÇÃO INICIAL Y DO FANTASMA ROSA
	SDL_BlitSurface(gPacFechado, NULL, gTela, &gDestino);//BLITANDO O PACMAN NO SEU DEVIDO LOCAL
	SDL_BlitSurface(gFantasmaRed, NULL, gTela, &gDestFantasmaRed);//BLITANDO O FANTASMA NO SEU DEVIDO LOCAL
	SDL_BlitSurface(gFantasmaBlue, NULL, gTela, &gDestFantasmaBlue);//BLITANDO O FANTASMA NO SEU DEVIDO LOCAL
	SDL_BlitSurface(gFantasmaOrange, NULL, gTela, &gDestFantasmaOrange);//BLITANDO O FANTASMA NO SEU DEVIDO LOCAL
	SDL_BlitSurface(gFantasmaPink, NULL, gTela, &gDestFantasmaPink);//BLITANDO O FANTASMA NO SEU DEVIDO LOCAL
	if(mapa[gLinha_atual][gColuna_atual] == INICIO){ //TESTE PARA VERIFICAÇÃO SE O PACMAN ESTA NO INICIO PARA COMEÇAR
		mapa[gLinha_atual][gColuna_atual] = 0;
		fprintf(gSomarPontos, "%d", gPontuacao); // ESCREVENDO ZERO NO PLACAR
		rewind(gSomarPontos); //VOLTANDO AO INICIO DO ARQUIVO
		fgets(gPontos, 10000, gSomarPontos); //LENDO A PONTIAÇÃO
		gttf = TTF_RenderText_Blended(gFonte, gPontos, gCor);//COR, FONTE E TEXTO DA PONTUAÇÃO, E APRESENTANDO NA TELA
	}

}
void Liberar_Imagens(  ){

	SDL_FreeSurface(gTelaInicial);
	SDL_FreeSurface(gParede);
	SDL_FreeSurface(gPacFechado);
	SDL_FreeSurface(gPacEsquerda);
	SDL_FreeSurface(gPacDireita);
	SDL_FreeSurface(gPacCima);
	SDL_FreeSurface(gPacBaixo);
	SDL_FreeSurface(gPacEsquerda2);
	SDL_FreeSurface(gPacDireita2);
	SDL_FreeSurface(gPacCima2);
	SDL_FreeSurface(gPacBaixo2);
	SDL_FreeSurface(gGameOver);
	SDL_FreeSurface(gStartGame);
	SDL_FreeSurface(gRed1);
	SDL_FreeSurface(gPilula);
	SDL_FreeSurface(gPilula2);
	SDL_FreeSurface(gRed2);
	SDL_FreeSurface(gPac);
	SDL_FreeSurface(gPac1);
	SDL_FreeSurface(gPac2);
	SDL_FreeSurface(gAbout);
	SDL_FreeSurface(gLives);
	SDL_FreeSurface(gVida);
	SDL_FreeSurface(gFantasmaRed);
	SDL_FreeSurface(gFantasmaBlue);
	SDL_FreeSurface(gFantasmaOrange);
	SDL_FreeSurface(gScore);
	SDL_FreeSurface(gControls);
	SDL_FreeSurface(gHighScore);
	SDL_FreeSurface(gPink1);
	SDL_FreeSurface(gPink2);
	SDL_FreeSurface(gBlue1);
	SDL_FreeSurface(gBlue2);
	SDL_FreeSurface(gOrange1);
	SDL_FreeSurface(gOrange2);
	SDL_FreeSurface(gMorrendo1);
	SDL_FreeSurface(gMorrendo2);
	SDL_FreeSurface(gMorrendo3);
	SDL_FreeSurface(gMorrendo4);
	SDL_FreeSurface(gMorrendo5);
	SDL_FreeSurface(gMorrendo6);
	SDL_FreeSurface(gMorrendo7);
	SDL_FreeSurface(gMorrendo8);
	SDL_FreeSurface(gMorrendo9);
	SDL_FreeSurface(gGetReady);
	SDL_FreeSurface(gDigiteName);
	SDL_FreeSurface(gPause);
	SDL_FreeSurface(gFantasmaFraco);
	SDL_FreeSurface(gLVL2);
	SDL_FreeSurface(gPonto200);
	SDL_FreeSurface(gPonto400);
	SDL_FreeSurface(gPonto800);
	SDL_FreeSurface(gPonto1600);
	}
void Liberar_Musicas(  ){
	Mix_FreeChunk(gPastilha);
	Mix_FreeChunk(gIntro);
	Mix_FreeChunk(gMortePacMan);
	Mix_FreeMusic(gMusicFundo);
}
void Ler_Nome_Jogador(  ){

	int i = 0, running = true;
	SDL_Event teclas;
	SDL_Rect gDestNomeUsuario = {115, 375, 0, 0};
	SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
	SDL_UpdateWindowSurface(gJanela);

	while(running){
		while(SDL_PollEvent(&teclas)){
			if(teclas.type == SDL_QUIT) running = false;
			if(teclas.key.keysym.sym == SDLK_RETURN){
				running = false;
				gTelaPrincipal = true;
				fprintf(gPlacarFinal, "%s %d\n", gNomeUsuario, gPontuacao);
				break;
			}else{
				if (teclas.type == SDL_KEYUP){
					switch(teclas.key.keysym.sym){
						case SDLK_a:
							gNomeUsuario[i] = 'a';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_b:
							gNomeUsuario[i] = 'b';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_c:
							gNomeUsuario[i] = 'c';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_d:
							gNomeUsuario[i] = 'd';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_e:
							gNomeUsuario[i] = 'e';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_f:
							gNomeUsuario[i] = 'f';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_g:
							gNomeUsuario[i] = 'g';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_h:
							gNomeUsuario[i] = 'h';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_i:
							gNomeUsuario[i] = 'i';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_j:
							gNomeUsuario[i] = 'j';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_k:
							gNomeUsuario[i] = 'k';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_l:
							gNomeUsuario[i] = 'l';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_m:
							gNomeUsuario[i] = 'm';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_n:
							gNomeUsuario[i] = 'n';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_o:
							gNomeUsuario[i] = 'o';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_p:
							gNomeUsuario[i] = 'p';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_q:
							gNomeUsuario[i] = 'q';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_r:
							gNomeUsuario[i] = 'r';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_s:
							gNomeUsuario[i] = 's';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_t:
							gNomeUsuario[i] = 't';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_u:
							gNomeUsuario[i] = 'u';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_v:
							gNomeUsuario[i] = 'v';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_w:
							gNomeUsuario[i] = 'w';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_y:
							gNomeUsuario[i] = 'y';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_z:
							gNomeUsuario[i] = 'z';
							i++;
							gNomeUsuario[i] = '\0';
							SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
							gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
							SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
							SDL_UpdateWindowSurface(gJanela);
							break;
						case SDLK_BACKSPACE:
							if(i>0){
								i--;
								gNomeUsuario[i] = '\0';
								SDL_BlitSurface(gDigiteName, NULL, gTela, NULL);
								gUsuario = TTF_RenderText_Blended(gFonte, gNomeUsuario, gCorUserName); //COR, FONTE E TEXTO DA PONTUAÇÃO
								SDL_BlitSurface(gUsuario, NULL, gTela, &gDestNomeUsuario);
								SDL_UpdateWindowSurface(gJanela);
							}
							break;
						default:
							break;
					}
				}
			}
		}
	}
}

