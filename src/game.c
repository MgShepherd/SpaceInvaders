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
static const float BULLET_WIDTH = 5.0f;
static const float BULLET_HEIGHT = 15.0f;
static const float PLAYER_SPEED = 300.0f;
static const float BULLET_SPEED = 200.0f;
static const int MAX_ENEMIES = 30;
static const int NUM_BULLETS = 5;
static const int IMAGE_SIZE = 16;

static const char *SPRITESHEET_PATH = "resources/SpriteSheet.png";

typedef struct {
  Rectangle image_region;
  Rectangle bounds;
  Vector2 velocity;
  bool alive;
} M_GameObject;

typedef struct {
  M_GameObject player;
  M_GameObject *enemies;
  M_GameObject *bullets;
  size_t bullet_idx;
} M_GameState;

void m_game_update_object_pos(M_GameObject *object) {
  object->bounds.x += object->velocity.x * GetFrameTime();
  object->bounds.y += object->velocity.y * GetFrameTime();

  if (object->bounds.x < 0.0f)
    object->bounds.x = 0.0f;
  else if (object->bounds.x + object->bounds.width > WIDTH) {
    object->bounds.x = WIDTH - object->bounds.width;
  }

  if (object->bounds.y < 0.0f - object->bounds.height) {
    object->bounds.y = 0.0f - object->bounds.height;
  } else if (object->bounds.y > HEIGHT) {
    object->bounds.y = HEIGHT;
  }
}

void m_game_handle_collisions(M_GameState *state) {
  for (size_t i = 0; i < NUM_BULLETS; i++) {
    if (state->bullets[i].alive) {
      for (size_t j = 0; j < MAX_ENEMIES; j++) {
        if (state->enemies[j].alive &&
            CheckCollisionRecs(state->bullets[i].bounds,
                               state->enemies[j].bounds)) {
          state->enemies[j].alive = false;
          state->bullets[i].alive = false;
          break;
        }
      }
    }
  }
}

void m_game_update(M_GameState *state) {
  if (IsKeyDown(KEY_A)) {
    state->player.velocity.x = -PLAYER_SPEED;
  } else if (IsKeyDown(KEY_D)) {
    state->player.velocity.x = PLAYER_SPEED;
  } else {
    state->player.velocity.x = 0;
  }

  m_game_update_object_pos(&state->player);
  if (IsKeyPressed(KEY_SPACE) && !state->bullets[state->bullet_idx].alive) {
    state->bullets[state->bullet_idx].bounds.x =
        state->player.bounds.x + OBJECT_SIZE / 2 - BULLET_WIDTH / 2;
    state->bullets[state->bullet_idx].bounds.y =
        state->player.bounds.y + OBJECT_SIZE / 2 - BULLET_HEIGHT / 2;
    state->bullets[state->bullet_idx].alive = true;
    state->bullets[state->bullet_idx].velocity.y = -BULLET_SPEED;
    state->bullet_idx = (state->bullet_idx + 1) % NUM_BULLETS;
  }

  for (size_t i = 0; i < NUM_BULLETS; i++) {
    if (state->bullets[i].alive) {
      m_game_update_object_pos(&state->bullets[i]);
      if (state->bullets[i].bounds.y < 0) {
        state->bullets[i].alive = false;
      }
    }
  }
  m_game_handle_collisions(state);
}

static inline void m_game_draw_object(Texture2D *sprite_sheet,
                                      const M_GameObject *object) {
  if (object->alive) {
    DrawTexturePro(*sprite_sheet, object->image_region, object->bounds,
                   (Vector2){.x = 0.0f, .y = 0.0f}, 0.0f, WHITE);
  }
}

void m_game_render(Texture2D *sprite_sheet, M_GameState *state) {
  BeginDrawing();

  ClearBackground(BLACK);

  DrawFPS(10, 10);
  m_game_draw_object(sprite_sheet, &state->player);
  for (size_t i = 0; i < MAX_ENEMIES; i++) {
    m_game_draw_object(sprite_sheet, &state->enemies[i]);
  }

  for (size_t i = 0; i < NUM_BULLETS; i++) {
    if (state->bullets[i].alive) {
      DrawRectangleRec(state->bullets[i].bounds, GREEN);
    }
  }

  EndDrawing();
}

void m_game_loop(Texture2D *sprite_sheet, M_GameState *state) {
  while (!WindowShouldClose()) {
    m_game_update(state);
    m_game_render(sprite_sheet, state);
  }
}

void init_game_object_extra(M_GameObject *object, int image_x, int image_y,
                            int xPos, int yPos, int width, int height,
                            bool alive, bool withImage) {
  if (withImage) {
    object->image_region = (Rectangle){.x = image_x * IMAGE_SIZE,
                                       .y = image_y * IMAGE_SIZE,
                                       .width = IMAGE_SIZE,
                                       .height = IMAGE_SIZE};
  }
  object->bounds =
      (Rectangle){.x = xPos, .y = yPos, .width = width, .height = height};
  object->velocity = (Vector2){.x = 0, .y = 0};
  object->alive = alive;
}

void init_game_object_standard(M_GameObject *object, int image_x, int image_y,
                               int xPos, int yPos) {
  init_game_object_extra(object, image_x, image_y, xPos, yPos, OBJECT_SIZE,
                         OBJECT_SIZE, true, true);
}

int m_game_load_objects(M_GameState *state) {
  init_game_object_standard(&state->player, 0, 0, WIDTH / 2 - OBJECT_SIZE / 2,
                            PLAYER_Y_POS);
  state->enemies = malloc(sizeof(M_GameObject) * MAX_ENEMIES);
  if (state->enemies == NULL) {
    return 1;
  }
  state->bullets = malloc(sizeof(M_GameObject) * NUM_BULLETS);
  if (state->bullets == NULL) {
    free(state->enemies);
    return 1;
  }

  for (size_t i = 0; i < MAX_ENEMIES; i++) {
    init_game_object_standard(
        &state->enemies[i], 1, 0,
        ENEMY_START_X + ((i % ENEMIES_PER_ROW) * (OBJECT_SIZE + ENEMY_SPACING)),
        ENEMY_START_Y +
            ((int)(i / ENEMIES_PER_ROW) * (OBJECT_SIZE + ENEMY_SPACING)));
  }

  for (size_t i = 0; i < NUM_BULLETS; i++) {
    init_game_object_extra(&state->bullets[i], -1, -1,
                           WIDTH / 2 - BULLET_WIDTH / 2,
                           PLAYER_Y_POS + OBJECT_SIZE / 2 - BULLET_HEIGHT / 2,
                           BULLET_WIDTH, BULLET_HEIGHT, false, false);
  }
  state->bullet_idx = 0;
  return 0;
}

int m_game_start() {
  Texture2D sprite_sheet = LoadTexture(SPRITESHEET_PATH);
  if (sprite_sheet.id <= 0) {
    printf("Unable to load texture %s\n", SPRITESHEET_PATH);
    return 1;
  }

  M_GameState state;
  if (m_game_load_objects(&state) != 0) {
    printf("Unable to load required game objects\n");
    UnloadTexture(sprite_sheet);
    return 1;
  }

  m_game_loop(&sprite_sheet, &state);

  UnloadTexture(sprite_sheet);
  free(state.enemies);
  free(state.bullets);
  return 0;
}

void m_game_run() {
  InitWindow(WIDTH, HEIGHT, "Space Invaders");
  SetConfigFlags(FLAG_VSYNC_HINT);

  m_game_start();

  CloseWindow();
}
