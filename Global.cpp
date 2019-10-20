#include "Global.hpp"
#include <SWI-cpp.h>
#include <string.h>

int SW = 1400;
int SH = 1000;
int posInicialX = 0;
int posInicialY = 0;
int MyZeroX = 10;
int MyZeroY = 100 - (MyZeroX - 2);
int mapEdgeX = ((LN+1)*DFLTSIZE) + MyZeroX;
int mapEdgeY = ((LN+1)*DFLTSIZE) + MyZeroY;
char UltimaCasaAcessada = 0;
char CasaAtual = 0;
int NLoop = 0;
int GAME_STATE = 0;
int MESSAGE_STATE = 0;
static char mapa_global[LN][COL];
static char mapa_global_backup[LN][COL];

void retornaEvento(char ident, char* str) {
	switch(ident) {
	case 'P':
		strcpy(str,"buraco");		break;
	case 'T':
		strcpy(str,"teleporter");	break;
	case 'U':
		strcpy(str,"powerUp");		break;
	case 'O':
		strcpy(str,"ouro");			break;
	case 'D':
		strcpy(str,"wumpus2");		break;
	case 'd':
		strcpy(str,"wumpus1");		break;
	case 'E':	case '.':
		strcpy(str,"vazio");		break;
	default:
		printf("Desiste desse curso e vai fazer comunicacao\n\n");	exit(1);
	}
}

void criaStringSala(char* str, int i, int j, char c) {
	strcpy(str,"sala(");
	char temp[15];
	sprintf(temp,"%d", i+1);
	strcat(str,temp);
	strcat(str,",");
	sprintf(temp,"%d", j+1);
	strcat(str,temp);
	strcat(str,",");
	retornaEvento(c,temp);
	strcat(str,temp);
	strcat(str,")");
}

void le_mapa(void)
{
	int i = 0;
	int j = 0;
	FILE *arq;
	arq = fopen("mapa.txt", "r");
	for(i=0; i<LN; i++)
	{
		for(j=0; j<COL; j++)
		{
			fscanf(arq," %c", &mapa_global[LN-(i+1)][j]);
			//mapa_global[i][j] = '.';
			//if(i==j) mapa_global[i][j] = 'T';
			//mapa_global[i][j] = '.';
			mapa_global_backup[LN-(i+1)][j]=mapa_global[LN-(i+1)][j];

			
		}
	}

	for (i = 0; i < LN; i++)
		for (j = 0; j < COL; j++) {
				char minhaLindaString[30];
				PlTermv assertArg(1);
				criaStringSala(minhaLindaString,i,j,mapa_global[i][j]);
				assertArg[0] = PlCompound(minhaLindaString);
				//printf("%s\n", minhaLindaString);
				PlQuery assert("assert",assertArg);
				assert.next_solution();
			}

	{
		PlTermv salaArg(3);
		PlQuery sala("sala",salaArg);
		while(sala.next_solution()) {
			printf("sala(%d,%d,%s)\n",(int)salaArg[0],(int)salaArg[1],(char*)salaArg[2]);
		}
	}

	{
		PlTermv NoArg(0);
		PlQuery iniciar("iniciar",NoArg);
		iniciar.next_solution();
	}

	mapa_global[LN-(j+1)][i] = 'E';
	fclose(arq);
}
void passa_mapa(char matriz[LN][COL])
{
	int i = 0;
	int j = 0;
	for(i=0; i<LN; i++)
	{
		for(j=0; j<LN; j++)
		{
			matriz[i][j] = mapa_global[i][j];
		}
	}
}
void deleta_ponto(int x, int y)
{
	mapa_global[x][y] = CasaVazia;
}
void passa_backup(char matriz[LN][COL])
{
	int i = 0;
	int j = 0;
	for(i=0; i<LN; i++)
	{
		for(j=0; j<LN; j++)
		{
			matriz[i][j] = mapa_global_backup[i][j];
		}
	}
}
int in_range(int time, int min, int max)
{
	if(time >= min && time <= max) 
	{
		return 1;
	}
	return 0;
}
int get_range(int duration, int nframes)
{
	int value;
	value = (duration/nframes);
	return value;
}
void adjust_scale(float scale, int tile_size, int* x, int *y, int *size)
{
	float posfixer;
	posfixer = ( (1.f - scale)/2.f )* (float)tile_size;
	*x = *x + (int)posfixer;
	*y = *y + (int)posfixer;
	*size = (int)((float)(*size) * scale);
}
int in_range_delayed(int _time, int prop, int acclrt)
{
	int time;
	time = (_time / acclrt)%100;
	if(time <= (prop/acclrt)) 
	{
		return 1;
	}
	return 0;
}


