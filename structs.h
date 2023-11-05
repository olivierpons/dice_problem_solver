#ifndef STRUCTS_H
#define STRUCTS_H

#define MAX_NORMAL (int)7

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

extern int string_const_to_int(const char* str);
extern const char* number_to_const_string(int number);
extern int has_value(tuple *t, int value);
extern tuple create_tuple_from_string(const char* str);
extern tuple_with_desc create_tuple_desc_from_string(const char* str);
extern void process_modifiers(
    tuple *t, int global[], int *global_len, int local[], int *local_len
);

#endif // STRUCTS_H
