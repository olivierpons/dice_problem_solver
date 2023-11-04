#ifndef CALC_ODIN_H
#define CALC_ODIN_H

#include "structs.h"

extern void calc_heroes(tuple *t);
extern void calc_captains(tuple *t);
extern void calc_soldiers(tuple *t);
extern void calc_traitors(tuple *t);
extern void calc_cursed(tuple *t);
extern void calc_mages(tuple *t);
extern void calc_wolfs_fenrir(tuple *t, int v);
extern void calc_snake_jormungand(tuple *t, int v);
extern void calc_horse_sleipnir(tuple *t, int v);
extern void calc_dragon_fafnir(tuple *t, int v);
extern void calc_wildboar_gullinbursti(tuple *t, int v);
extern void calc_wildboar_eagle_hraesvelg(tuple *t, int v);

#endif // CALC_ODIN_H
