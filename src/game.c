#include "game.h"
#include <raylib.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

void m_game_handle_input(Vector2 *player_pos) {
  if (IsKeyDown(KEY_A))
    player_pos->x -= 200 * GetFrameTime();
  else if (IsKeyDown(KEY_D))
    player_pos->x += 200 * GetFrameTime();
}

void m_game_loop() {
  Vector2 player_pos = {500, 500};
  while (!WindowShouldClose()) {
    m_game_handle_input(&player_pos);
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawFPS(10, 10);
    DrawRectangleV(player_pos, (Vector2){32, 32}, RED);

    EndDrawing();
  }
}

void m_game_run() {
  InitWindow(WIDTH, HEIGHT, "My Window");
  SetConfigFlags(FLAG_VSYNC_HINT);
  m_game_loop();
  CloseWindow();
}
