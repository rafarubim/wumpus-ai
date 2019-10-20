#pragma once
#include <stdio.h>
#include "Graphics.h"
#include <stdlib.h>
#include "Global.hpp"
#include "Animate.hpp"

class PLCls{
private:
	Image BG;
	Image Water;
	Image Borda;
	Image Treco;
	Image fakeBG;
	Image MSBOX;
	int _nframes;
	int _scsize;

	int _posX_0;
	int _posY_0;
public:
	PLCls();
	int _state;
	int _cicle;
	void loop_pre_load(Graphics& graph,Animation& anim);
	void load(void);
	void draw_bg(Graphics& graph);
	void draw_plyr(Graphics& graph,Animation& anim);       
	void draw_smoke(Graphics& graph,Animation& anim);
	void write_pretext(Graphics &graph, int timer);
	void writecima(Graphics &graph, int timer);
	void writebaixo(Graphics &graph, int timer);
};
