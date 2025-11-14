#include "raylib.h"

// Plataforma (player)
Vector2 plataformaPosicao = {350.0f, 550.0f};
Vector2 plataformaTamanho = {100.0f, 20.0f};

// Pelota
Vector2 bolaPosicao = {400.0f, 539.0f};
const int bolaTamanho = 10;

// Movimentação da bola
int direcao = -1;
int diagonal = 1; // 1 - Direita | -1 - Esquerda
float bolaVelocidadeY = 10.0f;
float bolaVelocidadeX = 10.0f;
bool moveDiagonal = false;

int main() {
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "raylib basic window");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    // Movimentação do player
    if (IsKeyDown(KEY_RIGHT) && plataformaPosicao.x < screenWidth - 110) plataformaPosicao.x += 10.0f;
    if (IsKeyDown(KEY_LEFT)  && plataformaPosicao.x > 10)               plataformaPosicao.x -= 10.0f;

    // Movimentação da bola
    if (bolaPosicao.y <= bolaTamanho) direcao = direcao * -1;
    if (bolaPosicao.y >= plataformaPosicao.y - bolaTamanho && bolaPosicao.x >= plataformaPosicao.x && bolaPosicao.x <= plataformaPosicao.x + plataformaTamanho.x) {
      direcao *= -1;
      // Vai verificar se a posição que a bola bateu na plataforma está mais a esqueda ou direita
      if (bolaPosicao.x - plataformaPosicao.x < 50) {
        //Mais a esquerda
        bolaVelocidadeX = 10.0f * (((bolaPosicao.x - plataformaPosicao.x) / 50);
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
    

    // Começa a pintar
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(TextFormat("%f", plataformaPosicao.x), 20, 20, 20, BLACK);
    DrawText(TextFormat("%f", bolaPosicao.y), 20, 50, 20, BLACK);
    DrawRectangleV(plataformaPosicao, plataformaTamanho, MAROON);
    DrawCircleV(bolaPosicao, 10, GREEN);
    
    EndDrawing();
  }
  CloseWindow();
  return 0;
}