#ifndef INTRIN_TEST_H   
#define INTRIN_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include <time.h>

#define NUM_FLOATS 10000000

float sum_scalar(const float *arr, size_t n);

__attribute__((target("avx2")))
float sum_avx2(const float *arr, size_t n);

__attribute__((target("avx512f")))
float sum_avx512(const float *arr, size_t n);

__attribute__((target("avx512f")))
float sum_avx512_improved(const float *arr, size_t n);

__attribute__((target("avx2")))
float sum_avx2_improved(const float *arr, size_t n);

double sum_double(const float *arr, size_t n);

void print_sum_result(const char *label, double ref, float (*sum_func)(const float *, size_t), const float *arr, size_t n);

#endif