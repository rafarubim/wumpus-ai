#include "FogOfWar.hpp"

FOW_cls::FOW_cls()
{
	int i, j;
	int x,y;
	for(i=0; i<LN; i++)
	{
		for(j=0; j<COL;j++)
		{
			_fow[i][j] = 0;
		}
	}
	y = posInicialX;
	x = LN-posInicialY-1;
	_fow[x][y] = 2;
}
void FOW_cls::load(void)
{
	FOW1.LoadPNGImage("Sprites\\FOW\\FOW.png");
	FOW2.LoadPNGImage("Sprites\\FOW\\FOW2.png");
}

void FOW_cls::atualiza_fow(AgenteCls& agnt)
{
	if(agnt.get_AGENT_STATE() == 0)
	{
		int i,j;
		i = 0, j = 0;
		getLastVisitedTile(&i,&j);
		_fow[i][j] = 1;
	
		getCurrentTile(agnt,&i,&j);
		_fow[i][j] = 2;
	}
	else if(agnt.get_AGENT_STATE() == 1)
	{
		if(agnt.metade_anim() == 1)
		{
			int i,j;
			i = 0, j = 0;
			getLastVisitedTile(&i,&j);
			_fow[i][j] = 1;
	
			getCurrentTile(agnt,&i,&j);
			_fow[i][j] = 2;
		}
	}
}
void FOW_cls::imprime_fow(Graphics& fog)
{
	int i,j;
	int coordX;
	int coordY;
	load();
	for(i=0; i<LN; i++)
	{
		for(j=0; j<COL;j++)
		{
			coordX = ((j+1)*DFLTSIZE)+MyZeroX;
			coordY = ((LN - i)*DFLTSIZE)+MyZeroY;
			if(_fow[i][j] == 0)
			{
				fog.DrawImage2D(coordX, coordY, DFLTSIZE,DFLTSIZE, FOW1);
			}
			else if(_fow[i][j] == 1)
			{

				fog.DrawImage2D(coordX, coordY, DFLTSIZE,DFLTSIZE, FOW2);
			}
		}
	}
}
void FOW_cls::getLastVisitedTile(int *i, int *j)
{
	int iaux, jaux;
	for(iaux=0; iaux<LN; iaux++)
	{
		for(jaux=0; jaux<COL; jaux++)
		{
			if(_fow[iaux][jaux] == 2)
			{
				*i = iaux;
				*j = jaux;
				return;
			}
		}
	}
	//exibe_fow();
	printf("Errmmm.... Nao foi achada a ultima casa visitada\n");
	exit(1);
}
void FOW_cls::getCurrentTile(AgenteCls& agnt, int *i, int *j)
{
	int cmpX, cmpY;
	cmpX = agnt.pos2index(agnt._posX,0);
	cmpY = agnt.pos2index(agnt._posY,1);
	*j = cmpX;
	*i = LN-cmpY-1;
	//printf("%d - %d\n", cmpX, cmpY);
	if(cmpX < 0 || cmpY < 0)
	{
		/*exibe_fow();
		printf("Errmmm.... Houston, perdemos o jogador!!\n");
		printf("PosX: %d\"tPosY: %d\n", agnt._posX, agnt._posY);
		printf("Ix: %d\tIy: %d\n", *j, *i);
		log_erro(agnt);
		exit(1);*/
		GAME_STATE = -1;
	}
	return;
}
void FOW_cls::exibe_fow(void)
{
	int i,j;
	for(i=0; i<LN; i++)
	{
		for(j=0; j<COL; j++)
		{
			printf("[ %d ]", _fow[i][j]);
		}
		printf("\n");
	}
}

void FOW_cls::log_erro(AgenteCls& agnt)
{
	int i,j;
	FILE *arq;
	char hora[10];
	char path[200] = "Logs\\log_erro_FOW_";
	char sufix[6] = ".txt";
	time_t now = time(0);
	time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
	printf("%d - %d - %d\n",timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	sprintf(hora, "%d%d", timeinfo->tm_hour, timeinfo->tm_min);
	strcat(path,hora);
//	strcat(path,minuto);
	strcat(path,sufix);
	arq = fopen(path,"w");
	for(i=0; i < LN; i++)
	{
		for(j=0; j< COL; j++)
		{
			fprintf(arq,"%c", _fow[i][j]);
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

int FOW_cls::checa_zero(void)
{
	int cont = -2;
	int i,j;
	for(i=0; i<LN; i++)
	{
		for(j=0; j<COL; j++)
		{
			cont += _fow[i][j];
		}
	}
	return cont;
}