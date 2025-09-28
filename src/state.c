#include "state.h"
#include "game.h"
#include "constants.h"
#include <raylib.h>

static M_State current_state = GAME;

int init();
int update();
int render();
void destroy();

void m_app_run() {
  if (init() != 0) return;
  while (!WindowShouldClose()) {
    if (update() != 0) break;
    if (render() != 0) break;
  }
  destroy();
}

void m_app_state_change(M_State new_state) {
}

int init() {
  InitWindow(WIDTH, HEIGHT, "Space Invaders");
  SetConfigFlags(FLAG_VSYNC_HINT);

  switch (current_state) {
  case GAME:
    return m_game_init();
  case MENU:
    return 0;
  }
}

int update() {
  switch (current_state) {
  case GAME:
    return m_game_update();
  case MENU:
    return 0;
  }
}

int render() {
  switch (current_state) {
  case GAME:
    return m_game_render();
  case MENU:
    return 0;
  }
}

void destroy() {
  switch (current_state) {
  case GAME:
    m_game_destroy();
  case MENU:
    return;
  }
}
