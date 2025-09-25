#include "game.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

static const float WIDTH = 600.0f;
static const float HEIGHT = 600.0f;

static const float PLAYER_Y_POS = 500.0f;
static const float ENEMY_START_Y = 100.0f;
static const float ENEMY_START_X = 50.0f;
static const float ENEMY_SPACING = 20.0f;
static const int ENEMIES_PER_ROW = 10;

static const float OBJECT_SIZE = 32.0f;
static const float PLAYER_SPEED = 300.0f;
static const int MAX_ENEMIES = 30;
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

static inline void m_game_draw_object(Texture2D *sprite_sheet,
                                      const M_GameObject *object) {
  DrawTexturePro(*sprite_sheet, object->image_region, object->bounds,
                 (Vector2){.x = 0.0f, .y = 0.0f}, 0.0f, WHITE);
}

void m_game_render(Texture2D *sprite_sheet, const M_GameObject *player,
                   const M_GameObject *enemies) {
  BeginDrawing();

  ClearBackground(BLACK);

  DrawFPS(10, 10);
  m_game_draw_object(sprite_sheet, player);
  for (size_t i = 0; i < MAX_ENEMIES; i++) {
    m_game_draw_object(sprite_sheet, &enemies[i]);
  }

  EndDrawing();
}

void m_game_loop(Texture2D *sprite_sheet, M_GameObject *player,
                 M_GameObject *enemies) {
  while (!WindowShouldClose()) {
    m_game_update(player);
    m_game_render(sprite_sheet, player, enemies);
  }
}

void init_game_object(M_GameObject *object, int image_x, int image_y, int xPos,
                      int yPos) {
  object->image_region = (Rectangle){.x = image_x * IMAGE_SIZE,
                                     .y = image_y * IMAGE_SIZE,
                                     .width = IMAGE_SIZE,
                                     .height = IMAGE_SIZE};
  object->bounds = (Rectangle){
      .x = xPos, .y = yPos, .width = OBJECT_SIZE, .height = OBJECT_SIZE};
  object->velocity = (Vector2){.x = 0, .y = 0};
}

int m_game_load_objects(M_GameObject *player, M_GameObject **enemies) {
  init_game_object(player, 0, 0, WIDTH / 2 - OBJECT_SIZE / 2, PLAYER_Y_POS);
  *enemies = malloc(sizeof(M_GameObject) * MAX_ENEMIES);
  if (*enemies == NULL) {
    return 1;
  }

  for (size_t i = 0; i < MAX_ENEMIES; i++) {
    init_game_object(
        &(*enemies)[i], 1, 0,
        ENEMY_START_X + ((i % ENEMIES_PER_ROW) * (OBJECT_SIZE + ENEMY_SPACING)),
        ENEMY_START_Y +
            ((int)(i / ENEMIES_PER_ROW) * (OBJECT_SIZE + ENEMY_SPACING)));
  }
  return 0;
}

int m_game_start() {
  Texture2D sprite_sheet = LoadTexture(SPRITESHEET_PATH);
  if (sprite_sheet.id <= 0) {
    printf("Unable to load texture %s\n", SPRITESHEET_PATH);
    return 1;
  }

  M_GameObject player;
  M_GameObject *enemies = NULL;
  if (m_game_load_objects(&player, &enemies) != 0) {
    printf("Unable to load required game objects\n");
    UnloadTexture(sprite_sheet);
    return 1;
  }

  m_game_loop(&sprite_sheet, &player, enemies);

  UnloadTexture(sprite_sheet);
  return 0;
}

void m_game_run() {
  InitWindow(WIDTH, HEIGHT, "Space Invaders");
  SetConfigFlags(FLAG_VSYNC_HINT);

  m_game_start();

  CloseWindow();
}
