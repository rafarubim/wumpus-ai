#pragma once
#include "Graphics.h"
#include <stdio.h>

class Animation{
private:
	Image Vortex[17];
	Image Spark[11];
	Image BEnemy[21];
	Image SEnemy[2];
	Image Telep[4];
	Image Coin[6];
	Image Gust[5];
	Image Agente_U[4];
	Image Agente_D[4];
	Image Agente_L[4];
	Image Agente_R[4];
	Image SmokeScreen[10];
	Image EndGame;
	Image Dead;
	Image CoinHUD[4];
	int n_frames;
public:
	void load_animations(char elemento);
	void load_em_all(void);
	void draw_animations(Graphics& graphics, char elemento, int frame,int posX, int posY);
	void test_animations(Graphics& graphics, int frame);
	Image AnimAgente(int direc, int time);
	Image PassaElementos(char elemento, int time);
	Image PassaOutros(char elemento, int time);
	void curtaincalls(Graphics& graph, int _time, int scX, int scY , bool closure);
	void Animation::AnimEndGame(Graphics& graph, int _time, int boo);
};