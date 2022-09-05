#include "functions.h"
#include "defines.h"
#include <string.h>
#include <stdlib.h>

void printArraySizeT(size_t array[], size_t size) {
    int i = 0;
    for(i = 0; i < size; ++i) {
        printf("%d  ", array[i]);
    }
    printf("\n");
}

void printArrayComplex(double complex array[], size_t size) {
    int i = 0;
    for(i = 0; i < size; i++) {
        printf("%1.1f, %1.1f)\n", creal(array[i]), cimag(array[i]));
    }
}

size_t getMax(size_t a, size_t b) {
    if (a > b)
        return a;
    else
        return b;
}

size_t comparePairs(Pair a, Pair b) {

    if (a.first < b.first)
        return 1; // true
    else if (a.first > b.first)
        return 0; // false
    else {
        if (a.second == 1)
            return 1; // true
        else
            return 0; // false
    }
}

void swapPairs(Pair *a, Pair *b) {

  Pair t = *a;
  *a = *b;
  *b = t;

}

void free3DArray(double ***data, size_t xlen, size_t ylen)
{
    size_t i, j;

    for (i=0; i < xlen; ++i) {
        if (data[i] != NULL) {
            for (j=0; j < ylen; ++j)
                free(data[i][j]);
            free(data[i]);
        }
    }
    free(data);
}

double ***alloc3DArray(size_t xlen, size_t ylen, size_t zlen)
{
    double *** p;
    size_t i, j;

    if ((p = malloc(xlen * sizeof *p)) == NULL) {
        perror("malloc 1");
        return NULL;
    }

    for (i=0; i < xlen; ++i)
        p[i] = NULL;

    for (i=0; i < xlen; ++i)
        if ((p[i] = malloc(ylen * sizeof *p[i])) == NULL) {
            perror("malloc 2");
            free3DArray(p, xlen, ylen);
            return NULL;
        }

    for (i=0; i < xlen; ++i)
        for (j=0; j < ylen; ++j)
            p[i][j] = NULL;

    for (i=0; i < xlen; ++i)
        for (j=0; j < ylen; ++j)
            if ((p[i][j] = malloc(zlen * sizeof *p[i][j])) == NULL) {
                perror("malloc 3");
                free3DArray(p, xlen, ylen);
                return NULL;
            }

    return p;
}

// ----------------------------------------------------------------------

size_t getPointsInside(double complex * points,
                       double *** dis,
                       size_t x,
                       size_t y,
                       size_t radius,
                       size_t num_points,
                       Answer * answer)
{
    Pair angles[num_points*2+1];
    int angles_count = 0;
    int i = 0;
    int j = 0;
    double complex z1;
    double a = 0;
    double b = 0;
    double alpha = 0;
    double beta = 0;
    int count = 0;
    int res = 0;

    memset(angles, 0, sizeof(Pair)*(num_points*2+1));

    for (i = 0; i < num_points; i++) {
        z1 = (x+y*I);
        if ( cabs(z1 - points[i]) > 0.001 && // Точка не та же самая
             dis[x][y][i] < 2*radius ) {     // Точно попадает в круг
            b = acos( dis[x][y][i]/(2*radius) );
            a = carg( z1 - points[i] );
            alpha = a-b;
            beta = a+b;
            angles[angles_count].first = alpha;
            angles[angles_count].second = 1;
            angles_count++;
            angles[angles_count].first = beta;
            angles[angles_count].second = 0;
            angles_count++;
        }
    }

    for ( i = 0; j < angles_count - 1; i++ ) {
        for (j = (int)angles_count - 1; j > i; j--) {
            if ( !comparePairs(angles[j-1], angles[j]) ) {
                swapPairs( &angles[j-1], &angles[j] );
            }
        }
    }

    count = 1;
    res = 1;
    for (j = 0; j < angles_count; j++) {

        if (angles[j].second)
            count++;
        else
            count--;

        if (count > res)
            res = count;
    }

    answer->amount = res;
    answer->x = x;
    answer->y = y;

    return (size_t)res;
}

size_t maxPoints(double complex * points, size_t num_points, double radius) {

    double *** dis;
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    double complex z1;
    size_t tmp = 0;
    size_t ans = 0;
    Answer A;
    Answer B;

    dis = alloc3DArray(MAX_POINTS_X, MAX_POINTS_Y, num_points);

    for(i = 0; i < MAX_POINTS_X; i++) {
        for (j = 0; j < MAX_POINTS_Y; j++) {
            z1 = (i+j*I);
            for (k = 0; k < num_points; k++) {
                dis[i][j][k] = cabs( z1 - points[k] );
            }
        }
    }

    for(i = 0; i < MAX_POINTS_X; i++) {
        for (j = 0; j < MAX_POINTS_Y; j++) {
            tmp = getMax( ans, getPointsInside(points, dis, i, j, radius, num_points, &A) );
            //tmp = getMax( ans, getPointsInside(points, dis, 44, 46, radius, num_points, &A) );
            if (tmp > ans) {
                ans = tmp;
                B = A;
            }
        }
    }


    printf("Angular sweep have found maximum %u points in circle with centre of (%u,%u)\n", B.amount, B.x, B.y);

    free3DArray(dis, MAX_POINTS_X, MAX_POINTS_Y);

    return ans;
}
