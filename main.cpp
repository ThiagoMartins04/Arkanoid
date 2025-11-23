#include "raylib.h"
#include <iostream>

using namespace std;

// Plataforma (player)
Vector2 plataformaPosicao = {350.0f, 550.0f};
Vector2 plataformaPosicaoFim = {450.0f, 550.0f};
Vector2 plataformaTamanho = {100.0f, 20.0f};
Color cor = MAROON;

// Pelota
Vector2 bolaPosicao = {400.0f, 539.0f};
const int bolaTamanho = 10;

// Movimentação da bola
int direcao = -1;
int diagonal = 1; // 1 - Direita | -1 - Esquerda
float bolaVelocidadeY = 10.0f;
float bolaVelocidadeX = 10.0f;
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
  Blocos blocos[6];
  Color coresBloco = GREEN;
  float posBlocosX = -120.0f;
  for (int i = 0; i < 6; i++) {
    posBlocosX += 130.0f;
    blocos[i].bloco.x = posBlocosX;
    blocos[i].bloco.y = 200.0f;
    blocos[i].bloco.height = 50.0f;
    blocos[i].bloco.width = 130.0f;
    blocos[i].cor = coresBloco;
  }

  while (!WindowShouldClose()) {
    // Movimentação do player
    if (IsKeyDown(KEY_RIGHT) && plataformaPosicao.x < screenWidth - 110) {
      plataformaPosicao.x += 10.0f;
      plataformaPosicaoFim.x += 10.0f;
    }
    if (IsKeyDown(KEY_LEFT)  && plataformaPosicao.x > 10){
      plataformaPosicao.x -= 10.0f;
      plataformaPosicaoFim.x -= 10.0f;
    }

    // Movimentação da bola
    if (bolaPosicao.y <= bolaTamanho) direcao = direcao * -1;
    if (bolaPosicao.y >= plataformaPosicao.y - bolaTamanho && bolaPosicao.x >= plataformaPosicao.x && bolaPosicao.x <= plataformaPosicao.x + plataformaTamanho.x) {
      direcao *= -1;
      // Vai verificar se a posição que a bola bateu na plataforma está mais a esqueda ou direita
      if (bolaPosicao.x - plataformaPosicao.x < 50) {
        //Mais a esquerda
        bolaVelocidadeX = 10.0f * ((50 - (bolaPosicao.x - plataformaPosicao.x)) * 0.02f);
        cout << bolaPosicao.x << " - " << plataformaPosicao.x << " = " << bolaPosicao.x - plataformaPosicao.x << endl;
        moveDiagonal = true;
        diagonal = -1;
      }
      if (bolaPosicao.x - plataformaPosicao.x > 50) {
        //Mais a direita
        bolaVelocidadeX = 10.0f * ((bolaPosicao.x - plataformaPosicao.x - 50) / 50);
        
        moveDiagonal = true;
        diagonal = 1;
      }
    }
    if (bolaPosicao.x >= 790) diagonal *= -1;
    if (bolaPosicao.x <= 10) diagonal *= -1;
    
    if (bolaPosicao.y < 20 && bolaPosicao.y > 10) {
      // Vai calcular a distancia que falta para a plataforma e mudar a velocidade para que a bola não ultrapasse a plataforma
      bolaVelocidadeY = bolaPosicao.y - 10;
    }

    bolaPosicao.y = bolaPosicao.y + (bolaVelocidadeY * direcao);
    if(moveDiagonal) {
      bolaPosicao.x = bolaPosicao.x + (bolaVelocidadeX * diagonal);
    }
    bolaVelocidadeY = 10.0f; // Vai voltar a velocidade padrão (para caso tenha mudado na funcao)
    
    // Verifica colisão
    for (int i = 0; i < 6; i++) {
      if(CheckCollisionCircleRec(bolaPosicao, 10.0f, blocos[i].bloco)) {
        blocos[i].cor = RED;
        direcao *= -1;
      }
    }
    
    if (CheckCollisionCircleLine(bolaPosicao, 10.0f, plataformaPosicao, plataformaPosicaoFim)) {
      cor = LIME;
    }


    // Começa a pintar
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(TextFormat("%f", plataformaPosicao.x), 20, 20, 20, BLACK);
    DrawText(TextFormat("%f", bolaPosicao.y), 20, 50, 20, BLACK);
    for (int i = 0; i < 6; i++) {
      DrawRectangle(blocos[i].bloco.x, blocos[i].bloco.y, blocos[i].bloco.width, blocos[i].bloco.height, blocos[i].cor);
    }
    DrawRectangleV(plataformaPosicao, plataformaTamanho, cor);
    DrawCircleV(bolaPosicao, 10, GREEN);
    
    EndDrawing();
  }
  CloseWindow();
  return 0;
}