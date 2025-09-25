#include "game.h"
#include <raylib.h>
#include <stdio.h>

static const float WIDTH = 800.0f;
static const float HEIGHT = 600.0f;
static const float PLAYER_Y_POS = 500.0f;
static const float PLAYER_SIZE = 32.0f;
static const int IMAGE_SIZE = 16;

static const char *SPRITESHEET_PATH = "resources/SpriteSheet.png";

typedef struct {
  Rectangle image_region;
  Rectangle bounds;
} M_GameObject;

void m_game_loop(Texture2D *sprite_sheet, M_GameObject *player) {
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(BLACK);

    DrawFPS(10, 10);
    DrawTexturePro(*sprite_sheet, player->image_region, player->bounds,
                   (Vector2){.x = 0.0f, .y = 0.0f}, 0.0f, WHITE);

    EndDrawing();
  }
}

void m_game_load_objects(M_GameObject *player) {
  player->image_region = (Rectangle){.x = 0 * IMAGE_SIZE,
                                     .y = 0 * IMAGE_SIZE,
                                     .width = IMAGE_SIZE,
                                     .height = IMAGE_SIZE};
  player->bounds = (Rectangle){.x = WIDTH / 2 - PLAYER_SIZE / 2,
                               .y = PLAYER_Y_POS,
                               .width = PLAYER_SIZE,
                               .height = PLAYER_SIZE};
}

int m_game_start() {
  Texture2D sprite_sheet = LoadTexture(SPRITESHEET_PATH);
  if (sprite_sheet.id <= 0) {
    // TODO: Improve error system
    printf("Unable to load texture %s\n", SPRITESHEET_PATH);
    return 1;
  }

  M_GameObject player;
  m_game_load_objects(&player);

  m_game_loop(&sprite_sheet, &player);

  UnloadTexture(sprite_sheet);
  return 0;
}

void m_game_run() {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(WIDTH, HEIGHT, "Space Invaders");

  m_game_start();

  CloseWindow();
}
