#include "game.h"
#include "constants.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

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
  Texture2D sprite_sheet;
} M_GameState;

int load_game_state();
void handle_movement();
void handle_collisions();
void update_object_pos(M_GameObject *object, bool restrict_bounds);
void init_game_object_standard(M_GameObject *object, int image_x, int image_y, int xPos, int yPos);
void init_game_object_extra(M_GameObject *object, int image_x, int image_y, int xPos, int yPos, int width, int height, bool alive, bool withImage);
void move_enemies_down(M_GameObject *enemies);
static inline void m_game_draw_object(const M_GameObject *object);

static M_GameState m_state;

int m_game_init() {
  if (load_game_state() != 0) {
    printf("Unable to load required game objects\n");
    return 1;
  }

  return 0;
}

int m_game_update() {
  handle_movement();
  handle_collisions();
  return 0;
}

int m_game_render() {
  BeginDrawing();

  ClearBackground(BLACK);

  DrawFPS(10, 10);
  m_game_draw_object(&m_state.player);
  for (size_t i = 0; i < MAX_ENEMIES; i++) {
    m_game_draw_object(&m_state.enemies[i]);
  }

  for (size_t i = 0; i < NUM_BULLETS; i++) {
    if (m_state.bullets[i].alive) {
      DrawRectangleRec(m_state.bullets[i].bounds, GREEN);
    }
  }

  EndDrawing();
  return 0;
}

void m_game_destroy() {
  UnloadTexture(m_state.sprite_sheet);
  free(m_state.enemies);
  free(m_state.bullets);
}

int load_game_state() {
  m_state.sprite_sheet = LoadTexture(SPRITESHEET_PATH);
  if (m_state.sprite_sheet.id <= 0) {
    printf("Unable to load texture %s\n", SPRITESHEET_PATH);
    return 1;
  }
  printf("Loaded state\n");

  init_game_object_standard(&m_state.player, 0, 0, WIDTH / 2 - OBJECT_SIZE / 2, PLAYER_Y_POS);
  m_state.enemies = malloc(sizeof(M_GameObject) * MAX_ENEMIES);
  if (m_state.enemies == NULL) {
    UnloadTexture(m_state.sprite_sheet);
    return 1;
  }
  m_state.bullets = malloc(sizeof(M_GameObject) * NUM_BULLETS);
  if (m_state.bullets == NULL) {
    UnloadTexture(m_state.sprite_sheet);
    free(m_state.enemies);
    return 1;
  }

  for (size_t i = 0; i < MAX_ENEMIES; i++) {
    init_game_object_standard(
        &m_state.enemies[i], 1, 0,
        ENEMY_START_X + ((i % ENEMIES_PER_ROW) * (OBJECT_SIZE + ENEMY_SPACING)),
        ENEMY_START_Y + ((int)(i / ENEMIES_PER_ROW) * (OBJECT_SIZE + ENEMY_SPACING)));
    m_state.enemies[i].velocity = (Vector2){.x = -ENEMY_SPEED, .y = 0};
  }

  for (size_t i = 0; i < NUM_BULLETS; i++) {
    init_game_object_extra(&m_state.bullets[i], -1, -1,
                           WIDTH / 2 - BULLET_WIDTH / 2,
                           PLAYER_Y_POS + OBJECT_SIZE / 2 - BULLET_HEIGHT / 2,
                           BULLET_WIDTH, BULLET_HEIGHT, false, false);
  }
  m_state.bullet_idx = 0;
  return 0;
}

void handle_movement() {
  if (IsKeyDown(KEY_A)) {
    m_state.player.velocity.x = -PLAYER_SPEED;
  } else if (IsKeyDown(KEY_D)) {
    m_state.player.velocity.x = PLAYER_SPEED;
  } else {
    m_state.player.velocity.x = 0;
  }

  update_object_pos(&m_state.player, true);
  if (IsKeyPressed(KEY_SPACE) && !m_state.bullets[m_state.bullet_idx].alive) {
    m_state.bullets[m_state.bullet_idx].bounds.x = m_state.player.bounds.x + OBJECT_SIZE / 2 - BULLET_WIDTH / 2;
    m_state.bullets[m_state.bullet_idx].bounds.y = m_state.player.bounds.y + OBJECT_SIZE / 2 - BULLET_HEIGHT / 2;
    m_state.bullets[m_state.bullet_idx].alive = true;
    m_state.bullets[m_state.bullet_idx].velocity.y = -BULLET_SPEED;
    m_state.bullet_idx = (m_state.bullet_idx + 1) % NUM_BULLETS;
  }

  for (size_t i = 0; i < MAX_ENEMIES; i++) {
    if (m_state.enemies[i].alive) {
      update_object_pos(&m_state.enemies[i], true);
      if (m_state.enemies[i].bounds.x <= 0 || m_state.enemies[i].bounds.x >= WIDTH - OBJECT_SIZE) {
        move_enemies_down(m_state.enemies);
      }
    }
  }

  for (size_t i = 0; i < NUM_BULLETS; i++) {
    if (m_state.bullets[i].alive) {
      update_object_pos(&m_state.bullets[i], false);
      if (m_state.bullets[i].bounds.y < 0) {
        m_state.bullets[i].alive = false;
      }
    }
  }
}

void handle_collisions() {
  for (size_t i = 0; i < NUM_BULLETS; i++) {
    if (m_state.bullets[i].alive) {
      for (size_t j = 0; j < MAX_ENEMIES; j++) {
        if (m_state.enemies[j].alive && CheckCollisionRecs(m_state.bullets[i].bounds, m_state.enemies[j].bounds)) {
          m_state.enemies[j].alive = false;
          m_state.bullets[i].alive = false;
          break;
        }
      }
    }
  }
}

void init_game_object_standard(M_GameObject *object, int image_x, int image_y, int xPos, int yPos) {
  init_game_object_extra(object, image_x, image_y, xPos, yPos, OBJECT_SIZE,
                         OBJECT_SIZE, true, true);
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


void move_enemies_down(M_GameObject *enemies) {
  for (size_t i = 0; i < MAX_ENEMIES; i++) {
    if (enemies[i].alive) {
      enemies[i].bounds.y += ENEMY_SPACING;
      enemies[i].velocity.x *= -1;
    }
  }
}

void update_object_pos(M_GameObject *object, bool restrict_bounds) {
  object->bounds.x += object->velocity.x * GetFrameTime();
  object->bounds.y += object->velocity.y * GetFrameTime();

  if (restrict_bounds) {
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
}

static inline void m_game_draw_object(const M_GameObject *object) {
  if (object->alive) {
    DrawTexturePro(m_state.sprite_sheet, object->image_region, object->bounds,
                   (Vector2){.x = 0.0f, .y = 0.0f}, 0.0f, WHITE);
  }
}
