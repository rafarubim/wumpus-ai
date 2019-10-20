#include "Animate.hpp"
#include "Global.hpp"
#include "string.h"
#include <stdio.h>
#include <Windows.h>

void Animation::load_animations(char elemento)
{
	char extensao[30] = ".png";
	char ext2[30] = "_delay-0.02s.png";
	char number[30];
	char paths[200][300];
	int i;
	if(elemento == 'U')
	{
		for(i=0; i<17;i++)
		{
			sprintf(number, "%d", i+1);
			strcpy(paths[i],"Sprites\\PowerUp\\PU");
			strcat(paths[i],number);
			strcat(paths[i],extensao);
			Vortex[i].LoadPNGImage(paths[i]);
		}
		for(i=0; i<11;i++)
		{
			sprintf(number, "%d", i+1);
			strcpy(paths[i],"Sprites\\Spark\\Spark");
			strcat(paths[i],number);
			strcat(paths[i],extensao);
			Spark[i].LoadPNGImage(paths[i]);
		}
	}
	else if(elemento == 'D')
	{
		for(i=0; i<21;i++)
		{
			sprintf(number, "%d", i);
			strcpy(paths[i],"Sprites\\Wut\\");
			strcat(paths[i],number);
			strcat(paths[i],extensao);
			BEnemy[i].LoadPNGImage(paths[i]);
		}
	}
	else if(elemento == 'd')
	{
		SEnemy[0].LoadPNGImage("Sprites\\Inimigos\\InimigoPequeno.png");
		SEnemy[1].LoadPNGImage("Sprites\\Inimigos\\InimigoPequeno2.png");
	}
	else if(elemento == 'T')
	{
		Telep[0].LoadPNGImage("Sprites\\Teleport\\Teleport1.png");
		Telep[1].LoadPNGImage("Sprites\\Teleport\\Teleport2.png");
		Telep[2].LoadPNGImage("Sprites\\Teleport\\Teleport3.png");
		Telep[3].LoadPNGImage("Sprites\\Teleport\\Teleport4.png");
	}
	else if(elemento == 'B')
	{
		for(i=0; i<5;i++)
		{
			sprintf(number, "%d", i+1);
			strcpy(paths[i],"Sprites\\Gust\\Gust");
			strcat(paths[i],number);
			strcat(paths[i],extensao);
			Gust[i].LoadPNGImage(paths[i]);
		}
	}
	else if(elemento == 'O')
	{
		for(i=0; i<11;i++)
		{
			sprintf(number, "%d", i+1);
			strcpy(paths[i],"Sprites\\Ouro\\Coin");
			strcat(paths[i],number);
			strcat(paths[i],extensao);
			Coin[i].LoadPNGImage(paths[i]);
		}
		for(i=0; i<4;i++)
		{
			sprintf(number, "%d", i);
			strcpy(paths[i],"Sprites\\Ouro\\hud");
			strcat(paths[i],number);
			strcat(paths[i],extensao);
			CoinHUD[i].LoadPNGImage(paths[i]);
		}
	}
	else if(elemento == 'A')
	{
		for(i=0; i<4;i++)
		{
			sprintf(number, "%d", i+1);
			strcpy(paths[i],"Sprites\\Agente\\AgenteD");
			strcat(paths[i],number);
			strcat(paths[i],extensao);
			Agente_D[i].LoadPNGImage(paths[i]);

			sprintf(number, "%d",i+1);
			strcpy(paths[i],"Sprites\\Agente\\AgenteU");
			strcat(paths[i],number);
			strcat(paths[i],extensao);
			Agente_U[i].LoadPNGImage(paths[i]);

			sprintf(number, "%d", i+1);
			strcpy(paths[i],"Sprites\\Agente\\AgenteL");
			strcat(paths[i],number);
			strcat(paths[i],extensao);
			Agente_L[i].LoadPNGImage(paths[i]);

			sprintf(number, "%d",i+1);
			strcpy(paths[i],"Sprites\\Agente\\AgenteR");
			strcat(paths[i],number);
			strcat(paths[i],extensao);
			Agente_R[i].LoadPNGImage(paths[i]);
		}
	}
	else if(elemento == 'L')
	{
		for(i=0; i<10; i++)
		{
			sprintf(number, "%d", i+1);
			strcpy(paths[i],"Sprites\\PreLoad\\SS");
			strcat(paths[i],number);
			strcat(paths[i],extensao);
			SmokeScreen[i].LoadPNGImage(paths[i]);
		}
		EndGame.LoadPNGImage("Sprites\\Preload\\EndGame.png");
		Dead.LoadPNGImage("Sprites\\Preload\\Dead.png");
	}

}
void Animation::draw_animations(Graphics &graphics, char elemento, int frame, int X, int Y)
{
	int index;
	int size;
	int posX, posY;
	float scale;
	float global_scale = 1.f;
	posX = (int)(	(float)(X) * global_scale );
	posY = (int)(	(float)(Y) * global_scale );
	if(elemento == 'U')
	{
		scale = 1.f * global_scale;
		size = (int)((float)DFLTSIZE * scale);
		index = (frame%11);
		graphics.DrawImage2D(posX, posY, size,size, Spark[index]);
	}
	if(elemento == 'S')
	{
		scale = 1.f * global_scale;
		size = (int)((float)DFLTSIZE * scale);
		index = (frame%17);
		graphics.DrawImage2D(posX, posY, size,size, Vortex[index]);
	}
	else if(elemento == 'D')
	{
		scale = 1.f * global_scale;
		size = (int)((float)DFLTSIZE * scale);
		index = (frame%2);
		graphics.DrawImage2D(posX, posY, size,size, BEnemy[index]);
	}
	else if(elemento == 'd')
	{
		scale = 1.f * global_scale;
		size = (int)((float)DFLTSIZE * scale);
		index = (frame%2);
		graphics.DrawImage2D(posX, posY, size,size, SEnemy[index]);
	}
	else if(elemento == 'T')
	{
		scale = 1.f * global_scale;
		size = (int)((float)DFLTSIZE * scale);
		index = (frame%4);
		graphics.DrawImage2D(posX, posY, size,size, Telep[index]);
	}
	else if(elemento == 'O')
	{
		scale = 0.5f * global_scale;
		size = (int)((float)DFLTSIZE * scale);
		index = (frame%6);
		graphics.DrawImage2D(posX, posY, size,size, Coin[index]);
	}
	else if(elemento == '1')
	{
		scale = 1.f * global_scale;
		size = (int)((float)DFLTSIZE * scale);
		index = (frame%4);
		graphics.DrawImage2D(posX, posY, size,size, Agente_U[index]);
	}
	else if(elemento == '2')
	{
		scale = 1.f * global_scale;
		size = (int)((float)DFLTSIZE * scale);
		index = (frame%4);
		graphics.DrawImage2D(posX, posY, size,size, Agente_L[index]);
	}
	else if(elemento == '3')
	{
		scale = 1.f * global_scale;
		size = (int)((float)DFLTSIZE * scale);
		index = (frame%4);
		graphics.DrawImage2D(posX, posY, size,size, Agente_D[index]);
	}
	else if(elemento == '4')
	{
		scale = 1.f * global_scale;
		size = (int)((float)DFLTSIZE * scale);
		index = (frame%4);
		graphics.DrawImage2D(posX, posY, size,size, Agente_R[index]);
	}
}
void Animation::load_em_all(void)
{
	load_animations('U');
	load_animations('D');
	load_animations('d');
	load_animations('T');
	load_animations('O');
	load_animations('A');
	load_animations('L');
	//load_animations('B');
}
void Animation::test_animations(Graphics& graphics, int frame)
{
	int i;
	char teste[100] = "UDdTO1234S";
	int ntestes = strlen(teste);
	for(i=0; i<ntestes; i++)
	draw_animations(graphics, teste[i],frame, i*64, 0);
}
Image Animation::AnimAgente(int direc, int time)
{
	int frame = time %4;
	if(direc == 1)//Up
	{
		return Agente_U[frame];
	}
	else if(direc == 2)//Left
	{
		return Agente_L[frame];
	}
	else if(direc == 3)//Down
	{
		return Agente_D[frame];
	}
	else// if(direc == 4)//Right
	{
		return Agente_R[frame];
	}
}
Image Animation::PassaElementos(char elemento, int time)
{
	int frame;
	int nframes;
	int delay = 3;
	int acclrt = 2;
	int tiempo;
	if(elemento == 'U')
	{
		nframes = 11;
		frame = (time/delay)%nframes;
		return Spark[frame];
	}
	else if(elemento == 'D')
	{
		tiempo = time%(4*delay*15);
		nframes = 20;
		frame = ((time/delay)%(nframes));
		if(tiempo >= (20*delay)) frame = 20;
		return BEnemy[frame];
	}
	else if(elemento == 'd')
	{
		frame = in_range_delayed(time,80,acclrt);
		return SEnemy[frame];
	}
	else if(elemento == 'T')
	{
		nframes = 4;
		frame = (time/delay)%nframes;
		return Telep[frame];
	}
	else if(elemento == 'O')
	{
		nframes = 6;
		frame = (time/delay)%nframes;
		return Coin[frame];
	}
	else
	{
		printf("Animation::PassaElementos recebendo 'elemento' errado\n");
		printf("Elemento recebido: %c\n", elemento);
		exit(1);
	}
}
Image Animation::PassaOutros(char elemento, int _time)
{
	int frame;
	int nframes;
	if(elemento == 'S')
	{
		nframes = 17;
		frame = _time%(nframes);
		return Vortex[frame];
	}
	else if(elemento == 'B')
	{
		nframes = 5;
		frame = _time%(nframes);
		return Gust[frame];
	}
	else if(elemento == 'L')
	{
		nframes = 10;
		frame = _time%nframes;
		return SmokeScreen[frame];
	}
	else if(elemento == 'O')
	{
		return CoinHUD[_time];
	}
	exit(0);
}

void Animation::curtaincalls(Graphics& graph, int _time, int scX, int scY , bool closure)
{
	if(_time%10 < 0 || _time%10 > 10)
	{
		printf("Animation: curtainCalls recebendo [_time] invalido\nRecebendo: %d\n",_time);
		exit(1);
	}
	int frame;
	int wait = 50;
	if(closure == true)
	{
		frame = 10 - (_time%10);
	}
	else
	{
		frame = (_time%10);
	}
	graph.DrawImage2D(0,0,scX,scY,PassaOutros('L',frame));
	Sleep(wait);
}

void Animation::AnimEndGame(Graphics& graph, int _time, int boo)
{
	int spdUp = 2;
	int t = spdUp * _time;
	int h = 420;
	int w = 280;
	int maxH = ((7*DFLTSIZE + MyZeroY) - h/2);
	int distY = t - h;
	int distX = (7*DFLTSIZE + MyZeroX) - w/2;
	if(distY > maxH) distY = maxH;
	if(boo == 0)graph.DrawImage2D(distX, distY, w, h, EndGame);
	else if(boo == 1) graph.DrawImage2D(distX, distY, w, h, Dead);
}