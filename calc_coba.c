#include <stdbool.h>
#include "calc_coba.h"

void calc_potter(tuple *t) {
    for (int i = 0; i < t->a_len; ++i) {
        if (t->a[i] == POTTER) {
            t->a_sum[i] = 1;
        }
    }
}

void calc_peasant(tuple *t) {
    for (int i = 0; i < t->a_len; ++i) {
        if (t->a[i] == PEASANT) {
            t->a_sum[i] = 2;
        }
    }
}

void calc_scribe(tuple *t) {
    int v = t->a_len % 2 ? 1 : 2;
    for (int i = 0; i < t->a_len; ++i) {
        if (t->a[i] == SCRIBE) {
            t->a_sum[i] = v;
        }
    }
}

void calc_thief(tuple *t) {
    for (int i = 0; i < t->a_len; ++i) {
        if (t->a[i] == THIEF) {
            t->a_sum[i] = -1;
        }
    }
}

void calc_shaman(tuple *t1, int other_len) {
    for (int i = 0; i < t1->a_len; ++i) {
        if (t1->a[i] == SHAMAN) {
            t1->a_sum[i] = other_len;
        }
    }
}

void calc_queen(tuple *t) {
    bool is_present = 0;
    int i;
    for (i = 0; i < t->a_len; ++i) {
        if (t->a[i] == QUEEN) {
            if (t->a_sum[i] == 3) { /* already done */
                return;
            }
            t->a_sum[i] = 3;
            is_present = true;
        }
    }
    if (!is_present) {
        return;
    }
    /* Get the lowest value, start at first which is NOT queen: */
    i = 0;
    while (t->a_sum[i] == QUEEN) {
        i++;
    }
    int min_value = t->a_sum[i];
    for (i = 1; i < t->a_len; ++i) {
        if (t->a[i] == QUEEN) {
            continue;
        }
        if (t->a_sum[i] < min_value) {
            min_value = t->a_sum[i];
        }
    }
    for (i = 0; i < t->a_len; ++i) {
        if (t->a[i] == QUEEN) {
            continue;
        }
        if (t->a_sum[i] == min_value) {
            t->a_sum[i] = 0;
        }
    }
}

void calc_armadillo(tuple *t, int v) {

}

void calc_deer(tuple *t, int v) {

}

void calc_iguana(tuple *t, int v) {

}

void calc_scorpion(tuple *t, int v) {

}

void calc_jaguar(tuple *t, int v) {

}

void calc_bee(tuple *t, int v) {

}
