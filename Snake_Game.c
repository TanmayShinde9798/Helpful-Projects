#include <graphics.h>
#include <conio.h>
#include <dos.h> // For delay()

#define MAX_LENGTH 100
#define BLOCK_SIZE 20
#define WIDTH 640
#define HEIGHT 480

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point body[MAX_LENGTH];
    int length;
    int dir; // 0 = up, 1 = right, 2 = down, 3 = left
} Snake;

Point food;

void initializeGraphics() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TC\\BGI"); // Path to BGI directory
    setfillstyle(SOLID_FILL, GREEN);
    setcolor(GREEN);
    setbkcolor(LIGHTBLUE); // Set background color
}

void drawSnake(Snake *snake) {
    int i;
    setfillstyle(SOLID_FILL, GREEN);
    for (i = 0; i < snake->length; i++) {
        bar(snake->body[i].x, snake->body[i].y, snake->body[i].x + BLOCK_SIZE, snake->body[i].y + BLOCK_SIZE);
    }
}

void drawFood() {
    setfillstyle(SOLID_FILL, RED);
    bar(food.x, food.y, food.x + BLOCK_SIZE, food.y + BLOCK_SIZE);
}

void generateFood() {
    food.x = (rand() % (WIDTH / BLOCK_SIZE)) * BLOCK_SIZE;
    food.y = (rand() % (HEIGHT / BLOCK_SIZE)) * BLOCK_SIZE;
}

void moveSnake(Snake *snake) {
    Point prevHead = snake->body[0];
    Point prevSeg;
    int i;
    switch (snake->dir) {
        case 0: // Up
            snake->body[0].y -= BLOCK_SIZE;
            break;
        case 1: // Right
            snake->body[0].x += BLOCK_SIZE;
            break;
        case 2: // Down
            snake->body[0].y += BLOCK_SIZE;
            break;
        case 3: // Left
            snake->body[0].x -= BLOCK_SIZE;
            break;
    }

    for (i = 1; i < snake->length; i++) {
        prevSeg = snake->body[i];
        snake->body[i] = prevHead;
        prevHead = prevSeg;
    }
}

int checkCollision(Snake *snake) {
    int i;
    // Check wall collision
    if (snake->body[0].x < 0 || snake->body[0].x >= WIDTH || 
        snake->body[0].y < 0 || snake->body[0].y >= HEIGHT) {
        return 1;
    }

    // Check self collision
    for (i = 1; i < snake->length; i++) {
        if (snake->body[0].x == snake->body[i].x && snake->body[0].y == snake->body[i].y) {
            return 1;
        }
    }
    return 0;
}

int checkFoodCollision(Snake *snake) {
    if (snake->body[0].x == food.x && snake->body[0].y == food.y) {
        return 1;
    }
    return 0;
}

void displayInstructions() {
    setcolor(WHITE);
    outtextxy(10, 10, "Use W A S D to move");
    outtextxy(10, 30, "Press ESC to exit");
}

void main() {
    Snake snake;
    char ch;
    int speed = 100;
    int score = 0;
    int i;

    // Initialize snake
    snake.length = 3;
    snake.dir = 1; // Start moving right
    for (i = 0; i < snake.length; i++) {
        snake.body[i].x = WIDTH / 2 - i * BLOCK_SIZE;
        snake.body[i].y = HEIGHT / 2;
    }

    initializeGraphics();
    generateFood();

    while (1) {
        cleardevice(); // Clear the screen to redraw everything
        drawSnake(&snake);
        drawFood();
        displayInstructions(); // Display controls on screen

        if (checkCollision(&snake)) {
            break; // End game on collision
        }

        if (checkFoodCollision(&snake)) {
            score++;
            snake.length++;
            generateFood();
        }

        moveSnake(&snake);

        if (kbhit()) {
            ch = getch();
            if (ch == 27) break; // Exit on ESC
            switch (ch) {
                case 'w': if (snake.dir != 2) snake.dir = 0; break; // Up
                case 'd': if (snake.dir != 3) snake.dir = 1; break; // Right
                case 's': if (snake.dir != 0) snake.dir = 2; break; // Down
                case 'a': if (snake.dir != 1) snake.dir = 3; break; // Left
            }
        }

        delay(speed); // Control game speed
    }

    closegraph();
    printf("Game Over! Your score: %d\n", score);
    getch();
}
