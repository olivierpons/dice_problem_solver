#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

void next_perm(int *nums, int numsSize) {
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

int** permute(const int *nums, int nums_size, int *return_size, int **block) {
    *return_size = factorial(nums_size);
    int **result = (int **)calloc(*return_size, sizeof(int *));
    *block = (int *)calloc(*return_size * nums_size, sizeof(int));

    if (result == NULL) {
        exit(EXIT_FAILURE);
    }
    if (block == NULL) {
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *return_size; i++) {
        result[i] = *block + i * nums_size;
    }

    for (int i = 0; i < nums_size; i++) {
        result[0][i] = nums[i];
    }

    for (int i = 1; i < *return_size; i++) {
        for (int j = 0; j < nums_size; j++) {
            result[i][j] = result[i - 1][j];
        }
        next_perm(result[i], nums_size);
    }
    return result;
}

bool combination_exists(tuples *t, int size, tuple *t1, tuple *t2) {
    for (int i = 0; i < size; ++i) {
        bool match1 = (memcmp(t[i].a1.a, t1->a, t1->a_len * sizeof(int)) == 0 &&
                       memcmp(t[i].a2.a, t2->a, t2->a_len * sizeof(int)) == 0 &&
                       memcmp(t[i].a1.local, t1->local, t1->len_local * sizeof(int)) == 0 &&
                       memcmp(t[i].a2.local, t2->local, t2->len_local * sizeof(int)) == 0);

        bool match2 = (memcmp(t[i].a2.a, t1->a, t1->a_len * sizeof(int)) == 0 &&
                       memcmp(t[i].a1.a, t2->a, t2->a_len * sizeof(int)) == 0 &&
                       memcmp(t[i].a2.local, t1->local, t1->len_local * sizeof(int)) == 0 &&
                       memcmp(t[i].a1.local, t2->local, t2->len_local * sizeof(int)) == 0);

        if (match1 || match2) {
            return true;
        }
    }
    return false;
}

void add_tuple_once(
    tuples **tps, int *size, int *capacity, tuple *a1, tuple *a2,
    const int globals[], int len_globals
) {
    if (combination_exists(*tps, *size, a1, a2)) {
        return;
    }

    if (*size == 0) {
        *capacity = 1;
        *tps = (tuples *)malloc((*capacity) * sizeof(tuples));
        if (!*tps) {
            fprintf(stderr, "Memory allocation error\n");
            return;
        }
        memset(*tps, 0, (*capacity) * sizeof(tuples));
    }
    if (*size >= *capacity) {
        *capacity *= 2;
        *tps = (tuples *)realloc(*tps, (*capacity) * sizeof(tuples));
        if (!*tps) {
            fprintf(stderr, "Memory allocation error\n");
            return;
        }
        memset(&((*tps)[*size]), 0, ((*capacity) - *size) * sizeof(tuples));
    }

    (*tps)[*size].a1 = *a1;
    (*tps)[*size].a2 = *a2;
    memcpy((*tps)[*size].global, globals, (*tps)[*size].len_global * sizeof(int));
    (*tps)[*size].len_global = len_globals < MAX_GLOBAL ? len_globals : MAX_GLOBAL;
//    print_tuples(&(*tps)[*size], false);
    (*size)++;
}

void generate_all_permutations_and_add_tuples(
    tuples **tps, int *size, int *capacity,
    const tuple *t1, const tuple *t2,
    const int globals[], int len_globals
) {
    int local_size = t1->len_local + t2->len_local;
    int return_size;
    int *block;
    int **perms;
    int c_local[MAX_LOCAL * 2];

    memcpy(c_local, t1->local, t1->len_local * sizeof(int));
    memcpy(c_local + t1->len_local, t2->local, t2->len_local * sizeof(int));

    perms = permute(c_local, local_size, &return_size, &block);

    for (int i = 0; i < return_size; ++i) {
        tuple n1 = *t1;
        tuple n2 = *t2;

        memcpy(n1.local, perms[i], t1->len_local * sizeof(int));
        memcpy(n2.local, perms[i] + t1->len_local, t2->len_local * sizeof(int));

        add_tuple_once(tps, size, capacity, &n1, &n2, globals, len_globals);
    }

    free(block);
    free(perms);
}

void add_tuples(
    tuples **t, int *size, int *capacity, int *arr, int cut,
    int globals[], int len_globals, const int local[], int len_local
) {
    tuple t1 = {.a_len = cut, .len_local = len_local};
    tuple t2 = {.a_len = MAX_NORMAL - cut, .len_local = len_local};

    memcpy(t1.a, arr, cut * sizeof(int));
    memcpy(t2.a, arr + cut, (MAX_NORMAL - cut) * sizeof(int));

    qsort(t1.a, t1.a_len, sizeof(int), compare_ints);
    qsort(t2.a, t2.a_len, sizeof(int), compare_ints);

    for (int mask = 0; mask < (1 << len_local); ++mask) {
        t1.len_local = 0;
        t2.len_local = 0;

        for (int i = 0; i < len_local; ++i) {
            if (mask & (1 << i)) {
                t1.local[t1.len_local++] = local[i];
            } else {
                t2.local[t2.len_local++] = local[i];
            }
        }
        generate_all_permutations_and_add_tuples(
            t, size, capacity, &t1, &t2, globals, len_globals
        );
    }
}

void sum_one_pass_1(tuple *t) {
    memset(t->a_sum, 0, sizeof(int) * MAX_NORMAL);
    calc_heroes(t);
    calc_captains(t);
    calc_soldiers(t);
    calc_cursed(t);
    calc_traitors(t);
    calc_mages(t);

    // Process each modifier in local
    for (int i = 0; i < t->len_local; ++i) {
        calc_wolf(t, t->local[i]);
        calc_snake(t, t->local[i]);
        calc_horse(t, t->local[i]);
        calc_dragon(t, t->local[i]);
    }

    calc_potter(t);
    calc_peasant(t);
    calc_scribe(t);
    calc_thief(t);
}

void sum_one_pass_2(tuple *t) {
    calc_queen(t);
//    calc_armadillo(t);
//    calc_deer(t);
//    calc_iguana(t);
//    calc_scorpion(t);
//    calc_jaguar(t);
//    calc_bee(t);
}

void sum_both(tuples *t, int v) {
    calc_wildboar(&t->a1, v);
    calc_wildboar(&t->a2, v);
    calc_wildboar_and_eagle(&t->a1, v);
    calc_wildboar_and_eagle(&t->a2, v);
}

void sum_final(tuple *t) {
    t->sum = 0;
    for (int i = 0; i < t->a_len; ++i) {
        t->sum = (int)(t->sum + t->a_sum[i]);
    }

}

void compute_dice_challenge(const char* description, tuple t) {
    int return_size = 0;
    int *block = NULL;
    int **result = permute(t.a, MAX_NORMAL, &return_size, &block);

    tuples *tps = NULL;
    int size = 0;
    int capacity = 0;

    int globals[MAX_GLOBAL] = {0};
    int len_globals = 0;
    int local[MAX_LOCAL] = {0};
    int len_local = 0;

    for (int i = 0; i < t.len_local; i++) {
        if (t.local[i] == WILDBOAR || t.local[i] == EAGLE) {
            globals[len_globals++] = t.local[i];
        } else {
            local[len_local++] = t.local[i];
        }
    }
    for (int i = 0; i < return_size; i++) {
        for (int j = 1; j < MAX_NORMAL; j++) {
            add_tuples(
                &tps, &size, &capacity, result[i], j,
                globals, len_globals, local, len_local
            );
        }
    }

    printf("\nChallenge %s - [ ", description);
    for (int i = 0; i < MAX_NORMAL; ++i) {
        print_role(t.a[i]);
        if (i < MAX_NORMAL - 1) {
            printf(", ");
        }
    }
    printf("]");
    if (len_globals) {
        printf(" (");
        for (int i = 0; i < len_globals; ++i) {
            print_role(globals[i]);
            if (i < len_globals - 1) {
                printf(", ");
            }
        }
        printf(")");
    }
    printf("\n");
    for (int i = 0; i < size; ++i) {
        sum_one_pass_1(&tps[i].a1);
        sum_one_pass_1(&tps[i].a2);

        calc_shaman(&tps[i].a1, tps[i].a2.a_len);
        calc_shaman(&tps[i].a2, tps[i].a1.a_len);

        sum_one_pass_2(&tps[i].a1);
        sum_one_pass_2(&tps[i].a2);

        for (int j = 0; j < len_globals; ++j) {
            sum_both(&tps[i], globals[j]);
        }
        sum_final(&tps[i].a1);
        sum_final(&tps[i].a2);

        if (tps[i].a1.sum == tps[i].a2.sum) {
            print_tuple(&tps[i].a1, ", ", " - ", true);
            print_tuple(&tps[i].a2, ", ", "", true);
            if (len_globals > 0) {
                printf(" - (");
                for (int j = 0; j < len_globals; ++j) {
                    print_role(globals[j]);
                    if (j < len_globals - 1) {
                        printf(", ");
                    }
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

//int main() {
//    char *s[] = {
//        // Odin:
//        "O 1,HERO,HERO,HERO,CAPTAIN,SOLDIER,SOLDIER,SOLDIER,0,0",
//        "O 2,HERO,HERO,CAPTAIN,CAPTAIN,CAPTAIN,CAPTAIN,TRAITOR,0,0",
//        "O 3,HERO,HERO,HERO,CAPTAIN,CAPTAIN,CAPTAIN,CURSED,0,0",
//        "O 4,HERO,HERO,HERO,HERO,SOLDIER,TRAITOR,CURSED,0,0",
//        "O 5,SOLDIER,SOLDIER,SOLDIER,SOLDIER,SOLDIER,SOLDIER,MAGE,0,0",
//        "O 6,HERO,HERO,CAPTAIN,SOLDIER,SOLDIER,SOLDIER,MAGE,0,0",
//        "O 7,HERO,HERO,CAPTAIN,SOLDIER,SOLDIER,CURSED,MAGE,0,0",
//        "O 8,HERO,CAPTAIN,CAPTAIN,CAPTAIN,CURSED,CURSED,MAGE,0,0",
//        "O 9,HERO,HERO,CAPTAIN,CAPTAIN,SOLDIER,TRAITOR,MAGE,0,0",
//        "O10,HERO,HERO,CAPTAIN,TRAITOR,TRAITOR,TRAITOR,MAGE,0,0",
//        "O11,HERO,HERO,CAPTAIN,CAPTAIN,TRAITOR,CURSED,MAGE,0,0",
//        "O12,HERO,HERO,SOLDIER,SOLDIER,TRAITOR,MAGE,MAGE,0,0",
//        "O13,SOLDIER,CURSED,CURSED,MAGE,MAGE,MAGE,MAGE,0,0",
//        "O14,HERO,CAPTAIN,CAPTAIN,CURSED,MAGE,MAGE,MAGE,0,0",
//        "O15,HERO,SOLDIER,TRAITOR,TRAITOR,MAGE,MAGE,MAGE,0,0",
//        "O16,HERO,HERO,HERO,CAPTAIN,SOLDIER,SOLDIER,MAGE,WOLF,0",
//        "O17,HERO,CAPTAIN,CAPTAIN,CAPTAIN,SOLDIER,TRAITOR,MAGE,WOLF,0",
//        "O18,HERO,SOLDIER,SOLDIER,SOLDIER,TRAITOR,MAGE,MAGE,WOLF,0",
//        "O19,HERO,SOLDIER,SOLDIER,CURSED,CURSED,CURSED,MAGE,SNAKE,0",
//        "O20,HERO,TRAITOR,TRAITOR,CURSED,MAGE,MAGE,MAGE,SNAKE,0",
//        "O21,HERO,HERO,CAPTAIN,TRAITOR,CURSED,MAGE,MAGE,SNAKE,0",
//        "O22,HERO,CAPTAIN,CAPTAIN,CAPTAIN,SOLDIER,SOLDIER,MAGE,HORSE,0",
//        "O23,HERO,HERO,CAPTAIN,CAPTAIN,SOLDIER,TRAITOR,MAGE,HORSE,0",
//        "O24,HERO,CAPTAIN,TRAITOR,CURSED,MAGE,MAGE,MAGE,HORSE,0",
//        "O25,SOLDIER,SOLDIER,SOLDIER,CURSED,CURSED,CURSED,MAGE,DRAGON,0",
//        "O26,HERO,HERO,HERO,HERO,TRAITOR,CURSED,MAGE,DRAGON,0",
//        "O27,HERO,HERO,CAPTAIN,SOLDIER,TRAITOR,MAGE,MAGE,DRAGON,0",
//        "O28,CAPTAIN,CAPTAIN,CAPTAIN,SOLDIER,CURSED,CURSED,MAGE,WILDBOAR,0",
//        "O29,HERO,HERO,TRAITOR,CURSED,CURSED,MAGE,MAGE,WILDBOAR,0",
//        "O30,HERO,TRAITOR,TRAITOR,CURSED,MAGE,MAGE,MAGE,WILDBOAR,0",
//        "O31,HERO,HERO,CAPTAIN,SOLDIER,CURSED,CURSED,MAGE,EAGLE,0",
//        "O32,HERO,HERO,CAPTAIN,SOLDIER,SOLDIER,TRAITOR,MAGE,EAGLE,0",
//        "O33,HERO,HERO,HERO,SOLDIER,TRAITOR,MAGE,MAGE,EAGLE,0",
//        "O34,HERO,HERO,SOLDIER,SOLDIER,TRAITOR,MAGE,MAGE,HORSE,WILDBOAR",
//        "O35,SOLDIER,SOLDIER,CURSED,CURSED,CURSED,MAGE,MAGE,WOLF,DRAGON",
//        "O36,HERO,SOLDIER,SOLDIER,TRAITOR,MAGE,MAGE,MAGE,HORSE,EAGLE",
//        "O37,HERO,SOLDIER,SOLDIER,TRAITOR,TRAITOR,MAGE,MAGE,DRAGON,WILDBOAR",
//        "O38,HERO,HERO,SOLDIER,TRAITOR,TRAITOR,TRAITOR,MAGE,SNAKE,EAGLE",
//        "O39,HERO,CAPTAIN,CAPTAIN,TRAITOR,MAGE,MAGE,MAGE,WILDBOAR,HORSE",
//        "O40,HERO,SOLDIER,SOLDIER,SOLDIER,CURSED,CURSED,MAGE,SNAKE,DRAGON",
//        "O41,HERO,HERO,SOLDIER,SOLDIER,TRAITOR,MAGE,MAGE,WILDBOAR,EAGLE",
//        "O42,HERO,HERO,HERO,TRAITOR,TRAITOR,CURSED,MAGE,DRAGON,WILDBOAR",
//        //43 = problem: not solvable!
//        "O43,HERO,HERO,TRAITOR,CURSED,CURSED,MAGE,MAGE,WOLF,WILDBOAR",
//        "O44,SOLDIER,SOLDIER,SOLDIER,SOLDIER,SOLDIER,CAPTAIN,MAGE,WOLF,SNAKE",
//        "O45,CAPTAIN,SOLDIER,SOLDIER,SOLDIER,MAGE,MAGE,MAGE,HORSE,EAGLE",
//        "O46,HERO,HERO,CAPTAIN,SOLDIER,TRAITOR,CURSED,MAGE,WOLF,WOLF",
//        "O47,HERO,SOLDIER,SOLDIER,TRAITOR,TRAITOR,TRAITOR,MAGE,SNAKE,WILDBOAR",
//        "O48,HERO,HERO,TRAITOR,TRAITOR,TRAITOR,CURSED,MAGE,WOLF,EAGLE",
//        "O49,HERO,HERO,CAPTAIN,CAPTAIN,CAPTAIN,TRAITOR,MAGE,WOLF,SNAKE",
//        "O50,HERO,HERO,CAPTAIN,TRAITOR,MAGE,MAGE,MAGE,SNAKE,HORSE",
//
//        // Coba:
//        "C 1,POTTER,POTTER,POTTER,POTTER,POTTER,POTTER,PEASANT,0,0",
//        "C 2,POTTER,POTTER,PEASANT,PEASANT,PEASANT,PEASANT,SCRIBE,0,0",
//        "C 3,PEASANT,PEASANT,PEASANT,PEASANT,SCRIBE,SCRIBE,THIEF,0,0",
//        "C 4,POTTER,POTTER,POTTER,POTTER,PEASANT,PEASANT,SHAMAN,0,0",
//        "C 5,POTTER,POTTER,PEASANT,PEASANT,PEASANT,THIEF,QUEEN,0,0",
//        "C 6,POTTER,POTTER,POTTER,POTTER,PEASANT,THIEF,SHAMAN,0,0",
//        "C 7,POTTER,PEASANT,PEASANT,SCRIBE,SCRIBE,SCRIBE,QUEEN,0,0",
//        "C 8,POTTER,POTTER,POTTER,PEASANT,THIEF,SHAMAN,QUEEN,0,0",
//        "C 9,PEASANT,PEASANT,SCRIBE,SCRIBE,SCRIBE,SHAMAN,QUEEN,0,0",
//        "C10,POTTER,SCRIBE,SCRIBE,THIEF,THIEF,SHAMAN,QUEEN,0,0",
//        "C11,PEASANT,THIEF,THIEF,THIEF,SHAMAN,SHAMAN,QUEEN,0,0",
//        "C12,POTTER,POTTER,PEASANT,THIEF,SHAMAN,QUEEN,QUEEN,0,0",
//        "C13,PEASANT,SCRIBE,SCRIBE,SHAMAN,SHAMAN,QUEEN,QUEEN,0,0",
//        "C14,POTTER,PEASANT,PEASANT,SHAMAN,SHAMAN,SHAMAN,QUEEN,0,0",
//        "C15,SCRIBE,SCRIBE,THIEF,SHAMAN,SHAMAN,SHAMAN,QUEEN,0,0",
//    };
//    const int s_size = sizeof(s) / sizeof(s[0]);
//    for (int i = 0; i < s_size; i++) {
//        tuple_with_desc t_d = create_tuple_desc_from_string(s[i]);
//        compute_dice_challenge(t_d.desc, t_d.t);
//    }
//    return 0;
//}


bool next_permutation(int *array, int size) {
    if (size < 2) return false;

    int i = size - 2;
    while (i >= 0 && array[i] >= array[i + 1]) {
        i--;
    }
    if (i < 0) return false;
    int j = size - 1;
    while (array[j] <= array[i]) {
        j--;
    }
    swap(&array[i], &array[j]);
    for (j = size - 1; i + 1 < j; i++, j--) {
        swap(&array[i + 1], &array[j]);
    }
    return true;
}



void test_tuples(
    tuples* tps, int *globals, int len_globals,
    tuples** tps_ok, int* tps_size, int* tps_capacity
) {
    sum_one_pass_1(&tps->a1);
    sum_one_pass_1(&tps->a2);

    calc_shaman(&tps->a1, tps->a2.a_len);
    calc_shaman(&tps->a2, tps->a1.a_len);

    sum_one_pass_2(&tps->a1);
    sum_one_pass_2(&tps->a2);

    for (int j = 0; j < len_globals; ++j) {
        sum_both(tps, globals[j]);
    }
    sum_final(&tps->a1);
    sum_final(&tps->a2);

    if (tps->a1.sum == tps->a2.sum) {
        add_tuple_once(
            tps_ok, tps_size, tps_capacity, &tps->a1, &tps->a2,
            globals, len_globals
        );
    }
}

void distribute(
    tuples* tps, int cut,
    const int *local, int len_local, int *global, int len_global,
    tuples** tps_ok, int* tps_size, int* tps_cap
) {
    tps->a1.len_local = cut;
    tps->a2.len_local = len_local - cut;
    for (int i = 0; i < cut; i++) {
        tps->a1.local[i] = local[i];
    }
    for (int i = 0; i < len_local - cut; i++) {
        tps->a2.local[i] = local[i + cut];
    }
    test_tuples(tps, global, len_global, tps_ok, tps_size, tps_cap);
//    print_tuples(&tps, false);
}

void distribute_all(
    tuples* tps,
    int *local, int len_local, int *global, int len_global,
    tuples** tps_ok, int* tps_size, int* tps_cap
) {
    do {
        for (int i = 0; i <= len_local; i++) {
            distribute(tps, i, local, len_local, global, len_global, tps_ok, tps_size, tps_cap);
        }
    } while (next_permutation(local, len_local));
}


void generate_all_groupings(
    const int *array, int size,
    int *local, int len_local, int *global, int len_global,
    tuples** tps_ok, int* tps_size, int* tps_cap
) {
    tuples tps;
    for (int i = 0; i < size - 1; i++) {
        memset(&tps, 0, sizeof(tps));
        for (int j = 0; j <= i; j++) {
            tps.a1.a[tps.a1.a_len++] = array[j];
        }
        for (int j = i + 1; j < size; j++) {
            tps.a2.a[tps.a2.a_len++] = array[j];
        }
        if (len_local) {
            distribute_all(
                &tps, local, len_local, global, len_global,
                tps_ok, tps_size, tps_cap
            );
        } else {
            test_tuples(&tps, global, len_global, tps_ok, tps_size, tps_cap);
//            print_tuples(&tps, false);
        }
    }
}

int main() {
    char *s[] = {
//        "O 1,HERO,CAPTAIN,SOLDIER,TRAITOR, CURSED, 0,0",
//        "O 1,HERO,HERO,HERO,CAPTAIN,SOLDIER,SOLDIER,SOLDIER,0,0",
        "O47,HERO,SOLDIER,SOLDIER,TRAITOR,TRAITOR,TRAITOR,MAGE,SNAKE,0",
    };
    const int s_size = sizeof(s) / sizeof(s[0]);
    for (int i = 0; i < s_size; i++) {
        tuple_with_desc t_d = create_tuple_desc_from_string(s[i]);

        int globals[MAX_GLOBAL] = {0};
        int len_globals = 0;
        int local[MAX_LOCAL] = {0};
        int len_local = 0;

        /* separate local to distribute and global for both groups: */
        for (int j = 0; j < t_d.t.len_local; j++) {
            if (t_d.t.local[j] == WILDBOAR || t_d.t.local[i] == EAGLE) {
                globals[len_globals++] = t_d.t.local[i];
            } else {
                local[len_local++] = t_d.t.local[i];
            }
        }

        tuples* tps_ok = NULL;
        int tps_size = 0, tps_capacity = 0;
        do {
            generate_all_groupings(
                t_d.t.a, t_d.t.a_len, local, len_local, globals, len_globals,
                &tps_ok, &tps_size, &tps_capacity
            );
        } while (next_permutation(t_d.t.a, t_d.t.a_len));

        for (int j = 0; j < tps_size; ++j) {
            print_tuple(&tps_ok[j].a1, ", ", " - ", true);
            print_tuple(&tps_ok[j].a2, ", ", "", true);
            if (len_globals > 0) {
                printf(" - (");
                for (int k = 0; k < len_globals; ++k) {
                    print_role(globals[k]);
                    if (k < len_globals - 1) {
                        printf(", ");
                    }
                }
                printf(")");
            }
            printf("\n");
        }
        free(tps_ok);

    }
    return 0;
}
