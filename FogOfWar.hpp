#pragma once
#include <stdio.h>
#include "Graphics.h"
#include "Agente.hpp"
#include <stdlib.h>
#include "Global.hpp"

class FOW_cls
{
private:

	Image FOW1;
	Image FOW2;
	char backup_mapa[LN][COL];
	//void log_erro(AgenteCls& agnt);
public:
	FOW_cls();
	void log_erro(AgenteCls& agnt);
	void atualiza_fow(AgenteCls& agnt);
	void imprime_fow(Graphics &fog);
	void getLastVisitedTile(int *i, int *j);
	void getCurrentTile(AgenteCls& agnt, int *i, int *j);
	int _fow[LN][COL];
	void exibe_fow(void);
	void load(void);
	int checa_zero(void);
};
