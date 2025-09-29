#include "state.h"
#include "game.h"
#include "menu.h"
#include "constants.h"
#include <raylib.h>
#include <stdio.h>

static M_State current_state = M_MENU;

int init();
int update();
int render();
void destroy();

void m_app_run() {
  InitWindow(WIDTH, HEIGHT, "Space Invaders");
  SetConfigFlags(FLAG_VSYNC_HINT);

  if (init() != 0) return;
  while (!WindowShouldClose()) {
    if (update() != 0) break;
    if (render() != 0) break;
  }
  destroy();
}

void m_app_state_change(M_State new_state) {
  if (new_state == current_state) return;
  destroy();
  current_state = new_state;
  init();
}

int init() {
  switch (current_state) {
  case M_GAME:
    return m_game_init();
  case M_MENU:
    return m_menu_init();
  }
}

int update() {
  switch (current_state) {
  case M_GAME:
    return m_game_update();
  case M_MENU:
    return m_menu_update();
  }
}

int render() {
  switch (current_state) {
  case M_GAME:
    return m_game_render();
  case M_MENU:
    return m_menu_render();
  }
}

void destroy() {
  switch (current_state) {
  case M_GAME:
    m_game_destroy();
  case M_MENU:
    m_menu_destroy();
  }
}
