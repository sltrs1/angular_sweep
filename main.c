/*
 * Задание:
 * На вход подаются файл с координатами точек и радиус окружности.
 * Каждая строка файла – это пара целых чисел от 0 до 99,
 * записанных через запятую.
 * Радиус окружности целочисленный.
 * Необходимо найти координаты окружности, в которую попадает
 * наибольшее число точек и вывести координаты и число точек.
 *
 * Комментарий:
 * В задании не совсем ясно сформулировано,
 * есть ли разделительный пробел между числами в файле.
 * Будем считать, что пробела нет.
 *
 * Для тестирования в репозитории находится файл с 2000
 * случайно сгенерированых точек.
 * Отладка проводилась для значения радиуса 10.
 *
 * Для хранения и работы с координатами точек используется
 * библиотека комплексных чисел, которая предоставляет
 * некоторый полезный готовый функционал.
 *
 * Так как это не оговаривалось, точка будет считаться попавшей
 * в окружность, если она находится на ее границе.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <string.h>

#include "defines.h"
#include "functions.h"

int main(int argc, char ** argv)
{
    FILE * F = NULL;
    double radius = 0;
    long int tmp = 0;
    size_t x = 0;
    size_t y = 0;
    double complex points[MAX_POINTS];
    size_t num_points = 0;
    //------------------------------------------------
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    double complex z1;
    double dist = 0;
    size_t naive_solution[MAX_POINTS_X][MAX_POINTS_Y];
    size_t max_i = 0;
    size_t max_j = 0;
    size_t naive_solution_max = 0;
    //------------------------------------------------
    size_t as_max_i = 0;
    size_t as_max_j = 0;
    size_t as_max   = 0;


// =================================================================
// Парсим входные аргументы

    if (argc != 3) {
        perror ("Invalid amount of arguments\n");
        printf("Usage:\n");
        printf("Argument 1 - Path to file with points\n");
        printf("Argument 2 - Radius of circle\n");
        return 1;
    }

    F = fopen(argv[1], "r");
    if (F == NULL) {
        perror ("Cannot open file");
        fprintf(stderr, "errno = %i\n", errno);
        printf("Probably invalid filename or invalid access rights\n");
        return 2;
    }

    tmp = atoi(argv[2]);
    if (tmp <= 0) {
        perror("Invalid radius");
        printf("Radius argument is not a number or invalid value");
        return 3;
    }
    radius = (double)tmp;

    printf("Filename = %s\n", argv[1]);
    printf("Radius = %u\n", (size_t)radius);
    printf("=================\n");

// =================================================================
//  Считываем данные из файла

    while( !feof(F) ) {
        if (num_points > MAX_POINTS) {
            printf("Read maximum possible amount of points. If there's others, they are most likely duplicates\n.");
            break;
        }
        fscanf_s(F, "%u,%u", &x, &y);
        // points[num_points] = _CBuild(x,y);
        points[num_points] = (double)x + ((double)y)*I;
        num_points++;
    }
    fclose(F);

// =================================================================
// Наивное решение.
// Просто для каждой точки на плоскости A(i,j) считется дистанция до
// каждой точки из данных B(k) и проверяется, попадает ли точка
// в окружность с центром в данной точке плоскости.
// Это самое медленное решение.
// Это решение работает для целых чисел.

    memset(naive_solution, 0, sizeof(size_t)*MAX_POINTS_X*MAX_POINTS_Y);

    for(i = 0; i < MAX_POINTS_X; i++) {
        for (j = 0; j < MAX_POINTS_Y; j++) {
            z1 = (double)i + ((double)j)*I;
            for (k = 0; k < num_points; k++) {
                // Расстояние от одной точки до другой эквивалентно
                // модулю разности соотетствующих им комплексных чисел
                dist = cabs(z1 - points[k]);
                if (dist <= radius) {
                    naive_solution[i][j]++;
                    if ( naive_solution[i][j] > naive_solution_max ) {
                        naive_solution_max = naive_solution[i][j];
                        max_i = i;
                        max_j = j;
                    }
                }
            }
        }
    }

    printf("Naive solution have found maximum %u points in circle with centre of (%u,%u)\n", naive_solution_max, max_i, max_j);
    printf("=================\n");

// =================================================================
// Более эффективное решение, angular sweep algorithm (алгоритм углового заметания)
// Однако, он дает результат с дробными координатами центра.
// Работает за O(N^2*logN)
// Это не то, что требуется по заданию, однако, мне так понравитлся этот алгоитм, что я решил реализовать и его.
// Округлив результаты, можно получить приблизительный ответ за немного меньшее время, чем дает наивный алгоритм.

    as_max = maxPoints(points, num_points, radius);

// =================================================================
    return 0;
}
