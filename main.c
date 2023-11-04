#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calc_odin.h"

void swap(int *a, int *b) {
    int temp = *a;
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

void nextPermutation(int *nums, int numsSize) {
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

int** permute(const int *nums, int numsSize, int *returnSize, int **block) {
    *returnSize = factorial(numsSize);
    int **result = (int **)malloc(*returnSize * sizeof(int *));
    *block = (int *)malloc(*returnSize * numsSize * sizeof(int));

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

int compare_ints(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void add_tuple(
    tuples **tps, int *size, int *capacity, tuple *new_a1, tuple *new_a2,
    int m1, int m2, int a1_more1, int a1_more2, int a2_more1, int a2_more2
) {
    if (*size == 0) {
        *capacity = 1;
        *tps = (tuples *) malloc((*capacity) * sizeof(tuples));
        memset(*tps, 0, sizeof(tuples));
    }
    if (*size == *capacity) {
        *capacity *= 2;
        *tps = (tuples *)realloc(*tps, (*capacity) * sizeof(tuples));
        memset(&((*tps)[*size]), 0, (*capacity - *size) * sizeof(tuples));
    }
    (*tps)[*size].m1 = m1;
    (*tps)[*size].m2 = m2;
    (*tps)[*size].a1 = *new_a1;
    (*tps)[*size].a2 = *new_a2;
    (*tps)[*size].a1.m1 = a1_more1;
    (*tps)[*size].a1.m2 = a1_more2;
    (*tps)[*size].a2.m1 = a2_more1;
    (*tps)[*size].a2.m2 = a2_more2;
    (*size)++;
}

void add_tuples(
    tuples **t, int *size, int *capacity, int *arr, int cut,
    int mg1, int mg2, int ml1, int ml2
) {
    tuple t1 = {.a_len = cut};
    tuple t2 = {.a_len = ARRAY_MAX_SIZE - cut};

    memcpy(t1.a, arr, cut * sizeof(int));
    memcpy(t2.a, arr + cut, (ARRAY_MAX_SIZE - cut) * sizeof(int));

    qsort(t1.a, t1.a_len, sizeof(int), compare_ints);
    qsort(t2.a, t2.a_len, sizeof(int), compare_ints);

    for (int i = 0; i < *size; ++i) {
        if (memcmp((*t)[i].a1.a, t1.a, cut * sizeof(int)) == 0 &&
            memcmp((*t)[i].a2.a, t2.a, (ARRAY_MAX_SIZE - cut) * sizeof(int)) == 0) {
            return;
        }
        if (memcmp((*t)[i].a2.a, t1.a, cut * sizeof(int)) == 0 &&
            memcmp((*t)[i].a1.a, t2.a, (ARRAY_MAX_SIZE - cut) * sizeof(int)) == 0) {
            return;
        }
    }

    if (ml1) {
        if (ml2) {
            add_tuple(t, size, capacity, &t1, &t2, mg1, mg2, ml1, ml2, 0, 0);
            add_tuple(t, size, capacity, &t1, &t2, mg1, mg2, ml2, ml1, 0, 0);
            add_tuple(t, size, capacity, &t1, &t2, mg1, mg2, ml1, 0, ml2, 0);
            add_tuple(t, size, capacity, &t1, &t2, mg1, mg2, ml2, 0, ml1, 0);
            add_tuple(t, size, capacity, &t1, &t2, mg1, mg2, 0, 0, ml1, ml2);
            add_tuple(t, size, capacity, &t1, &t2, mg1, mg2, 0, 0, ml2, ml1);
        } else {
            add_tuple(t, size, capacity, &t1, &t2, mg1, mg2, ml1, 0, 0, 0);
            add_tuple(t, size, capacity, &t1, &t2, mg1, mg2, 0, 0, ml1, 0);
        }
    } else {
        add_tuple(t, size, capacity, &t1, &t2, mg1, mg2, 0, 0, 0, 0);
    }
}

void print_role(int role) {
    if (role >= NUM_ROLES || roleNames[role] == NULL) {
        printf("Unknown: %d", (int)role);
    } else {
        printf("%s", roleNames[role]);
    }
}

void print_tuple(const tuple *tuple, const char *delimiter, const char *end) {
    printf("[");
    for (int i = 0; i < tuple->a_len; ++i) {
        print_role(tuple->a[i]);
        if (i < tuple->a_len - 1) {
            printf("%s", delimiter);
        }
    }
    printf("]");
    if (tuple->m1 && tuple->m2) {
        printf(" (");
        print_role(tuple->m1);
        printf(" / ");
        print_role(tuple->m2);
        printf(")");
    } else if (tuple->m1) {
        printf(" (");
        print_role(tuple->m1);
        printf(")");
    }
    printf(" = %d", tuple->sum);
    printf("%s", end);
}

void comp_one(tuple *t) {
    memset(t->a_sum, 0, sizeof(int) * ARRAY_MAX_SIZE);
    calc_heroes(t);
    calc_captains(t);
    calc_soldiers(t);
    calc_cursed(t);
    calc_traitors(t);
    calc_mages(t);
    calc_wolfs_fenrir(t, t->m1);
    calc_snake_jormungand(t, t->m1);
    calc_horse_sleipnir(t, t->m1);
    calc_dragon_fafnir(t, t->m1);
    calc_wolfs_fenrir(t, t->m2);
    calc_snake_jormungand(t, t->m2);
    calc_horse_sleipnir(t, t->m2);
    calc_dragon_fafnir(t, t->m2);
}

void comp_both(tuples *t, int v) {
    calc_wildboar_gullinbursti(&t->a1, v);
    calc_wildboar_gullinbursti(&t->a2, v);
    calc_wildboar_eagle_hraesvelg(&t->a1, v);
    calc_wildboar_eagle_hraesvelg(&t->a2, v);
}

void calc_sum(tuple *t) {
    t->sum = 0;
    for (int i = 0; i < t->a_len; ++i) {
        t->sum = (int)(t->sum + t->a_sum[i]);
    }

}
void compute_dice_challenge(int challenge_no, tuple t) {
    int returnSize = 0;
    int *block = NULL;
    int **result = permute(t.a, ARRAY_MAX_SIZE, &returnSize, &block);
    tuples *tps = NULL;
    int size = 0;
    int capacity = 0;


    int g1 = 0, g2 = 0, l1 = 0, l2 = 0;
    if (t.m1) {
        if (t.m1 == WILDBOAR_GULLINBURSTI || t.m1 == EAGLE_HRAESVELG) {
            g1 = t.m1;
        } else {
            l1 = t.m1;
        }
        if (t.m2) {
            if (t.m2 == WILDBOAR_GULLINBURSTI || t.m2 == EAGLE_HRAESVELG) {
                if (g1) {
                    g2 = t.m2;
                } else {
                    g1 = t.m2;
                }
            } else {
                if (l1) {
                    l2 = t.m2;
                } else {
                    l1 = t.m2;
                }
            }
        }
    }

    for (int i = 0; i < returnSize; i++) {
        for (int j = 1; j < ARRAY_MAX_SIZE; j++) {
            add_tuples(&tps, &size, &capacity, result[i], j, g1, g2, l1, l2);
        }
    }
    printf("\nChallenge %d - [ ", challenge_no);
    for (int i = 0; i < ARRAY_MAX_SIZE; ++i) {
        print_role(t.a[i]);
        if (i < ARRAY_MAX_SIZE - 1) {
            printf(", ");
        }
    }
    printf("]\n");
    for (int i = 0; i < size; ++i) {
        /* For debugging purposes:
        if (
            tps[i].a2.a_len == 3 &&
            tps[i].a2.a[0] == CURSED &&
            tps[i].a2.a[1] == MAGE &&
            tps[i].a2.a[2] == MAGE &&
            tps[i].a2.m1 != WOLF_FENRIR &&
            tps[i].a1.m2 == WILDBOAR_GULLINBURSTI
            ) {
            printf("here\n");
        }
         */
        comp_one(&tps[i].a1);
        comp_one(&tps[i].a2);
        if (tps[i].m1) {
            comp_both(&tps[i], tps[i].m1);
            comp_both(&tps[i], tps[i].m2);
        }
        calc_sum(&tps[i].a1);
        calc_sum(&tps[i].a2);

        if (tps[i].a1.sum == tps[i].a2.sum) { // (1) { //
            print_tuple(&tps[i].a1, ", ", " - ");
            print_tuple(&tps[i].a2, ", ", "");
            if (g1) {
                printf(" - (");
                print_role(g1);
                if (g2) {
                    printf(", ");
                    print_role(g2);
                }
                printf(")");
            }
            printf("\n");
        }
    }
    free(block);
    free(result);
    free(tps);
}

int main() {
    tuple nums[] = {
        { .a = { HERO, HERO, HERO, CAPTAIN, SOLDIER, SOLDIER, SOLDIER }, .m1 = 0, .m2 = 0 },
        { .a = { HERO, HERO, CAPTAIN, CAPTAIN, CAPTAIN, CAPTAIN, TRAITOR }, .m1 = 0, .m2 = 0 },
        { .a = { HERO, HERO, HERO, CAPTAIN, CAPTAIN, CAPTAIN, CURSED }, .m1 = 0, .m2 = 0 },
        { .a = { HERO, HERO, HERO, HERO, SOLDIER, TRAITOR, CURSED }, .m1 = 0, .m2 = 0 },
        { .a = { SOLDIER, SOLDIER, SOLDIER, SOLDIER, SOLDIER, SOLDIER, MAGE }, .m1 = 0, .m2 = 0 },
        { .a = { HERO, HERO, CAPTAIN, SOLDIER, SOLDIER, SOLDIER, MAGE }, .m1 = 0, .m2 = 0 },
        { .a = { HERO, HERO, CAPTAIN, SOLDIER, SOLDIER, CURSED, MAGE }, .m1 = 0, .m2 = 0 },
        { .a = { HERO, CAPTAIN, CAPTAIN, CAPTAIN, CURSED, CURSED, MAGE }, .m1 = 0, .m2 = 0 },
        { .a = { HERO, HERO, CAPTAIN, CAPTAIN, SOLDIER, TRAITOR, MAGE }, .m1 = 0, .m2 = 0 },
        { .a = { HERO, HERO, CAPTAIN, TRAITOR, TRAITOR, TRAITOR, MAGE }, .m1 = 0, .m2 = 0 },
        { .a = { HERO, HERO, CAPTAIN, CAPTAIN, TRAITOR, CURSED, MAGE }, .m1 = 0, .m2 = 0 },
        { .a = { HERO, HERO, SOLDIER, SOLDIER, TRAITOR, MAGE, MAGE }, .m1 = 0, .m2 = 0 },
        { .a = { SOLDIER, CURSED, CURSED, MAGE, MAGE, MAGE, MAGE }, .m1 = 0, .m2 = 0 },
        { .a = { HERO, CAPTAIN, CAPTAIN, CURSED, MAGE, MAGE, MAGE }, .m1 = 0, .m2 = 0 },
        { .a = { HERO, SOLDIER, TRAITOR, TRAITOR, MAGE, MAGE, MAGE }, .m1 = 0, .m2 = 0 },
        { .a = { HERO, HERO, HERO, CAPTAIN, SOLDIER, SOLDIER, MAGE }, .m1 = WOLF_FENRIR, .m2 = 0 },
        { .a = { HERO, CAPTAIN, CAPTAIN, CAPTAIN, SOLDIER, TRAITOR, MAGE }, .m1 = WOLF_FENRIR, .m2 = 0 },
        { .a = { HERO, SOLDIER, SOLDIER, SOLDIER, TRAITOR, MAGE, MAGE }, .m1 = WOLF_FENRIR, .m2 = 0 },
        { .a = { HERO, SOLDIER, SOLDIER, CURSED, CURSED, CURSED, MAGE }, .m1 = SNAKE_JORMUNGAND, .m2 = 0 },
        { .a = { HERO, TRAITOR, TRAITOR, CURSED, MAGE, MAGE, MAGE }, .m1 = SNAKE_JORMUNGAND, .m2 = 0 },
        { .a = { HERO, HERO, CAPTAIN, TRAITOR, CURSED, MAGE, MAGE }, .m1 = SNAKE_JORMUNGAND, .m2 = 0 },
        { .a = { HERO, CAPTAIN, CAPTAIN, CAPTAIN, SOLDIER, SOLDIER, MAGE }, .m1 = HORSE_SLEIPNIR, .m2 = 0 },
        { .a = { HERO, HERO, CAPTAIN, CAPTAIN, SOLDIER, TRAITOR, MAGE }, .m1 = HORSE_SLEIPNIR, .m2 = 0 },
        { .a = { HERO, CAPTAIN, TRAITOR, CURSED, MAGE, MAGE, MAGE }, .m1 = HORSE_SLEIPNIR, .m2 = 0 },
        { .a = { SOLDIER, SOLDIER, SOLDIER, CURSED, CURSED, CURSED, MAGE }, .m1 = DRAGON_FAFNIR, .m2 = 0 },
        { .a = { HERO, HERO, HERO, HERO, TRAITOR, CURSED, MAGE }, .m1 = DRAGON_FAFNIR, .m2 = 0 },
        { .a = { HERO, HERO, CAPTAIN, SOLDIER, TRAITOR, MAGE, MAGE }, .m1 = DRAGON_FAFNIR, .m2 = 0 },
        { .a = { CAPTAIN, CAPTAIN, CAPTAIN, SOLDIER, CURSED, CURSED, MAGE }, .m1 = WILDBOAR_GULLINBURSTI, .m2 = 0 },
        { .a = { HERO, HERO, TRAITOR, CURSED, CURSED, MAGE, MAGE }, .m1 = WILDBOAR_GULLINBURSTI, .m2 = 0 },
        { .a = { HERO, TRAITOR, TRAITOR, CURSED, MAGE, MAGE, MAGE }, .m1 = WILDBOAR_GULLINBURSTI, .m2 = 0 },
        { .a = { HERO, HERO, CAPTAIN, SOLDIER, CURSED, CURSED, MAGE }, .m1 = EAGLE_HRAESVELG, .m2 = 0 },
        { .a = { HERO, HERO, CAPTAIN, SOLDIER, SOLDIER, TRAITOR, MAGE }, .m1 = EAGLE_HRAESVELG, .m2 = 0 },
        { .a = { HERO, HERO, HERO, SOLDIER, TRAITOR, MAGE, MAGE }, .m1 = EAGLE_HRAESVELG, .m2 = 0 },
        { .a = { HERO, HERO, SOLDIER, SOLDIER, TRAITOR, MAGE, MAGE }, .m1 = HORSE_SLEIPNIR, .m2 = WILDBOAR_GULLINBURSTI },
        { .a = { SOLDIER, SOLDIER, CURSED, CURSED, CURSED, MAGE, MAGE }, .m1 = WOLF_FENRIR, .m2 = DRAGON_FAFNIR },
        { .a = { HERO, SOLDIER, SOLDIER, TRAITOR, MAGE, MAGE, MAGE }, .m1 = HORSE_SLEIPNIR, .m2 = EAGLE_HRAESVELG },
        { .a = { HERO, SOLDIER, SOLDIER, TRAITOR, TRAITOR, MAGE, MAGE }, .m1 = DRAGON_FAFNIR, .m2 = WILDBOAR_GULLINBURSTI },
        { .a = { HERO, HERO, SOLDIER, TRAITOR, TRAITOR, TRAITOR, MAGE }, .m1 = SNAKE_JORMUNGAND, .m2 = EAGLE_HRAESVELG },
        { .a = { HERO, CAPTAIN, CAPTAIN, TRAITOR, MAGE, MAGE, MAGE }, .m1 = WILDBOAR_GULLINBURSTI, .m2 = HORSE_SLEIPNIR },
        { .a = { HERO, SOLDIER, SOLDIER, SOLDIER, CURSED, CURSED, MAGE }, .m1 = SNAKE_JORMUNGAND, .m2 = DRAGON_FAFNIR },
        { .a = { HERO, HERO, SOLDIER, SOLDIER, TRAITOR, MAGE, MAGE }, .m1 = WILDBOAR_GULLINBURSTI, .m2 = EAGLE_HRAESVELG },
        { .a = { HERO, HERO, HERO, TRAITOR, TRAITOR, CURSED, MAGE }, .m1 = DRAGON_FAFNIR, .m2 = WILDBOAR_GULLINBURSTI },
        //43 = problem: not solvable!
        { .a = { HERO, HERO, TRAITOR, CURSED, CURSED, MAGE, MAGE }, .m1 = WOLF_FENRIR, .m2 = WILDBOAR_GULLINBURSTI },
        { .a = { SOLDIER, SOLDIER, SOLDIER, SOLDIER, SOLDIER, CAPTAIN, MAGE }, .m1 = WOLF_FENRIR, .m2 = SNAKE_JORMUNGAND },
        { .a = { CAPTAIN, SOLDIER, SOLDIER, SOLDIER, MAGE, MAGE, MAGE }, .m1 = HORSE_SLEIPNIR, .m2 = EAGLE_HRAESVELG },
        { .a = { HERO, HERO, CAPTAIN, SOLDIER, TRAITOR, CURSED, MAGE }, .m1 = WOLF_FENRIR, .m2 = WOLF_FENRIR },
        { .a = { HERO, SOLDIER, SOLDIER, TRAITOR, TRAITOR, TRAITOR, MAGE }, .m1 = SNAKE_JORMUNGAND, .m2 = WILDBOAR_GULLINBURSTI },
        { .a = { HERO, HERO, TRAITOR, TRAITOR, TRAITOR, CURSED, MAGE }, .m1 = WOLF_FENRIR, .m2 = EAGLE_HRAESVELG },
        { .a = { HERO, HERO, CAPTAIN, CAPTAIN, CAPTAIN, TRAITOR, MAGE }, .m1 = WOLF_FENRIR, .m2 = SNAKE_JORMUNGAND },
        { .a = { HERO, HERO, CAPTAIN, TRAITOR, MAGE, MAGE, MAGE }, .m1 = SNAKE_JORMUNGAND, .m2 = HORSE_SLEIPNIR },
    };

    for (int i = 0; i < sizeof(nums) / sizeof(nums[0]); i++) {
        compute_dice_challenge(i + 1, nums[i]);
    }

    return 0;
}
