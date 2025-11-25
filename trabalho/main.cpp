#include "raylib.h"
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

// CONSTANTES - JOGO
#define SCREENWIDTH 800
#define SCREENHEIGHT 600
// CONSTANTES - BLOCOS
#define QUANTIDADEBLOCOS 12
#define TAMANHOBLOCO 65.0f
#define ALTURABLOCO 50.0f
#define QUANTIDADELINHASBLOCOS 3
// CONSTANTES - BOLA
#define SPEEDBOLA 5.0f
#define TAMANHOBOLA 10.0f
// CONSTANTES - PLAYER
#define SPEEDPLAYER 10.0f

// Plataforma (player)
Vector2 plataformaPosicao = {350.0f, 550.0f};
Vector2 plataformaPosicaoFim = {450.0f, 550.0f};
Vector2 plataformaTamanho = {100.0f, 20.0f};
Color cor = MAROON;
struct Player
{
  int maiorPontuacao, pontuacaoAtual;
  string nome;
};


// Pelota
Vector2 bolaPosicao = {400.0f, 539.0f};
Vector2 bolaPosicaoFutura;

// Botões
Rectangle botaoSair = {400.0f, 300.0f, 100.0f, 50.0f};
const char botaoSairTexto[5] = "Sair";

// Movimentação da bola
int direcao = -1;
int diagonal = 1; // 1 - Direita | -1 - Esquerda
float bolaVelocidadeY = SPEEDBOLA;
float bolaVelocidadeX = SPEEDBOLA;
bool moveDiagonal = false;

struct Blocos {
  Rectangle bloco;
  Color cor;
  bool ativo = true;
};

// FUNCOES

void movimentacaoPlayer() {
  if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && plataformaPosicao.x < SCREENWIDTH - 110) {
      plataformaPosicao.x += SPEEDPLAYER;
      plataformaPosicaoFim.x += SPEEDPLAYER;
    }
    if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  && plataformaPosicao.x > 10){
      plataformaPosicao.x -= SPEEDPLAYER;
      plataformaPosicaoFim.x -= SPEEDPLAYER;
    }
}

int main() {
  InitWindow(SCREENWIDTH, SCREENHEIGHT, "raylib basic window");
  SetTargetFPS(60);

  //Blocos do ARKANOID
  Blocos blocos[3][12];
  Color coresBloco = GREEN;
  float posBlocosX = (SCREENWIDTH - (QUANTIDADEBLOCOS * TAMANHOBLOCO)) / 2;
  float posBlocosY = 10.0f;
  int cont = 1;
  for (int i = 0; i < QUANTIDADELINHASBLOCOS; i++) {
    posBlocosX = 10.0f;
    for (int j = 0; j < QUANTIDADEBLOCOS; j++) {
      blocos[i][j].bloco.x = posBlocosX;
      blocos[i][j].bloco.y = posBlocosY;
      blocos[i][j].bloco.height = 50.0f;
      blocos[i][j].bloco.width = TAMANHOBLOCO;
      posBlocosX += TAMANHOBLOCO;
      if (cont % 2 == 0) {
        blocos[i][j].cor = BLUE;
      } else {
        blocos[i][j].cor = RED;
      }
      cont++;
    }
    cont++;
    posBlocosY += 50.0f;
  }

  int vidasPlayer = 3;
  int escolha = 0;

  while (!WindowShouldClose()) {
    if (vidasPlayer == 0) break;

    if (IsKeyPressed(MOUSE_BUTTON_LEFT)) {
      Vector2 posicaoMouse = GetMousePosition();
      if(CheckCollisionPointRec(posicaoMouse, botaoSair)) {
        escolha = 1;
      }
    }

    switch (escolha)
    {
    case 1:
      // Movimentação do player
      movimentacaoPlayer();

      // Movimentação da bola
      //Se bater na plataforma:
      if (CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA, plataformaPosicao, plataformaPosicaoFim)) {
        direcao *= -1;
        // Vai verificar se a posição que a bola bateu na plataforma está mais a esqueda ou direita
        if (bolaPosicao.x - plataformaPosicao.x < 50) {
          //Mais a esquerda
          bolaVelocidadeX = SPEEDBOLA * ((50 - (bolaPosicao.x - plataformaPosicao.x)) * 0.02f);
          moveDiagonal = true;
          diagonal = -1;
        }
        if (bolaPosicao.x - plataformaPosicao.x > 50) {
          //Mais a direita
          bolaVelocidadeX = SPEEDBOLA * ((bolaPosicao.x - plataformaPosicao.x - 50) / 50);
          moveDiagonal = true;
          diagonal = 1;
        }
      }

      if(CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA, {0.0f, 10.0f}, {800.0f, 10.0f})) {
        direcao *= -1; //Se bater no canto superior vai pra baixo
      }
      if(CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA, {10.0f, 0.0f}, {10.0f, 600.0f})) {
        diagonal = 1; // Se bater no canto esquerdo vai para a direita
      }
      if(CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA, {790.0f, 0.0f}, {790.0f, 600.0f})) {
        diagonal = -1; // Se bater no canto direito vai para a esquerda
      }
      if(CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA, {0.0f, (600.0f + TAMANHOBOLA * 2)}, {790.0f, (600.0f + TAMANHOBOLA * 2)})) {
        direcao *= -1; // Se sair do jogo perde uma vida
        vidasPlayer--;
      }

      bolaPosicao.y = bolaPosicao.y + (bolaVelocidadeY * direcao);
      if(moveDiagonal) {
        bolaPosicao.x = bolaPosicao.x + (bolaVelocidadeX * diagonal);
      }

      // Pega a posição futura para evitar que a bola ultrapasse o bloco
      if (moveDiagonal) {
        bolaPosicaoFutura = {bolaPosicao.x + bolaVelocidadeX, bolaPosicao.y + SPEEDBOLA};
      } else {
        bolaPosicaoFutura = {bolaPosicao.x, bolaPosicao.y + SPEEDBOLA};
      }

      // Verifica colisão
      Vector2 futuraX = { bolaPosicao.x + (bolaVelocidadeX * diagonal), bolaPosicao.y };
      Vector2 futuraY = { bolaPosicao.x, bolaPosicao.y + (bolaVelocidadeY * direcao) };

      bool houveColisao = false;

      for (int i = 0; i < QUANTIDADELINHASBLOCOS && !houveColisao; i++) {
          for (int j = 0; j < QUANTIDADEBLOCOS && !houveColisao; j++) {

              Rectangle r = blocos[i][j].bloco;
              if (r.x < 0) continue;

              bool colisaoX = CheckCollisionCircleRec(futuraX, TAMANHOBOLA, r);
              bool colisaoY = CheckCollisionCircleRec(futuraY, TAMANHOBOLA, r);

              if (colisaoX || colisaoY) {

                  if (colisaoX && !colisaoY) {
                      // bateu na lateral
                      diagonal = -diagonal;
                  }
                  else if (colisaoY && !colisaoX) {
                      // bateu por cima ou por baixo
                      direcao = -direcao;
                  }
                  else {
                      // bateu exatamente na quina → escolha a NORMAL do bloco
                      if (fabs((bolaPosicao.x - (r.x + r.width/2))) >
                          fabs((bolaPosicao.y - (r.y + r.height/2))))
                          diagonal = -diagonal;
                      else
                          direcao = -direcao;
                  }

                  blocos[i][j].bloco.x = -200;
                  blocos[i][j].bloco.y = -200;

                  houveColisao = true;
              }
          }
      }
      break;
    }

    







    // Começa a pintar
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawRectangleRec(botaoSair, GRAY);
    DrawText(botaoSairTexto, botaoSair.x, botaoSair.y, 20, BLACK);
    
    
    for (int i = 0; i < QUANTIDADELINHASBLOCOS; i++) {
      for (int j = 0; j < QUANTIDADEBLOCOS; j++) {
        DrawRectangle(blocos[i][j].bloco.x, blocos[i][j].bloco.y, blocos[i][j].bloco.width, blocos[i][j].bloco.height, blocos[i][j].cor);
      }
    }
    DrawRectangleV(plataformaPosicao, plataformaTamanho, cor);
    DrawCircleV(bolaPosicao, TAMANHOBOLA, GREEN);

    DrawText(TextFormat("%f", plataformaPosicao.x), 0, 500, 20, BLACK);
    DrawText(TextFormat("%f", bolaPosicao.y), 700, 550, 20, BLACK);
    DrawText(TextFormat("%i", vidasPlayer), 700, 500, 20, BLACK);
    
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
