#include <stdio.h>
#include "raylib.h"
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#define MAX_SNAKE_SIZE 256
#define TILE_SIZE 32
#define GRID_WIDTH 44
#define GRID_HEIGHT 22
#define MOVE_INTERVAL 0.15f

//Actions (wasd)
typedef enum ActionType{
    NO_ACTION = 0,
    ACTION_UP = KEY_W,
    ACTION_DOWN = KEY_S,
    ACTION_LEFT = KEY_A,
    ACTION_RIGHT = KEY_D,
}ActionType;

//struct for 2d coordinates
typedef struct {
    i16 x, y;
}vector2i;

//struct for the snake
typedef struct {
    vector2i body[MAX_SNAKE_SIZE];
    int length;
    vector2i speed;
}Snake;

//struct for the apples
typedef struct {
    vector2i position;
}Apple;

bool isOnSnake(vector2i pos, Snake snake);


bool isOnSnake(const vector2i pos, const Snake snake) {
    for (u16 i = 0; i < snake.length; i++) {
        if (snake.body[i].x == pos.x && snake.body[i].y == pos.y) {
            return true; //found an overlap
        }
    }
    return false;
}



vector2i prevBody[MAX_SNAKE_SIZE];
vector2i inputQueue[2];
f32 moveTimer = 0.0f;
bool gameOver = false;

int main() {
    srand(time(NULL)); //seed once

    //initialize Window {880x440 matching our 44x22 grid of 20px tiles}
    const i16 screenWidth = GRID_WIDTH * TILE_SIZE;
    const i16 screenHeight = GRID_HEIGHT * TILE_SIZE;
    InitWindow(screenWidth, screenHeight, "Snake Game");
    SetTargetFPS(240);

    Snake snake = {0};
    snake.length = 3;
    snake.speed = (vector2i){1, 0}; //moving right

    //start snake in the middle
    snake.body[0] = (vector2i){12, 10};
    snake.body[1] = (vector2i){11, 10};
    snake.body[2] = (vector2i){10, 10};

    Apple apple = {(vector2i){rand() % GRID_WIDTH, rand() % GRID_HEIGHT}};

    i16 score = 0;
    vector2i nextSpeed = snake.speed;


    //Main game loop
    while (!WindowShouldClose()) {

        //check round status
        if (gameOver && IsKeyPressed(KEY_SPACE)) {
            snake.length = 3;
            snake.speed = (vector2i){1, 0};
            nextSpeed = (vector2i){1, 0};

            snake.body[0] = (vector2i){12, 10};
            snake.body[1] = (vector2i){11, 10};
            snake.body[2] = (vector2i){10, 10};

            apple = (Apple){(vector2i){rand() % GRID_WIDTH, rand() % GRID_HEIGHT}};
            score = 0;

            //reset prevBody to match the fresh body so there's no visual "slide" on the first frame after restart
            for (u16 i = 0; i < snake.length; i++) {
                prevBody[i] = snake.body[i];
            }

            gameOver = false;
        }


        //Controls wasd or arrows
        if ((IsKeyPressed(ACTION_UP) || IsKeyPressed(KEY_UP)) && nextSpeed.y == 0) {nextSpeed = (vector2i){0, -1};}
        else if ((IsKeyPressed(ACTION_DOWN) || IsKeyPressed(KEY_DOWN)) && nextSpeed.y == 0) {nextSpeed = (vector2i){0, 1};}
        else if ((IsKeyPressed(ACTION_RIGHT) || IsKeyPressed(KEY_RIGHT)) && nextSpeed.x == 0) {nextSpeed = (vector2i){1, 0};}
        else if ((IsKeyPressed(ACTION_LEFT) || IsKeyPressed(KEY_LEFT)) && nextSpeed.x == 0) {nextSpeed = (vector2i){-1, 0};}


        if (!gameOver) {

            moveTimer += GetFrameTime();
            if (moveTimer >= MOVE_INTERVAL) {
                moveTimer -= MOVE_INTERVAL;
                snake.speed = nextSpeed;

                for (u16 i = 0; i < snake.length; i++) {
                    prevBody[i] = snake.body[i]; //save current positions before moving for interpolation
                }

                //shift body positions
                for (u16 i = snake.length - 1; i > 0; i--) {
                    snake.body[i] = snake.body[i - 1];
                }

                //move head
                snake.body[0].x += snake.speed.x;
                snake.body[0].y += snake.speed.y;

                //wall collision
                if (snake.body[0].x < 0 || snake.body[0].x >= GRID_WIDTH || snake.body[0].y < 0 || snake.body[0].y >= GRID_HEIGHT) {
                    gameOver = true;
                }

                //self collision
                for (u16 i = 1; i < snake.length; i++) {
                    if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y) {
                        gameOver = true;
                        break;
                    }
                }


                //checks if the snake eats the apple
                if (snake.body[0].x == apple.position.x && snake.body[0].y == apple.position.y) {
                    if (snake.length < MAX_SNAKE_SIZE) {
                        snake.length++;
                    }
                    score += 10;

                    vector2i newPos;
                    do {
                        newPos.x = rand() % GRID_WIDTH;
                        newPos.y = rand() % GRID_HEIGHT;
                    }while (isOnSnake(newPos, snake));
                    apple.position = newPos;
                }
            }
        }

        //render
        BeginDrawing();
        ClearBackground(BLACK);
        const f32 t = moveTimer / MOVE_INTERVAL;

        //score board
        DrawText(TextFormat("Score %d", score), 15, 15, 24, BLUE);

        //apple
        DrawRectangle(apple.position.x * TILE_SIZE, apple.position.y * TILE_SIZE, TILE_SIZE - 2, TILE_SIZE - 2, RED);

        //draw snake
        for (u32 i = 0; i < snake.length; i++) {
            f32 drawX = prevBody[i].x + (snake.body[i].x - prevBody[i].x) * t;
            f32 drawY = prevBody[i].y + (snake.body[i].y - prevBody[i].y) * t;


            const Color segementColor = i == 0 ? DARKPURPLE : PURPLE;
            DrawRectangle((int) (drawX * TILE_SIZE), (int) (drawY * TILE_SIZE), TILE_SIZE - 2, TILE_SIZE - 2, segementColor);
        }

        //game over overlay
        if (gameOver) {
            DrawText(TextFormat("GAME OVER!"), 570, 300, 32, WHITE);
            DrawText(TextFormat("Press SPACE to restart"), 570, 330, 20, WHITE);
        }

        EndDrawing();
    }

    //De-initialization
    CloseWindow();
    return 0;
}
