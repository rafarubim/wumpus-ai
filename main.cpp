#include <stdio.h>
#include <stdlib.h>
#include "interface.hpp"
#include "Global.hpp"
#include <stdio.h>
#include <SWI-cpp.h>
#include <Windows.h>
#include <conio.h>
#include <Windows.h>
#include <stdio.h>
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;

int main(void)
{
	char* argv[] = {"swipl.dll", "-s", "Wumpus.pl", NULL};
	_putenv("SWI_HOME_DIR=Prolog 32bits");
	PlEngine e(3,argv);



	/*PlTermv NoArg(0);
	PlTermv testeRafaArg(1);
	PlTermv assertArg(1);
	assertArg[0] = PlCompound("testeRafa(35)");

	{
		PlQuery assert("assert",assertArg);
		assert.next_solution();
	}

	{
		PlQuery testeRafa("testeRafa",testeRafaArg);
		testeRafa.next_solution();
		printf("%d\n",(int)testeRafaArg[0]);
	}

	{
		printf("************************************************************* INICIO:\n\n");
		PlQuery iniciar("iniciar", NoArg);
		iniciar.next_solution();
	}

	int i = 0;
	while(1) {
		i += 1;
		printf("************************************************************* PASSO %d:\n\n", i);
		{
			PlQuery agir("agir", NoArg);
			if (!agir.next_solution())
				break;
		}
		getch();
	}*/

	InterGraf graf;
	graf.executa();

	return 0;
}