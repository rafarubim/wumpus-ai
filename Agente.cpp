#include <string.h>
#include "Agente.hpp"
#include <stdlib.h>

static Image Agent;

void AgenteCls::drawstatic(Graphics& agent_pkm, Animation& anim)
{
	agent_pkm.DrawImage2D(_posX,_posY,_spritesize,_spritesize,anim.AnimAgente(_direc,0));
}
AgenteCls::AgenteCls()
{
	_scale = 0.75f;
	_posX = (posInicialX+1) * DFLTSIZE + MyZeroX;
	_posY = (posInicialY+1) * DFLTSIZE + MyZeroY;
	_spritesize = DFLTSIZE;
	adjust_scale(_scale,DFLTSIZE, &_posX, &_posY, &_spritesize);
	_state = -1;
	_health = 100;
	_gold = 0;
	_kills = 0;
	_spdX = 0;
	_spdY = 0;
	_durwalk = 0;
	AGENT_STATE = 0;
	_direc = 3;
	_dist = 0;
	_walked = 0;
	_frame = 0;
	_nframes = 0;
	_splitposY=0;
	_splitposX=0;
	_passos = 0;
	_getgold = 0;
	_getpu = 0;
	_bullets = 0;
	_checkgetitem = false;
	//passa_mapa(a_map);
}
char AgenteCls::checa_casa_atual(void)
{
	int indexX;
	int indexY;
	indexY = pos2index(_posX,0);
	indexX = pos2index(_posY,1);
	if(GAME_STATE == 0)
	{
		if(AGENT_STATE == 0)
		{
			atualiza_agente(indexX,indexY);
		}
	}
	return UltimaCasaAcessada;
}
int AgenteCls::pos2index(int pos, int axis)
{
	if(axis == 0) return ((pos - MyZeroX)/DFLTSIZE)-1;
	else if(axis == 1) return ((pos - MyZeroY)/DFLTSIZE)-1;
	return -1;
}
int AgenteCls::checkWin(void)
{
	if(_gold == 3 && UltimaCasaAcessada == 'E') return 1;
	return 0;
}
void AgenteCls::load(void)
{
	Agent.LoadPNGImage("Sprites\\Agente\\AgentSD.png");
}
void AgenteCls::inicia_mvmt(int direc,int dist)
{
	//printf("INICIANDO MOVIMENTO!!\n");
	_passos++;
	GAME_STATE = 0;
	AGENT_STATE = 1; //Agente em movimento
	_direc = direc;
	_dist = DFLTSIZE;
	_walked = 0;
	_frame = 0;
	_nframes = 6;
	_spdX = (_dist / _nframes);
	_spdY = (_dist / _nframes);
	_splitposX = _posX;
	_splitposY = _posY;
	if(direc == 1)//Up
	{
		_spdX = 0;
		_splitposY = _posY;
	}
	else if(direc == 2)//Left
	{
		_spdY = 0;
		_spdX *= (-1);
		_splitposX = _posX;
	}
	else if(direc == 3)//Down
	{
		_spdY *= (-1);
		_spdX = 0;
		_splitposY = _posY;
	}
	else if(direc == 4)//Right
	{
		_spdY = 0;
		_splitposX = _posX;
	}
}
void AgenteCls::finaliza_mvmt(void)
{
	AGENT_STATE = 0;
	_dist = 0;
	_spdX = 0;
	_spdY = 0;
	_splitposY=0;
	_splitposX=0;
	//printf("FINALIZANDO MOVIMENTO!!\n");
}
void AgenteCls::walk(int direc, int *x, int *y)
{
	int dist = DFLTSIZE;
	int indexX, indexY;
	indexX = pos2index(_posX,0);
	indexY = pos2index(_posY,1);
	if(direc == 1)//UP
	{
		if(indexY < LN-1)
		{
			inicia_mvmt(direc,dist);
			*y += dist;
		}
	}
	else if(direc == 2) //Left
	{
		if(indexX > 0)
		{
			inicia_mvmt(direc,dist);
			*x -= dist;
		}
	}
	else if(direc == 3) //Down
	{
		if(indexY > 0)
		{
			inicia_mvmt(direc,dist);
			*y -= dist;
		}
	}
	else if(direc == 4) //Right
	{
		if(indexX < COL-1)
		{
			inicia_mvmt(direc,dist);
			*x += dist;
		}
	}
}
void AgenteCls::drawmvmt(Graphics& agent_pkm, int timer, Animation& anim)
{
	int time;
	int posX, posY;
	time = timer;
	_frame++;
	if(_direc == 1 || _direc == 3) //vertical
	{
		_splitposY += _spdY;
	}
	else if(_direc == 2 || _direc == 4) //horizontal
	{
		_splitposX += _spdX;
	}
	posX = _splitposX;
	posY = _splitposY;
	agent_pkm.DrawImage2D(posX,posY,_spritesize,_spritesize,anim.AnimAgente(_direc,_frame));
	if(_frame == _nframes)
	{
		finaliza_mvmt();
	}
}
int AgenteCls::get_AGENT_STATE(void)
{
	return AGENT_STATE;
}
int AgenteCls::metade_anim(void)
{
	if( (_frame*2) == _nframes) return 1;
	return 0;
}
int AgenteCls::RDMpos(int x)
{
	int index;
	int desloc = (x == 0)?MyZeroX:MyZeroY;
	index = (GetTickCount()%(LN));
	printf("Index %d e %d\n",index, x);
	valid_pos_ind(index);
	return ((index*DFLTSIZE)+ desloc);
}
void AgenteCls::atualiza_agente(int indexX, int indexY)
{
	int aux;
	char a_map[LN][COL];
	passa_mapa(a_map);
	aux = DFLTSIZE;
	if(a_map[indexX][indexY] != '.' && a_map[indexX][indexY] != 'E')
	{
		GAME_STATE = 1;
	}
	UltimaCasaAcessada = a_map[indexX][indexY];
	if(UltimaCasaAcessada == 'P')
	{
		_health = 0;
	}
	else if(UltimaCasaAcessada == 'D')
	{
		//_health -= 20;
	}
	else if(UltimaCasaAcessada == 'O')
	{
		_gold++;
		deleta_ponto(indexX,indexY);
		//a_map[indexX][indexY] = CasaVazia;
	}
	else if(UltimaCasaAcessada == 'U')
	{
		int hp_up = 30;
		if(_health < 100)
		{
			_health += hp_up;
			if(_health > 100)_health = 100;
			deleta_ponto(indexX,indexY);
		}
	}
	else if(a_map[indexX][indexY] == 'd')
	{
		//_health -= 10;
	}
	else if(UltimaCasaAcessada == 'T')
	{
		/*
		_posX = RDMpos(0);
		valid_pos_ind(pos2index(_posY,1));
		//printf("C: Agnt\tF: att_agnt\tPosX:\n");
		_posY = RDMpos(1);
		valid_pos_ind(pos2index(_posX,0));
		//printf("C: Agnt\tF: att_agnt\tPosY:\n");
		adjust_scale(_scale, DFLTSIZE, &_posX, &_posY, &aux);
		//printf("C: Agnt\tF: att_agnt\tPosZ:\n");*/
	}
	else if(UltimaCasaAcessada == 'E')
	{
		if(checkWin() == 1)
		{
			GAME_STATE = -1;
		}
	}
	if(_health <= 0)
	{
		GAME_STATE = -1;
	}
}
void AgenteCls::valid_pos_ind(int index)
{
	if(index <= (-1) || index > 11)
	{
		printf("AgenteCls::Valid_pos_ind\nIndex Invalido no valor de: %d\n", index);
		log_erro();
		exit(1);
	}
}
void AgenteCls::log_erro(void)
{
	FILE *arq;
	char hora[10];
	char minuto[10];
	char ponto[2] = "_";
	char path[200] = "Logs\\log_erro_AGNT_";
	char sufix[6] = ".txt";
	time_t now = time(0);
	time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
	sprintf(hora,"%d", timeinfo->tm_hour);
	sprintf(minuto,"%d", timeinfo->tm_min);
	strcat(path,hora);
	strcat(path,ponto);
	strcat(path,minuto);
	strcat(path,sufix);
	arq = fopen(path,"w");
	fprintf(arq,"Game State %d\n", GAME_STATE);
	fprintf(arq,"Agnt State %d\n", AGENT_STATE);
	fprintf(arq,"PosX %d\n", _posX);
	fprintf(arq,"PosY %d\n", _posY);
	fprintf(arq,"IndexX: %d\n", pos2index(_posX,0));
	fprintf(arq,"IndexY: %d\n", pos2index(_posY,1));
	fprintf(arq,"Elemento: %c\n", UltimaCasaAcessada);
	fprintf(arq,"SPDx %d\n", _spdX);
	fprintf(arq,"SPDy %d\n", _spdY);
	fprintf(arq,"Direct: %d\n", _direc);
	fprintf(arq,"Dist: %d\n", _dist);
	fprintf(arq,"Walked: %d\n", _walked);
	fprintf(arq,"Frame: %d\n", _frame);
	fprintf(arq,"SplitPosX: %d\n", _splitposX);
	fprintf(arq,"SplitPosY: %d\n", _splitposY);
	fclose(arq);
}

int AgenteCls::index2pos(int index, int axis)
{
	int value;
	if(axis == 0)
	{
		value = ((index+1)*DFLTSIZE)+MyZeroX;
	}
	else
	{
		value = ((index+1)*DFLTSIZE)+MyZeroY;
	}
	return value + 8;
}