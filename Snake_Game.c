#include <graphics.h>
#include <conio.h>
#include <dos.h>  // For delay()

#define MAX_LENGTH 100
#define BLOCK_SIZE 20
#define WIDTH 640
#define HEIGHT 480

// Structures for Snake and Food
typedef struct 
{
    int x, y;
} Point;

typedef struct 
{
    Point body[MAX_LENGTH];
    int length;
    int dir;  // 0 = Up, 1 = Right, 2 = Down, 3 = Left
} Snake;

Point food;

// Initialize graphics mode
void initializeGraphics() 
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");  // Change path if needed

    setbkcolor(LIGHTGRAY);  // Background color
    setcolor(WHITE);    // Default color (used for border and text)
    cleardevice();
}

// Draw the game border inside the screen
void drawBorder() 
{
    setcolor(WHITE);  // Border color
    rectangle(1, 1, WIDTH - 2, HEIGHT - 2);
}

// Draw the snake
void drawSnake(Snake *snake) 
{
    int i;
    setfillstyle(SOLID_FILL, LIGHTGREEN);  // Snake color
    for (i = 0; i < snake->length; i++) 
    {
        bar(snake->body[i].x, snake->body[i].y, 
            snake->body[i].x + BLOCK_SIZE, snake->body[i].y + BLOCK_SIZE);
    }
}

// Draw food
void drawFood() 
{
    setfillstyle(SOLID_FILL, RED);  // Food color
    bar(food.x, food.y, food.x + BLOCK_SIZE, food.y + BLOCK_SIZE);
}

// Generate new food position
void generateFood() 
{
    food.x = (rand() % ((WIDTH - BLOCK_SIZE) / BLOCK_SIZE)) * BLOCK_SIZE;
    food.y = (rand() % ((HEIGHT - BLOCK_SIZE) / BLOCK_SIZE)) * BLOCK_SIZE;
}

// Move the snake
void moveSnake(Snake *snake) 
{
    Point prevHead = snake->body[0];
    Point prevSeg;
    int i;

    // Move the head in the current direction
    switch (snake->dir) 
    {
        case 0: snake->body[0].y -= BLOCK_SIZE; break;  // Up
        case 1: snake->body[0].x += BLOCK_SIZE; break;  // Right
        case 2: snake->body[0].y += BLOCK_SIZE; break;  // Down
        case 3: snake->body[0].x -= BLOCK_SIZE; break;  // Left
    }

    // Move the body segments
    for (i = 1; i < snake->length; i++) 
    {
        prevSeg = snake->body[i];
        snake->body[i] = prevHead;
        prevHead = prevSeg;
    }
}

// Check for collision with walls or itself
int checkCollision(Snake *snake) 
{
    int i;
    
    // Check wall collision
    if (snake->body[0].x <= 0 || snake->body[0].x >= WIDTH - BLOCK_SIZE || 
        snake->body[0].y <= 0 || snake->body[0].y >= HEIGHT - BLOCK_SIZE) 
    {
        return 1;
    }

    // Check self-collision
    for (i = 1; i < snake->length; i++) 
    {
        if (snake->body[0].x == snake->body[i].x && 
            snake->body[0].y == snake->body[i].y) {
            return 1;
        }
    }

    return 0;
}

// Check if snake eats food
int checkFoodCollision(Snake *snake) 
{
    return (snake->body[0].x == food.x && snake->body[0].y == food.y);
}

// Display game instructions
void displayInstructions() 
{
    setcolor(YELLOW);  // Text color
    outtextxy(10, 10, "Use W A S D to move");
    outtextxy(10, 30, "Press ESC to exit");
}

// Main game function
void main() 
{
    Snake snake;
    char ch;
    int speed = 150;  // Adjusted for slower snake speed
    int score = 0;
    int i;

    // Initialize the snake
    snake.length = 3;
    snake.dir = 1;  // Start moving right
    for (i = 0; i < snake.length; i++) 
    {
        snake.body[i].x = WIDTH / 2 - i * BLOCK_SIZE;
        snake.body[i].y = HEIGHT / 2;
    }

    initializeGraphics();
    generateFood();

    while (1) 
    {
        cleardevice();  // Clear screen before redrawing
        drawBorder();   // Draw the border
        drawSnake(&snake);
        drawFood();
        displayInstructions();

        // Check for collisions
        if (checkCollision(&snake)) 
        {
            break;  // Game over
        }

        // Check if snake eats food
        if (checkFoodCollision(&snake)) 
        {
            score++;
            snake.length++;
            generateFood();
        }

        moveSnake(&snake);

        // Handle user input
        if (kbhit()) {
            ch = getch();
            if (ch == 27) break;  // ESC to exit
            switch (ch) {
                case 'w': if (snake.dir != 2) snake.dir = 0; break;  // Up
                case 'd': if (snake.dir != 3) snake.dir = 1; break;  // Right
                case 's': if (snake.dir != 0) snake.dir = 2; break;  // Down
                case 'a': if (snake.dir != 1) snake.dir = 3; break;  // Left
            }
        }

        delay(speed);  // Adjusted speed for smoother gameplay
    }

    closegraph();
    printf("Game Over! Your score: %d\n", score);
    getch();
}
