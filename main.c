#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define ARRAY_SIZE 7
#define ARRAY_SIZE 6
typedef unsigned char uchar;

typedef struct {
    uchar a1[ARRAY_SIZE];
    int len_a1;
    uchar a2[ARRAY_SIZE];
    int len_a2;
} tuples;

void swap(unsigned char *a, unsigned char *b) {
    unsigned char temp = *a;
    *a = *b;
    *b = temp;
}

int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

void nextPermutation(unsigned char *nums, int numsSize) {
    int i = numsSize - 2, j = numsSize - 1;
    while (i >= 0 && nums[i] >= nums[i + 1]) {
        i--;
    }
    if (i >= 0) {
        while (nums[j] <= nums[i]) {
            j--;
        }
        swap(&nums[i], &nums[j]);
    }
    for (int start = i + 1, end = numsSize - 1; start < end; start++, end--) {
        swap(&nums[start], &nums[end]);
    }
}

uchar** permute(const uchar *nums, int numsSize, int *returnSize, uchar **block) {
    *returnSize = factorial(numsSize);
    uchar **result = (uchar **)malloc(*returnSize * sizeof(uchar *));
    *block = (uchar *)malloc(*returnSize * numsSize * sizeof(uchar));

    for (int i = 0; i < *returnSize; i++) {
        result[i] = *block + i * numsSize;
    }

    for (int i = 0; i < numsSize; i++) {
        result[0][i] = nums[i];
    }

    for (int i = 1; i < *returnSize; i++) {
        for (int j = 0; j < numsSize; j++) {
            result[i][j] = result[i - 1][j];
        }
        nextPermutation(result[i], numsSize);
    }

    return result;
}

void add_tuples(tuples **tps, int *size, int *capacity, uchar *arr, int cut) {
    for (int i = 0; i < *size; ++i) {
        if (memcmp((*tps)[i].a1, arr, cut) == 0 &&
            memcmp((*tps)[i].a2, arr + cut, ARRAY_SIZE - cut) == 0) {
            return;
        }
    }

    if (*size == 0) {
        *capacity = 1;
        *size = 0;
        *tps = (tuples *) malloc((*capacity) * sizeof(tuples));
        memset(*tps, 0, sizeof(tuples));
    }
    if (*size == *capacity) {
        *capacity *= 2;
        *tps = (tuples *)realloc(*tps, (*capacity) * sizeof(tuples));
        memset(&((*tps)[*size]), 0, (*capacity - *size) * sizeof(tuples));
    }

    memcpy((*tps)[*size].a1, arr, cut);
    (*tps)[*size].len_a1 = cut;
    memcpy((*tps)[*size].a2, arr + cut, ARRAY_SIZE - cut);
    (*tps)[*size].len_a2 = ARRAY_SIZE - cut;

    (*size)++;
}

void print_tuples(tuples *tps, int size) {
    for (int i = 0; i < size; ++i) {
        printf("[");
        for (int j = 0; j < tps[i].len_a1; ++j) {
            printf("%d", tps[i].a1[j]);
            if (j < tps[i].len_a1 - 1) {
                printf(", ");
            }
        }
        printf("] - [");
        for (int j = 0; j < tps[i].len_a2; ++j) {
            printf("%d", tps[i].a2[j]);
            if (j < tps[i].len_a2 - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

int main() {
    unsigned char nums[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6};
    int returnSize = 0;
    uchar *block = NULL;
    uchar **result = permute(nums, ARRAY_SIZE, &returnSize, &block);
    tuples *tps = NULL;
    int size = 0;
    int capacity = 0;

    for (int i = 0; i < returnSize; i++) {
        for (int j = 1; j < ARRAY_SIZE; j++) {
            add_tuples(&tps, &size, &capacity, result[i], j);
        }
    }
    print_tuples(tps, size);
    free(block);
    free(result);
    free(tps);
    return 0;
}