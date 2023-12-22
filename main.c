#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>
#include <sys/sysinfo.h>

#include "calc_odin.h"
#include "calc_coba.h"

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void count_elements(const int *array, int length, int *counts, int max_element) {
    memset(counts, 0, max_element * sizeof(int));
    for (int i = 0; i < length; ++i) {
        if (array[i] > 0 && array[i] <= max_element) {
            counts[array[i] - 1]++;
        }
    }
}

bool combination_exists(tuples *tps, int size, tuple *t1, tuple *t2) {
    int t1_a_counts[NUM_ROLES];
    int t2_a_counts[NUM_ROLES];
    int t1_local_counts[MAX_LOCAL];
    int t2_local_counts[MAX_LOCAL];

    count_elements(t1->a, t1->a_len, t1_a_counts, NUM_ROLES);
    count_elements(t2->a, t2->a_len, t2_a_counts, NUM_ROLES);
    count_elements(t1->local, t1->len_local, t1_local_counts, MAX_LOCAL);
    count_elements(t2->local, t2->len_local, t2_local_counts, MAX_LOCAL);

    for (int i = 0; i < size; ++i) {
        int tps_t1_a_counts[NUM_ROLES];
        int tps_t2_a_counts[NUM_ROLES];
        int tps_t1_local_counts[MAX_LOCAL];
        int tps_t2_local_counts[MAX_LOCAL];

        count_elements(tps[i].a1.a, tps[i].a1.a_len, tps_t1_a_counts, NUM_ROLES);
        count_elements(tps[i].a2.a, tps[i].a2.a_len, tps_t2_a_counts, NUM_ROLES);
        count_elements(tps[i].a1.local, tps[i].a1.len_local, tps_t1_local_counts, MAX_LOCAL);
        count_elements(tps[i].a2.local, tps[i].a2.len_local, tps_t2_local_counts, MAX_LOCAL);

        bool match1 = (
            memcmp(t1_a_counts, tps_t1_a_counts, NUM_ROLES * sizeof(int)) == 0 &&
            memcmp(t2_a_counts, tps_t2_a_counts, NUM_ROLES * sizeof(int)) == 0 &&
            memcmp(t1_local_counts, tps_t1_local_counts, MAX_LOCAL * sizeof(int)) == 0 &&
            memcmp(t2_local_counts, tps_t2_local_counts, MAX_LOCAL * sizeof(int)) == 0
        );

        bool match2 = (
            memcmp(t1_a_counts, tps_t2_a_counts, NUM_ROLES * sizeof(int)) == 0 &&
            memcmp(t2_a_counts, tps_t1_a_counts, NUM_ROLES * sizeof(int)) == 0 &&
            memcmp(t1_local_counts, tps_t2_local_counts, MAX_LOCAL * sizeof(int)) == 0 &&
            memcmp(t2_local_counts, tps_t1_local_counts, MAX_LOCAL * sizeof(int)) == 0
        );

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
    calc_armadillo(t);
    calc_deer(t);
    calc_iguana(t);
    calc_scorpio(t);
    calc_jaguar(t);
    calc_bee(t);
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

void distribute_local(
    tuples* tps,
    const int *local, int len_local, int *global, int len_global,
    tuples** tps_found, int* tps_found_size, int* tps_found_capacity
) {
    int local_clean[MAX_LOCAL];
    int len_local_clean = 0;
    int nb_armadillos = 0;
    int nb_deers = 0;
    for (int i = 0; i < len_local; ++i) {
        if (local[i] == ARMADILLO) {
            nb_armadillos++;
        } else if (local[i] == DEER) {
            nb_deers++;
        } else {
            local_clean[len_local_clean++] = local[i];
        }
    }

    do {
        for (int i = 0; i <= len_local_clean; i++) {
            tps->a1.len_local = i;
            tps->a2.len_local = len_local_clean - i;
            for (int j = 0; j < i; j++) {
                tps->a1.local[j] = local_clean[j];
            }
            for (int j = 0; j < tps->a2.len_local; j++) {
                tps->a2.local[j] = local_clean[j + i];
            }
            if (nb_armadillos || nb_deers) {
                /* If armadillos / deers, we need to have different lengths: */
                if (tps->a1.a_len == tps->a2.a_len) {
                    return;
                }
                /* Armadillo = goes to the smallest group: */
                tuple *t = tps->a1.a_len > tps->a2.a_len ? &tps->a2 : &tps->a1;
                for (int j = 0; j < nb_armadillos; j++) {
                    t->local[t->len_local++] = ARMADILLO;
                }
                /* Deer = goes to the biggest group: */
                t = tps->a1.a_len > tps->a2.a_len ? &tps->a1 : &tps->a2;
                for (int j = 0; j < nb_deers; j++) {
                    t->local[t->len_local++] = DEER;
                }
            }
            int a2_base[MAX_LOCAL];
            memcpy(a2_base, tps->a2.local, tps->a2.len_local * sizeof(int));
            do {
                do {
                    /* To breakpoint for debugging:
                    if (tps->a1.a_len == 3 &&
                        tps->a1.a[0] == PEASANT &&
                        tps->a1.a[1] == SCRIBE &&
                        tps->a1.a[2] == SCRIBE &&
                        tps->a1.len_local == 2
                        ) {
                        print_tuples(tps, false);
                    } */
                    test_tuples(
                        tps, global, len_global,
                        tps_found, tps_found_size, tps_found_capacity
                    );
                } while (next_permutation(tps->a2.local, tps->a2.len_local));
                memcpy(tps->a2.local, a2_base, tps->a2.len_local * sizeof(int));
            } while (next_permutation(tps->a1.local, tps->a1.len_local));
        }
    } while (next_permutation(local_clean, len_local_clean));
}


void generate_all_groupings(
    const int *array, int size,
    int *local, int len_local, int *global, int len_global,
    tuples** tps_found, int* tps_found_size, int* tps_found_capacity
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
            distribute_local(
                &tps, local, len_local, global, len_global,
                tps_found, tps_found_size, tps_found_capacity
            );
        } else {
            test_tuples(
                &tps, global, len_global,
                tps_found, tps_found_size, tps_found_capacity
            );
        }
    }
}

#define NUM_DICE 7
int dice_odin[] = {HERO, CAPTAIN, SOLDIER, TRAITOR, CURSED, MAGE};
int dice_coba[] = {POTTER, PEASANT, SCRIBE, THIEF, SHAMAN, QUEEN};

void printArray(int* array, int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", array[i]);
    printf("\n");
}

/*
 * First found:
odin1=0, odin2=0, odin3=0, odin4=0, odin5=0, odin6=0, odin7=0, coba1=0, coba2=0, coba3=0, coba4=0, coba5=0
[Hero/3, Hero/3, Hero/3, Hero/3, Potter/1, Potter/1] = 14 - [Hero/3, Hero/3, Hero/3, Potter/1, Potter/1, Potter/1, Potter/1, Potter/1] = 14
[Hero/3, Hero/3, Hero/3, Hero/3, Hero/3, Potter/1, Potter/1] = 17 - [Hero/3, Hero/3, Potter/1, Potter/1, Potter/1, Potter/1, Shaman/7] = 17
[Hero/3, Hero/3, Hero/3, Hero/3] = 12 - [Hero/3, Hero/3, Hero/3, Potter/0, Potter/0, Potter/0, Potter/0, Potter/0, Potter/0, Queen/3] = 12
[Hero/3, Hero/3, Hero/3, Hero/3, Potter/1, Scribe/2] = 15 - [Hero/3, Hero/3, Hero/3, Potter/1, Potter/1, Potter/1, Potter/1, Scribe/2] = 15
[Hero/3, Hero/3, Hero/3, Hero/3, Potter/1, Thief/-1] = 12 - [Hero/3, Hero/3, Hero/3, Potter/0, Potter/0, Potter/0, Potter/0, Queen/3] = 12
[Hero/3, Hero/3, Hero/3, Hero/3, Hero/3, Potter/1, Potter/1] = 17 - [Hero/3, Hero/3, Potter/1, Potter/1, Potter/1, Potter/1, Shaman/7] = 17
[Hero/3, Hero/3, Hero/3, Hero/3] = 12 - [Hero/3, Hero/3, Hero/3, Potter/0, Potter/0, Potter/0, Potter/0, Potter/0, Queen/3, Potter/0] = 12
[Hero/3, Hero/3, Hero/3, Hero/3, Potter/1, Thief/-1] = 12 - [Hero/3, Hero/3, Hero/3, Potter/0, Potter/0, Potter/0, Potter/0, Queen/3] = 12
Took 0 h 0 mn 24 s to execute.
odin1=0, odin2=0, odin3=0, odin4=0, odin5=0, odin6=0, odin7=0, coba1=0, coba2=0, coba3=0, coba4=0, coba5=1
Took 0 h 1 mn 40 s to execute.
odin1=0, odin2=0, odin3=0, odin4=0, odin5=0, odin6=0, odin7=0, coba1=0, coba2=0, coba3=0, coba4=0, coba5=2
[Hero/3, Hero/3, Hero/3, Hero/3, Potter/1, Scribe/2] = 15 - [Hero/3, Hero/3, Hero/3, Potter/1, Potter/1, Potter/1, Potter/1, Scribe/2] = 15
[Hero/3, Hero/3, Hero/3, Hero/3, Potter/1, Scribe/2] = 15 - [Hero/3, Hero/3, Hero/3, Potter/1, Potter/1, Potter/1, Potter/1, Scribe/2] = 15
[Hero/3, Hero/3, Hero/3, Hero/3, Scribe/0, Queen/3] = 15 - [Hero/3, Hero/3, Hero/3, Potter/1, Potter/1, Potter/1, Potter/1, Scribe/2] = 15
[Hero/3, Hero/3, Hero/3, Hero/3, Scribe/0, Queen/3] = 15 - [Hero/3, Hero/3, Hero/3, Potter/1, Potter/1, Potter/1, Potter/1, Scribe/2] = 15
Took 0 h 1 mn 41 s to execute.
odin1=0, odin2=0, odin3=0, odin4=0, odin5=0, odin6=0, odin7=0, coba1=0, coba2=0, coba3=0, coba4=0, coba5=3
[Hero/3, Hero/3, Hero/3, Hero/3, Potter/1, Thief/-1] = 12 - [Hero/3, Hero/3, Hero/3, Potter/0, Potter/0, Potter/0, Potter/0, Queen/3] = 12
[Hero/3, Hero/3, Hero/3, Hero/3, Potter/1, Thief/-1] = 12 - [Hero/3, Hero/3, Hero/3, Potter/0, Potter/0, Potter/0, Potter/0, Queen/3] = 12
Took 0 h 1 mn 42 s to execute.
odin1=0, odin2=0, odin3=0, odin4=0, odin5=0, odin6=0, odin7=0, coba1=0, coba2=0, coba3=0, coba4=0, coba5=4
[Hero/3, Hero/3, Hero/3, Hero/3, Hero/3, Potter/1, Potter/1] = 17 - [Hero/3, Hero/3, Potter/1, Potter/1, Potter/1, Potter/1, Shaman/7] = 17
Took 0 h 1 mn 42 s to execute.
odin1=0, odin2=0, odin3=0, odin4=0, odin5=0, odin6=0, odin7=0, coba1=0, coba2=0, coba3=0, coba4=0, coba5=5
[Hero/3, Hero/3, Hero/3, Hero/3] = 12 - [Hero/3, Hero/3, Hero/3, Potter/0, Potter/0, Potter/0, Potter/0, Queen/3, Potter/0, Potter/0] = 12
[Hero/3, Hero/3, Hero/3, Hero/3, Potter/1, Thief/-1] = 12 - [Hero/3, Hero/3, Hero/3, Potter/0, Potter/0, Potter/0, Potter/0, Queen/3] = 12
[Hero/3, Hero/3, Hero/3, Hero/3, Scribe/0, Queen/3] = 15 - [Hero/3, Hero/3, Hero/3, Potter/1, Potter/1, Potter/1, Potter/1, Scribe/2] = 15
[Hero/3, Hero/3, Hero/3, Hero/3, Potter/1, Thief/-1] = 12 - [Hero/3, Hero/3, Hero/3, Potter/0, Potter/0, Potter/0, Potter/0, Queen/3] = 12
Took 0 h 1 mn 49 s to execute.
odin1=0, odin2=0, odin3=0, odin4=0, odin5=0, odin6=0, odin7=0, coba1=0, coba2=0, coba3=0, coba4=1, coba5=0
Took 0 h 1 mn 42 s to execute.
 */

typedef struct CombinationNode {
    int combination[MAX_NORMAL];
    int len;
    struct CombinationNode *next;
} CombinationNode;

typedef struct {
    CombinationNode *head;
} CombinationSet;

float getAvailableMemoryPercentage() {
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        return -1.0f;
    }
    unsigned long long total_ram = info.totalram * info.mem_unit;
    unsigned long long free_ram = info.freeram * info.mem_unit;
    double total_ram_double = (double)total_ram;
    double free_ram_double = (double)free_ram;
    return (float)(free_ram_double / total_ram_double * 100.0);
}

bool combinationExists(CombinationSet *set, int *combination, int len) {
    CombinationNode *node = set->head;
    while (node) {
        if (node->len == len && memcmp(node->combination, combination, len * sizeof(int)) == 0) {
            return true;
        }
        node = node->next;
    }
    return false;
}

void initCombinationSet(CombinationSet *set) {
    set->head = NULL;
}

void freeCombinationSet(CombinationSet *set) {
    CombinationNode *node = set->head;
    while (node) {
        CombinationNode *temp = node;
        node = node->next;
        free(temp);
    }
    set->head = NULL;
}

bool addCombination(CombinationSet *set, int *combination, int len) {
    if (getAvailableMemoryPercentage() < 2.0) {
        return false;
    }

    CombinationNode *newNode = malloc(sizeof(CombinationNode));
    if (newNode == NULL) {
        return false;
    }
    memcpy(newNode->combination, combination, len * sizeof(int));
    newNode->len = len;
    newNode->next = set->head;
    set->head = newNode;
    return true;
}

void sortGroup(int *group, int len) {
    int i, key, j;
    for (i = 1; i < len; i++) {
        key = group[i];
        j = i - 1;
        while (j >= 0 && group[j] > key) {
            group[j + 1] = group[j];
            j = j - 1;
        }
        group[j + 1] = key;
    }
}


void prepareCombination(int *combination) {
    sortGroup(combination, 7);
    sortGroup(combination + 7, 7);
}

void print_omp_critical(
    int o1, int o2, int o3, int o4, int o5, int o6, int o7,
    int c1, int c2, int c3, int c4, int c5, int c6, int c7,
    const tuple *t1, const tuple *t2
) {
    #pragma omp critical
    {
        print_role(o1); printf(",");
        print_role(o2); printf(",");
        print_role(o3); printf(",");
        print_role(o4); printf(",");
        print_role(o5); printf(",");
        print_role(o6); printf(",");
        print_role(o7); printf(",");
        print_role(c1); printf(",");
        print_role(c2); printf(",");
        print_role(c3); printf(",");
        print_role(c4); printf(",");
        print_role(c5); printf(",");
        print_role(c6); printf(",");
        print_role(c7); printf(" =>\n");
        print_tuple(t1, ", ", " - ", true);
        print_tuple(t2, ", ", "\n", true);
    }
}

#define PRINT_CURRENT_COMB(more) printf(\
    "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%s%s", \
    odin1, odin2, odin3, odin4, odin5, odin6, odin7, \
    coba1, coba2, coba3, coba4, coba5, coba6, coba7, \
    more, (strlen(more) ? "\n": ""))
void generateCombinations() {
    int globals[MAX_GLOBAL] = {0};
    int len_globals = 0;
    int local[MAX_LOCAL] = {0};
    int len_local = 0;
    tuples* tps_ok = NULL;
    int tps_size = 0, tps_capacity = 0;

    CombinationSet combSet;
    initCombinationSet(&combSet);

    // Set the number of threads to use based on the number of available
    // processors:
    omp_set_num_threads((omp_get_num_procs() / 4) * 3);
    // The 'collapse(x)' directive merges the x innermost loops into a single
    // parallel loop:
#pragma omp parallel for collapse(3) \
    private(tps_ok, tps_size, tps_capacity) \
    default(none) shared(globals, len_globals, local, len_local, combSet, dice_odin, dice_coba)

    for (int odin1=0; odin1 < 6; odin1++) {
        for (int odin2=0; odin2 < 6; odin2++) {
            for (int odin3=0; odin3 < 6; odin3++) {
                for (int odin4=0; odin4 < 6; odin4++) {
                    for (int odin5=0; odin5 < 6; odin5++) {
                        for (int odin6=0; odin6 < 6; odin6++) {
                            for (int odin7=0; odin7 < 6; odin7++) {
                                for (int coba1=0; coba1 < 6; coba1++) {
                                    for (int coba2=0; coba2 < 6; coba2++) {
                                        for (int coba3=0; coba3 < 6; coba3++) {
                                            for (int coba4=0; coba4 < 6; coba4++) {
                                                for (int coba5=0; coba5 < 6; coba5++) {
                                                    for (int coba6=0; coba6 < 6; coba6++) {
                                                        for (int coba7=0; coba7 < 6; coba7++) {
                                                            tuple_with_desc t_d;

                                                            int currentCombination[14] = {
                                                                odin1, odin2, odin3, odin4, odin5, odin6, odin7,
                                                                coba1, coba2, coba3, coba4, coba5, coba6, coba7,
                                                            };
                                                            prepareCombination(currentCombination);
                                                            bool already_done;
                                                            #pragma omp critical
                                                            {
                                                                already_done = combinationExists(
                                                                    &combSet, currentCombination, 14
                                                                );
                                                            }
                                                            if (already_done) {
                                                                // PRINT_CURRENT_COMB(" exists, continue.");
                                                                continue;
                                                            }
                                                            #pragma omp critical
                                                            {
                                                                if (!addCombination(&combSet, currentCombination, 14)) {
                                                                    printf("Not enough memory, stopping.\n");
                                                                    freeCombinationSet(&combSet);
                                                                    exit(-1);
                                                                }
                                                            }
                                                            double start, end, cpu_time_used;
                                                            start = omp_get_wtime();
                                                            PRINT_CURRENT_COMB("...");
                                                            tps_ok = NULL;
                                                            tps_size = tps_capacity = 0;
                                                            memset(&t_d, 0, sizeof(tuple_with_desc));
                                                            t_d.t.a[ 0] = dice_odin[odin1];
                                                            t_d.t.a[ 1] = dice_odin[odin2];
                                                            t_d.t.a[ 2] = dice_odin[odin3];
                                                            t_d.t.a[ 3] = dice_odin[odin4];
                                                            t_d.t.a[ 4] = dice_odin[odin5];
                                                            t_d.t.a[ 5] = dice_odin[odin6];
                                                            t_d.t.a[ 6] = dice_odin[odin7];
                                                            t_d.t.a[ 7] = dice_coba[coba1];
                                                            t_d.t.a[ 8] = dice_coba[coba2];
                                                            t_d.t.a[ 9] = dice_coba[coba3];
                                                            t_d.t.a[10] = dice_coba[coba4];
                                                            t_d.t.a[11] = dice_coba[coba5];
                                                            t_d.t.a[12] = dice_coba[coba6];
                                                            t_d.t.a[13] = dice_coba[coba7];
                                                            t_d.t.a_len = 14;
                                                            do {
                                                                generate_all_groupings(
                                                                    t_d.t.a, t_d.t.a_len,
                                                                    local, len_local,
                                                                    globals, len_globals,
                                                                    &tps_ok, &tps_size, &tps_capacity
                                                                );
                                                            } while (next_permutation(t_d.t.a, t_d.t.a_len));
                                                            if (tps_size == 1) {
                                                                print_omp_critical(
                                                                    dice_odin[odin1],
                                                                    dice_odin[odin2],
                                                                    dice_odin[odin3],
                                                                    dice_odin[odin4],
                                                                    dice_odin[odin5],
                                                                    dice_odin[odin6],
                                                                    dice_odin[odin7],
                                                                    dice_coba[coba1],
                                                                    dice_coba[coba2],
                                                                    dice_coba[coba3],
                                                                    dice_coba[coba4],
                                                                    dice_coba[coba5],
                                                                    dice_coba[coba6],
                                                                    dice_coba[coba7],
                                                                    &tps_ok[tps_size - 1].a1,
                                                                    &tps_ok[tps_size - 1].a2
                                                                );
                                                            }
                                                            free(tps_ok);
                                                            end = omp_get_wtime();
                                                            cpu_time_used = end - start;
                                                            // Convert to hours, minutes, and seconds
                                                            int hours = (int) cpu_time_used / 3600;
                                                            int minutes = ((int) cpu_time_used % 3600) / 60;
                                                            int seconds = (int) cpu_time_used % 60;
                                                            #pragma omp critical
                                                            {
                                                                PRINT_CURRENT_COMB("");
                                                                printf(
                                                                    " took %d h %d mn %d s to execute.\n",
                                                                    hours, minutes, seconds
                                                                );
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    freeCombinationSet(&combSet);
}

int main() {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    setbuf(stdin, NULL);

    #ifdef __linux__
    char buffer[128];
    FILE *fp = fopen("/etc/os-release", "r");
    if (fp == NULL) {
        printf("Cannot open /etc/os-release. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    int isDebianBased = 0;
    while (fgets(buffer, 128, fp) != NULL) {
        if (strstr(buffer, "ID_LIKE=") && strstr(buffer, "debian")) {
            isDebianBased = 1;
            break;
        }
        if (strstr(buffer, "ID=debian")) {
            isDebianBased = 1;
            break;
        }
    }
    fclose(fp);

    if (!isDebianBased) {
        printf("This program is intended to be run on Debian or its derivatives. Exiting.\n");
        exit(EXIT_FAILURE);
    }

//    int currentOdin[NUM_DICE];
//    int currentCoba[NUM_DICE];
    generateCombinations();
    return 0;

    /* region problems */
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
        // "CEx,POTTER,POTTER,PEASANT,PEASANT,THIEF,THIEF,QUEEN,ARMADILLO,0",
        "C16,PEASANT,PEASANT,PEASANT,PEASANT,THIEF,THIEF,SHAMAN,ARMADILLO,0",
        "C17,POTTER,POTTER,SCRIBE,SCRIBE,SCRIBE,SHAMAN,SHAMAN,ARMADILLO,0",
        "C18,PEASANT,SCRIBE,THIEF,SHAMAN,QUEEN,QUEEN,QUEEN,ARMADILLO,0",
        // "CEx,POTTER,POTTER,POTTER,POTTER,PEASANT,PEASANT,QUEEN,DEER,0",
        "C19,POTTER,THIEF,THIEF,THIEF,SHAMAN,SHAMAN,SHAMAN,DEER,0",
        "C20,POTTER,POTTER,PEASANT,SHAMAN,SHAMAN,QUEEN,QUEEN,DEER,0",
        "C21,POTTER,SCRIBE,SCRIBE,THIEF,THIEF,SHAMAN,QUEEN,DEER,0",
        "C22,POTTER,PEASANT,PEASANT,PEASANT,PEASANT,THIEF,SHAMAN,IGUANA,0",
        "C23,PEASANT,THIEF,THIEF,THIEF,THIEF,SHAMAN,QUEEN,IGUANA,0",
        "C24,PEASANT,SCRIBE,SCRIBE,SCRIBE,SHAMAN,SHAMAN,QUEEN,IGUANA,0",
        "C25,POTTER,POTTER,POTTER,PEASANT,PEASANT,SHAMAN,SHAMAN,SCORPIO,0",
        "C26,POTTER,POTTER,PEASANT,SHAMAN,QUEEN,QUEEN,QUEEN,SCORPIO,0",
        "C27,POTTER,POTTER,SCRIBE,SCRIBE,SHAMAN,SHAMAN,QUEEN,SCORPIO,0",
        "C28,PEASANT,THIEF,THIEF,THIEF,THIEF,SHAMAN,SHAMAN,JAGUAR,0",
        "C29,PEASANT,THIEF,THIEF,SHAMAN,SHAMAN,QUEEN,QUEEN,JAGUAR,0",
        "C30,POTTER,SCRIBE,SCRIBE,THIEF,SHAMAN,QUEEN,QUEEN,JAGUAR,0",
        "C31,POTTER,PEASANT,PEASANT,PEASANT,THIEF,THIEF,SHAMAN,BEE,0",
        "C32,POTTER,THIEF,THIEF,THIEF,SHAMAN,SHAMAN,QUEEN,BEE,0",
        "C33,SCRIBE,SCRIBE,THIEF,SHAMAN,SHAMAN,QUEEN,QUEEN,BEE,0",
        "C34,POTTER,POTTER,SCRIBE,SCRIBE,SCRIBE,SHAMAN,SHAMAN,ARMADILLO,JAGUAR",
        "C35,POTTER,PEASANT,PEASANT,PEASANT,PEASANT,SCRIBE,SHAMAN,DEER,IGUANA",
        "C36,SCRIBE,SCRIBE,SCRIBE,SCRIBE,SHAMAN,SHAMAN,SHAMAN,SCORPIO,BEE",
        "C37,POTTER,THIEF,THIEF,SHAMAN,QUEEN,QUEEN,QUEEN,ARMADILLO,IGUANA",
        "C38,PEASANT,THIEF,THIEF,THIEF,SHAMAN,QUEEN,QUEEN,JAGUAR,BEE",
        "C39,SCRIBE,THIEF,THIEF,SHAMAN,SHAMAN,SHAMAN,QUEEN,ARMADILLO,DEER",
        "C40,PEASANT,THIEF,THIEF,THIEF,SHAMAN,QUEEN,QUEEN,ARMADILLO,SCORPIO",
        "C41,POTTER,POTTER,POTTER,POTTER,SHAMAN,QUEEN,QUEEN,DEER,JAGUAR",
        "C42,PEASANT,SCRIBE,SCRIBE,SHAMAN,SHAMAN,SHAMAN,QUEEN,SCORPIO,SCORPIO",
        "C43,POTTER,POTTER,SCRIBE,SCRIBE,SHAMAN,QUEEN,QUEEN,ARMADILLO,IGUANA",
        "C44,SCRIBE,SCRIBE,THIEF,SHAMAN,QUEEN,QUEEN,QUEEN,DEER,BEE",
        "C45,PEASANT,PEASANT,SHAMAN,SHAMAN,SHAMAN,QUEEN,QUEEN,SCORPIO,JAGUAR",
        "C46,PEASANT,SCRIBE,THIEF,THIEF,THIEF,SHAMAN,QUEEN,IGUANA,IGUANA",
        "C47,POTTER,POTTER,SCRIBE,SHAMAN,SHAMAN,SHAMAN,QUEEN,ARMADILLO,BEE",
        "C48,POTTER,SCRIBE,THIEF,SHAMAN,SHAMAN,SHAMAN,QUEEN,BEE,BEE",
        "C49,SCRIBE,SCRIBE,SCRIBE,THIEF,SHAMAN,SHAMAN,QUEEN,DEER,IGUANA",
        "C50,SCRIBE,SCRIBE,SHAMAN,QUEEN,QUEEN,QUEEN,QUEEN,SCORPIO,BEE",
    };
    /* endregion problems */

    const int s_size = sizeof(s) / sizeof(s[0]);
    for (int i = 0; i < s_size; i++) {
        tuple_with_desc  t_d;
        create_tuple_desc_from_string(&t_d, s[i]);

        int globals[MAX_GLOBAL] = {0};
        int len_globals = 0;
        int local[MAX_LOCAL] = {0};
        int len_local = 0;

        /* separate local to distribute and global for both groups: */
        for (int j = 0; j < t_d.t.len_local; j++) {
            if (t_d.t.local[j] == WILDBOAR || t_d.t.local[j] == EAGLE) {
                globals[len_globals++] = t_d.t.local[j];
            } else {
                local[len_local++] = t_d.t.local[j];
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

        printf("%s\n", t_d.desc);
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
    #else
    printf("This program can only be run on Linux Debian. Exiting.\n");
    exit(EXIT_FAILURE);
    #endif
}
