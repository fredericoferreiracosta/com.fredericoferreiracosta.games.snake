#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>

using namespace std;

bool gameOver;
const int WIDTH = 36;
const int HEIGHT = 14;
int speedY = 200; // The lesser the faster
int speedX = 100;
int snakeX[100], snakeY[100], fruitX, fruitY;
int snakeSize;
int score;
enum
{
    STOP,
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction;

void generateNewFruitPosition()
{
    bool validPosition = false;
    while (!validPosition)
    {
        fruitX = rand() % WIDTH;
        if (fruitX != 0 && fruitX != WIDTH)
            validPosition = true;
        if (!validPosition)
        {
            for (int i = 0; i <= snakeSize; i++)
            {
                if (snakeX[i] == fruitX)
                {
                    validPosition = false;
                    break;
                }
                validPosition = true;
            }
        }
    }
    validPosition = false;
    while (!validPosition)
    {
        fruitY = rand() % HEIGHT;
        if (fruitY != 0 && fruitY != HEIGHT)
            validPosition = true;
        if (!validPosition)
        {
            for (int i = 0; i <= snakeSize; i++)
            {
                if (snakeY[i] == fruitY)
                {
                    validPosition = false;
                    break;
                }
                validPosition = true;
            }
        }
    }
}

void setup()
{
    gameOver = false;
    score = 0;
    snakeX[0] = WIDTH / 2;
    snakeY[0] = HEIGHT / 2;
    snakeSize = 0;
    direction = STOP;
    timeout(speedX);
    generateNewFruitPosition();
}

void draw()
{
    system("clear");
    for (int  i = 0; i <= HEIGHT; i++)
    {
        for (int j = 0; j <= WIDTH; j++)
        {
            if (i == 0 || i == HEIGHT || j == 0 || j == WIDTH)
            {
                printf("#");
            }
            else if (i == fruitY && j == fruitX)
            {
                printf("F");
            }
            else if (i == snakeY[0] && j == snakeX[0])
            {
                printf("O");
            }
            else
            {
                bool placeTaken = false;
                for (int z = 1; z <= snakeSize; z++)
                {
                    if (i == snakeY[z] && j == snakeX[z])
                    {
                        printf("o");
                        placeTaken = true;
                    }
                }

                if (!placeTaken)
                {
                    printf(" ");
                }
            }
        }
        printf("\n\r");
    }
    printf("Score: %d\n", score);
}

void input()
{
    switch (getch())
    {
        case 120: //x
            gameOver = true;
            break;
        case 119: //w
            if (direction != DOWN)
                direction = UP;
            break;
        case 115: //s
            if (direction != UP)
                direction = DOWN;
            break;
        case 97: //a
            if (direction != RIGHT)
                direction = LEFT;
            break;
        case 100: //d
            if (direction != LEFT)
                direction = RIGHT;
            break;
    }
}

void logic()
{
    int tempX = snakeX[0];
    int tempY = snakeY[0];

    /* Movement */
    if (direction != STOP)
    {
        for (int i = 1; i <= snakeSize; i++)
        {
            int currentElementPositionX = snakeX[i];
            int currentElementPositionY = snakeY[i];
            snakeX[i] = tempX;
            snakeY[i] = tempY;
            tempX = currentElementPositionX;
            tempY = currentElementPositionY;
        }
    }

    /* Movement */
    switch (direction)
    {
        case UP:
            snakeY[0]--;
            timeout(speedY);
            break;
        case DOWN:
            snakeY[0]++;
            timeout(speedY);
            break;
        case LEFT:
            snakeX[0]--;
            timeout(speedX);
            break;
        case RIGHT:
            snakeX[0]++;
            timeout(speedX);
            break;
    }

    /* Validation - Checkpoint */
    if (snakeX[0] == fruitX && snakeY[0] == fruitY)
    {
        score+= 10;
        snakeSize++;
        speedX-=3;
        speedY-=6;
        generateNewFruitPosition();
    }

    /* Movement - Prevent to move against the wall */
    if (snakeX[0] == WIDTH)
    {
        snakeX[0] = 1;
    }
    else if (snakeX[0] == 0)
    {
        snakeX[0] = WIDTH - 1;
    }

    if (snakeY[0] == HEIGHT)
    {
        snakeY[0] = 1;
    }
    else if (snakeY[0] == 0)
    {
        snakeY[0] = HEIGHT - 1;
    }

    /* Validation */
    for (int i = 1; i <= snakeSize; i++)
    {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
        {
            gameOver = true;
            break;
        }
    }

}

void gameOverOptions()
{
    printw("\n\nGame Over!!!");
    printw("\n\nPress X to exit...!!!");
    refresh();
    while(true)
    {
        if (getch() == 120)
        {
            break;
        }
    }
}

int main()
{
    initscr();
    noecho();
    setup();
    while (!gameOver)
    {
        draw();
        input();
        logic();
    }
    gameOverOptions();
    endwin();
    return 0;
}