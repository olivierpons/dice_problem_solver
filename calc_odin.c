#include <string.h>
#include "calc_odin.h"

void calc_heroes(tuple *t) {
    for (int i = 0; i < t->a_len; ++i) {
        if (t->a[i] == HERO) {
            t->a_sum[i] = 3;
        }
    }
}

void calc_captains(tuple *t) {
    for (int i = 0; i < t->a_len; ++i) {
        if (t->a[i] == CAPTAIN) {
            t->a_sum[i] = 2;
        }
    }
}

void calc_soldiers(tuple *t) {
    for (int i = 0; i < t->a_len; ++i) {
        if (t->a[i] == SOLDIER) {
            t->a_sum[i] = 1;
        }
    }
}

void calc_traitors(tuple *t) {
    for (int i = 0; i < t->a_len; ++i) {
        if (t->a[i] == TRAITOR) {
            t->a_sum[i] = 1;
            for (int j = 0; j < t->a_len; ++j) {
                if (t->a[j] == HERO && t->a_sum[j]) {
                    t->a_sum[j] = 0;
                    break;
                }
            }
        }
    }
}

void calc_cursed(tuple *t) {
    for (int i = 0; i < t->a_len; ++i) {
        if (t->a[i] == CURSED) {
            t->a_sum[i] = -1;
        }
    }
}

void calc_mages(tuple *t) {
    for (int i = 0; i < t->a_len; ++i) {
        if (t->a[i] == MAGE) {
            int s = 0;
            for (int j = 0; j < t->a_len; ++j) {
                if (t->a[j] != MAGE) {
                    s++;
                }
            }
            t->a_sum[i] = s;
        }
    }
}
void calc_wolf(tuple *t, int v) {
    if (v != WOLF) {
        return;
    }
    int mx = 0, j = 0;
    for (int i = 0; i < t->a_len; ++i) {
        if (t->a_sum[i] > mx) {
            mx = t->a_sum[i];
            j = i;
        }
    }
    if (mx) {
        t->a_sum[j] *= 2;
    }
}

void calc_snake(tuple *t, int v) {
    if (v != SNAKE) {
        return;
    }
    int mx = 0, j = 0;
    for (int i = 0; i < t->a_len; ++i) {
        if (t->a_sum[i] > mx) {
            mx = t->a_sum[i];
            j = i;
        }
    }
    if (mx) {
        t->a_sum[j] = (int)-(t->a_sum[j]);
    }
}

void calc_horse(tuple *t, int v) {
    if (v != HORSE) {
        return;
    }
    for (int i = 0; i < t->a_len; ++i) {
        t->a_sum[i]++;
    }
}

void calc_dragon(tuple *t, int v) {
    if (v != DRAGON) {
        return;
    }
    for (int i = 0; i < t->a_len; ++i) {
        t->a_sum[i]--;
    }
}

void calc_wildboar(tuple *t, int v) {
    if (v != WILDBOAR) {
        return;
    }
    int ok[ARRAY_MAX_SIZE];
    memset(ok, 0, sizeof(ok[0]) * ARRAY_MAX_SIZE);
    for (int i = 0; i < t->a_len; ++i) {
        int value = t->a[i];
        for (int j = i + 1; j < t->a_len; ++j) {
            if (t->a[j] == value) {
                ok[i] = 1;
                ok[j] = 1;
            }
        }
    }
    for (int i = 0; i < t->a_len; ++i) {
        if (ok[i]) {
            t->a_sum[i]++;
        }
    }
}

void calc_wildboar_and_eagle(tuple *t, int v) {
    if (v != EAGLE) {
        return;
    }
    int ok[ARRAY_MAX_SIZE];
    memset(ok, 0, sizeof(ok[0]) * ARRAY_MAX_SIZE);
    for (int i = 0; i < t->a_len; ++i) {
        int value = t->a[i];
        for (int j = i + 1; j < t->a_len; ++j) {
            if (t->a[j] == value) {
                ok[i] = 1;
                ok[j] = 1;
            }
        }
    }
    for (int i = 0; i < t->a_len; ++i) {
        if (ok[i]) {
            t->a_sum[i]--;
        }
    }
}

