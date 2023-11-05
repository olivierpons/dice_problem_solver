#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calc_odin.h"
#include "calc_coba.h"

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

void comp_one_first(tuple *t) {
    memset(t->a_sum, 0, sizeof(int) * ARRAY_MAX_SIZE);
    calc_heroes(t);
    calc_captains(t);
    calc_soldiers(t);
    calc_cursed(t);
    calc_traitors(t);
    calc_mages(t);
    /* m1: */
    calc_wolf(t, t->m1);
    calc_snake(t, t->m1);
    calc_horse(t, t->m1);
    calc_dragon(t, t->m1);
    /* m2: */
    calc_wolf(t, t->m2);
    calc_snake(t, t->m2);
    calc_horse(t, t->m2);
    calc_dragon(t, t->m2);


    calc_potter(t);
    calc_peasant(t);
    calc_scribe(t);
    calc_thief(t);
}
void comp_one_second(tuple *t) {
    calc_queen(t);
//    calc_armadillo(t);
//    calc_deer(t);
//    calc_iguana(t);
//    calc_scorpion(t);
//    calc_jaguar(t);
//    calc_bee(t);
}

void comp_both(tuples *t, int v) {
    calc_wildboar(&t->a1, v);
    calc_wildboar(&t->a2, v);
    calc_wildboar_and_eagle(&t->a1, v);
    calc_wildboar_and_eagle(&t->a2, v);
}

void calc_sum(tuple *t) {
    t->sum = 0;
    for (int i = 0; i < t->a_len; ++i) {
        t->sum = (int)(t->sum + t->a_sum[i]);
    }

}
int has_fig(tuple *t, int v) {
    for (int i = 0; i < t->a_len; ++i) {
        if (t->a[i] == v) {
            return 1;
        }
    }
    return 0;
}
void compute_dice_challenge(const char* description, tuple t) {
    int returnSize = 0;
    int *block = NULL;
    int **result = permute(t.a, ARRAY_MAX_SIZE, &returnSize, &block);
    tuples *tps = NULL;
    int size = 0;
    int capacity = 0;


    int g1 = 0, g2 = 0, l1 = 0, l2 = 0;
    if (t.m1) {
        if (t.m1 == WILDBOAR || t.m1 == EAGLE) {
            g1 = t.m1;
        } else {
            l1 = t.m1;
        }
        if (t.m2) {
            if (t.m2 == WILDBOAR || t.m2 == EAGLE) {
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
    printf("\nChallenge %s - [ ", description);
    for (int i = 0; i < ARRAY_MAX_SIZE; ++i) {
        print_role(t.a[i]);
        if (i < ARRAY_MAX_SIZE - 1) {
            printf(", ");
        }
    }
    printf("]\n");
    for (int i = 0; i < size; ++i) {
        /* For debugging purposes: */
        if (
            tps[i].a1.a_len == 5 &&
            has_fig(&tps[i].a1, POTTER) &&
            has_fig(&tps[i].a1, PEASANT) &&
            has_fig(&tps[i].a1, SHAMAN) &&
            has_fig(&tps[i].a1, QUEEN) /* &&
            tps[i].a1.m1 != WOLF &&
            tps[i].a1.m2 == WILDBOAR */
            ) {
//            printf("here\n");
        }
        /* */
        comp_one_first(&tps[i].a1);
        comp_one_first(&tps[i].a2);

        calc_shaman(&tps[i].a1, tps[i].a2.a_len);
        calc_shaman(&tps[i].a2, tps[i].a1.a_len);

        comp_one_second(&tps[i].a1);
        comp_one_second(&tps[i].a2);

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
    char *s[] = {
        // Odin:
        "O 1,HERO,HERO,HERO,CAPTAIN,SOLDIER,SOLDIER,SOLDIER,0,0",
        "O 2,HERO,HERO,CAPTAIN,CAPTAIN,CAPTAIN,CAPTAIN,TRAITOR,0,0",
        "O 3,HERO,HERO,HERO,CAPTAIN,CAPTAIN,CAPTAIN,CURSED,0,0",
        "O 4,HERO,HERO,HERO,HERO,SOLDIER,TRAITOR,CURSED,0,0",
        "O 5,SOLDIER,SOLDIER,SOLDIER,SOLDIER,SOLDIER,SOLDIER,MAGE,0,0",
        "O 6,HERO,HERO,CAPTAIN,SOLDIER,SOLDIER,SOLDIER,MAGE,0,0",
        "O 7,HERO,HERO,CAPTAIN,SOLDIER,SOLDIER,CURSED,MAGE,0,0",
        "O 8,HERO,CAPTAIN,CAPTAIN,CAPTAIN,CURSED,CURSED,MAGE,0,0",
        "O 9,HERO,HERO,CAPTAIN,CAPTAIN,SOLDIER,TRAITOR,MAGE,0,0",
        "O10,HERO,HERO,CAPTAIN,TRAITOR,TRAITOR,TRAITOR,MAGE,0,0",
        "O11,HERO,HERO,CAPTAIN,CAPTAIN,TRAITOR,CURSED,MAGE,0,0",
        "O12,HERO,HERO,SOLDIER,SOLDIER,TRAITOR,MAGE,MAGE,0,0",
        "O13,SOLDIER,CURSED,CURSED,MAGE,MAGE,MAGE,MAGE,0,0",
        "O14,HERO,CAPTAIN,CAPTAIN,CURSED,MAGE,MAGE,MAGE,0,0",
        "O15,HERO,SOLDIER,TRAITOR,TRAITOR,MAGE,MAGE,MAGE,0,0",
        "O16,HERO,HERO,HERO,CAPTAIN,SOLDIER,SOLDIER,MAGE,WOLF,0",
        "O17,HERO,CAPTAIN,CAPTAIN,CAPTAIN,SOLDIER,TRAITOR,MAGE,WOLF,0",
        "O18,HERO,SOLDIER,SOLDIER,SOLDIER,TRAITOR,MAGE,MAGE,WOLF,0",
        "O19,HERO,SOLDIER,SOLDIER,CURSED,CURSED,CURSED,MAGE,SNAKE,0",
        "O20,HERO,TRAITOR,TRAITOR,CURSED,MAGE,MAGE,MAGE,SNAKE,0",
        "O21,HERO,HERO,CAPTAIN,TRAITOR,CURSED,MAGE,MAGE,SNAKE,0",
        "O22,HERO,CAPTAIN,CAPTAIN,CAPTAIN,SOLDIER,SOLDIER,MAGE,HORSE,0",
        "O23,HERO,HERO,CAPTAIN,CAPTAIN,SOLDIER,TRAITOR,MAGE,HORSE,0",
        "O24,HERO,CAPTAIN,TRAITOR,CURSED,MAGE,MAGE,MAGE,HORSE,0",
        "O25,SOLDIER,SOLDIER,SOLDIER,CURSED,CURSED,CURSED,MAGE,DRAGON,0",
        "O26,HERO,HERO,HERO,HERO,TRAITOR,CURSED,MAGE,DRAGON,0",
        "O27,HERO,HERO,CAPTAIN,SOLDIER,TRAITOR,MAGE,MAGE,DRAGON,0",
        "O28,CAPTAIN,CAPTAIN,CAPTAIN,SOLDIER,CURSED,CURSED,MAGE,WILDBOAR,0",
        "O29,HERO,HERO,TRAITOR,CURSED,CURSED,MAGE,MAGE,WILDBOAR,0",
        "O30,HERO,TRAITOR,TRAITOR,CURSED,MAGE,MAGE,MAGE,WILDBOAR,0",
        "O31,HERO,HERO,CAPTAIN,SOLDIER,CURSED,CURSED,MAGE,EAGLE,0",
        "O32,HERO,HERO,CAPTAIN,SOLDIER,SOLDIER,TRAITOR,MAGE,EAGLE,0",
        "O33,HERO,HERO,HERO,SOLDIER,TRAITOR,MAGE,MAGE,EAGLE,0",
        "O34,HERO,HERO,SOLDIER,SOLDIER,TRAITOR,MAGE,MAGE,HORSE,WILDBOAR",
        "O35,SOLDIER,SOLDIER,CURSED,CURSED,CURSED,MAGE,MAGE,WOLF,DRAGON",
        "O36,HERO,SOLDIER,SOLDIER,TRAITOR,MAGE,MAGE,MAGE,HORSE,EAGLE",
        "O37,HERO,SOLDIER,SOLDIER,TRAITOR,TRAITOR,MAGE,MAGE,DRAGON,WILDBOAR",
        "O38,HERO,HERO,SOLDIER,TRAITOR,TRAITOR,TRAITOR,MAGE,SNAKE,EAGLE",
        "O39,HERO,CAPTAIN,CAPTAIN,TRAITOR,MAGE,MAGE,MAGE,WILDBOAR,HORSE",
        "O40,HERO,SOLDIER,SOLDIER,SOLDIER,CURSED,CURSED,MAGE,SNAKE,DRAGON",
        "O41,HERO,HERO,SOLDIER,SOLDIER,TRAITOR,MAGE,MAGE,WILDBOAR,EAGLE",
        "O42,HERO,HERO,HERO,TRAITOR,TRAITOR,CURSED,MAGE,DRAGON,WILDBOAR",
        //43 = problem: not solvable!
        "O43,HERO,HERO,TRAITOR,CURSED,CURSED,MAGE,MAGE,WOLF,WILDBOAR",
        "O44,SOLDIER,SOLDIER,SOLDIER,SOLDIER,SOLDIER,CAPTAIN,MAGE,WOLF,SNAKE",
        "O45,CAPTAIN,SOLDIER,SOLDIER,SOLDIER,MAGE,MAGE,MAGE,HORSE,EAGLE",
        "O46,HERO,HERO,CAPTAIN,SOLDIER,TRAITOR,CURSED,MAGE,WOLF,WOLF",
        "O47,HERO,SOLDIER,SOLDIER,TRAITOR,TRAITOR,TRAITOR,MAGE,SNAKE,WILDBOAR",
        "O48,HERO,HERO,TRAITOR,TRAITOR,TRAITOR,CURSED,MAGE,WOLF,EAGLE",
        "O49,HERO,HERO,CAPTAIN,CAPTAIN,CAPTAIN,TRAITOR,MAGE,WOLF,SNAKE",
        "O50,HERO,HERO,CAPTAIN,TRAITOR,MAGE,MAGE,MAGE,SNAKE,HORSE",

        // Coba:
        "C 1,POTTER,POTTER,POTTER,POTTER,POTTER,POTTER,PEASANT,0,0",
        "C 2,POTTER,POTTER,PEASANT,PEASANT,PEASANT,PEASANT,SCRIBE,0,0",
        "C 3,PEASANT,PEASANT,PEASANT,PEASANT,SCRIBE,SCRIBE,THIEF,0,0",
        "C 4,POTTER,POTTER,POTTER,POTTER,PEASANT,PEASANT,SHAMAN,0,0",
        "C 5,POTTER,POTTER,PEASANT,PEASANT,PEASANT,THIEF,QUEEN,0,0",
        "C 6,POTTER,POTTER,POTTER,POTTER,PEASANT,THIEF,SHAMAN,0,0",
        "C 7,POTTER,PEASANT,PEASANT,SCRIBE,SCRIBE,SCRIBE,QUEEN,0,0",
        "C 8,POTTER,POTTER,POTTER,PEASANT,THIEF,SHAMAN,QUEEN,0,0",
        "C 9,PEASANT,PEASANT,SCRIBE,SCRIBE,SCRIBE,SHAMAN,QUEEN,0,0",
        "C10,POTTER,SCRIBE,SCRIBE,THIEF,THIEF,SHAMAN,QUEEN,0,0",
        "C11,PEASANT,THIEF,THIEF,THIEF,SHAMAN,SHAMAN,QUEEN,0,0",
        "C12,POTTER,POTTER,PEASANT,THIEF,SHAMAN,QUEEN,QUEEN,0,0",
        "C13,PEASANT,SCRIBE,SCRIBE,SHAMAN,SHAMAN,QUEEN,QUEEN,0,0",
        "C14,POTTER,PEASANT,PEASANT,SHAMAN,SHAMAN,SHAMAN,QUEEN,0,0",
        "C15,SCRIBE,SCRIBE,THIEF,SHAMAN,SHAMAN,SHAMAN,QUEEN,0,0",
    };
    const int s_size = sizeof(s) / sizeof(s[0]);
    for (int i = 0; i < s_size; i++) {
        tuple_with_desc t_d = create_tuple_desc_from_string(s[i]);
        compute_dice_challenge(t_d.desc, t_d.t);
    }
    return 0;
}
//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//
//#define HERO 1
//#define CAPTAIN 2
//#define SOLDIER 3
//#define TRAITOR 4
//#define CURSED 5
//#define MAGE 6
//
//#define POTTER 13
//#define PEASANT 14
//#define SCRIBE 15
//#define THIEF 16
//#define SHAMAN 17
//#define QUEEN 18
//
//#define NUM_DICE 7
//#define NUM_SIDES 6
//
//int firstSet[NUM_SIDES] = {HERO, CAPTAIN, SOLDIER, TRAITOR, CURSED, MAGE};
//int secondSet[NUM_SIDES] = {POTTER, PEASANT, SCRIBE, THIEF, SHAMAN, QUEEN};
//
//void nextCombination(int *combination, int len) {
//    // Move to the next combination by incrementing like a number in base-NUM_SIDES
//    for (int i = len - 1; i >= 0; --i) {
//        if (++combination[i] >= NUM_SIDES) {
//            combination[i] = 0;
//        } else {
//            break;
//        }
//    }
//}
//
//void processCombination(const int *combination, int len, int current, int total) {
//    if ((rand() % 50000000) == 0) {
//        double percentage = 100.0 * current / total;
//        printf("Progress: %.2f%%\n", percentage);
//    }
//    // Add any additional processing logic here...
//}
//
//int main() {
//    srand(time(NULL));
//    int totalCombinations = 1;
//    for (int i = 0; i < NUM_DICE * 2; i++) {
//        totalCombinations *= NUM_SIDES;
//    }
//
//    int *combination = (int *)calloc(NUM_DICE * 2, sizeof(int));
//    if (combination == NULL) {
//        perror("Memory allocation failed");
//        return 1;
//    }
//
//    for (int i = 0; i < totalCombinations; i++) {
//        processCombination(combination, NUM_DICE * 2, i, totalCombinations);
//        nextCombination(combination, NUM_DICE * 2);
//    }
//
//    free(combination);
//    return 0;
//}