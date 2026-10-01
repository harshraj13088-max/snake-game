#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

using namespace std;

const int WIDTH = 30;
const int HEIGHT = 15;

struct Point {
    int x;
    int y;
};

vector<Point> snake;
Point food;

char direction = 'd';
bool gameOver = false;
int score = 0;

// Check if keyboard key is pressed
bool keyPressed() {
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);

    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    int ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

// Generate food
void generateFood() {

    food.x = rand() % WIDTH;
    food.y = rand() % HEIGHT;

    for (auto part : snake) {
        if (part.x == food.x && part.y == food.y) {
            generateFood();
            return;
        }
    }
}

// Draw the game
void draw() {

    system("clear");

    cout << "================================" << endl;
    cout << "          🐍 SNAKE GAME         " << endl;
    cout << "================================" << endl;

    cout << "Score: " << score << endl << endl;

    for (int y = 0; y < HEIGHT + 2; y++) {

        for (int x = 0; x < WIDTH + 2; x++) {

            // Wall
            if (x == 0 || x == WIDTH + 1 ||
                y == 0 || y == HEIGHT + 1) {

                cout << "#";
            }

            // Snake head
            else if (x - 1 == snake[0].x &&
                     y - 1 == snake[0].y) {

                cout << "O";
            }

            else {

                bool body = false;

                // Snake body
                for (int i = 1; i < snake.size(); i++) {

                    if (x - 1 == snake[i].x &&
                        y - 1 == snake[i].y) {

                        cout << "o";
                        body = true;
                        break;
                    }
                }

                if (body) {
                    continue;
                }

                // Food
                if (x - 1 == food.x &&
                    y - 1 == food.y) {

                    cout << "*";
                }
                else {
                    cout << " ";
                }
            }
        }

        cout << endl;
    }

    cout << endl;
    cout << "W = Up | S = Down | A = Left | D = Right" << endl;
    cout << "Q = Quit" << endl;
}

// Get keyboard input
void input() {

    if (keyPressed()) {

        char key = getchar();

        if (key == 'w' || key == 'W') {

            if (direction != 's')
                direction = 'w';
        }

        else if (key == 's' || key == 'S') {

            if (direction != 'w')
                direction = 's';
        }

        else if (key == 'a' || key == 'A') {

            if (direction != 'd')
                direction = 'a';
        }

        else if (key == 'd' || key == 'D') {

            if (direction != 'a')
                direction = 'd';
        }

        else if (key == 'q' || key == 'Q') {

            gameOver = true;
        }
    }
}

// Game logic
void update() {

    Point head = snake[0];

    // Move head
    if (direction == 'w')
        head.y--;

    else if (direction == 's')
        head.y++;

    else if (direction == 'a')
        head.x--;

    else if (direction == 'd')
        head.x++;

    // Wall collision
    if (head.x < 0 ||
        head.x >= WIDTH ||
        head.y < 0 ||
        head.y >= HEIGHT) {

        gameOver = true;
        return;
    }

    // Snake collision
    for (auto part : snake) {

        if (head.x == part.x &&
            head.y == part.y) {

            gameOver = true;
            return;
        }
    }

    // Add new head
    snake.insert(snake.begin(), head);

    // Eat food
    if (head.x == food.x &&
        head.y == food.y) {

        score += 10;

        generateFood();
    }
    else {

        // Remove last part
        snake.pop_back();
    }
}

int main() {

    srand(time(0));

    // Starting snake
    snake.push_back({10, 7});
    snake.push_back({9, 7});
    snake.push_back({8, 7});

    // Create first food
    generateFood();

    // Game loop
    while (!gameOver) {

        draw();

        input();

        update();

        // Game speed
        usleep(120000);
    }

    // Game over screen
    system("clear");

    cout << endl;
    cout << "================================" << endl;
    cout << "           GAME OVER            " << endl;
    cout << "================================" << endl;
    cout << endl;
    cout << "Final Score: " << score << endl;
    cout << endl;

    return 0;
}