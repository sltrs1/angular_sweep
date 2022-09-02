#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <complex.h>
#include <math.h>
#include "pair.h"

void printArraySizeT(size_t array[], size_t size);
void printArrayComplex(double complex array[], size_t size);
size_t getMax(size_t a, size_t b);

size_t comparePairs(Pair a, Pair b);
void swapPairs(Pair *a, Pair *b);

// ----------------------------------------------------------------------

size_t getPointsInside(double complex * points, double ** dis, size_t i, size_t radius, size_t num_points);
size_t maxPoints(double complex * points, size_t num_points, double radius);

#endif // FUNCTIONS_H
