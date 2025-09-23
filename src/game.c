#include "game.h"
#include <raylib.h>

#define WIDTH 800
#define HEIGHT 600

void m_game_init() {
  InitWindow(WIDTH, HEIGHT, "My Window");
  CloseWindow();
}
