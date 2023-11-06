#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <stddef.h>
#include "structs.h"

const char *roleNames[] = {
    [HERO] = "Hero",
    [CAPTAIN] = "Captain",
    [SOLDIER] = "Soldier",
    [TRAITOR] = "Traitor",
    [CURSED] = "Cursed",
    [MAGE] = "Mage",
    [WOLF] = "Wolf",
    [SNAKE] = "Snake",
    [HORSE] = "Horse",
    [DRAGON] = "Dragon",
    [WILDBOAR] = "Wildboar",
    [EAGLE] = "Eagle",

    [POTTER] = "Potter",
    [PEASANT] = "Peasant",
    [SCRIBE] = "Scribe",
    [THIEF] = "Thief",
    [SHAMAN] = "Shaman",
    [QUEEN] = "Queen",
    [ARMADILLO] = "Armadillo",
    [DEER] = "Deer",
    [IGUANA] = "Iguana",
    [SCORPION] = "Scorpion",
    [JAGUAR] = "Jaguar",
    [BEE] = "Bee",
};

void print_role(int role) {
//    printf("%d", (int)role);
//    return;
    if (role >= NUM_ROLES || roleNames[role] == NULL) {
        printf("Unknown: %d", (int)role);
    } else {
        printf("%s", roleNames[role]);
    }
}

void print_tuple(
    const tuple *t, const char *delimiter, const char *end, bool with_sum
) {
    printf("[");
    for (int i = 0; i < t->a_len; ++i) {
        print_role(t->a[i]);
        if (with_sum) {
            printf("/%d", t->a_sum[i]);
        }
        if (i < t->a_len - 1) {
            printf("%s", delimiter);
        }
    }
    printf("]");
    if (t->len_local > 0) {
        printf(" (");
        for (int i = 0; i < t->len_local; ++i) {
            print_role(t->local[i]);
            if (i < t->len_local - 1) {
                printf("%s", delimiter);
            }
        }
        printf(")");
    }
    if (with_sum) {
        printf(" = %d", t->sum);
    }
    printf("%s", end);
}

void print_tuples(tuples* t, bool with_sum) {
    print_tuple(&t->a1, ", ", " - ", with_sum);
    print_tuple(&t->a2, ", ", "", with_sum);
    if (t->len_global) {
        printf(" - (");
        for (int j = 0; j < t->len_global; ++j) {
            print_role(t->global[j]);
            if (j < t->len_global - 1) {
                printf(", ");
            }
        }
        printf(")");
    }
    printf("\n");
}
int string_const_to_int(const char* str) {
    if (strcmp(str, "HERO") == 0) return HERO;
    if (strcmp(str, "CAPTAIN") == 0) return CAPTAIN;
    if (strcmp(str, "SOLDIER") == 0) return SOLDIER;
    if (strcmp(str, "TRAITOR") == 0) return TRAITOR;
    if (strcmp(str, "CURSED") == 0) return CURSED;
    if (strcmp(str, "MAGE") == 0) return MAGE;
    if (strcmp(str, "WOLF") == 0) return WOLF;
    if (strcmp(str, "SNAKE") == 0) return SNAKE;
    if (strcmp(str, "HORSE") == 0) return HORSE;
    if (strcmp(str, "DRAGON") == 0) return DRAGON;
    if (strcmp(str, "WILDBOAR") == 0) return WILDBOAR;
    if (strcmp(str, "EAGLE") == 0) return EAGLE;
    if (strcmp(str, "POTTER") == 0) return POTTER;
    if (strcmp(str, "PEASANT") == 0) return PEASANT;
    if (strcmp(str, "SCRIBE") == 0) return SCRIBE;
    if (strcmp(str, "THIEF") == 0) return THIEF;
    if (strcmp(str, "SHAMAN") == 0) return SHAMAN;
    if (strcmp(str, "QUEEN") == 0) return QUEEN;
    if (strcmp(str, "ARMADILLO") == 0) return ARMADILLO;
    if (strcmp(str, "DEER") == 0) return DEER;
    if (strcmp(str, "IGUANA") == 0) return IGUANA;
    if (strcmp(str, "SCORPION") == 0) return SCORPION;
    if (strcmp(str, "JAGUAR") == 0) return JAGUAR;
    if (strcmp(str, "BEE") == 0) return BEE;

    char* end_ptr;
    long val = strtol(str, &end_ptr, 10);
    if (str == end_ptr) {
        return -1;
    }
    if (*end_ptr != '\0') {
        return -1;
    }
    if (val < INT_MIN || val > INT_MAX) {
        return -1;
    }
    return (int)val;
}


typedef struct {
    int number;
    const char* string;
} NumberStringMapping;

NumberStringMapping mappings[] = {
    {HERO, "HERO"},
    {CAPTAIN, "CAPTAIN"},
    {SOLDIER, "SOLDIER"},
    {TRAITOR, "TRAITOR"},
    {CURSED, "CURSED"},
    {MAGE, "MAGE"},
    {WOLF, "WOLF"},
    {SNAKE, "SNAKE"},
    {HORSE, "HORSE"},
    {DRAGON, "DRAGON"},
    {WILDBOAR, "WILDBOAR"},
    {EAGLE, "EAGLE"},
    {POTTER, "POTTER"},
    {PEASANT, "PEASANT"},
    {SCRIBE, "SCRIBE"},
    {THIEF, "THIEF"},
    {SHAMAN, "SHAMAN"},
    {QUEEN, "QUEEN"},
    {ARMADILLO, "ARMADILLO"},
    {DEER, "DEER"},
    {IGUANA, "IGUANA"},
    {SCORPION, "SCORPION"},
    {JAGUAR, "JAGUAR"},
    {BEE, "BEE"},
    {0, NULL}
};

const char* number_to_const_string(int number) {
    for (int i = 0; mappings[i].string != NULL; ++i) {
        if (mappings[i].number == number) {
            return mappings[i].string;
        }
    }
    return NULL;
}

int has_value(tuple *t, int value) {
    for (int i = 0; i < t->a_len; ++i) {
        if (t->a[i] == value) {
            return 1;
        }
    }
    return 0;
}

tuple create_tuple_from_string(const char* str) {
    tuple t;
    memset(&t, 0, sizeof(tuple));
    char* temp_str = strdup(str);
    if (!temp_str) {
        fprintf(stderr, "Memory allocation error\n");
        t.a[0] = -1;
        return t;
    }
    int total = 0;
    char* token = strtok(temp_str, ",");
    while (token != NULL) {
        total++;
        token = strtok(NULL, ",");
    }
    free(temp_str);
    if (total < 2 || total > MAX_NORMAL + MAX_GLOBAL) {
        fprintf(stderr, "Invalid number of elements\n");
        t.a[0] = -1;
        return t;
    }

    temp_str = strdup(str);
    int index = 0;
    token = strtok(temp_str, ",");
    while (token != NULL) {
        while (*token == ' ') token++;
        int v = string_const_to_int(token);
        if ( v &&
            (v != WOLF) &&
            (v != SNAKE) &&
            (v != HORSE) &&
            (v != DRAGON) &&
            (v != WILDBOAR) &&
            (v != EAGLE) &&
            (v != ARMADILLO) &&
            (v != DEER) &&
            (v != IGUANA) &&
            (v != SCORPION) &&
            (v != JAGUAR) &&
            (v != BEE)
            ) {
            t.a[t.a_len++] = v;
        } else if (v) {
            t.local[t.len_local++] = v;
        }
        index++;
        token = strtok(NULL, ",");
    }
    free(temp_str);
    return t;
}


tuple_with_desc create_tuple_desc_from_string(const char* str) {
    tuple_with_desc t_desc;
    memset(&t_desc, 0, sizeof(t_desc));

    char* temp_str = strdup(str);
    if (!temp_str) {
        fprintf(stderr, "Memory allocation error\n");
        return t_desc;
    }
    char* comma_pos = strchr(temp_str, ',');
    if (comma_pos != NULL) {
        ptrdiff_t desc_len = comma_pos - temp_str;
        char temp_desc[50];
        strncpy(temp_desc, temp_str, desc_len);
        temp_desc[desc_len] = '\0';
        if (strncmp(temp_desc, "O", 1) == 0 && strlen(temp_desc) == 3) {
            sprintf(t_desc.desc, "Odin %s", temp_desc + 1);
        } else if (strncmp(temp_desc, "C", 1) == 0 && strlen(temp_desc) == 3) {
            sprintf(t_desc.desc, "Coba %s", temp_desc + 1);
        } else {
            strncpy(t_desc.desc, temp_desc, sizeof(t_desc.desc));
            t_desc.desc[sizeof(t_desc.desc) - 1] = '\0';
        }
        t_desc.t = create_tuple_from_string(comma_pos + 1);
    } else {
        fprintf(stderr, "No description found\n");
    }
    free(temp_str);
    return t_desc;
}

int compare_ints(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

bool compare_arrays(int *arr1, int len1, int *arr2, int len2) {
    if (len1 != len2) {
        return false;
    }
    qsort(arr1, len1, sizeof(int), compare_ints);
    qsort(arr2, len2, sizeof(int), compare_ints);
    for (int i = 0; i < len1; i++) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }
    return true;
}

bool tuple_exists(tuples *array, int array_len, tuples target) {
    for (int i = 0; i < array_len; i++) {
        if (
            compare_arrays(array[i].a1.a, array[i].a1.a_len, target.a1.a, target.a1.a_len) &&
            compare_arrays(array[i].a2.a, array[i].a2.a_len, target.a2.a, target.a2.a_len) &&
            compare_arrays(array[i].global, array[i].len_global, target.global, target.len_global)
        ) {
            return true;
        }
    }
    return false;
}