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

void calc_wolfs_fenrir(tuple *t, int v) {
    if (v != WOLF_FENRIR) {
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

void calc_snake_jormungand(tuple *t, int v) {
    if (v != SNAKE_JORMUNGAND) {
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

void calc_horse_sleipnir(tuple *t, int v) {
    if (v != HORSE_SLEIPNIR) {
        return;
    }
    for (int i = 0; i < t->a_len; ++i) {
        t->a_sum[i]++;
    }
}

void calc_dragon_fafnir(tuple *t, int v) {
    if (v != DRAGON_FAFNIR) {
        return;
    }
    for (int i = 0; i < t->a_len; ++i) {
        t->a_sum[i]--;
    }
}

void calc_wildboar_gullinbursti(tuple *t, int v) {
    if (v != WILDBOAR_GULLINBURSTI) {
        return;
    }
    int ok[t->a_len];
    memset(ok, 0, t->a_len * sizeof(*ok));
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

void calc_wildboar_eagle_hraesvelg(tuple *t, int v) {
    if (v != EAGLE_HRAESVELG) {
        return;
    }
    int ok[t->a_len];
    memset(ok, 0, t->a_len * sizeof(*ok));
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

