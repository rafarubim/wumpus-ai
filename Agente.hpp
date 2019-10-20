#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "Global.hpp"
#include "Graphics.h"
#include "Animate.hpp"

class AgenteCls
{
private:
	int _durwalk;
	int AGENT_STATE;
	int _spdX;
	int _spdY;
	int _dist;
	int _walked;
	int _frame;
	int _nframes;
	int _splitposX;
	int _splitposY;
	int RDMpos(int x);
	void atualiza_agente(int indexX, int indexY);
	void log_erro(void);
public:
	bool _checkgetitem;
	int _getgold;
	int _getpu;
	int get_AGENT_STATE(void);
	int pos2index(int pos, int axis);
	int index2pos(int index, int axis);
	AgenteCls();
	int _passos;
	int _posX;
	int _posY;
	int _direc;
	float _scale;
	int _spritesize;
	int _state;
	int _health;
	int _gold;
	int _bullets;
	int _kills;
	void drawstatic(Graphics& agent_pkm, Animation& anim);
	char checa_casa_atual(void);
	int checkWin(void);
	void load(void);
	void walk(int direc, int *x, int *y);
	void inicia_mvmt(int direc,int dist);
	void finaliza_mvmt(void);
	void drawmvmt(Graphics& agent_pkm, int timer, Animation& anim);
	int metade_anim(void);
	void valid_pos_ind(int index);
};
