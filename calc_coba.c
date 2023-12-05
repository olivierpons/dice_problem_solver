#include <stdbool.h>
#include <string.h>
#include <values.h>
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
    bool is_present = false;
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
    while (t->a[i] == QUEEN) {
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

void calc_armadillo(tuple *t) {
    int to_change = 0;
    for (int i = 0; i < t->len_local; ++i) {
        if (t->local[i] == ARMADILLO) {
            to_change += 1;
        }
    }
    if (to_change) {
        for (int i = 0; i < t->a_len; ++i) {
            if ((t->a[i] >= HERO && t->a[i] <= MAGE) ||
                (t->a[i] >= POTTER && t->a[i] <= QUEEN)) {
                t->a_sum[i] += to_change;
            }
        }
    }
}

void calc_deer(tuple *t) {
    int to_change = 0;
    for (int i = 0; i < t->len_local; ++i) {
        if (t->local[i] == DEER) {
            to_change -= 1;
        }
    }
    if (to_change) {
        for (int i = 0; i < t->a_len; ++i) {
            if ((t->a[i] >= HERO && t->a[i] <= MAGE) ||
                (t->a[i] >= POTTER && t->a[i] <= QUEEN)) {
                t->a_sum[i] += to_change;
            }
        }
    }
}

void calc_iguana(tuple *t) {
    int to_change = 0;
    for (int i = 0; i < t->len_local; ++i) {
        if (t->local[i] == IGUANA) {
            to_change += 1;
        }
    }
    if (to_change) {
        int nb_found[NUM_ROLES];
        memset(nb_found, 0, NUM_ROLES * sizeof(int));
        for (int i = 0; i < t->a_len; i++) {
            nb_found[t->a[i]]++;
        }
        for (int i = 0; i < NUM_ROLES; i++) {
            if (nb_found[i] == 1) {
                for (int j = 0; j < t->a_len; j++) {
                    if (t->a[j] == i) {
                        t->a_sum[j] += to_change;
                    }
                }
            }
        }
    }
}

void calc_scorpio(tuple *t) {
    int to_change = 0;
    for (int i = 0; i < t->len_local; ++i) {
        if (t->local[i] == SCORPIO) {
            to_change -= 1;
        }
    }
    if (to_change) {
        int nb_found[NUM_ROLES];
        memset(nb_found, 0, NUM_ROLES * sizeof(int));
        for (int i = 0; i < t->a_len; i++) {
            nb_found[t->a[i]]++;
        }
        for (int i = 0; i < NUM_ROLES; i++) {
            if (nb_found[i] == 1) {
                for (int j = 0; j < t->a_len; j++) {
                    if (t->a[j] == i) {
                        t->a_sum[j] += to_change;
                    }
                }
            }
        }
    }
}

void calc_jaguar(tuple *t) {
    bool found = false;
    for (int i = 0; i < t->len_local; ++i) {
        if (t->local[i] == JAGUAR) {
            found = true;
            break;
        }
    }
    if (found) {
        int low_idx = 0;
        int low_value = t->a_sum[0];
        int big_value = low_value;
        for (int i = 1; i < t->a_len; i++) {
            if (t->a_sum[i] < low_value) {
                low_idx = i;
                low_value = t->a_sum[i];
            }
            if (t->a_sum[i] > big_value) {
                big_value = t->a_sum[i];
            }
        }
        t->a_sum[low_idx] = big_value;
    }
}

void calc_bee(tuple *t) {
    bool found = false;
    for (int i = 0; i < t->len_local; ++i) {
        if (t->local[i] == BEE) {
            found = true;
            break;
        }
    }
    if (found) {
        int big_idx = 0;
        int big_value = t->a_sum[0];
        int low_value = big_value;
        for (int i = 1; i < t->a_len; i++) {
            if (t->a_sum[i] > big_value) {
                big_idx = i;
                big_value = t->a_sum[i];
            }
            if (t->a_sum[i] < low_value) {
                low_value = t->a_sum[i];
            }
        }
        t->a_sum[big_idx] = low_value;
    }
}
