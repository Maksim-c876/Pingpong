#include <iostream>
#include <conio.h> 
#include <thread>
#include <chrono>
#include <windows.h> 

using namespace std;

const int WIDTH = 40; // Ширина игрового поля
const int HEIGHT = 20; // Высота игрового поля

// Структура для представления мяча
struct Ball {
    int x, y; // Позиция мяча
    int dirX, dirY; // Направление движения мяча
};

// Функция для перемещения курсора в консоли
void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Функция для отрисовки игрового поля
void draw(int paddle1Y, int paddle2Y, Ball ball, int score1, int score2) {
    // Отрисовываем верхнюю стенку
    gotoXY(0, 0);
    for (int j = 0; j < WIDTH; ++j) {
        cout << "_"; // Верхняя граница
    }
    cout << endl;

    // Отрисовываем игровое поле и ракетки
    for (int i = 1; i < HEIGHT - 1; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (j == 0) {
                cout << "|"; // Левая граница
            }
            else if (j == WIDTH - 1) {
                cout << "|"; // Правая граница
            }
            else if (j == 1 && i >= paddle1Y - 1 && i <= paddle1Y + 1) {
                cout << "]"; // Первая ракетка (длина 3)
            }
            else if (j == WIDTH - 2 && i >= paddle2Y - 1 && i <= paddle2Y + 1) {
                cout << "["; // Вторая ракетка (длина 3)
            }
            else if (i == ball.y && j == ball.x) {
                cout << "O"; // Мяч
            }
            else {
                cout << " "; // Пустое пространство
            }
        }
        cout << endl;
    }

    // Отрисовываем нижнюю стенку
    for (int j = 0; j < WIDTH; ++j) {
        cout << "_"; // Нижняя граница
    }
    cout << endl;

    // Отображение счета
    gotoXY(WIDTH / 2 - 5, HEIGHT); // Перемещаем курсор для отображения счета
    cout << "Очки: Игрока 1: " << score1 << " | Игрока 2: " << score2 << endl;
}

int main() {
    setlocale(LC_ALL, "ru");
    int paddle1Y, paddle2Y; // Позиции ракеток
    int score1, score2; // Счет игроков
    char restart;

    do {
        paddle1Y = HEIGHT / 2; // Начальная позиция первой ракетки
        paddle2Y = HEIGHT / 2; // Начальная позиция второй ракетки
        score1 = 0; // Счет первого игрока
        score2 = 0; // Счет второго игрока

        Ball ball = { WIDTH / 2, HEIGHT / 2, 1, 1 }; // Начальная позиция и направление мяча

        // Инициализация консоли
        gotoXY(0, 0); // Перемещаем курсор в верхний левый угол
        draw(paddle1Y, paddle2Y, ball, score1, score2);

        while (true) {
            if (_kbhit()) {
                char ch = _getch();
                if (ch == 'w' && paddle1Y > 1) {
                    paddle1Y--;
                }
                if (ch == 's' && paddle1Y < HEIGHT - 2) {
                    paddle1Y++;
                }
                if (ch == 'i' && paddle2Y > 1) {
                    paddle2Y--;
                }
                if (ch == 'k' && paddle2Y < HEIGHT - 2) {
                    paddle2Y++;
                }
                if (ch == 'q') {
                    gotoXY(0, HEIGHT + 2);
                    cout << "Игра закончена." << endl;
                    return 0;
                }
            }

            // Движение мяча
            ball.x += ball.dirX;
            ball.y += ball.dirY;

            // Проверка на столкновение с верхней и нижней границей
            if (ball.y <= 1 || ball.y >= HEIGHT - 2) { // Изменение границ на 1, чтобы учитывать стенки
                ball.dirY *= -1; // Изменяем направление по Y
            }

            // Проверка на столкновение с ракетками
            if (ball.x == 2 && ball.y >= paddle1Y - 1 && ball.y <= paddle1Y + 1) {
                ball.dirX *= -1; // Изменяем направление по X
            }
            if (ball.x == WIDTH - 3 && ball.y >= paddle2Y - 1 && ball.y <= paddle2Y + 1) {
                ball.dirX *= -1; // Изменяем направление по X
            }

            if (ball.x <= 0) { // Мяч выходит за левую границу
                score2++; // Увеличиваем счет второго игрока
                if (score2 >= 5) break; // Проверяем, не достиг ли второй игрок 5 очков
                ball.x = WIDTH / 2; // Сброс мяча в центр
                ball.y = HEIGHT / 2;
                ball.dirX = 1; // Сброс направления
                ball.dirY = 1;
            }
            if (ball.x >= WIDTH - 1) { // Мяч выходит за правую границу
                score1++; // Увеличиваем счет первого игрока
                if (score1 >= 5) break; // Проверяем, не достиг ли первый игрок 5 очков
                ball.x = WIDTH / 2; // Сброс мяча в центр
                ball.y = HEIGHT / 2;
                ball.dirX = -1; // Сброс направления
                ball.dirY = 1;
            }

            // Перемещение курсора в верхний левый угол
            gotoXY(0, 0);
            draw(paddle1Y, paddle2Y, ball, score1, score2);
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Уменьшена задержка для более плавного управления
        }

        // Предложение начать заново
        gotoXY(WIDTH / 2 - 10, HEIGHT + 2);
        cout << "Игра закончена! Игрок 1: " << score1 << " | Игрок 2: " << score2 << endl;
        cout << "Начать заново? (y/n): ";
        cin >> restart;

    } while (restart == 'y' || restart == 'Y');

    return 0;
}
