#include "menu.h"
#include "state.h"
#include <raylib.h>

int m_menu_init() {
  return 0;
}

int m_menu_update() {
  if (IsKeyPressed(KEY_ENTER)) {
    m_app_state_change(M_GAME);
  }
  return 0;
}

int m_menu_render() {
  BeginDrawing();

  ClearBackground(BLACK);

  DrawText("Space Invaders", 100, 100, 32, WHITE);

  EndDrawing();
  return 0;
}

void m_menu_destroy() {
}
