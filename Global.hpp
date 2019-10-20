#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "Graphics.h"
#include <time.h>

#define LN 12
#define COL 12
 
#define FS 0
#define DFLTSIZE 64
#define MOVSPD 64
#define CasaVazia '.'

extern int ModuloErrado;
extern int SW;
extern int SH;
extern int posInicialX;
extern int posInicialY;
extern int MyZeroX;
extern int MyZeroY;
extern int mapEdgeX;
extern int mapEdgeY;
extern char UltimaCasaAcessada;
extern char CasaAtual;
extern int NLoop;
extern int GAME_STATE;
extern int PRE_LOAD_STATE;
extern int MESSAGE_STATE;

void le_mapa();
void passa_mapa(char mapa[LN][COL]);
void deleta_ponto(int x, int y);
void passa_backup(char mapa[LN][COL]);

int in_range(int time, int min, int max);
int get_range(int duration, int nframes);
int in_range_delayed(int time, int prop, int acclrt);

void adjust_scale(float scale, int tile_size, int* x, int *y, int *size);

/*
#define SW = 1400;
#define SH = 1000;
#define posInicialX = 1;
#define posInicialY = 1;
#define MyZeroX = 10;
#define MyZeroY = 100 - (MyZeroX - 2);
#define mapEdgeX = ((LN+1)*DFLTSIZE) + MyZeroX;
#define mapEdgeY = ((LN+1)*DFLTSIZE) + MyZeroY;
#define UltimaCasaAcessada = 0;
#define CasaAtual = 0;
#define NLoop = 0;
*/
/*
extern int SW = 1400;
extern int SH = 1000;
extern int posInicialX = 1;
extern int posInicialY = 1;
extern int MyZeroX = 10;
extern int MyZeroY = 100 - (MyZeroX - 2);
extern int mapEdgeX = ((LN+1)*DFLTSIZE) + MyZeroX;
extern int mapEdgeY = ((LN+1)*DFLTSIZE) + MyZeroY;
extern char UltimaCasaAcessada = 0;
extern char CasaAtual = 0;
extern int NLoop = 0;
*/
/*
int SW;
int SH;
int posInicialX;
int posInicialY;
int MyZeroX;
int MyZeroY;
int mapEdgeX;
int mapEdgeY;
char UltimaCasaAcessada;
int NLoop;*/
/*
int SW = 1400;
int SH = 1000;
int posInicialX = 1;
int posInicialY = 1;
int MyZeroX = 10;
int MyZeroY = 100 - (MyZeroX - 2);
int mapEdgeX = ((LN+1)*DFLTSIZE) + MyZeroX;
int mapEdgeY = ((LN+1)*DFLTSIZE) + MyZeroY;
char UltimaCasaAcessada = 0;
char CasaAtual = 0;
int NLoop = 0;
*/