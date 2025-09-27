#include "intrin_test.h"
#include <math.h>

float sum_scalar(const float *arr, size_t n) {
    float sum = 0.0;
    for (size_t i = 0; i < n; i++)
    {
        sum += arr[i];
    }
    return sum;
}

__attribute__((target("avx2")))
float sum_avx2(const float *arr, size_t n) {
    __m256 vsum = _mm256_setzero_ps();
    size_t i = 0;
    for (; i + 7 < n; i += 8) {
        __m256 v = _mm256_loadu_ps(&arr[i]);
        vsum = _mm256_add_ps(vsum, v);
    }
    float temp[8];
    _mm256_storeu_ps(temp, vsum);
    float sum = 0.0f;
    for (int j = 0; j < 8; ++j) sum += temp[j];
    for (; i < n; ++i) sum += arr[i];
    return sum;
}

__attribute__((target("avx512f")))
float sum_avx512(const float *arr, size_t n) {
    __m512 vsum = _mm512_setzero_ps();
    size_t i = 0;
    for( ; i + 15 < n; i += 16) {
        __m512 v  = _mm512_loadu_ps(&arr[i]);
        vsum = _mm512_add_ps(vsum, v);
    }
    float temp[16];
    _mm512_storeu_ps(temp, vsum);
    float sum = 0.0f;
    for (int j = 0; j < 16; ++j) sum += temp[j];
    for (; i < n; ++i) sum += arr[i];
    return sum;
}

__attribute__((target("avx512f")))
float sum_avx512_improved(const float *arr, size_t n) {
    __m512 acc0 = _mm512_setzero_ps();
    __m512 acc1 = _mm512_setzero_ps();
    __m512 acc2 = _mm512_setzero_ps();
    __m512 acc3 = _mm512_setzero_ps();
    __m512 acc4 = _mm512_setzero_ps();
    __m512 acc5 = _mm512_setzero_ps();
    __m512 acc6 = _mm512_setzero_ps();
    __m512 acc7 = _mm512_setzero_ps();

    size_t i = 0;
    // Unroll: process 8*16 = 128 floats per iteration
    for (; i + 127 < n; i += 128) {
        const float *p = arr + i;
        acc0 = _mm512_add_ps(acc0, _mm512_loadu_ps(p + 0));
        acc1 = _mm512_add_ps(acc1, _mm512_loadu_ps(p + 16));
        acc2 = _mm512_add_ps(acc2, _mm512_loadu_ps(p + 32));
        acc3 = _mm512_add_ps(acc3, _mm512_loadu_ps(p + 48));
        acc4 = _mm512_add_ps(acc4, _mm512_loadu_ps(p + 64));
        acc5 = _mm512_add_ps(acc5, _mm512_loadu_ps(p + 80));
        acc6 = _mm512_add_ps(acc6, _mm512_loadu_ps(p + 96));
        acc7 = _mm512_add_ps(acc7, _mm512_loadu_ps(p + 112));
    }
    // Process remaining 16-float blocks
    for (; i + 15 < n; i += 16) {
        acc0 = _mm512_add_ps(acc0, _mm512_loadu_ps(arr + i));
    }

    // Combine all 8 accumulators
    __m512 acc01 = _mm512_add_ps(acc0, acc1);
    __m512 acc23 = _mm512_add_ps(acc2, acc3);
    __m512 acc45 = _mm512_add_ps(acc4, acc5);
    __m512 acc67 = _mm512_add_ps(acc6, acc7);

    __m512 acc0123 = _mm512_add_ps(acc01, acc23);
    __m512 acc4567 = _mm512_add_ps(acc45, acc67);

    __m512 total_vec = _mm512_add_ps(acc0123, acc4567);

    float temp[16];
    _mm512_storeu_ps(temp, total_vec);
    float total = 0.0f;
    for (int j = 0; j < 16; ++j) total += temp[j];
    for (; i < n; ++i) total += arr[i];
    return total;
}

__attribute__((target("avx2")))
float sum_avx2_improved(const float *arr, size_t n) {
    __m256 acc0 = _mm256_setzero_ps();
    __m256 acc1 = _mm256_setzero_ps();
    __m256 acc2 = _mm256_setzero_ps();
    __m256 acc3 = _mm256_setzero_ps();

    size_t i = 0;
    for (; i + 63 < n; i += 64) {
        const float *p = arr + i;
        acc0 = _mm256_add_ps(acc0, _mm256_loadu_ps(p));
        acc1 = _mm256_add_ps(acc1, _mm256_loadu_ps(p + 8));
        acc2 = _mm256_add_ps(acc2, _mm256_loadu_ps(p + 16));
        acc3 = _mm256_add_ps(acc3, _mm256_loadu_ps(p + 24));

        acc0 = _mm256_add_ps(acc0, _mm256_loadu_ps(p + 32));
        acc1 = _mm256_add_ps(acc1, _mm256_loadu_ps(p + 40));
        acc2 = _mm256_add_ps(acc2, _mm256_loadu_ps(p + 48));
        acc3 = _mm256_add_ps(acc3, _mm256_loadu_ps(p + 56));
    }

    for (; i + 31 < n; i += 32) {
        const float *p = arr + i;
        acc0 = _mm256_add_ps(acc0, _mm256_loadu_ps(p));
        acc1 = _mm256_add_ps(acc1, _mm256_loadu_ps(p + 8));
        acc2 = _mm256_add_ps(acc2, _mm256_loadu_ps(p + 16));
        acc3 = _mm256_add_ps(acc3, _mm256_loadu_ps(p + 24));
    }

    __m256 acc = _mm256_add_ps(_mm256_add_ps(acc0, acc1), _mm256_add_ps(acc2, acc3));
    float temp[8];
    _mm256_storeu_ps(temp, acc);
    float total = 0.0f;
    for (int j = 0; j < 8; ++j) total += temp[j];
    for (; i < n; ++i) total += arr[i];
    return total;
}

double sum_double(const float *arr, size_t n) {
    double sum = 0.0;
    for (size_t i = 0; i < n; i++) {
        sum += (double)arr[i];
    }
    return sum;
}

void print_sum_result(const char *label, double ref, float (*sum_func)(const float *, size_t), const float *arr, size_t n) {
    struct timespec start, end;
    timespec_get(&start, TIME_UTC);
    float sum = sum_func(arr, n);
    timespec_get(&end, TIME_UTC);
    long elapsed_us = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_nsec - start.tv_nsec) / 1000L;
    double error = fabs(ref - (double)sum);
    printf("%-7s sum=%.10f error=%.10f time=%ldus\n", label, sum, error, elapsed_us);
}