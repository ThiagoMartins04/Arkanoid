#include "raylib.h"
#include <iostream>

using namespace std;

// CONSTANTES - BLOCOS
#define QUANTIDADEBLOCOS 12
#define TAMANHOBLOCO 65.0f
#define ALTURABLOCO 50.0f
#define QUANTIDADELINHASBLOCOS 3
// CONSTANTES - BOLA
#define SPEEDBOLA 3.0f
#define TAMANHOBOLA 10.0f

// Plataforma (player)
Vector2 plataformaPosicao = {350.0f, 550.0f};
Vector2 plataformaPosicaoFim = {450.0f, 550.0f};
Vector2 plataformaTamanho = {100.0f, 20.0f};
Color cor = MAROON;

// Pelota
Vector2 bolaPosicao = {400.0f, 539.0f};
Vector2 bolaPosicaoFutura;
const int bolaTamanho = 10;

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

int main() {
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "raylib basic window");
  SetTargetFPS(60);

  //Blocos do ARKANOID
  Blocos blocos[3][12];
  Color coresBloco = GREEN;
  float posBlocosX = (screenWidth - (QUANTIDADEBLOCOS * TAMANHOBLOCO)) / 2;
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

  while (!WindowShouldClose()) {
    // Movimentação do player
    if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && plataformaPosicao.x < screenWidth - 110) {
      plataformaPosicao.x += 10.0f;
      plataformaPosicaoFim.x += 10.0f;
    }
    if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  && plataformaPosicao.x > 10){
      plataformaPosicao.x -= 10.0f;
      plataformaPosicaoFim.x -= 10.0f;
    }

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
      diagonal = -1; // Se bater no cando direito vai para a esquerda
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
    for (int i = 0; i < QUANTIDADELINHASBLOCOS; i++) {
        for (int j = 0; j < QUANTIDADEBLOCOS; j++) {
            // Pontos laterais
            Vector2 esq1 = {blocos[i][j].bloco.x, blocos[i][j].bloco.y};
            Vector2 esq2 = {blocos[i][j].bloco.x, blocos[i][j].bloco.y + ALTURABLOCO};

            Vector2 dir1 = {blocos[i][j].bloco.x + TAMANHOBLOCO, blocos[i][j].bloco.y};
            Vector2 dir2 = {blocos[i][j].bloco.x + TAMANHOBLOCO, blocos[i][j].bloco.y + ALTURABLOCO};

            // Pontos superior e inferior
            Vector2 cima1 = {blocos[i][j].bloco.x, blocos[i][j].bloco.y + ALTURABLOCO};
            Vector2 cima2 = {blocos[i][j].bloco.x + TAMANHOBLOCO, blocos[i][j].bloco.y + ALTURABLOCO};
            
            Vector2 baixo1 = {blocos[i][j].bloco.x, blocos[i][j].bloco.y};
            Vector2 baixo2 = {blocos[i][j].bloco.x + TAMANHOBLOCO, blocos[i][j].bloco.y};

            bool bateuEsquerda = CheckCollisionCircleLine(bolaPosicaoFutura, TAMANHOBOLA, esq1, esq2);
            bool bateuDireita = CheckCollisionCircleLine(bolaPosicaoFutura, TAMANHOBOLA, dir1, dir2);
            bool bateuCima = CheckCollisionCircleLine(bolaPosicaoFutura, TAMANHOBOLA, cima1, cima2);
            bool bateuBaixo = CheckCollisionCircleLine(bolaPosicaoFutura, TAMANHOBOLA, baixo1, baixo2);

            float raio = TAMANHOBOLA;

            // LADO ESQUERDO
            if (bateuEsquerda) {
                bolaPosicao.x = blocos[i][j].bloco.x - raio;  
                diagonal *= -1;
            }
            // LADO DIREITO
            else if (bateuDireita) {
                bolaPosicao.x = blocos[i][j].bloco.x + blocos[i][j].bloco.width + raio;
                diagonal *= -1;
            }
            // TOPO
            else if (bateuCima) {
                bolaPosicao.y = blocos[i][j].bloco.y - raio;
                direcao *= -1;
            }
            // BASE
            else if (bateuBaixo) {
                bolaPosicao.y = blocos[i][j].bloco.y + blocos[i][j].bloco.height + raio;
                direcao *= -1;
            }


            if (CheckCollisionCircleRec(bolaPosicao, TAMANHOBOLA, blocos[i][j].bloco)) {

                bool bateuEsquerda = CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA, esq1, esq2);
                bool bateuDireita  = CheckCollisionCircleLine(bolaPosicao, TAMANHOBOLA, dir1, dir2);

                // Teste topo/base
                bool bateuTopo = bolaPosicao.y < blocos[i][j].bloco.y;
                bool bateuBase = bolaPosicao.y > blocos[i][j].bloco.y + ALTURABLOCO;

                if (bateuEsquerda || bateuDireita) {
                    // Lateral
                    diagonal *= -1;
                }
                else if (bateuTopo || bateuBase) {
                    // Vertical
                    direcao *= -1;
                }
                else {
                    // Bateu na quina
                    diagonal *= -1;
                    direcao  *= -1;
                }

                // Remove o bloco
                blocos[i][j].bloco.x = -200.0f;
                blocos[i][j].bloco.y = -200.0f;
            }
        }
    }



    // Começa a pintar
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(TextFormat("%f", plataformaPosicao.x), 20, 20, 20, BLACK);
    DrawText(TextFormat("%f", bolaPosicao.y), 20, 50, 20, BLACK);
    for (int i = 0; i < QUANTIDADELINHASBLOCOS; i++) {
      for (int j = 0; j < QUANTIDADEBLOCOS; j++) {
        DrawRectangle(blocos[i][j].bloco.x, blocos[i][j].bloco.y, blocos[i][j].bloco.width, blocos[i][j].bloco.height, blocos[i][j].cor);
      }
    }
    DrawRectangleV(plataformaPosicao, plataformaTamanho, cor);
    DrawCircleV(bolaPosicao, TAMANHOBOLA, GREEN);
    
    EndDrawing();
  }
  CloseWindow();
  return 0;
}