#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Точности для вычислений
static double eps1 = 0.001;  // для нахождения корней
static double eps2 = 0.001;  // для вычисления интегралов
int cnt;  // счетчик итераций (используется в root())

extern float f1(float x);
extern float f2(float x);
extern float f3(float x);

// Функция, возвращающая значение одной из трех заданных функций
// f: exp(x) + 2
// g: -2x + 8
// h: -5 / x
float func(float x, char f) {
    switch (f) {
        case 'f': return f1(x);
        case 'g': return f2(x);
        case 'h': return f3(x);
        default:
            printf("Error!\n");
            break;
    }
    return -1;
}

// Функция нахождения корня уравнения, методом деления отрезка пополам
float root(char f, char g, float a, float b, float eps) {
    cnt = 0;  // обнуляем счетчик итераций
    float x = a;

    // Если границы перепутаны - меняем их местами
    if (b < a) {
        float temp = a;
        a = b;
        b = temp;
    }

    // Основной цикл метода деления отрезка пополам
    while (eps < fabsf(a - b)) {
        cnt++;
        // Проверяем условие смены границы
        if ((func(a, f) - func(a, g)) * (func((b - fabsf(a - b) / 2), f) - func((b - fabsf(a - b) / 2), g)) < 0) {
            b = b - fabsf(a - b) / 2;
        } else {
            a = a + fabsf(a - b) / 2;
        }
        x = a;
    }
    return x;
}

// Функция вычисления определенного интеграла методом трапеций
float integral(char f, float a, float b, float eps) {
    int n = 4;  // начальное количество разбиений
    float h = (b - a) / n;  // шаг

    float I1 = (func(a, f) + func(b, f)) / 2, I2;  // начальное приближение

    // Первое приближение интеграла
    for (int i = 1; i < n; i++) {
        I1 += func(a + i * h, f);
    }

    I2 = I1;

    // Уточнение интеграла с увеличением количества разбиений
    do {
        I1 = I2 * h;  // предыдущее значение интеграла

        n *= 2;  // удваиваем количество разбиений
        h = (b - a) / n;  // новый шаг

        // Добавляем только новые точки
        for (int i = 1; i < n; i += 2) {
            I2 += func(a + i * h, f);
        }

    } while (fabsf(I1 - I2 * h) / 3 > eps);  // проверка точности

    return I2 * h;
}

int main(int argc, char *argv[]) {
    // Проверка наличия аргументов командной строки
    if (argc < 2) {
        printf("Error\n");
        printf("Используете -help для получения списка команд\n");
        return 1;
    }

    // Структура для хранения точек пересечения функций
    struct point {
        float x;
        float y;
        int cnt;  // количество итераций для нахождения этой точки
    };

    // Проверка корректности работы интегралов
    if (integral('h', -5, 5, eps2) - 168.406616 < eps2) {
        if (integral('g', -5, 5, eps2) - 80.000000 < eps2) {
            if (integral('f', -2, -1, eps2) - 3.466041 < eps2) {
                // Находим точки пересечения функций
                struct point p1, p2, p3, p4;

                p1.x = root('f', 'h', -5, 0, eps1);
                p1.y = func(p1.x, 'h');
                p1.cnt = cnt;

                p2.x = root('g', 'h', -5, 0, eps1);
                p2.y = func(p2.x, 'g');
                p2.cnt = cnt;

                p3.x = root('f', 'g', 0, 2, eps1);
                p3.y = func(p3.x, 'g');
                p3.cnt = cnt;

                p4.x = root('g', 'h', 2, 5, eps1);
                p4.y = func(p4.x, 'g');
                p4.cnt = cnt;

                // Вычисление площади фигуры между кривыми
                float s = integral('h', p1.x, p2.x, eps2)
                        + integral('g', p2.x, p3.x, eps2)
                        - integral('f', p1.x, p3.x, eps2);

                // Обработка команд из аргументов
                if (strcmp(argv[1], "-help") == 0) {
                    printf("-area площадь фигуры между точкам\n-pdoc точки пересечения\n-pcoi количество итераций\n-tr тест функции корня (input : f g a b exp)\n-ti тест функции интеграла (input : f a b exp)\n");

                } else if (strcmp(argv[1], "-pdoc") == 0) {
                    // Вывод точек пересечения
                    printf("x = %f, y = %f; p1\n", p1.x, p1.y);
                    printf("x = %f, y = %f; p2\n", p2.x, p2.y);
                    printf("x = %f, y = %f; p3\n", p3.x, p3.y);
                    printf("x = %f, y = %f; p4\n", p4.x, p4.y);
                } else if (strcmp(argv[1], "-pcoi") == 0) {
                    // Вывод количества итераций для нахождения точек
                    printf("f = g; %d - iterations\n", p1.cnt);
                    printf("g = h; %d - iterations\n", p2.cnt);
                    printf("g = h; %d - iterations\n", p3.cnt);
                    printf("h = f; %d - iterations\n", p4.cnt);
                } else if (strcmp(argv[1], "-tr") == 0) {
                    // Тестирование функции root()
                    if (argc < 7) {
                        printf("Не достаточно аргументов для -tr\n");
                        return 1;
                    }
                    char f = argv[2][0];
                    char g = argv[3][0];
                    float a = atof(argv[4]);
                    float b = atof(argv[5]);
                    float eps3 = atof(argv[6]);
                    printf("x = %f, y = %f; %c = %c\n", root(f, g, a, b, eps3), func(root(f, g, a, b, eps3), f), f, g);
                } else if (strcmp(argv[1], "-ti") == 0) {
                    // Тестирование функции integral()
                    if (argc < 6) {
                        printf("Не достаточно аргументов -ti\n");
                        return 1;
                    }
                    char f = argv[2][0];
                    float a = atof(argv[3]);
                    float b = atof(argv[4]);
                    float eps3 = atof(argv[5]);
                    printf("s = %f; %c[%f, %f]\n", integral(f, a, b, eps3), f, a, b);
                } else if (strcmp(argv[1], "-area") == 0) {
                    // Вывод вычисленной площади
                    printf("Area is %f\n", s);
                } else {
                    printf("Неизвестная команда: %s\n", argv[1]);
                }
            }
        }
    }

    return 0;
}
