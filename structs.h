#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>


#define HERO (int) 1
#define CAPTAIN (int) 2
#define SOLDIER (int) 3
#define TRAITOR (int) 4
#define CURSED (int) 5
#define MAGE (int) 6
#define WOLF (int) 7
#define SNAKE (int) 8
#define HORSE (int) 9
#define DRAGON (int) 10
#define WILDBOAR (int) 11
#define EAGLE (int) 12

#define POTTER (int) 13
#define PEASANT (int) 14
#define SCRIBE (int) 15
#define THIEF (int) 16
#define SHAMAN (int) 17
#define QUEEN (int) 18
#define ARMADILLO (int) 19
#define DEER (int) 20
#define IGUANA (int) 21
#define SCORPION (int) 22
#define JAGUAR (int) 23
#define BEE (int) 24

#define MAX_NORMAL (int)7
#define NUM_ROLES 24
#define MAX_GLOBAL 4
#define MAX_LOCAL MAX_GLOBAL

typedef struct {
    int a[MAX_NORMAL];
    int a_len;
    int a_sum[MAX_NORMAL];
    int sum;
    int local[MAX_LOCAL];
    int len_local;
} tuple;

typedef struct {
    char desc[50];
    tuple t;
} tuple_with_desc;

typedef struct {
    tuple a1;
    tuple a2;
    int global[MAX_GLOBAL];
    int len_global;
} tuples;

extern const char *roleNames[];

extern void print_role(int role);
extern void print_tuple(
    const tuple *t, const char *delimiter, const char *end, bool with_sum
);
extern void print_tuples(tuples* t, bool with_sum);
extern int string_const_to_int(const char* str);
extern const char* number_to_const_string(int number);
extern int has_value(tuple *t, int value);
extern tuple create_tuple_from_string(const char* str);
extern tuple_with_desc create_tuple_desc_from_string(const char* str);
extern int compare_ints(const void *a, const void *b);
extern bool compare_arrays(int *arr1, int len1, int *arr2, int len2);
extern bool tuple_exists(tuples *array, int array_len, tuples target);

#endif // STRUCTS_H
