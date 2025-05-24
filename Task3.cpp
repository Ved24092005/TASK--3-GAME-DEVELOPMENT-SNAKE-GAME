#include <iostream>
#include <conio.h>   // _kbhit(), _getch()
#include <windows.h> // Sleep()
#include <vector>
#include <cstdlib>   // rand()
#include <ctime>     // time()

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 20;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

struct Point {
    int x, y;
};

class SnakeGame {
    bool gameOver;
    Direction dir;
    Point food;
    vector<Point> snake;

public:
    SnakeGame() {
        gameOver = false;
        dir = STOP;
        snake.push_back({ WIDTH / 2, HEIGHT / 2 });  // initial position
        placeFood();
    }

    void placeFood() {
        srand(time(0));
        while (true) {
            food.x = rand() % (WIDTH - 2) + 1;  // inside border
            food.y = rand() % (HEIGHT - 2) + 1;

            bool onSnake = false;
            for (auto &s : snake) {
                if (s.x == food.x && s.y == food.y) {
                    onSnake = true;
                    break;
                }
            }
            if (!onSnake) break;
        }
    }

    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'w': if (dir != DOWN) dir = UP; break;
                case 's': if (dir != UP) dir = DOWN; break;
                case 'a': if (dir != RIGHT) dir = LEFT; break;
                case 'd': if (dir != LEFT) dir = RIGHT; break;
                case 'x': gameOver = true; break;
            }
        }
    }

    void logic() {
        if (dir == STOP) return;  // don't move until direction chosen

        // Move snake body
        for (int i = snake.size() - 1; i > 0; --i)
            snake[i] = snake[i - 1];

        // Move head
        switch (dir) {
            case LEFT: snake[0].x--; break;
            case RIGHT: snake[0].x++; break;
            case UP: snake[0].y--; break;
            case DOWN: snake[0].y++; break;
            default: break;
        }

        // Check collision with walls (game over)
        if (snake[0].x <= 0 || snake[0].x >= WIDTH - 1 ||
            snake[0].y <= 0 || snake[0].y >= HEIGHT - 1) {
            gameOver = true;
            return;
        }

        // Check collision with self (game over)
        for (int i = 1; i < snake.size(); i++) {
            if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
                gameOver = true;
                return;
            }
        }

        // Check if food eaten
        if (snake[0].x == food.x && snake[0].y == food.y) {
            snake.push_back({ -1, -1 });  // add segment
            placeFood();
        }
    }

    void draw() {
        system("cls");  // clear screen (Windows only)
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1)
                    cout << "#";  // border
                else {
                    bool printed = false;
                    if (x == food.x && y == food.y) {
                        cout << "F";  // food
                        printed = true;
                    } else {
                        for (int k = 0; k < snake.size(); k++) {
                            if (snake[k].x == x && snake[k].y == y) {
                                cout << (k == 0 ? "O" : "o");  // head or body
                                printed = true;
                                break;
                            }
                        }
                    }
                    if (!printed) cout << " ";
                }
            }
            cout << endl;
        }
        cout << "Score: " << snake.size() - 1 << endl;
        cout << "Controls: W A S D to move, X to quit\n";
    }

    void run() {
        while (!gameOver) {
            draw();
            input();
            logic();
            Sleep(100);  // adjust speed here (lower is faster)
        }
        cout << "Game Over! Final Score: " << snake.size() - 1 << endl;
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}
