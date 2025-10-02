#include "menu.h"
#include "state.h"
#include "constants.h"
#include <raylib.h>
#include <stddef.h>

static size_t menu_position = 0;

void render_menu_item(const char* text, size_t position);
void handle_element_pressed();

int m_menu_init() {
  return 0;
}

int m_menu_update() {
  if (IsKeyPressed(KEY_ENTER)) {
    handle_element_pressed();
  } else if (IsKeyPressed(KEY_DOWN) && menu_position < NUM_MENU_POSITIONS - 1) {
    menu_position++;
  } else if (IsKeyPressed(KEY_UP) && menu_position > 0) {
    menu_position--;
  }
  return 0;
}

int m_menu_render() {
  BeginDrawing();

  DrawText("Space Invaders", 100, 100, 32, WHITE);
  ClearBackground(BLACK);

  render_menu_item("Play", 0);
  render_menu_item("Exit", 1);

  EndDrawing();
  return 0;
}

void m_menu_destroy() {
}

void render_menu_item(const char* text, size_t position) {
  const Color text_color = menu_position == position ? GREEN : WHITE;
  DrawText(text, MENU_ITEM_X, MENU_ITEM_START_Y + (position * MENU_ITEM_SPACING_Y), 32, text_color);
}

void handle_element_pressed() {
  switch (menu_position) {
  case 0:
    m_app_state_change(M_GAME);
    break;
  case 1:
    m_app_close();
    break;
  }
}
