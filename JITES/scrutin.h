#ifndef _SCRUTIN_H_
#define _SCRUTIN_H_

#include "list.h"

typedef struct
{
    char code_s;
    char scrutin_max_cand[3];
    char scrutin_max_supp[3];
    char nom_s;
    int  TYPE_SCRUTIN;
    char nbrelist_s[3];
    s_List *ptlist;
}s_Scrutin;

#endif

