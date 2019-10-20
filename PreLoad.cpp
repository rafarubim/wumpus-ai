#include "PreLoad.hpp"
#include "Windows.h"
#include "Global.hpp"

PLCls::PLCls()
{
	_state = 0;
	_cicle = -150;
	_nframes = 0;
	_scsize = 850;
}

void PLCls::load(void)
{
	Water.LoadPNGImage("Sprites\\PreLoad\\Water.png");
	BG.LoadPNGImage("Sprites\\PreLoad\\BG.png");
	Borda.LoadPNGImage("Sprites\\PreLoad\\Borda.png");
	Treco.LoadPNGImage("Sprites\\PreLoad\\Treco.png");
	MSBOX.LoadPNGImage("Sprites\\MessageBox\\BigMB.png");
	if(DFLTSIZE == 64)
	{
		fakeBG.LoadPNGImage("Sprites\\PreLoad\\FakeBG.png");
	}
	else
	{
		fakeBG.LoadPNGImage("Sprites\\PreLoad\\Water.png");
	}
}

void PLCls::draw_bg(Graphics& graph)
{
	if(_cicle > 0)
	{
		int tilesize = 16;
		int x,y;
		int time;
		time = _cicle;
		x = ((time/10)%8)+(-(tilesize%4));
		y = x;
		graph.DrawImage2D(x+1,y+1,840,840,Water);
		graph.DrawImage2D(0,30,_scsize,_scsize,BG);
	}
}

void PLCls::loop_pre_load(Graphics& graph,Animation& anim)
{
	if(_cicle < 0)
	{
		write_pretext(graph,_cicle+150);
	}
	else if(_cicle >= 380)
	{
		PRE_LOAD_STATE = 0;
	}
	else
	{
		draw_bg(graph);
		if(_cicle > 0) draw_plyr(graph,anim);
		graph.DrawImage2D(-1,-1,_scsize,_scsize,Borda);
		if(_cicle < 10 || _cicle > 360)
		{
			draw_smoke(graph,anim);
		}
	}
	if(_cicle > 350)
	{
		Sleep(100);
	}
	if(_cicle == 370)
	{
		graph.SetColor(0,0,0);
		graph.FillRectangle2D(0,0,SW,SH);
	}
	if(_cicle > 370 && _cicle < 380)
	{
		graph.DrawImage2D(0,0,SW,SH,fakeBG);
		anim.curtaincalls(graph, _cicle, SW,SH, true);
	}
	_cicle++;
}


void PLCls::draw_plyr(Graphics& graph, Animation& anim)
{
	int posX;
	int posY;
	if(_cicle > 20)
	{
		int frame = (_cicle / 10);
		posX = 400;
		posY = -50 + (_nframes * 2);
		if(posY > 550) posY = 550;
		graph.DrawImage2D(posX,posY,48,48,anim.AnimAgente(1,frame));
		_nframes++;
	}
	graph.DrawImage2D(0,30,_scsize,_scsize,Treco);
}

void PLCls::draw_smoke(Graphics& graph, Animation& anim)
{
	bool closure;
	int valid = 0;
	int x,y;
	x = _scsize;
	y = _scsize;
	if(_cicle < 10)
	{
		valid = 1;
		closure = true;
	}
	else if(_cicle > 350)
	{
		valid = 1;
		closure = false;
	}
	else return;
	if(valid == 1)
	{
		anim.curtaincalls(graph, _cicle, x,y, closure);
	}
}

void PLCls::write_pretext(Graphics &graph, int timer)
{
	int box_x = 900;//650;
	int box_y = 150;
	_posX_0 = (mapEdgeX/2) + MyZeroX - (box_x/2);
	if(_posX_0 < 0) _posX_0 = 0;
	_posY_0 = (mapEdgeY/2) + MyZeroY - (box_y/2);
	if(_posY_0 < 0) _posY_0 = 0;
	graph.SetTextFont("Pokemon R/S",30, false, false, false);
	graph.SetColor(100,100,100);
	//strcpy(s[0],"Legend tells, of a myth, of a story fabled narrative");
	//strcpy(s[1],"... shrouded in darkness!!");
	graph.DrawImage2D(_posX_0, _posY_0, box_x, box_y, MSBOX);
	writecima(graph, timer);
	if(timer < 60)
	{
		Sleep(100);
	}
	else if(timer == 60)
	{
		Sleep(1000);
		printf("Sleeping...ZzzzzZZZZzzz\n");
	}
	else if(timer > 60)
	{
		writebaixo(graph, timer-60);
	}
}

void PLCls::writecima(Graphics &graph, int timer)
{
	int i;
	int x = _posX_0 + 88;
	int y = _posY_0 + 85;
	char s[200] = "Legend tells... Of a myth, of a story fabled narrative";
	int leng = strlen(s);
	int size;
	int sizechar = 12;
	//printf("Leng: %d\n",leng);
	if(timer <= leng)
	{
		size = timer;
	}
	else
	{
		size = leng;
	}
	for(i=0 ; i<size; i++)
	{
		if(i > 0)
		{
			if((s[i-1] == 'i') || (s[i-1] == 'l'))
			{
				sizechar = 8;
			}
			else if(s[i-1] == 'r')
			{
				sizechar = 10;
			}
			else
			{
				sizechar = 12;
			}
		}
		x += sizechar;
		graph.DrawText2D(x,y, "%c", s[i]);
	}
}

void PLCls::writebaixo(Graphics &graph, int timer)
{
	int i;
	int x = _posX_0 + 88;
	int y = _posY_0 + 50;
	char s[200] = "... shrouded in darkness!!";
	int leng = strlen(s);
	int size;
	int sizechar = 12;
	//printf("Leng: %d\n",leng);
	if(timer <= leng)
	{
		size = timer;
	}
	else
	{
		size = leng;
	}
	for(i=0 ; i<size; i++)
	{
		if(i > 0)
		{
			if((s[i-1] == 'i') || (s[i-1] == 'l'))
			{
				sizechar = 8;
			}
			else if(s[i-1] == 'r')
			{
				sizechar = 10;
			}
			else
			{
				sizechar = 12;
			}
		}
		x += sizechar;
		graph.DrawText2D(x,y, "%c", s[i]);
	}
}
