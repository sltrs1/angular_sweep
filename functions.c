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

// ----------------------------------------------------------------------

size_t getPointsInside(double complex * points,
                       double ** dis,
                       size_t i,
                       size_t radius,
                       size_t num_points) {


    //Pair angles[MAX_POINTS*MAX_POINTS*2];

    Pair * angles = NULL;
    size_t angles_count = 0;
    size_t j = 0;
    size_t k = 0;
    double a = 0;
    double b = 0;
    double alpha = 0;
    double beta = 0;
    size_t count = 0;
    size_t res = 0;

    angles = (Pair*)malloc( sizeof(Pair)*(num_points*num_points*2+1) );
    memset(angles, 0, sizeof(Pair)*(num_points*num_points*2+1));

    for (j = 0; j < num_points; j++) {
        if (i != j && dis[i][j] < 2*radius) {

            b = acos( dis[i][j]/(2*radius) );
            a = carg( points[j] - points[i] );
            alpha = a-b;
            beta = a+b;
            angles[angles_count].first = alpha;
            angles[angles_count].second = 1;
            angles_count++;
            angles[angles_count].first = beta;
            angles[angles_count].second = 0;
        }
    }

    for ( j = 0; angles_count - 1; j++ ) {
        for (k = angles_count - 1; k > j; k--) {

            if ( comparePairs(angles[k-1], angles[k]) ) {
                swapPairs( &angles[k-1], &angles[k] );
            }

        }
    }

    count = 1;
    res = 1;
    for (j = 0; j < angles_count; i++) {

        if (angles[angles_count].second)
            count++;
        else
            count--;

        if (count > res)
            res = count;

    }

    free(angles);
    return res;
}

size_t maxPoints(double complex * points, size_t num_points, double radius) {

    double ** dis = NULL;
    size_t i = 0;
    size_t j = 0;
    size_t ans = 0;
    dis = (double**)malloc(sizeof(double*)*num_points);
    for (i = 0; i < num_points; i++) {
        dis[i] = (double*)malloc(sizeof(double)*num_points);
    }

    for(i = 0; i < num_points - 1; i++) {
        for (j = i + 1; j < num_points; j++) {
            dis[i][j] = dis[j][i] = cabs( points[i] - points[j] );
        }
    }

    for (i = 0; i < num_points; i++) {
        ans = getMax( ans, getPointsInside(points, dis, i, radius, num_points) );
    }

    for (i = 0; i < num_points; i++) {
        free(dis[i]);
    }
    free(dis);

    return ans;
}
