#include "intrin_test.h"

int main() {
    float *arr = malloc(NUM_FLOATS * sizeof(float));
    if (!arr) {
        printf("Memory allocation failed\n");
        return 1;
    }

    srand((unsigned int)time(NULL));
    for (size_t i = 0; i < NUM_FLOATS; ++i) {
        // Generate float in [-1.0, 1.0]
        arr[i] = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
    }

    double sumDouble = sum_double(arr, NUM_FLOATS);
    printf("Sum of array (double precision): %.10f\n", sumDouble);

    print_sum_result("Scalar:", sumDouble, sum_scalar, arr, NUM_FLOATS);
    print_sum_result("AVX2:", sumDouble, sum_avx2, arr, NUM_FLOATS);
    print_sum_result("AVX2 improived:", sumDouble, sum_avx2_improved, arr, NUM_FLOATS);
    print_sum_result("AVX512:", sumDouble, sum_avx512, arr, NUM_FLOATS);
    print_sum_result("AVX512 improived:", sumDouble, sum_avx512_improved, arr, NUM_FLOATS);

    free(arr);
    return 0;
}