% Eventos: vazio, wumpus1, wumpus2, teleporter, powerUp, buraco, ouro, missingno
% Sensacoes: brisa, fedor, flash
% Passos: direita, esquerda, cima, baixo
% Direc: direita, esquerda, cima, baixo
% Ações: andar, virarDireita, atirar, pegar

% **************************************************** Execução do programa ****************************************************************************************************************

% Variáveis atributo do jogador:
:- dynamic playerPos/2. % playerPos(int X, int Y) -> Posição atual do jogador
:- dynamic playerDirec/1. % playerDirec(Direc direc) -> Direção atual do jogador
:- dynamic playerOuros/1. % playerOuros(int ouros) -> Quantidade de ouros que o jogador possui
:- dynamic playerVida/1. % playerVida(int vida) -> Quantidade de vida do jogador (máximo 100)
:- dynamic playerPontos/1. % playerPontos(int pontos) - > Quantidade de pontos do jogador
:- dynamic playerMunicao/1. % playerMunicao(int municao) - > Munição do jogador

% Flow do programa:
:- dynamic objetivo/1. % objetivo(Objetivo) -> Objetivo é uma lista de ações a serem tomadas no momento
:- dynamic vidaWumpusLutando/1. % vidaWumpusLutando(Int) -> descreve a vida do wumpus com o qual se luta

% Banco de dados de conhecimento:
:- dynamic sala/3. % sala(int Linha, int Coluna, Evento evento) -> Mapeia evento real em [Lin,Col]. É dynamic, pois após o ouro ou powerUp terem sido pegos, altera-se.
:- dynamic memoria/4. % memoria(int X, int Y, Evento evento, Lista<sensacoes> sensacoes) -> Mapeia, em [X,Y], evento e sensações conhecidas pelo player. Incerteza é indicada por "missingno".
:- dynamic desconfia/2. % desconfia(Evento evento, Lista<Lista<int>> listaParesOrdenados) -> Mapaia um evento e uma lista de par ordenados que representam posições onde se desconfia possuir tal evento
% --------------------------------------- Ações manuais -----------------------------------------
% debugSituacoes/0: imprime situação atual do player. -- FLOW
% debugConhecimento/0: imprime o banco de dados do conhecimento. -- CONHECIMENTO
% debugDecisoes/0: imprime as decisões atuais do jogador. -- DECISÕES
% atualizar/0: atualiza banco de dados do conhecimento através do que se pode observar da posição do player.
% anda/0: executa ação de andar com o player para direção que ele está virado.
% viraDir/0: executa a ação de virar o player 90º à direita.
% decideObjetivo/0: determina a próxima lista de ações para alcançar objetivo atual
% executarAcao/0: executa primeira ação da lista de objetivos, retirando-a da lista

% *************************************************** Inicializações ************************************************************************************************************************************

% playerPos/2: fato que é único a todo momento. Representa posição do jogador no mapa.
playerPos(1,1).

% playerDirec/1: fato que é único a todo momento. Representa direção do jogador no mapa.
playerDirec(direita).

% playerOuros/1: fato que é único a todo momento. Representa a quantidade de ouros que o jogador já pegou.
playerOuros(0).

% playerVida/1: fato que é único a todo momento. Representa a vida do jogador.
playerVida(100).

% playerPontos/1: fato que é único a todo momento. Representa a pontuação do jogador.
playerPontos(0).

% playerMunicao/1: fato que é único a todo momento. Representa a munição do jogador.
playerMunicao(5).

% minOuros/1: fato que é único a todo momento. Mínimo de ouros necessários para o jogador sair da dungeon.
minOuros(3).

% objetivo/1: fato que é único  a todo momento. Representa a lista de ações a serem tomadas até o próximo objetivo do jogador ser concluído.
objetivo([]).

vidaWumpus1(100).
vidaWumpus2(100).
danoWumpus1(20).
danoWumpus2(50).
danoArma(20).

% mapa: mapeia cada evento presente em X,Y do mapa, caso Evento seja variável.
mapa(X,Y,Evento) :- 
	var(Evento),
	sala(Y,X,Evento), !.

% mapa: substitui (ou cria) sala X,Y do mapa por sala colocada (Após pegar o ouro, por exemplo, sala fica vazia).
mapaBota(X,Y,Evento) :-
	nonvar(Evento),
	retract(sala(Y,X,_)),
	assert(sala(Y,X,Evento)).

% limitesMapa: descreve limites xMin,xMax,yMin,yMax do mapa
limitesMapa(1,12,1,12).

% mapaEntrada: X e Y da entrada da dungeon.
mapaEntrada(1,1).

% sala: mapeia cada evento presente em Linha,Coluna da matriz do mapa, lida pelo executável. As linhas estão em contagem invertida.

% virandoDir: mapeia direções antes/após virar à direita.
virandoDir(direita,baixo).
virandoDir(baixo,esquerda).
virandoDir(esquerda,cima).
virandoDir(cima,direita).

% **************************************************** IMPLEMENTAÇÕES DE FLOW ********************************************************************************************************
agir(0).

agir(N) :-
	New is N-1,
	agir,
	agir(New), !.
	

resolverIA :-
	agir,
	resolverIA, !.

iniciar :-
	atualizar,
	decideObjetivo,
	debugGeral.

agir :-
	executarAcao,
	tentaRecalcularObjetivo,
	debugGeral.

tentaRecalcularObjetivo :-
	objetivo([]),
	decideObjetivo, !.

tentaRecalcularObjetivo.

anda :-
	retract(playerPos(X,Y)),
	andaDirec(X,Y),
	atualizar,
	retract(playerPontos(N)),
	NovaPontuacao is N-1,
	assert(playerPontos(NovaPontuacao)), !.

andaDirec(X,Y) :-
	playerDirec(direita),
	X1 is X + 1,
	assert(playerPos(X1,Y)).

andaDirec(X,Y) :-
	playerDirec(esquerda),
	X1 is X - 1,
	assert(playerPos(X1,Y)).

andaDirec(X,Y) :-
	playerDirec(cima),
	Y1 is Y + 1,
	assert(playerPos(X,Y1)).

andaDirec(X,Y) :-
	playerDirec(baixo),
	Y1 is Y - 1,
	assert(playerPos(X,Y1)).

viraDir :-
	retract(playerDirec(Direc)),
	virandoDir(Direc,NovaDirec),
	assert(playerDirec(NovaDirec)),
	retract(playerPontos(N)),
	NovaPontuacao is N-1,
	assert(playerPontos(NovaPontuacao)), !.

pega :-
	playerPos(X,Y),
	memoria(X,Y,ouro,_),
	retract(playerOuros(N)),
	NN is N + 1,
	assert(playerOuros(NN)),
	mapaBota(X,Y,vazio),
	atualizar,
	retract(playerPontos(Pontos)),
	NovaPontuacao is Pontos+1000,
	assert(playerPontos(NovaPontuacao)), !.

pega :-
	playerPos(X,Y),
	memoria(X,Y,powerUp,_),
	retract(playerVida(N)),
		(NN is N + 20, NN =< 100;
		NN is 100),
	assert(playerVida(NN)),
	mapaBota(X,Y,vazio),
	atualizar,
	retract(playerPontos(Pontos)),
	NovaPontuacao is Pontos-1,
	assert(playerPontos(NovaPontuacao)), !.
	
atira :-
	playerPos(X,Y),
	X1 is X + 1, tentaAtirar(X1,Y),
	X2 is X - 1, tentaAtirar(X2,Y),
	Y1 is Y + 1, tentaAtirar(X,Y1),
	Y2 is Y - 1, tentaAtirar(X,Y2), !.

% tentaAtirar: sempre retorna TRUE. Atira no wumpus se ele estiver lá.
tentaAtirar(X,Y) :-
	posValida(X,Y),
	memoria(X,Y,wumpus1,_),
	vidaWumpus1(Vida),
	assert(vidaWumpusLutando(Vida)),
	tentaAtirarAcc(X,Y,5),
	testaSeMatou(X,Y),
	retract(vidaWumpusLutando(_)), !.
	
tentaAtirar(X,Y) :-
	posValida(X,Y),
	memoria(X,Y,wumpus2,_),
	vidaWumpus2(Vida),
	assert(vidaWumpusLutando(Vida)),
	tentaAtirarAcc(X,Y,5),
	testaSeMatou(X,Y),
	retract(vidaWumpusLutando(_)), !.
	
tentaAtirar(_,_) :- !.

% testaSeMatou: sempre retorna TRUE. Se o wumpus com que se lutava morre, troca-o no mapa por vazio
testaSeMatou(X,Y) :-
	vidaWumpusLutando(Vida),
	Vida =< 0,
	mapaBota(X,Y,vazio), !.
	
testaSeMatou(_,_) :- !.

% tentaAtirarAcc: sempre retorna TRUE. Atira até Acc ser 0
tentaAtirarAcc(_,_,0) :- !.

tentaAtirarAcc(X,Y,Acc) :-
	playerMunicao(N),
	N > 0,
	retract(playerMunicao(N)),
	NovoN is N - 1,
	assert(playerMunicao(NovoN)),
	retract(vidaWumpusLutando(Vida)),
	danoArma(Dano),
	NovaVida is Vida - Dano,
	assert(vidaWumpusLutando(NovaVida)),
	NovoAcc is Acc - 1,
	retract(playerPontos(Pontos)),
	NovaPontuacao is Pontos-10,
	assert(playerPontos(NovaPontuacao)),
	tentaAtirarAcc(X,Y,NovoAcc).

% executarAcao: executa primeira acao da lista para alcançar o objetivo, retirando-a da lista. Caso haja uma ação imediatamente recompensante, a prioriza (como pegar um powerUp)
executarAcao :-
	objetivo([H|T]),
	executarAcaoAux(H),
	retract(objetivo([H|T])),
	assert(objetivo(T)), !.
	
% tentaPowerUp: sempre retorna true. Se puder pegar powerUp, coloca-o como primeiro objetivo.
tentaPowerUp :-
	playerPos(X,Y),
	playerVida(Vida),
	memoria(X,Y,powerUp,_),
	Vida =< 80,
	retract(objetivo(Lst)),
	assert(objetivo([pegar|Lst])), !.
	
tentaPowerUp :- !.

% No meio de um caminho (não numa tomada de decisão), pode por acaso haver um powerup que pode ser pego. tentaPowerUp garante que ele será pego.
executarAcaoAux(andar) :-
	anda,
	tentaPowerUp.

executarAcaoAux(virarDireita) :-
	viraDir.

executarAcaoAux(pegar) :-
	pega.
	
executarAcaoAux(atirar) :-
	atira.

novoObjetivo(Objetivo) :-
	retract(objetivo(_)),
	assert(objetivo(Objetivo)).

% Para debug:
perderVida(N) :-
	retract(playerVida(Vida)),
	NovaVida is Vida - N,
	assert(playerVida(NovaVida)), !.

debugGeral :-
	debugSituacao,
	debugConhecimento,
	debugDecisoes.

debugSituacao :-
	listing(playerPos),
	listing(playerDirec),
	listing(playerOuros),
	listing(playerVida),
	listing(playerPontos),
	listing(playerMunicao).

% *********************************************************************** COMANDOS DE MANIPULAÇÃO DE LISTA **********************************************************************************************************************************************************

% lstPertence: retorna a pertinência de um elemento em uma lista.
lstPertence(X,[X|_]).
lstPertence(X,[_|T]) :- lstPertence(X,T).

% lstIntersetam: retorna se duas listas se intersetam ou não
lstIntersetam([H|T],Lst2) :-
	lstPertence(H,Lst2);
	lstIntersetam(T,Lst2).

% lstInsere: insere, na lista passada, o elemento passado, retornando o resultado no terceiro argumento.
lstInsere(Lst1,Item,[Item|Lst1]).

% lstIntersecao: sempre retorna TRUE (caso os argumentos sejam válidos, os dois primeiros listas e o terceiro variável).
% Retorna o terceiro argumento, como intersecao das listas passadas.
lstIntersecao(Lst1,Lst2,Intersecao) :-
	var(Intersecao),
	lstIntersecaoAcc(Lst1,Lst2,[],Intersecao).

% lstIntersecaoAcc: solucao com acumulador para regra "lstIntersecao"
lstIntersecaoAcc([H|T],Lst2,Acc,Intersecao) :-
	lstPertence(H,Lst2),
	lstIntersecaoAcc(T,Lst2,[H|Acc],Intersecao).

lstIntersecaoAcc([_|T],Lst2,Acc,Intersecao) :-
	lstIntersecaoAcc(T,Lst2,Acc,Intersecao).

lstIntersecaoAcc([],_,Intersecao,Intersecao).

% lstDiferenca: sempre retorna TRUE (caso os argumentos sejam válidos, os dois primeiros listas e o terceiro variável).
% Retorna o terceiro argumento, como diferenca das listas passadas.
lstDiferenca(Lst1,Lst2,Diferenca) :-
	var(Diferenca),
	lstDiferencaAcc(Lst1,Lst2,[],Diferenca).

% lstDiferencaAcc: solucao com acumulador para regra "lstDiferenca"
lstDiferencaAcc([H|T],Lst2,Acc,Diferenca) :-
	lstPertence(H,Lst2),
	lstDiferencaAcc(T,Lst2,Acc,Diferenca).

lstDiferencaAcc([H|T],Lst2,Acc,Diferenca) :-
	lstDiferencaAcc(T,Lst2,[H|Acc],Diferenca).

lstDiferencaAcc([],_,Diferenca,Diferenca).

% lstMaiorNum: retorna TRUE desde que a lista tenha pelo menos 1 elemento, retornando maior numero de lst
lstMaiorNum(Lst,Maior) :-
	var(Maior),
	Lst = [H|T],
	\+ number(H),
	!, lstMaiorNum(T,Maior).

lstMaiorNum(Lst,Maior) :-
	var(Maior),
	Lst = [H|T],
	lstMaiorNumAcc(T,H,Maior), !.

% lstMaiorNumAcc: implementacao com acumulador para "lstMaiorNum".
lstMaiorNumAcc([H|T],Acc,Maior) :-
	number(H),
	H > Acc,
	lstMaiorNumAcc(T,H,Maior).

lstMaiorNumAcc([_|T],Acc,Maior) :-
	lstMaiorNumAcc(T,Acc,Maior).

lstMaiorNumAcc([],Maior,Maior).

% lstMenorNum: retorna TRUE desde que a lista tenha pelo menos 1 elemento, retornando menor numero de lst
lstMenorNum(Lst,Menor) :-
	var(Menor),
	Lst = [H|T],
	\+ number(H),
	!, lstMenorNum(T,Menor).

lstMenorNum(Lst,Menor) :-
	var(Menor),
	Lst = [H|T],
	lstMenorNumAcc(T,H,Menor), !.

% lstMenorNumAcc: implementacao com acumulador para "lstMaiorNum".
lstMenorNumAcc([H|T],Acc,Menor) :-
	number(H),
	H < Acc,
	lstMenorNumAcc(T,H,Menor).

lstMenorNumAcc([_|T],Acc,Menor) :-
	lstMenorNumAcc(T,Acc,Menor).

lstMenorNumAcc([],Menor,Menor).

% lstBusca: retorna true se encontrou elemento buscado, retornando seu índice na lista (índice começa em 1).
lstBusca(Lst,ElemBuscado,Indice) :-
	var(Indice),
	lstBuscaAcc(Lst,ElemBuscado,1,Indice).

% lstBuscaAcc: solucao com acumulador para "lstBusca".
lstBuscaAcc([H|_],ElemBuscado,Acc,Indice) :-
	H = ElemBuscado,
	Acc = Indice.

lstBuscaAcc([_|T],ElemBuscado,Acc,Indice) :-
	NovoAcc is Acc + 1,
	lstBuscaAcc(T,ElemBuscado,NovoAcc,Indice).

% lstPegaElemIndice: retorna TRUE caso indice seja valido na lista, retornando elemento na posicao desejada (índice começa em 1).
lstPegaElemIndice([H|_],1,H).

lstPegaElemIndice([_|T],Indice,Elem) :-
	Indice > 1,
	NovoIndice is Indice - 1,
	lstPegaElemIndice(T,NovoIndice,Elem).


% juntaListasInternas: sempre retorna TRUE, se a lista passada é uma lista de listas e "Resultado" não está instanciado. 
% Recebe lista de listas e monta uma nova lista que é o append das listas internas, retornando-a no segundo argumento.
juntaListasInternas(ListaDeListas, Resultado) :-
	var(Resultado),
	juntaListasInternasAcc(ListaDeListas,[],Resultado).

% juntaListasInternasAcc: solucao com acumulador para "juntaListasInternas".
juntaListasInternasAcc(ListaDeListas,Acc,Resultado) :-
	ListaDeListas = [H|T],
	append(Acc,H,NovoAcc),
	juntaListasInternasAcc(T,NovoAcc,Resultado).

juntaListasInternasAcc([],Resultado,Resultado).








% ************************************************ IMPLEMENTAÇÃO DE BANCO DE DADOS DE CONHECIMENTO ********************************************************************************

debugConhecimento :-
	listing(memoria),
	listing(desconfia).	

% memoriaLivre: falha quando existe alguma memoria com os valores iniciais X,Y no BD. Passar variáveis não as instancia.
memoriaLivre(X,Y) :- \+ memoria(X,Y,_,_).

% insereSensacaoMemoria: Sempre retorna TRUE. Se a memoria já existir, atualiza-a, somando-se a nova sensação.
insereSensacaoMemoria(X,Y,Sensacao) :- 
	memoriaLivre(X,Y),
	assert(memoria(X,Y,missingno,[Sensacao])), !.

insereSensacaoMemoria(X,Y,Sensacao) :- 
	retract(memoria(X,Y,Evento,Sensacoes)),
	reinsereSensacaoMemoria(X,Y,Evento,Sensacoes,Sensacao), !.

% reinsereSensacaoMemoria: Sempre retorna TRUE. Chamada após retract de memória.
reinsereSensacaoMemoria(X,Y,Evento,Sensacoes,Sensacao) :-
	Sensacoes = missingno,
	assert(memoria(X,Y,Evento,[Sensacao])), !.

reinsereSensacaoMemoria(X,Y,Evento, Sensacoes, Sensacao) :-
	\+ lstPertence(Sensacao,Sensacoes),
	assert(memoria(X,Y,Evento,[Sensacao|Sensacoes])), !.

reinsereSensacaoMemoria(X,Y,Evento, Sensacoes, _) :-
	assert(memoria(X,Y,Evento,Sensacoes)), !.

% insereEventoMemoria: Sempre retorna TRUE. Se a memoria já existir, atualiza-a, trocando o Evento pelo novo.
insereEventoMemoria(X,Y,Evento) :-
	memoriaLivre(X,Y),
	assert(memoria(X,Y,Evento,missingno)), !.

insereEventoMemoria(X,Y,Evento) :-
	retract(memoria(X,Y,_,Sensacoes)),
	assert(memoria(X,Y,Evento,Sensacoes)), !.

% eventoSensacao: Mapeia, para cada evento, a sensação associada (não-injetivo).
eventoSensacao(buraco,brisa).
eventoSensacao(wumpus1,fedor).
eventoSensacao(wumpus2,fedor).
eventoSensacao(teleporter,flash).

% posValida: retorna a validade de uma posição no mapa (dentro de seus limites)
posValida(X,Y) :-
	limitesMapa(XMin,XMax,YMin,YMax),
	X >= XMin, X =< XMax,
	Y >= YMin, Y =< YMax, !.

atualizar :-
	observar,
	desconfiar,
	inferir.

% getEventoSensacao: Retorna apensa um par eventoSensacao (o primeiro encontrado), caso possível
getEventoSensacao(Evento,Sensacao) :- eventoSensacao(Evento,Sensacao), !.

% eventosDesconfiados: A partir de uma lista de sensações determinada, retorna uma lista de eventos possivelmente associados.
eventosDesconfiados(Sensacoes,Eventos) :-
	nonvar(Sensacoes),
	eventosDesconfiadosAcc(Sensacoes,[],Eventos), !.

% eventosDesconfiadosAcc: Solução recursiva com acumulador para eventosDesconfiados.
eventosDesconfiadosAcc([HSens|TSens],EventosAcc,Eventos) :- findall(Evento,eventoSensacao(Evento,HSens),LstEventos), append(EventosAcc,LstEventos,EventosRes), eventosDesconfiadosAcc(TSens,EventosRes,Eventos).
eventosDesconfiadosAcc([],Eventos,Eventos).

% observar: sempre retorna TRUE (quando não há bug).
% Atualiza a memória do jogador com o que é observado nos arredores, recebendo sensações e salvando o evento da sala atual
observar :- 
	playerPos(X,Y), posValida(X,Y), mapa(X,Y,Evento),
	insereEventoMemoria(X,Y,Evento),
	X1 is X+1, avistar(X1,Y),
	X2 is X-1, avistar(X2,Y),
	Y1 is Y+1, avistar(X,Y1),
	Y2 is Y-1, avistar(X,Y2),
	corrigeFaltaSensacao(X,Y), !.

% corrigeFaltaSensacao: sempre retorna TRUE. Troca o "missingno" de uma memória por [].
corrigeFaltaSensacao(X,Y) :-
	memoria(X,Y,Ev,missingno),
	retract(memoria(X,Y,Ev,missingno)),
	assert(memoria(X,Y,Ev,[])).

corrigeFaltaSensacao(_,_).

% Sempre retorna TRUE. Traz sensações para o player dependendo do que há na casa avistada.
avistar(Xa,Ya) :-
	posValida(Xa,Ya),
	mapa(Xa,Ya,Evento),
	getEventoSensacao(Evento,Sensacao),
	playerPos(X,Y),
	insereSensacaoMemoria(X,Y,Sensacao), !.

avistar(_,_).

% desconfiar: sempre retorna TRUE (quando não há bug).
% A partir das sensações que o player tem, cria um vetor de possíveis eventos e cria/atualiza suas desconfianças (desconfia/2) no BD
desconfiar :-
	playerPos(X,Y), posValida(X,Y),
	memoria(X,Y,_,Sensacoes),
	eventosDesconfiados(Sensacoes,EvsDesconfiados),
	geraDesconfiancas(X,Y,EvsDesconfiados), !.

% geraDesconfiancas: sempre retorna TRUE. Chama regra geraDesconfianca para cada evento desconfiado da lista passada
geraDesconfiancas(X,Y,EvsDesconfiados) :-
	EvsDesconfiados = [H|T],
	geraDesconfianca(X,Y,H),
	geraDesconfiancas(X,Y,T).

geraDesconfiancas(_,_,_) :- !.

% checarLocal: falha se parâmetros forem inválidos ou se a memoria já possui evento desconfiado no local.
% Retorna TRUE caso contrário, instanciando "PosDesconfiada" em: [[X,Y]] ou [].
checarLocal(X,Y,EvDesconfiado,PosDesconfiada) :-
	(var(X); var(Y); var(EvDesconfiado); nonvar(PosDesconfiada)),
	print_message(error,"Argumentos invalidos para 'checarDesconfianca'."),
	!, fail.

checarLocal(X,Y,_,PosDesconfiada) :-
	\+ posValida(X,Y),
	PosDesconfiada = [], !.

checarLocal(X,Y,EvDesconfiado,_) :-
	memoria(X,Y,EvReal,_),
	EvReal = EvDesconfiado,
	!, fail.

checarLocal(X,Y,_,PosDesconfiada) :-
	memoria(X,Y,EvReal,_),
	EvReal = missingno,
	PosDesconfiada = [[X,Y]], !.

checarLocal(X,Y,_,PosDesconfiada) :-
	memoriaLivre(X,Y),
	var(PosDesconfiada),
	PosDesconfiada = [[X,Y]], !.

checarLocal(_,_,_,PosDesconfiada) :-
	PosDesconfiada = [], !.

% geraDesconfianca: Sempre retorna TRUE. Cria (caso necessario) uma nova desconfianca no BD ou melhora as desconfianças já existentes
geraDesconfianca(X,Y,EvDesconfiado) :-
	X1 is X+1, checarLocal(X1,Y,EvDesconfiado,Pos1),
	X2 is X-1, checarLocal(X2,Y,EvDesconfiado,Pos2),
	Y1 is Y+1, checarLocal(X,Y1,EvDesconfiado,Pos3),
	Y2 is Y-1, checarLocal(X,Y2,EvDesconfiado,Pos4),
	append(Pos3,Pos4,L34), append(Pos2,L34,L234), append(Pos1,L234,L1234),
	findall(Locais,desconfia(EvDesconfiado,Locais),ListaDesconfiancaLocais),
	tratarNovaDesconfianca(EvDesconfiado,L1234,ListaDesconfiancaLocais), !.

geraDesconfianca(_,_,_).

% tratarNovaDesconfianca: Dá erro, retornando FALSE caso a desconfianca passada seja vazia.
% Em condições normais, retorna TRUE e atualiza um fato "desconfia"ou cria novo fato pelas informações passadas
tratarNovaDesconfianca(_,DesconfiancaLocais,_) :-
	DesconfiancaLocais = [],
	print_message(error,"Erro: sensacao encontrada nao tem motivo aparente"),
	!, fail.

tratarNovaDesconfianca(Evento,DesconfiancaLocais,ListaDesconfiancaLocais) :-
	\+ validaDesconfianca(Evento,DesconfiancaLocais,ListaDesconfiancaLocais),
	assert(desconfia(Evento,DesconfiancaLocais)), !.

tratarNovaDesconfianca(_,_,_).

% validaDesconfianca: retorna FALSE se desconfianca ainda nao existe em BD.
% Se existir, retorna TRUE, aprimorando as informações já existentes mantendo apenas a interseção de informações.
validaDesconfianca(Evento,DesconfiancaLocais,ListaDesconfiancaLocais) :-
	ListaDesconfiancaLocais = [H|_],
	lstIntersetam(DesconfiancaLocais,H),
	lstIntersecao(DesconfiancaLocais,H,Intersec),
	lstDiferenca(H,DesconfiancaLocais,AntesDesconfiados),
	retract(desconfia(Evento,H)),
	assert(desconfia(Evento,Intersec)),
	verificaSobrouDesconfiancaLocais(AntesDesconfiados), !.

validaDesconfianca(Evento,DesconfiancaLocais,ListaDesconfiancaLocais) :-
	ListaDesconfiancaLocais = [_|T],
	validaDesconfianca(Evento,DesconfiancaLocais,T).
	
% verificaSobrouDesconfiancaLocais: sempre retorna TRUE. Após uma desconfiança ser aprimorada, os locais retirados,
% passados para esta regra, são colocados como "vazios" caso não haja outra desconfiança neles.
verificaSobrouDesconfiancaLocais(LocaisRetirados) :-
	LocaisRetirados = [H|T],
	findall(DesconfiancaLocais,desconfia(_,DesconfiancaLocais),ListaDesconfiancaLocais),
	juntaListasInternas(ListaDesconfiancaLocais,ListaTodosLocais),
	verificaSobrouDesconfiancaLocal(H,ListaTodosLocais),
	verificaSobrouDesconfiancaLocais(T).
	
verificaSobrouDesconfiancaLocais([]) :- !.

% verificaSobrouDesconfiancaLocal: sempre retorna TRUE. Se não houver nenhuma desconfianca no local passado, salva-o como "vazio" na memória.
verificaSobrouDesconfiancaLocal([X,Y],ListaTodosLocais) :-
	\+ lstPertence([X,Y],ListaTodosLocais),
	insereEventoMemoria(X,Y,vazio).
	
verificaSobrouDesconfiancaLocal(_,_).

% inferir: sempre retorna TRUE (quando não há bug). Faz as seguintes inferências:
% -Se uma desconfiança reduz-se a um único local possível, assume com certeza o evento que está lá (caso uma nova desconfiança tenha confirmado o local)
% -Determina (adicionando à memória) se salas adjacentes são vazias, caso não haja nenhuma sensação na casa atual (ou caso já se saiba a origem das sensações)
% -Para as salas que já foram determinadas como vazias (ou com um evento qualquer), exclui todas as desconfiaças relativas ao local
% -Se uma desconfiança reduz-se a um único local possível, assume com certeza o evento que está lá (caso uma falta de desconfiança tenha, por eliminação, descoberto qual é o local)
inferir :-
	playerPos(X,Y), posValida(X,Y),
	trataConfirmacaoDesconfianca,
	trataAdjacenciasVazias(X,Y),
	trataDesconfiancasInvalidas,
	trataConfirmacaoDesconfianca.

% trataAdjacenciasVazias: sempre retorna TRUE. Trata o primeiro caso de inferir.
trataAdjacenciasVazias(X,Y) :-
	memoria(X,Y,_,Sensacoes),
	eventosDesconfiados(Sensacoes,EvsDesconfiados),
	adjacenciasVazias(X,Y,EvsDesconfiados),
	X1 is X+1, tentaBotarVazio(X1,Y),
	X2 is X-1, tentaBotarVazio(X2,Y),
	Y1 is Y+1, tentaBotarVazio(X,Y1),
	Y2 is Y-1, tentaBotarVazio(X,Y2), !.
	
trataAdjacenciasVazias(_,_) :- !.
	
% tentaBotarVazio: sempre retorna TRUE. Se o evento da memória passada é desconhecido, salva-o como true (criando a memória, caso necessário).
tentaBotarVazio(X,Y) :-
	posValida(X,Y),
	memoriaLivre(X,Y),
	assert(memoria(X,Y,vazio,missingno)), !.

tentaBotarVazio(X,Y) :-
	posValida(X,Y),
	memoria(X,Y,Evento,_),
	Evento = missingno,
	retract(memoria(X,Y,missingno,Sensacoes)),
	assert(memoria(X,Y,vazio,Sensacoes)), !.
	
tentaBotarVazio(_,_) :- !.

% adjacenciasVazias: retorna true caso adjacências desconhecidas estejam vazias, false caso haja alguma desconfiança sobrando dos arredores
adjacenciasVazias(X,Y,EvsDesconfiados) :-
	EvsDesconfiados = [H|T],
	procuraEvMemoria(X,Y,H),
	adjacenciasVazias(X,Y,T), !.
	
adjacenciasVazias(_,_,[]) :- !.
	
% procuraEvMemoria: Falha quando não encontra EvDesconfiado nas memórias adjacentes a X,Y. Retorna TRUE caso encontre
procuraEvMemoria(X,Y,Evento) :-
	X1 is X+1, eventoNaoEstaMemoria(X1,Y,Evento),
	X2 is X-1, eventoNaoEstaMemoria(X2,Y,Evento),
	Y1 is Y+1, eventoNaoEstaMemoria(X,Y1,Evento),
	Y2 is Y-1, eventoNaoEstaMemoria(X,Y2,Evento),
	!, fail.

procuraEvMemoria(_,_,_) :- !.

% eventoNaoEstaMemoria: retorna se evento está na memória passada
eventoNaoEstaMemoria(X,Y,Evento) :-
	\+ posValida(X,Y);
	\+ memoria(X,Y,Evento,_), !.

% trataDesconfiancasInvalidas: sempre retorna TRUE. Trata do segundo caso de inferir.
trataDesconfiancasInvalidas :-
	findall([Evento,DesconfiancaLocais],desconfia(Evento,DesconfiancaLocais),ListaDesconfiancas),
	corrigirDesconfiancas(ListaDesconfiancas).

% corrigirDesconfiancas: sempre retorna TRUE. Chama corrigirDesconfianca para cada desconfianca no BD.
corrigirDesconfiancas(ListaDesconfiancas) :-
	ListaDesconfiancas = [[DesconfiancaEvento,DesconfiancaLocais]|T],
	corrigirDesconfiancaAcc(DesconfiancaEvento,DesconfiancaLocais,DesconfiancaLocais),
	corrigirDesconfiancas(T), !.

corrigirDesconfiancas([]) :- !.
	
% corrigirDesconfianca: sempre retorna TRUE. Corrige desconfianca passada, retirando locais desconfiados que já se sabe serem vazios ou possuirem evento.
corrigirDesconfiancaAcc(DesconfiancaEvento,Acc,DesconfiancaLocais) :-
	Acc = [[X,Y]|T],
	corrigirLocalDesconfianca(X,Y,DesconfiancaEvento,DesconfiancaLocais,LocaisRestantes),
	corrigirDesconfiancaAcc(DesconfiancaEvento,T,LocaisRestantes), !.
	
corrigirDesconfiancaAcc(_,[],_) :- !.
	
% corrigirLocalDesconfianca: sempre retorna TRUE. Corrige local desconfiado que já se sabe ser vazio ou possuir evento. Retorna Locais descondiados restantes.
corrigirLocalDesconfianca(X,Y,DesconfiancaEvento,DesconfiancaLocais,NovosLocaisDesconfianca) :-
	memoria(X,Y,Evento,_),
	Evento \= missingno,
	retract(desconfia(DesconfiancaEvento,DesconfiancaLocais)),
	lstDiferenca(DesconfiancaLocais,[[X,Y]],NovosLocaisDesconfianca),
	assert(desconfia(DesconfiancaEvento,NovosLocaisDesconfianca)), !.
	
corrigirLocalDesconfianca(_,_,_,DesconfiancaLocais,DesconfiancaLocais) :- !.

% trataConfirmacaoDesconfianca: sempre retorna TRUE. Trata o terceiro caso de inferir.
trataConfirmacaoDesconfianca :-
	findall([Evento,[X,Y]],desconfia(Evento,[[X,Y]]),DesconfiancasAConfirmar),
	confirmaDesconfiancas(DesconfiancasAConfirmar).

% confirmaDesconfiancas: sempre retorna TRUE. Para cada desconfiança a confirmar,
%  retira desconfianca da memoria, inserindo na memoria o evento que, com certeza, está na posição que se espera
confirmaDesconfiancas(DesconfiancasAConfirmar) :-
	DesconfiancasAConfirmar = [[Evento,[X,Y]]|T],
	retract(desconfia(Evento,[[X,Y]])),
	insereEventoMemoria(X,Y,Evento),
	confirmaDesconfiancas(T), !.
	
confirmaDesconfiancas([]) :- !.


% ************************************************************* IMPLEMENTAÇÃO DE TOMADA DE DECISÕES *******************************************************************************

debugDecisoes :-
	listing(objetivo).

decideObjetivo :-
	playerPos(X,Y),
	memoria(X,Y,ouro,_),
	novoObjetivo([pegar]), !.

decideObjetivo :-
	minOuros(N),
	playerOuros(N),
	playerPos(X,Y),
	pegaCaminhoEntrada(X,Y,Caminho),
	playerDirec(Direc),
	passosParaAcoes(Caminho,Acoes,Direc),
	novoObjetivo(Acoes), !.

decideObjetivo :-
	playerPos(X,Y),
	memoria(X,Y,powerUp,_),
	playerVida(Vida),
	Vida =< 80,
	novoObjetivo([pegar]), !.

decideObjetivo :-
	playerPos(X,Y),
	pegaCaminhoSeguroNaoVisitado(X,Y,Caminho),
	Caminho \= semCaminho,
	playerDirec(Direc),
	passosParaAcoes(Caminho,Acoes,Direc),
	novoObjetivo(Acoes), !.

decideObjetivo :-
	playerPos(X,Y),
	pegaCaminhoTeleporte(X,Y,Caminho),
	Caminho \= semCaminho,
	playerDirec(Direc),
	passosParaAcoes(Caminho,Acoes,Direc),
	novoObjetivo(Acoes), !.
	
decideObjetivo :-
	playerMunicao(N),
	N > 0,
	playerPos(X,Y),
	pegaCaminhoAdjacenteMonstro(X,Y,Caminho),
	Caminho \= semCaminho,
	playerDirec(Direc),
	passosParaAcoes(Caminho,Acoes,Direc),
	append(Acoes,[atirar],Acoes2),
	novoObjetivo(Acoes2), !.
	
decideObjetivo :-
	playerPos(X,Y),
	pegaCaminhoMonstro1(X,Y,Caminho),
	Caminho \= semCaminho,
	playerDirec(Direc),
	passosParaAcoes(Caminho,Acoes,Direc),
	novoObjetivo(Acoes), !.
	
decideObjetivo :-
	playerPos(X,Y),
	pegaCaminhoMonstro2(X,Y,Caminho),
	Caminho \= semCaminho,
	playerDirec(Direc),
	passosParaAcoes(Caminho,Acoes,Direc),
	novoObjetivo(Acoes), !.

% ehPerigoso: mapeia eventos perigosos.
ehPerigoso(buraco).
ehPerigoso(wumpus1).
ehPerigoso(wumpus2).
ehPerigoso(teleporter).

pegaCaminhoEntrada(X,Y,Caminho) :-
	mapaEntrada(XEntrada,YEntrada),
	pegaCaminhoEficiente(X,Y,Caminho,_,[XEntrada,YEntrada,_,_]), !.

pegaCaminhoSeguroNaoVisitado(X,Y,Caminho) :-
	pegaCaminhoEficiente(X,Y,Caminho,_,[_,_,vazio,missingno]), !.
	
pegaCaminhoTeleporte(X,Y,Caminho) :-
	pegaCaminhoEficiente(X,Y,Caminho,_,[_,_,teleporte,_]), !.
	
pegaCaminhoAdjacenteMonstro(X,Y,Caminho) :-
	pegaCaminhoEficiente(X,Y,Caminho,_,[_,_,_,[fedor]]), !.
	
pegaCaminhoMonstro1(X,Y,Caminho) :-
	pegaCaminhoEficiente(X,Y,Caminho,_,[_,_,wumpus1,_]), !.
	
pegaCaminhoMonstro2(X,Y,Caminho) :-
	pegaCaminhoEficiente(X,Y,Caminho,_,[_,_,wumpus2,_]), !.
	
	
% unificaSemInstanciar: recebe uma constante como primeiro parâmetro. O segundo argumento é incógnito, podendo ou não ser variável.
% Retorna TRUE se IncognitaM for variável ou, no caso de constante, se as duas forem iguais. A regra procura equivaler à regra "=" de
% unificação (com pelo menos uma constante), porém não instanciando a variável caso os argumentos unifiquem.
% A unificação de variável é simples, ou seja, gato(X) não unificaria com gato(Rafael).
unificaSemInstanciar(NaoVarN,_) :-
	var(NaoVarN),
	print_message(error,"Primeiro argumento deve ser constante!"), !, fail.

unificaSimplesSemInstanciar(_,IncognitaM) :-
	var(IncognitaM), !.
	
unificaSimplesSemInstanciar(NaoVarN,IncognitaM) :-
	NaoVarN == IncognitaM, !.

/****************************** FUNÇÕES INUTILIZADAS -> pegaCaminhoEficiente é uma melhora
% pegaCaminho: sempre retorna TRUE. A partir de uma posição [X,Y], retorna uma lista "Caminho" de passos a tomar, sem passar por locais perigosos, para chegar
% na sala com determinadas entradas de memória: XBuscado,YBuscado,EventoBuscado,SensacoesBuscadas. ListaLugaresVisitados deve ser inicialmente vazia.
% pegaCaminho(X,Y,Caminho,Distancia,ListaLugaresVisitados,[XBuscado,YBuscado,EventoBuscado,SensacoesBuscadas])
pegaCaminho(X,Y,Caminho,Distancia,_,[XBuscado,YBuscado,EventoBuscado,SensacoesBuscadas]) :-
	memoria(X,Y,Evento,Sensacoes),
	unificaSimplesSemInstanciar(X,XBuscado),
	unificaSimplesSemInstanciar(Y,YBuscado),
	unificaSimplesSemInstanciar(Evento,EventoBuscado),
	unificaSimplesSemInstanciar(Sensacoes,SensacoesBuscadas),
	Caminho = [],
	Distancia = 0, !.
	
pegaCaminho(X,Y,Caminho,Distancia,_,_) :-
	memoria(X,Y,Evento,_),
	ehPerigoso(Evento),
	Caminho = semCaminho,
	Distancia = semDistancia, !.
	
pegaCaminho(X,Y,Caminho,Distancia,_,_) :-
	findall(LocaisDesconfianca,desconfia(_,LocaisDesconfianca),ListaLocaisDesconfianca),
	juntaListasInternas(ListaLocaisDesconfianca,TodasDesconfiancas),
	lstPertence([X,Y],TodasDesconfiancas),
	Caminho = semCaminho,
	Distancia = semDistancia, !.

pegaCaminho(X,Y,Caminho,Distancia,ListaLugaresVisitados,LstBuscados) :-
	NovaListaLugaresVisitados = [[X,Y]|ListaLugaresVisitados],
	X1 is X+1, tentaPegarCaminho(X1,Y,Caminho1,Distancia1,NovaListaLugaresVisitados,LstBuscados),
	X2 is X-1, tentaPegarCaminho(X2,Y,Caminho2,Distancia2,NovaListaLugaresVisitados,LstBuscados),
	Y1 is Y+1, tentaPegarCaminho(X,Y1,Caminho3,Distancia3,NovaListaLugaresVisitados,LstBuscados),
	Y2 is Y-1, tentaPegarCaminho(X,Y2,Caminho4,Distancia4,NovaListaLugaresVisitados,LstBuscados),
	ListaCaminhos = [Caminho1,Caminho2,Caminho3,Caminho4],
	ListaDistancias = [Distancia1,Distancia2,Distancia3,Distancia4],
	melhorCaminho(ListaDistancias,ListaCaminhos,Caminho,Distancia), !.
	
% sempre retorna TRUE. Chama a função pegaCaminho, caso lugar ainda não tenha sido visitado.
% retorna retorno da função, se foi chamada, ou "semDistancia" e "semCaminho", caso lugar já foi visitado.
tentaPegarCaminho(X,Y,Caminho,Distancia,ListaLugaresVisitados,LstBuscados) :-
	posValida(X,Y),
	\+ lstPertence([X,Y],ListaLugaresVisitados),
	pegaCaminho(X,Y,Caminho,Distancia,ListaLugaresVisitados,LstBuscados).

tentaPegarCaminho(_,_,semCaminho,semDistancia,_,_).	

% melhorCaminho: sempre retorna TRUE. Retorna o melhorCaminho, se existir (já atualizado com novo passo), ou "semCaminho".
% Retornará também a distância deste caminho (também já atualizada com o novo passo), ou "semDistancia".
melhorCaminho(ListaDistancias,ListaCaminhos,Caminho,Distancia) :-
	lstMenorNum(ListaDistancias,Menor),
	lstBusca(ListaDistancias,Menor,Indice),
	lstPegaElemIndice(ListaCaminhos,Indice,CaminhoPequeno),
	getPassoPorIndice(Indice,Passo),
	Caminho = [Passo|CaminhoPequeno],
	Distancia is Menor + 1, !.

melhorCaminho(_,_,semCaminho,semDistancia).

% getPassoPorIndice: Dependendo do indice na lista montada (ListaDistancias), então um novo passo deve ser adicionado à esquerda do caminho
getPassoPorIndice(1,direita).
getPassoPorIndice(2,esquerda).
getPassoPorIndice(3,cima).
getPassoPorIndice(4,baixo).
*/

% passosParaAcoes: a partir de uma lista de passos, gera uma lista de acoes para executar esses passos (andar e virar). Também inclui pegar powerUps pelo caminho.
passosParaAcoes(ListaPassos,ListaAcoes,DirInicial) :-
	ListaPassos = [H|T],
	passosParaVirarAcc(H,[],AcoesVirar,DirInicial),
	append(AcoesVirar,[andar],Acoes),
	passosParaAcoes(T,AcoesDaFrente,H),
	append(Acoes,AcoesDaFrente,ListaAcoes), !.	
	
passosParaAcoes([],[],_).

% passoParaAcoesAcc: a partir de um passo, um Acc inicialmente [], e uma direção inicial, retorna uma lista de ações "virarDireita"
% para que a direção final seja a direção do passo (solução com acumulador).
passosParaVirarAcc(Dir,Acoes,Acoes,Dir).

passosParaVirarAcc(Passo,Acc,Acoes,Dir) :-
	virandoDir(Dir,NovaDir),
	passosParaVirarAcc(Passo,[virarDireita|Acc],Acoes,NovaDir).
	
	
	
	
pegaCaminhoEficiente(X,Y,Caminho,Distancia,[XBuscado,YBuscado,EventoBuscado,SensacoesBuscadas]) :-
	FilaNosAAbrir = [[[X,Y],[],0]],
	pegaCaminhoAcc(Caminho,Distancia,FilaNosAAbrir,[[X,Y]],[XBuscado,YBuscado,EventoBuscado,SensacoesBuscadas]), !.

% pegaCaminhoAcc: sempre retorna true. Vai abrindo os nos da fila, até encontrar um procurado, retornando o caminho e distância até ele (ou semCaminho/semDistancia caso não encontre).
pegaCaminhoAcc(semCaminho,semDistancia,[],_,_) :- !.
	
pegaCaminhoAcc(Caminho,Distancia,FilaNosAAbrir,_,[XBuscado,YBuscado,EventoBuscado,SensacoesBuscadas]) :-
	FilaNosAAbrir = [[[X,Y],CaminhoAcc,DistAcc]|_],
	memoria(X,Y,Evento,Sensacoes),
	unificaSimplesSemInstanciar(X,XBuscado),
	unificaSimplesSemInstanciar(Y,YBuscado),
	unificaSimplesSemInstanciar(Evento,EventoBuscado),
	unificaSimplesSemInstanciar(Sensacoes,SensacoesBuscadas),
	Caminho = CaminhoAcc,
	Distancia = DistAcc, !.
	
pegaCaminhoAcc(Caminho,Distancia,FilaNosAAbrir,NosVisitados,SalaProcurada) :-
	FilaNosAAbrir = [[[X,Y],_,_]|OutrosNos],
	memoria(X,Y,Evento,_),
	ehPerigoso(Evento),
	pegaCaminhoAcc(Caminho,Distancia,OutrosNos,NosVisitados,SalaProcurada), !.
	
pegaCaminhoAcc(Caminho,Distancia,FilaNosAAbrir,NosVisitados,SalaProcurada) :-
	FilaNosAAbrir = [[[X,Y],_,_]|OutrosNos],
	findall(LocaisDesconfianca,desconfia(_,LocaisDesconfianca),ListaLocaisDesconfianca),
	juntaListasInternas(ListaLocaisDesconfianca,TodasDesconfiancas),
	lstPertence([X,Y],TodasDesconfiancas),
	pegaCaminhoAcc(Caminho,Distancia,OutrosNos,NosVisitados,SalaProcurada), !.
	
pegaCaminhoAcc(Caminho,Distancia,FilaNosAAbrir,NosVisitados,SalaProcurada) :-
	FilaNosAAbrir = [[[X,Y],CaminhoAcc,DistAcc]|OutrosNos],
	NovaDistAcc is DistAcc + 1,
	X1 is X+1, append(CaminhoAcc,[direita],NovoCaminhoAcc1), tentaBotarFila([[X1,Y],NovoCaminhoAcc1,NovaDistAcc],OutrosNos,Fila1,NosVisitados,Visitados1),
	X2 is X-1, append(CaminhoAcc,[esquerda],NovoCaminhoAcc2), tentaBotarFila([[X2,Y],NovoCaminhoAcc2,NovaDistAcc],Fila1,Fila2,Visitados1,Visitados2),
	Y1 is Y+1, append(CaminhoAcc,[cima],NovoCaminhoAcc3), tentaBotarFila([[X,Y1],NovoCaminhoAcc3,NovaDistAcc],Fila2,Fila3,Visitados2,Visitados3),
	Y2 is Y-1, append(CaminhoAcc,[baixo],NovoCaminhoAcc4), tentaBotarFila([[X,Y2],NovoCaminhoAcc4,NovaDistAcc],Fila3,Fila4,Visitados3,Visitados4),
	pegaCaminhoAcc(Caminho,Distancia,Fila4,Visitados4,SalaProcurada), !.

% tentaBotarFila: sempre retorna TRUE. Bota nó na fila (caso não esteja marcado como visitado e seja válido), retornando nova fila e nova lista de visitados (se for enfileirado, é marcado como visitado)
tentaBotarFila(No,FilaAntes,FilaDepois,VisitadosAntes,VisitadosDepois) :-
	No = [[X,Y],_,_],
	posValida(X,Y),
	\+ lstPertence([X,Y],VisitadosAntes),
	append(FilaAntes,[No],FilaDepois),
	VisitadosDepois = [[X,Y]|VisitadosAntes], !.
	
tentaBotarFila(_,Fila,Fila,Visitados,Visitados) :- !.

% :- iniciar.