#include <iostream>
#include <conio.h> // Для _kbhit() и _getch()
#include <thread>
#include <chrono>
#include <windows.h> // Для SetConsoleCursorPosition

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
void draw(int paddle1Y, int paddle2Y, Ball ball) {
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
}

int main() {
    int paddle1Y = HEIGHT / 2; // Начальная позиция первой ракетки
    int paddle2Y = HEIGHT / 2; // Начальная позиция второй ракетки

    Ball ball = { WIDTH / 2, HEIGHT / 2, 1, 1 }; // Начальная позиция и направление мяча

    // Инициализация консоли
    gotoXY(0, 0); // Перемещаем курсор в верхний левый угол
    draw(paddle1Y, paddle2Y, ball);

    while (true) {
        // Обработка ввода с клавиатуры
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'w' && paddle1Y > 1) {
                paddle1Y--; // Движение первой ракетки вверх
            }
            if (ch == 's' && paddle1Y < HEIGHT - 2) {
                paddle1Y++; // Движение первой ракетки вниз
            }
            if (ch == 'i' && paddle2Y > 1) {
                paddle2Y--; // Движение второй ракетки вверх
            }
            if (ch == 'k' && paddle2Y < HEIGHT - 2) {
                paddle2Y++; // Движение второй ракетки вниз
            }
            if (ch == 'q') {
                break; // Выход из программы
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

        // Проверка на выход мяча за границы
        if (ball.x <= 0 || ball.x >= WIDTH - 1) {
            ball.x = WIDTH / 2; // Сброс мяча в центр
            ball.y = HEIGHT / 2;
            ball.dirX = 1; // Сброс направления
            ball.dirY = 1;
        }

        // Перемещение курсора в верхний левый угол
        gotoXY(0, 0);
        draw(paddle1Y, paddle2Y, ball);
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Уменьшена задержка для более плавного управления
    }

    return 0;
}
