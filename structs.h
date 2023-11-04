#ifndef STRUCTS_H
#define STRUCTS_H

#define ARRAY_MAX_SIZE (int)7

#define HERO (int) 1
#define CAPTAIN (int) 2
#define SOLDIER (int) 3
#define TRAITOR (int) 4
#define CURSED (int) 5
#define MAGE (int) 6
#define WOLF_FENRIR (int) 7
#define SNAKE_JORMUNGAND (int) 8
#define HORSE_SLEIPNIR (int) 9
#define DRAGON_FAFNIR (int) 10
#define WILDBOAR_GULLINBURSTI (int) 11
#define EAGLE_HRAESVELG (int) 12

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

#define NUM_ROLES 12

extern const char *roleNames[];

#endif // STRUCTS_H
