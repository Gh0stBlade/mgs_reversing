#include "libgv/libgv.h"
#include "kogaku2.h"
#include "unknown.h"
#include "libgcl/hash.h"

extern int GV_Clock_800AB920;
extern int GM_GameStatus_800AB3CC;

#pragma INCLUDE_ASM("asm/Equip/kogaku2_tpage_uv_update_80060F98.s")     // 268 bytes
#pragma INCLUDE_ASM("asm/Equip/kogaku2_tpage_uv_rgb_update_800610A4.s") // 352 bytes

void kogaku2_update_prims1_80061204(Actor_kogaku2 *pActor)
{
    DG_OBJS  *objs;     // $v0
    int       n_models; // $s2
    DG_OBJ   *i;        // $s1
    POLY_GT4 *pPack;    // $v0
    DG_OBJ   *j;        // $s0

    objs = pActor->field_20_pObj->objs;
    n_models = objs->n_models;
    for (i = objs->objs; n_models > 0; ++i)
    {
        pPack = i->packs[1 - GV_Clock_800AB920];
        if (pPack)
        {
            for (j = i; j; j = j->extend)
            {
                pPack = kogaku2_tpage_uv_update_80060F98(pPack, j->n_packs);
            }
        }
        --n_models;
    }
}

void kogaku2_update_prims2_800612BC(Actor_kogaku2 *pActor)
{
    DG_OBJS  *objs;     // $v0
    int       n_models; // $s2
    DG_OBJ   *i;        // $s1
    POLY_GT4 *pPack;    // $v0
    DG_OBJ   *j;        // $s0

    objs = pActor->field_20_pObj->objs;
    n_models = objs->n_models;
    for (i = objs->objs; n_models > 0; ++i)
    {
        pPack = i->packs[1 - GV_Clock_800AB920];
        if (pPack)
        {
            for (j = i; j; j = j->extend)
            {
                pPack = kogaku2_tpage_uv_rgb_update_800610A4(pPack, j->n_packs, pActor->field_2C_ypos2);
            }
        }
        --n_models;
    }
}

void kogaku2_kill_helper_80061384(Actor_kogaku2 *pActor)
{
    DG_OBJS *objs;     // $a2
    DG_OBJ  *pIter;    // $s0
    int      n_models; // $s1

    objs = pActor->field_20_pObj->objs;
    pIter = objs->objs;
    n_models = objs->n_models;
    objs->flag = (objs->flag & DG_FLAG_INVISIBLE) | (pActor->field_28_obj_old_flag & ~DG_FLAG_INVISIBLE);
    while (n_models > 0)
    {
        DG_WriteObjPacketUV_8001A774(pIter, 0);
        DG_WriteObjPacketUV_8001A774(pIter, 1);
        ++pIter;
        --n_models;
    }
}

void kogaku2_act_800613FC(Actor_kogaku2 *pActor)
{
    int field_2C_ypos2 = pActor->field_2C_ypos2;
    if (pActor->field_30_ypos1 < field_2C_ypos2)
    {
        pActor->field_2C_ypos2 = field_2C_ypos2 - pActor->field_38_ninja_var;
        kogaku2_update_prims2_800612BC(pActor);

        field_2C_ypos2 = pActor->field_2C_ypos2;
        if (pActor->field_30_ypos1 >= field_2C_ypos2)
        {
            EQ_InvisibleUnit_80060E68(pActor->field_20_pObj->objs, pActor->field_40_rgb, 0);
        }
    }
    else
    {
        EQ_InvisibleUnit_80060E68(pActor->field_20_pObj->objs, pActor->field_40_rgb, 1);
        kogaku2_update_prims1_80061204(pActor);
    }
    if ((GM_GameStatus_800AB3CC & 8) != 0)
    {
        pActor->field_20_pObj->objs->flag = pActor->field_28_obj_old_flag;
        DG_FreeObjsPacket_8001ABA8(pActor->field_20_pObj->objs, 0);
        DG_FreeObjsPacket_8001ABA8(pActor->field_20_pObj->objs, 1);
        pActor->field_0_actor.mFnUpdate = (TActorFunction)kogaku2_act_helper_80061528;
        pActor->field_0_actor.mFnShutdown = (TActorFunction)kogaku2_act_nullsub_800615F4;
    }
}

void kogaku2_kill_80061508(Actor_kogaku2 *pActor)
{
    kogaku2_kill_helper_80061384(pActor);
}

void kogaku2_act_helper_80061528(Actor_kogaku2 *pActor)
{
    if (!(GM_GameStatus_800AB3CC & 8))
    {
        pActor->field_20_pObj->objs->flag &= ~DG_FLAG_SHADE;
        pActor->field_20_pObj->objs->flag &= ~DG_FLAG_BOUND;
        pActor->field_20_pObj->objs->flag |= DG_FLAG_GBOUND;
        EQ_InvisibleUnit_80060E68(pActor->field_20_pObj->objs, pActor->field_40_rgb, 0);
        pActor->field_0_actor.mFnUpdate = (TActorFunction)kogaku2_act_800613FC;
        pActor->field_0_actor.mFnShutdown = (TActorFunction)kogaku2_kill_80061508;
    }
    else
    {
        pActor->field_2C_ypos2 = pActor->field_30_ypos1;
    }
}

void kogaku2_act_nullsub_800615F4(Actor_kogaku2 *pActor)
{
}


extern const char aKogaku2C[];

Actor_kogaku2 *NewKogaku2_800615FC(GM_Control *pCtrl, OBJECT *pObj, int unit)
{
  Actor_kogaku2 *pActor;
  DG_OBJS *objs;
  pActor = (Actor_kogaku2 *) GV_NewActor_800150E4(1, sizeof(Actor_kogaku2));
  if (pActor)
  {
    GV_SetNamedActor_8001514C(&pActor->field_0_actor, (TActorFunction) kogaku2_act_800613FC, (TActorFunction) kogaku2_kill_80061508, aKogaku2C);

    pActor->field_20_pObj = pObj;
    pActor->field_24_unit = unit;
    pActor->field_2C_ypos2 = 0;
    pActor->field_30_ypos1 = 1;
    objs = pActor->field_20_pObj->objs;
      
    pActor->field_28_obj_old_flag = objs->flag;
    DG_UnShadeObjs(objs);
    DG_UnBoundObjs(objs);
    DG_GBoundObjs(objs);
      
    if (pCtrl->field_30_scriptData == CHARA_SNAKE)
    {
      pActor->field_3C_msg_is_8650 = 1;
      pActor->field_40_rgb = 0x3C60A080;
      EQ_InvisibleUnit_80060E68(objs, pActor->field_40_rgb , 0);
    }
    else
    {
      pActor->field_40_rgb = 0x3C808080;
      EQ_InvisibleUnit_80060E68(objs, pActor->field_40_rgb , 0);
    }
  }
  return pActor;
}


#pragma INCLUDE_ASM("asm/Equip/NewKogaku3_80061708.s") // 596 bytes
