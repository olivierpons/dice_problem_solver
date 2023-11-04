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

void calc_scribe(tuple *t, int v) {

}

void calc_thief(tuple *t, int v) {

}

void calc_shaman(tuple *t, int v) {

}

void calc_queen(tuple *t, int v) {

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
