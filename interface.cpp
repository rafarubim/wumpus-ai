#include "Graphics.h"
#include "interface.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "Animate.hpp"
#include <Windows.h>
#include "Global.hpp"
#include "FogOfWar.hpp"
#include "Agente.hpp"
#include "PreLoad.hpp"
#include <SWI-cpp.h>
#include <string.h>

#define TEMPO_DECISAO 10

int AI = 1;

static Graphics graphics;
static InterGraf graf;
static FOW_cls fogow;
static AgenteCls agnt;
static EndGameCls endgame;
static Animation anim;
static char mapa[LN][COL];
static PLCls preload;
static TalkProlog info;
static ClsHUD HUD;
static EndGame end;

Image MapFloor,Grade;
Image RightWall, LeftWall, TopWall, DownWall, TLCorner, TRCorner, DLCorner, DRCorner;
Image Buraco;
Image MarcaBuraco;
Image MarcaInimigo;
Image MarcaTeleport;
Image HealthHUD[11];
Image Entrada;
Image AuxSee;
Image HUDBox;

int numero;
int getSecond(float t);
float getTime(void);
void teclado(int key, int state, int x, int y);
int treco = 0;
int gS = 1; //gS = gridSize
int timer = 0;
int PRE_LOAD_STATE = 1;
int POS_LOAD_STATE = 0;
int contaLoop = 0;

void MainLoop()
{
	float elapsedTime = graphics.GetElapsedTime();
	if(PRE_LOAD_STATE == 1)
	{
		preload.loop_pre_load(graphics,anim);
		//PRE_LOAD_STATE = 0;
	}
	else
	{
		if(NLoop == 0)
		{
			graf.iniciaMapa();
			fogow.imprime_fow(graphics);
			NLoop++;
		}
		else
		{

			graf.iniciaMapa();
			CasaAtual = agnt.checa_casa_atual();
			fogow.atualiza_fow(agnt);
			fogow.imprime_fow(graphics);
			if(agnt.get_AGENT_STATE() == 0)
			{
				agnt.drawstatic(graphics,anim);
			}
			else
			{
				agnt.drawmvmt(graphics, 0, anim);
				//Sleep(100);
				Sleep(10);
			}
			if(POS_LOAD_STATE == 0)
			{
				endgame.checaJogo();
				if(AI == 1)
				{

					if (contaLoop < TEMPO_DECISAO) {
						contaLoop++;
					} else {
						if (info.le_prolog())
						contaLoop = 0;
					}
				}
			}
			else
			{
				end.EndHUD(NLoop);
			}
			graf.auxilia();
			passa_mapa(mapa);
			HUD.insereHUD();
			NLoop++;
			//Sleep(1000);
		}
	}
}
InterGraf::InterGraf()
{
	GAME_STATE = 0;
}
void InterGraf::executa()
{
	le_mapa();
	passa_mapa(mapa);
	graf.iniciaGraficos(FS);
	graf.carregaImagens();
	graphics.SetKeyboardInput(teclado);
	graphics.SetMainLoop(MainLoop);
	graphics.StartMainLoop();
}
void InterGraf::iniciaGraficos(int FlSn)
{
	graphics.CreateMainWindow(SW, SH, "Teste");
	graphics.SetBackgroundColor(40,40,40);
	//graphics.SetBackgroundColor(200,200,200);
	if(FlSn == 1)
	{
		graphics.SetFullscreen(true);
	}
}
void InterGraf::carregaImagens(void)
{
	//MapFloor;
	//RightWall, LeftWall, TopWall, DownWall, TLCorner, TRCorner, DLCorner, DRCorner;
	fogow.load();
	agnt.load();
	anim.load_em_all();
	preload.load();
	MapFloor.LoadPNGImage("Sprites\\Floor\\Floor.png");
	RightWall.LoadPNGImage("Sprites\\Wall\\RightWall.png");
	LeftWall.LoadPNGImage("Sprites\\Wall\\LeftWall.png");
	TopWall.LoadPNGImage("Sprites\\Wall\\TopWall.png");
	DownWall.LoadPNGImage("Sprites\\Wall\\DownWall.png");
	TLCorner.LoadPNGImage("Sprites\\Wall\\TLCorner.png");
	TRCorner.LoadPNGImage("Sprites\\Wall\\TRCorner.png");
	DLCorner.LoadPNGImage("Sprites\\Wall\\DLCorner.png");
	DRCorner.LoadPNGImage("Sprites\\Wall\\DRCorner.png");
	Buraco.LoadPNGImage("Sprites\\Buraco\\Buraco.png");
	Grade.LoadPNGImage("Sprites\\grid.png");
	MarcaBuraco.LoadPNGImage("Sprites\\Marks\\HoleMark.png");
	MarcaInimigo.LoadPNGImage("Sprites\\Marks\\SwordMark.png");
	MarcaTeleport.LoadPNGImage("Sprites\\Marks\\Portal.png");
	HealthHUD[0].LoadPNGImage("Sprites\\HUDHP\\HUD_0.png");
	HealthHUD[1].LoadPNGImage("Sprites\\HUDHP\\HUD_1.png");
	HealthHUD[2].LoadPNGImage("Sprites\\HUDHP\\HUD_2.png");
	HealthHUD[3].LoadPNGImage("Sprites\\HUDHP\\HUD_3.png");
	HealthHUD[4].LoadPNGImage("Sprites\\HUDHP\\HUD_4.png");
	HealthHUD[5].LoadPNGImage("Sprites\\HUDHP\\HUD_5.png");
	HealthHUD[6].LoadPNGImage("Sprites\\HUDHP\\HUD_6.png");
	HealthHUD[7].LoadPNGImage("Sprites\\HUDHP\\HUD_7.png");
	HealthHUD[8].LoadPNGImage("Sprites\\HUDHP\\HUD_8.png");
	HealthHUD[9].LoadPNGImage("Sprites\\HUDHP\\HUD_9.png");
	HealthHUD[10].LoadPNGImage("Sprites\\HUDHP\\HUD_10.png");
	Entrada.LoadPNGImage("Sprites\\Entrada\\Entrada.png");
	AuxSee.LoadPNGImage("Sprites\\Spark\\Auxiliar.png");
	HUDBox.LoadPNGImage("Sprites\\MessageBox\\MessageBox.png");
}
void InterGraf::iniciaMapa()
{
	int i;
	int j;
	int coordX;
	int coordY;
	graf.insereChao();
	for(i=1; i<LN+1; i++)
	{
		for(j=1; j<COL+1; j++)
		{
			coordX = (j*DFLTSIZE)+MyZeroX;
			coordY = (i*DFLTSIZE)+MyZeroY;
			graf.insereMarcas(mapa[i-1][j-1], coordX, coordY);
		}
	}
	for(i=1; i<LN+1; i++)
	{
		for(j=1; j<COL+1; j++)
		{
			coordX = (j*DFLTSIZE)+MyZeroX;
			coordY = (i*DFLTSIZE)+MyZeroY;
			graf.insereElementos(mapa[i-1][j-1], coordX, coordY);
		}
	}
	graf.insereParedes();
	graphics.DrawImage2D(MyZeroX+DFLTSIZE,MyZeroY+DFLTSIZE,mapEdgeX-MyZeroX,mapEdgeY-MyZeroY, Grade);
}
void InterGraf::exibeMapa()
{
	int i,j;
	for(i=0; i<LN; i++)
	{
		for(j=0; j<COL; j++) printf("%c", mapa[LN-(i+1)][j]);
		printf("\n");
	}
	return;
}
void InterGraf::insereElementos(char elemento, int coordX, int coordY)
{
	int tile = DFLTSIZE;
	int icon_size = DFLTSIZE;
	int x,y;
	float scale = 1.f;
	x = coordX;
	y = coordY;
	adjust_scale(scale,tile, &x, &y, &icon_size);
	if(elemento == 'P') //Buraco
	{
		graphics.DrawImage2D(x,y,icon_size,icon_size, Buraco);
	}
	else if(elemento == 'E') //Entrada
	{
		float stair_scale = scale * 1.25f;
		adjust_scale(stair_scale,tile, &x, &y, &icon_size);
		graphics.DrawImage2D(x,y+5,icon_size,icon_size, Entrada);
	}
	else if(elemento != '.')
	{
		if(elemento == 'O')
		{
			float gold_scale = scale * 0.5f;
			adjust_scale(gold_scale,tile, &x, &y, &icon_size);
			graphics.DrawImage2D(x,y,icon_size,icon_size, anim.PassaElementos(elemento, NLoop));
		}
		else if(elemento == 'U')
		{
			int a,b;
			a = x;
			b = y;
			float power_scale = scale * 1.25f;
			adjust_scale(power_scale,tile, &x, &y, &icon_size);
			graphics.DrawImage2D(x,y,icon_size,icon_size, anim.PassaElementos(elemento, NLoop));
			graphics.DrawImage2D(a,b,DFLTSIZE,DFLTSIZE, AuxSee);
		}
		else
		{
			graphics.DrawImage2D(x,y,icon_size,icon_size, anim.PassaElementos(elemento, NLoop));
		}
	}
}
void InterGraf::insereMarcas(char elemento, int coordX, int coordY)
{
	int tile = DFLTSIZE;
	int posX;
	int posY;
	if(elemento == 'P') //Buraco
	{
		int posMB_x,posMB_y;
		posMB_x = 0;
		posMB_y = 0;
		posX = coordX + posMB_x + tile;
		posY = coordY + posMB_y;
		graphics.DrawImage2D(posX,posY, 16,16, MarcaBuraco);
		posX = coordX + posMB_x;
		posY = coordY + posMB_y + tile;
		graphics.DrawImage2D(posX,posY, 16,16, MarcaBuraco);
		posX = coordX + posMB_x - tile;
		posY = coordY + posMB_y;
		graphics.DrawImage2D(posX,posY, 16,16, MarcaBuraco);
		posX = coordX + posMB_x;
		posY = coordY + posMB_y - tile;
		graphics.DrawImage2D(posX,posY, 16,16, MarcaBuraco);
	}
	else if(elemento == 'T') //Portal
	{
		int posMT_x,posMT_y;
		posMT_x = 47;
		posMT_y = 47;

		posX = coordX + posMT_x + tile;
		posY = coordY + posMT_y;
		graphics.DrawImage2D(posX,posY, 16,16, MarcaTeleport);

		posX = coordX + posMT_x;
		posY = coordY + posMT_y + tile;
		graphics.DrawImage2D(posX,posY, 16,16, MarcaTeleport);

		posX = coordX + posMT_x - tile;
		posY = coordY + posMT_y;
		graphics.DrawImage2D(posX,posY, 16,16, MarcaTeleport);

		posX = coordX + posMT_x;
		posY = coordY + posMT_y - tile;
		graphics.DrawImage2D(posX,posY, 16,16, MarcaTeleport);
	}
	else if(elemento == 'D' || elemento == 'd') //inimigo
	{
		int posMI_x, posMI_y;
		posMI_x = 1;
		posMI_y = 47;

		posX = coordX + posMI_x + tile;
		posY = coordY + posMI_y;
		graphics.DrawImage2D(posX,posY, 16,16, MarcaInimigo);

		posX = coordX + posMI_x;
		posY = coordY + posMI_y + tile;
		graphics.DrawImage2D(posX,posY, 16,16, MarcaInimigo);

		posX = coordX + posMI_x - tile;
		posY = coordY + posMI_y;
		graphics.DrawImage2D(posX,posY, 16,16, MarcaInimigo);

		posX = coordX + posMI_x;
		posY = coordY + posMI_y - tile;
		graphics.DrawImage2D(posX,posY, 16,16, MarcaInimigo);
	}
}
void InterGraf::insereParedes(void)
{
	int i;
	graphics.DrawImage2D(MyZeroX,MyZeroY,DFLTSIZE,DFLTSIZE, DLCorner);
	graphics.DrawImage2D(mapEdgeX,MyZeroY,DFLTSIZE,DFLTSIZE, DRCorner);
	graphics.DrawImage2D(MyZeroX,mapEdgeY,DFLTSIZE,DFLTSIZE, TLCorner);
	graphics.DrawImage2D(mapEdgeX,mapEdgeY,DFLTSIZE,DFLTSIZE, TRCorner);
	for(i=1; i<COL+1; i++)
	{
		graphics.DrawImage2D((i*DFLTSIZE)+MyZeroX,mapEdgeY,DFLTSIZE,DFLTSIZE, TopWall);
		graphics.DrawImage2D(mapEdgeX,(i*DFLTSIZE)+MyZeroY,DFLTSIZE,DFLTSIZE, RightWall);
		graphics.DrawImage2D(MyZeroX,(i*DFLTSIZE)+MyZeroY,DFLTSIZE,DFLTSIZE, LeftWall);
		graphics.DrawImage2D((i*DFLTSIZE)+MyZeroX,MyZeroY,DFLTSIZE,DFLTSIZE, DownWall);

	}
}
void InterGraf::insereChao(void)
{
	int i;
	int j;
	int coordX;
	int coordY;
	for(i=1; i<LN+1; i++)
	{
		for(j=1; j<COL+1; j++)
		{
			coordX = (j*DFLTSIZE)+MyZeroX;
			coordY = (i*DFLTSIZE)+MyZeroY;
			graphics.DrawImage2D(coordX, coordY, DFLTSIZE,DFLTSIZE, MapFloor);
		}
	}
}
void teclado(int key, int state, int x, int y)
{
	if (state == KEY_STATE_UP)
		return;
	if(AI == 0)
	{
		if(agnt.get_AGENT_STATE() == 0)
		{
			if(POS_LOAD_STATE == 0)
				graf.novoKBoard2(&key);
		}
	}
	if(key == 'h') //help
	{
		printf("Agente:\n");
		printf("HP: %d\n", agnt._health);
		printf("Gold: %d\n",agnt._gold);
		printf("PosX: %d\n",agnt._posX);
		printf("PosY: %d\n",agnt._posY);
		printf("MapEdgeX: %d\n", mapEdgeX);
		printf("MapEdgeY: %d\n", mapEdgeY);
		printf("index X: %d\n",agnt.pos2index(agnt._posX,0));
		printf("index Y: %d\n",agnt.pos2index(agnt._posY,1));
		printf("Elemento: %c\n", agnt.checa_casa_atual());
		printf("Mapa:\n");
		graf.exibeMapa();

		printf("Fog of War:\n");
		fogow.exibe_fow();
		printf("--------------------\n");
	}
	else if(key == '0')
	{
		system("cls");
	}
	else if(key == KEY_ESC)
	{
		exit(0);
	}
	else if(key == 'r') //resetGame
	{
		endgame.reset();
	}
	else if(key == '1')
	{
		POS_LOAD_STATE = 1;
	}

}
void EndGameCls::checaJogo(void)
{
	if(agnt._health <= 0)
	{
		POS_LOAD_STATE = 1;
		NLoop = 0;
	}
	if(CasaAtual == 'E')
	{
		if(agnt._gold == 3)
		{
			POS_LOAD_STATE = 1;
			NLoop = 0;
		}
	}
}
void EndGameCls::reset(void)
{
	printf("---- PROGRAMA REINICIADO ----\n\n");
	int i,j;
	float posfixer;
	//resetaFOW
	for(i=0; i<LN; i++)
	{
		for(j=0; j<COL; j++)
		{
			fogow._fow[i][j] = 0;
		}
	}
	j = posInicialX;
	i = LN-posInicialY-1;
	fogow._fow[i][j] = 2;
//	fogow.exibe_fow();

	//reseta jogador
	agnt._gold = 0;
	agnt._health = 100;
	posfixer = ((1-agnt._scale)/2)*DFLTSIZE;
	agnt._posX = DFLTSIZE + MyZeroX + (int)posfixer;
	agnt._posY = DFLTSIZE + MyZeroY + (int)posfixer;
	agnt._passos = 0;

	//reseta mapa
	for(i=0; i<LN; i++)
	{
		for(j=0; j<COL; j++)
		{
			passa_backup(mapa);
		}
	}
	POS_LOAD_STATE = 0;
	GAME_STATE = 0;
//	graf.exibeMapa();
}
float getTime(void)
{
	float value;
	value = graphics.GetElapsedTime();
	value *= 1000;
	return value;
}
int getSecond(float t)
{
	int value;
	value = (int)(t);
	return value;
}
void InterGraf::novoKBoard2(int *_key)
{
	int key;
	key = *_key;
	int x,y;
	x = agnt._posX;
	y = agnt._posY;
	int indexX;
	int indexY;
	indexY = agnt.pos2index(agnt._posX,0);
	indexX = agnt.pos2index(agnt._posY,1);
	if (key == KEY_RIGHT)
	{
		if(agnt.get_AGENT_STATE() == 0)
		{
			agnt.walk(4,&agnt._posX,&agnt._posY);
		}
	}
	else if (key == KEY_LEFT)
	{
		if(agnt.get_AGENT_STATE() == 0)
		{
			agnt.walk(2,&agnt._posX,&agnt._posY);
		}
	}
	else if (key == KEY_DOWN)
	{
		if(agnt.get_AGENT_STATE() == 0)
		{
			agnt.walk(3,&agnt._posX,&agnt._posY);
		}
	}
	else if (key == KEY_UP)
	{
		if(agnt.get_AGENT_STATE() == 0)
		{
			agnt.walk(1,&agnt._posX,&agnt._posY);
		}
	}
	else if(key == 'g')
	{
		if(mapa[indexX][indexY] == 'O')
		{
			agnt._gold++;
			deleta_ponto(indexX,indexY);
			mapa[indexX][indexY] = CasaVazia;
		}
		else if(mapa[indexX][indexY] == 'U')
		{
			int hp_up = 30;
			if(agnt._health < 100)
			{
				agnt._health += hp_up;
				if(agnt._health > 100) agnt._health = 100;
				deleta_ponto(indexX,indexY);
				mapa[indexX][indexY] = CasaVazia;
			}
		}
	}
	else if(key == 'e')
	{
		if(mapa[indexX][indexY] == 'E')
		{
			if(agnt._gold == 1)
			{
				//exit(0);
			}
		}
	}
	else if(key == 'z')
	{
		printf("A: %c\n",mapa[indexX][indexY]);
	}
}
void InterGraf::auxilia(void)
{
	int _distX = 1000;
	int _distY = 400;
	int y = 0;
	int dy = 20;
	graphics.SetTextFont("Pokemon R/S",20, false, false, false);
	graphics.SetColor(200,200,200);//(100,100,100);
	graphics.DrawText2D(_distX, _distY+y, "GAME_STATE: %d", GAME_STATE);y+=20;
	graphics.DrawText2D(_distX, _distY+y, "AGNT_STATE: %d", agnt.get_AGENT_STATE());y+=dy;
	graphics.DrawText2D(_distX, _distY+y, "PosX: %d", agnt._posX);y+=dy;
	graphics.DrawText2D(_distX, _distY+y, "PosY: %d", agnt._posY);y+=dy;
	graphics.DrawText2D(_distX, _distY+y, "Index X: %d", agnt.pos2index(agnt._posX,0));y+=dy;
	graphics.DrawText2D(_distX, _distY+y, "Index Y: %d", agnt.pos2index(agnt._posY,1));y+=dy;
	graphics.DrawText2D(_distX, _distY+y, "Passos: %d", agnt._passos);y+=dy;
	graphics.DrawText2D(_distX, _distY+y, "Casas Visitadas: %d", fogow.checa_zero());y+=dy;
	graphics.DrawText2D(_distX, _distY+y, "Errmm: %d", numero);y+=dy;
	graphics.DrawText2D(_distX, _distY+y, "Health: %d", agnt._health);y+=dy;
	graphics.DrawText2D(_distX, _distY+y, "Ouro: %d", agnt._gold);y+=dy;
	graphics.DrawText2D(_distX, _distY+y, "Kills: %d", agnt._kills);y+=dy;
	graphics.DrawText2D(_distX, _distY+y, "Elemento: %c",UltimaCasaAcessada);y+=dy;
}
void InterGraf::log_erro(void)
{
	int i,j;
	FILE *arq;
	arq = fopen("Logs\\log_erro.txt","w");
	for(i=0; i < LN; i++)
	{
		for(j=0; j< COL; j++)
		{
			fprintf(arq,"%c", mapa[i][j]);
		}
		fprintf(arq,"\n");
	}
	fprintf(arq,"Game State %d\n", GAME_STATE);
	fprintf(arq,"Agnt State %d\n", agnt.get_AGENT_STATE());
	fprintf(arq,"PosX %d\n", agnt._posX);
	fprintf(arq,"PosY %d\n", agnt._posY);
	fprintf(arq,"IndexX: %d\n", agnt.pos2index(agnt._posX,0));
	fprintf(arq,"IndexY: %d\n", agnt.pos2index(agnt._posY,1));
	fclose(arq);
}

TalkProlog::TalkProlog()
{
	_pace = 0;
	_health = 100;
	_bullets = 0;
	_x = 1;
	_y = 1;
}
int TalkProlog::get_direction(int x, int y)
{
	int value = 0;
	//printf("x antes: %d - x depois: %d\n", _y, y);
	//printf("y antes: %d - y depois: %d\n", _x, x);

	if((_x != x && _y == y) || (_x == x && _y != y))
	{
		if( _x == x)
		{
			//printf("y: %d - _y: %d\n", y, _y);
			if(_y < y)
			{
				value = 1;
			}
			else
			{
				value = 3;
			}
		}
		else
		{
			//printf("x: %d - _x: %d\n", x, _x);
			if(_x > x)
			{
				value = 2;
			}
			else
			{
				value = 4;
			}
		}
	}
	//printf("value: %d\n\n", value);
	return value;
}
bool TalkProlog::le_prolog(void)
{
	int x,y;
	int direc;
	int teleportado = 0;
	int direcProlog;
	x = 0, y = 0;
	if(agnt.get_AGENT_STATE() == 0)
	{
		talk_prolog(&_health, &_bullets, &x, &y, &direcProlog);
		//direc = (rand()%4)+1;//get_direction(x,y);
		direc = get_direction(x,y);
		agnt.checa_casa_atual();
		if(direc != 0)
		{
			if(UltimaCasaAcessada == 'T')
			{
				teleportado = 1;
				x = 6;
				y = 6;
			}
			_x = x;
			_y = y;
			agnt.walk(direc,&agnt._posX,&agnt._posY);
			if(teleportado == 1)
			{
				agnt._posX = agnt.index2pos(x,0);
				agnt._posY = agnt.index2pos(LN-(y+1),1);
			}
			agnt.checa_casa_atual();
			Sleep(50);
		} else 
			agnt._direc = direcProlog;
		return true;
	}
	return false;
}
void TalkProlog::talk_prolog(int* h, int* b, int* x, int* y, int* direc)
{
	int health, bullets, cx,cy;
	health = 100;
	bullets = 5;

	{
		PlTermv NoArg(0);
		PlQuery agir("agir",NoArg);
		agir.next_solution();
	}

	{
		PlTermv playerPosArg(2);
		PlQuery playerPos("playerPos",playerPosArg);
		playerPos.next_solution();
		*x = (int) playerPosArg[0];
		*y = (int) playerPosArg[1];
		//printf("y: %d\n", *x);
		//printf("x: %d\n", *y);
	}

	{
		char* direcao;
		PlTermv playerDirecArg(1);
		PlQuery playerDirec("playerDirec",playerDirecArg);
		playerDirec.next_solution();
		direcao = (char*) playerDirecArg[0];
		if (strcmp(direcao,"cima") == 0)
			*direc = 1;
		else
		if (strcmp(direcao,"esquerda") == 0)
			*direc = 2;
		else
		if (strcmp(direcao,"baixo") == 0)
			*direc = 3;
		else
		if (strcmp(direcao,"direita") == 0)
			*direc = 4;
	}

	/*if(_pace == 0)
	{
		cx = 2;
		cy = 1;
	}
	else if(_pace == 1)
	{
		cx = 3;
		cy = 1;
	}
	else if(_pace == 2)
	{
		cx = 4;
		cy = 1;
	}
	else if(_pace == 3)
	{
		cx = 3;
		cy = 1;
	}
	else if(_pace == 4)
	{
		cx = 3;
		cy = 0;
	}
	else if(_pace == 5)
	{
		cx = 1;
		cy = 1;
	}
	else if(_pace == 6)
	{
		cx = 1;
		cy = 1;
	}
	else
	{
		cx = 1;
		cy = 1;
	}*/
	/**x = cx;
	*y = cy;*/
	_pace++;
}


void ClsHUD::insereHealthHUD(void)
{
	int HUDWidth;
	int HUDHeight;
	int coordX, coordY;
	HUDWidth = 351;
	HUDWidth = 175 * (DFLTSIZE/32);
	HUDHeight = 63 * (DFLTSIZE/32);
	coordX = mapEdgeX + (50*scale);
	coordY = mapEdgeY - HUDHeight;
	graphics.DrawImage2D(coordX,coordY,HUDWidth,HUDHeight,HealthHUD[agnt._health/10]);
}
void ClsHUD::insereCoinHUD(void)
{
	int x;
	int y;
	x = mapEdgeX + (140*scale);
	y = mapEdgeY - (100*scale);
	graphics.DrawImage2D(x,y,150,65, anim.PassaOutros('O', agnt._gold));
}
void ClsHUD::insereDownHUD(void)
{
	int y = mapEdgeY - (170*scale);
	int x = mapEdgeX + (25*scale);
	int w = 225 * scale;
	int h = 75 * scale;
	int dx = 80;
	int dy = 55;
	graphics.DrawImage2D(x, y, w,h, HUDBox);
	graphics.SetColor(100,100,100);
	graphics.SetTextFont("Pokemon R/S",30, false, false, false);
	graphics.DrawText2D(x+dx,y+dy+(dy/2), "Passos: %d", agnt._passos);
	graphics.SetTextFont("Pokemon R/S",30, false, false, false);
	graphics.DrawText2D(x+dx,y+dy-3, "Balas: %d", agnt._bullets);
}
void ClsHUD::insereHUD(void)
{
	insereHealthHUD();
	insereCoinHUD();
	insereDownHUD();
}

ClsHUD::ClsHUD()
{
	scale = DFLTSIZE/32;
}

EndGame::EndGame()
{
	w = 420;
	h = 280;
}
void EndGame::EndHUD(int _time)
{
	if(agnt._health <= 0)
	{
		anim.AnimEndGame(graphics, _time-1,1);
		if(_time > 700)
		{
			//endgame.reset();
		}
	}
	else
	{
		anim.AnimEndGame(graphics, _time-100,0);
		int delay = 50;
		if(_time > 500)
		{
			writeScore(0);
			if(_time > 500 + delay)
			{
				writeScore(1);
				if(_time > 500+ 2*delay)
				{
						writeScore(2);
						if(_time > 500 + 3*delay)
						{
							writeScore(3);
						}
				}
			}
		}
	}
}

void EndGame::writeScore(int elem)
{
	int i;
	int val;
	char valor[10];
	int x = (7*DFLTSIZE + MyZeroX) - 20;
	int y = 592;
	int dx, dy;
	dy = 33 + 11;
	dx = 0;
	if(elem == 0)
	{
		val = agnt._gold;
	}
	else if(elem == 1)
	{
		val = agnt._health;
		dx = -3;

	}
	else if(elem == 2)
	{
		val = agnt._bullets;
		dx = 14;
	}
	else if(elem == 3)
	{
		val = agnt._passos;
		dx = 41;
		dy--;
	}
	sprintf(valor,"%d",val);
	for(i=0; valor[i]!='\0';i++);
	dx += (i-1)*15;
	if(i>1) dx -= (15*(i-1));
	graphics.SetColor(30,30,30);
	graphics.SetTextFont("Pokemon R/S",48, false, false, false);
	graphics.DrawText2D(x+dx,y - (elem*dy),"%s", valor);
}