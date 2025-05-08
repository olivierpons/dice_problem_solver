#ifndef CALC_ODIN_H
#define CALC_ODIN_H

#include "structs.h"

extern void calc_heroes(tuple *t);
extern void calc_captains(tuple *t);
extern void calc_soldiers(tuple *t);
extern void calc_traitors(tuple *t);
extern void calc_cursed(tuple *t);
extern void calc_mages(tuple *t);
extern void calc_wolf(tuple *t, int v);
extern void calc_snake(tuple *t, int v);
extern void calc_horse(tuple *t, int v);
extern void calc_dragon(tuple *t, int v);
extern void calc_wildboar(tuple *t, int v);
extern void calc_wildboar_and_eagle(tuple *t, int v);

#endif // CALC_ODIN_H
