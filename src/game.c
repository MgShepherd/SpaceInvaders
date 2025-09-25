#include "game.h"
#include <raylib.h>
#include <stdio.h>

static const float WIDTH = 800.0f;
static const float HEIGHT = 600.0f;
static const float PLAYER_Y_POS = 500.0f;
static const float PLAYER_SIZE = 32.0f;
static const float PLAYER_SPEED = 300.0f;
static const int IMAGE_SIZE = 16;

static const char *SPRITESHEET_PATH = "resources/SpriteSheet.png";

typedef struct {
  Rectangle image_region;
  Rectangle bounds;
  Vector2 velocity;
} M_GameObject;

void m_game_update_player_pos(M_GameObject *player) {
  player->bounds.x += player->velocity.x * GetFrameTime();
  if (player->bounds.x < 0.0f)
    player->bounds.x = 0.0f;
  else if (player->bounds.x + player->bounds.width > WIDTH) {
    player->bounds.x = WIDTH - player->bounds.width;
  }
}

void m_game_update(M_GameObject *player) {
  if (IsKeyDown(KEY_A)) {
    player->velocity.x = -PLAYER_SPEED;
  } else if (IsKeyDown(KEY_D)) {
    player->velocity.x = PLAYER_SPEED;
  } else {
    player->velocity.x = 0;
  }
  m_game_update_player_pos(player);
}

void m_game_render(Texture2D *sprite_sheet, M_GameObject *player) {
  BeginDrawing();

  ClearBackground(BLACK);

  DrawFPS(10, 10);
  DrawTexturePro(*sprite_sheet, player->image_region, player->bounds,
                 (Vector2){.x = 0.0f, .y = 0.0f}, 0.0f, WHITE);

  EndDrawing();
}

void m_game_loop(Texture2D *sprite_sheet, M_GameObject *player) {
  while (!WindowShouldClose()) {
    m_game_update(player);
    m_game_render(sprite_sheet, player);
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
  player->velocity = (Vector2){.x = 0, .y = 0};
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
  InitWindow(WIDTH, HEIGHT, "Space Invaders");
  SetConfigFlags(FLAG_VSYNC_HINT);

  m_game_start();

  CloseWindow();
}
