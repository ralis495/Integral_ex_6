#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

static double eps1 = 0.001;
static double eps2 = 0.001;
int cnt;

//extern float f1(float x);
//extern float f2(float x);
//extern float f3(float x);

float func(float x, char f) {
    switch (f) {
        case 'f': return exp(x) + 2;
        case 'g': return -2*x + 8;
        case 'h': return -5 / x;
        default:
            printf("Error!\n");
            break;
    }
    return -1;
}

float root(char f, char g, float a, float b, float eps) {
    cnt = 0;
    float x = a;

    if (b < a) {
        float temp;
        temp = a;
        a = b;
        b = temp;
    }

    while (eps < fabsf(a - b)) {
        cnt++;
        if ((func(a, f) - func(a, g)) * (func((b - fabsf(a - b) / 2), f) - func((b - fabsf(a - b) / 2), g)) < 0) {
            b = b - fabsf(a - b) / 2;
        } else {
            a = a + fabsf(a - b) / 2;
        }
        x = a;
    }
    return x;
}

float integral(char f, float a, float b, float eps) {
    int n = 4;
    float h = (b - a) / n;

    float I1 = (func(a, f) + func(b, f)) / 2, I2;

    for (int i = 1; i < n; i++) {
        I1 += func(a + i * h, f);
    }

    I2 = I1;

    do{
        I1 = I2 * h;

        n *= 2;
        h = (b - a) / n;

        for (int i = 1; i < n; i += 2) {
            I2 += func(a + i * h, f);
        }

    } while (fabsf(I1 - I2 * h) / 3 > eps);

    return I2 * h;
}

int main(void) {
    struct point{
        float x;
        float y;
        int cnt;
    };

    if (integral('h', -5, 5, eps2) - 168.406616 < eps2){
        if (integral('g', -5, 5, eps2) - 80.000000 < eps2) {
            if (integral('f', -2, -1, eps2) - 3.466041 < eps2) {
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

                float s = integral('h', p1.x, p2.x, eps2) + integral('g', p2.x, p3.x, eps2) - integral('f', p1.x, p3.x, eps2);

                char input[20];

                scanf("%s", input);
                while (strcmp(input, "-end") != 0) {
                    if (strcmp(input, "-help") == 0) {
                        printf("-area (area of a figure between dots\n-pdoc (print dots of cross)\n-pcoi (print count of interations)\n-tr (test root) (input : f g a b exp)\n-ti (test integral) (input : f a b exp)\n");
                    } else if (strcmp(input, "-pdoc") == 0) {
                        printf("x = %f, y = %f; p1\n", p1.x, p1.y);
                        printf("x = %f, y = %f; p2\n", p2.x, p2.y);
                        printf("x = %f, y = %f; p3\n", p3.x, p3.y);
                        printf("x = %f, y = %f; p4\n", p4.x, p4.y);
                    } else if (strcmp(input, "-pcoi") == 0) {
                        printf("f = g; %d - interations\n", p1.cnt);
                        printf("g = h; %d - interations\n", p2.cnt);
                        printf("g = h; %d - interations\n", p3.cnt);
                        printf("h = f; %d - interations\n", p4.cnt);
                    } else if (strcmp(input, "-tr") == 0) {
                        char f, g;
                        float a, b, eps3;
                        printf("Input: f, g, a, b, eps\n");
                        scanf("\n%c %c %f %f %f", &f, &g, &a, &b, &eps3);
                        printf("x = %f, y = %f; %c = %c\n", root(f, g, a, b, eps3), func(root(f, g, a, b, eps3), f), f, g);

                    } else if (strcmp(input, "-ti") == 0) {
                        char f;
                        float a, b, eps3;
                        printf("Input: f, a, b, eps\n");
                        scanf("\n%c %f %f %f", &f, &a, &b, &eps3);
                        printf("s = %f; %c[%f, %f]\n", integral(f, a, b, eps3), f, a, b);
                    } else if (strcmp(input, "-area") == 0){
                        printf("Area is %f\n", s);
                    } else {
                        //printf("Error!\n");
                    }
                    scanf("%s", input);
                }
            }
        }
    }

    return 0;
}
