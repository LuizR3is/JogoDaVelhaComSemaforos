# JogoDaVelhaComSemaforos

O jogo a ser desenvolvido com aplicações dos conceitos de threads e semáforos será o já conhecido “Jogo da Velha”. O jogo é composto por um tabuleiro de matriz 3x3, é jogado por duas pessoas e cada uma delas possui um símbolo, “X” ou “O”. Essas pessoas fazem jogadas alternadas para marcar o seu próprio símbolo em um dos espaços vazios no tabuleiro. Vence o jogo aquele que primeiro conseguir completar uma linha vertical, horizontal ou diagonal com três elementos iguais. Caso o tabuleiro tenha sido preenchido e nenhum dos dois tenha conseguido completar uma linha, o jogo é dado como empatado.</br></br>
O jogo será escrito na linguagem C. No desenvolvimento, cada jogador será representado por uma thread e a cada jogada a thread deverá informar ao semáforo que ela está acessando o tabuleiro naquele momento, para impedir que a outra thread consiga acessar o tabuleiro ao mesmo tempo. Ao terminar uma jogada, o sinal do semáforo é desbloqueado, para que o outro jogador possa acessar o tabuleiro e realizar uma nova jogada. A definição de qual thread começará jogando se dará através da concorrência, logo, o jogador (thread 0 ou thread 1) que chegar mais rapidamente ao semáforo, dará início ao jogo.</br></br>
Para rodar o programa é necessário abrir o terminal, acessar o diretório correspondente e compilar digitando os comandos: </br></br>gcc -Wall jogoDaVelhaComSemaforos.c -o jdv -lpthread</br>./jdv</br></br>
Os códigos relacionados aos semáforos e às threads tiveram como referência as aulas gravadas do Prof. Carlos Maziero</br></br>


