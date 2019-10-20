#pragma once
#include <stdio.h>
#include "Global.hpp"

class InterGraf 
{
private:
	void insereElementos(char elemento, int coordX, int coordY);
	void insereMarcas(char elemento, int coordX, int coordY);
	void insereParedes(void);
	void insereChao(void);
	void carregaImagens(void);

public:
	InterGraf();
	void iniciaGraficos(int FlSn);
	void executa(void);
	void iniciaMapa(void);
	void exibeMapa(void);
	void novoKBoard2(int *_key);
	void auxilia(void);
	void log_erro(void);
};

class EndGameCls
{
private:
	void load_eg(void);
	void show_result(void);
	void victory(void);
	void defeat(void);
public:
	void checaJogo(void);
	void reset(void);
};

class TalkProlog
{
private:
	int _pace;
	int _health;
	int _gold;
	int _bullets;
	int _x;
	int _y;
public:
	TalkProlog();
	int get_direction(int x, int y);
	bool le_prolog(void);
	void talk_prolog(int *h, int *b, int *x, int *y, int* direc);
};

class ClsHUD
{
private:
	int scale;
public:
	ClsHUD();
	void insereHUD(void);
	void insereHealthHUD(void);
	void insereCoinHUD(void);
	void insereDownHUD(void);
};

class EndGame{
private:
	int loop;
	int w;
	int h;
public:
	EndGame();
	void EndHUD(int _time);
	void writeScore(int elem);
	void YouDied(void);
};