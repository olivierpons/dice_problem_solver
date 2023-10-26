#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE 7
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

void ajoute_tuples(tuples ***tps, int *size, uchar *arr, int cut) {
    for (int i = 0; i < *size; ++i) {
        if (memcmp((*tps)[i]->a1, arr, cut) == 0 &&
            memcmp((*tps)[i]->a2, arr + cut, ARRAY_SIZE - cut) == 0) {
            return;
        }
    }

    *tps = (tuples **)realloc(*tps, (*size + 1) * sizeof(tuples *));
    (*tps)[*size] = (tuples *)malloc(sizeof(tuples));

    memcpy((*tps)[*size]->a1, arr, cut);
    (*tps)[*size]->len_a1 = cut;
    memcpy((*tps)[*size]->a2, arr + cut, ARRAY_SIZE - cut);
    (*tps)[*size]->len_a2 = ARRAY_SIZE - cut;

    (*size)++;
}

int main() {
    unsigned char nums[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7};
    int returnSize;
    uchar *block;
    uchar **result = permute(nums, ARRAY_SIZE, &returnSize, &block);
    tuples **tps = NULL;
    int size = 0;  // Ajout de la variable size pour suivre la taille du tableau de tuples

    printf("returnSize=%d\n", returnSize);
    for (int i = 0; i < returnSize; i++) {
        for (int j = 1; j < ARRAY_SIZE; j++) {
            ajoute_tuples(&tps, &size, result[i], j);  // Remarquez que je passe &size et non result[i]
        }
        printf("\n");
    }

    // Libération de la mémoire
    free(block);
    free(result);
    for (int i = 0; i < size; ++i) {
        free(tps[i]);
    }
    free(tps);

    return 0;
}