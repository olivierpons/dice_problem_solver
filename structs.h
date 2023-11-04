#ifndef STRUCTS_H
#define STRUCTS_H

#define ARRAY_MAX_SIZE (int)7

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

typedef struct {
    int a[ARRAY_MAX_SIZE];
    int a_len;
    int a_sum[ARRAY_MAX_SIZE];
    int sum;
    int m1;
    int m2;
} tuple;

typedef struct {
    tuple a1;
    tuple a2;
    int m1;
    int m2;
} tuples;

#define NUM_ROLES 24

extern const char *roleNames[];

#endif // STRUCTS_H
