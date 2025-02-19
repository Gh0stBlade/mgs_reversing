#ifndef _GASMASK_H_
#define _GASMASK_H_

#include "gmsight.h"
#include "libgv/libgv.h"
#include "Game/game.h"
#include "Game/GM_Control.h"

typedef struct Actor_gasmask
{
    Actor          field_0_actor;
    OBJECT         field_20_obj;
    GM_Control    *field_44_pCtrl;
    OBJECT        *field_48_pParent;
    int            field_4C_unit;
    int            field_50_count;
    Actor_gmsight *field_54_gmsight;
    short          field_58_maybe_pad; // not sure if padding
    short          field_5A_head_saved_packs;
    short          field_5C_head_saved_raise;
    short          field_5E_maybe_pad; // padding
} Actor_gasmask;

Actor *gasmask_init_80060C14(GM_Control *pCtrl, OBJECT *pParent, int unit);

// This has an issue with unused params
Actor_gmsight * NewGasmaskSight_80063668(void);

#endif // _GASMASK_H_
