#define _BUILDING_MENUMAN_
#include "menuman.h"
#include "radar.h"
#include "linker.h"
#include <LIBGTE.H>
#include <LIBGPU.H>
#include "libdg/dgd.h"
#include "psyq.h"

extern Actor_MenuMan gMenuMan_800BD360;
extern unsigned char gPrimBackingBuffers_800B9360[2][8192];

extern const char aItem[]; // sdata
extern const char aMenumanC[];
extern const char aMenuPrimOver[];

extern MenuGlue    gMenuPrimBuffer_8009E2D0;
extern TInitKillFn gMenuInitFns_8009E290[];
extern int         GV_Clock_800AB920;
extern DG_CHNL     DG_Chanls_800B1800[3];
extern int         MENU_PrimUse_800AB68C;
extern TInitKillFn gMenuKillFns_8009E2B4[];

extern int GV_PauseLevel_800AB928;
extern int GM_LoadComplete_800ABA38;
extern int GM_GameStatus_800AB3CC;
extern int GM_LoadRequest_800AB3D0;

// force GP
extern GV_PAD *GM_CurrentPadData_800AB91C; // sbss
GV_PAD*        SECTION(".sdata") GM_CurrentPadData_800AB91C;

void menuman_act_800386A4(Actor_MenuMan *pActor)
{
  unsigned char *pOtStart;
  int idx_as_flag;
  int field_28_flags;
  int i;
  pOtStart = (&gMenuPrimBuffer_8009E2D0)->mPrimBuf.mOt;
  pActor->field_24_pInput = &GM_CurrentPadData_800AB91C[2];
  menu_jimaku_act_80048FD4(pActor, (unsigned int *)pOtStart);
  if ((((GV_PauseLevel_800AB928 & 2) == 0) && (GM_LoadComplete_800ABA38 > 0)) && (!GM_LoadRequest_800AB3D0))
  {
    idx_as_flag = 1;
    if (GM_GameStatus_800AB3CC >= 0)
    {
      field_28_flags = pActor->field_28_flags;
      for (i = 0; i < 8; i++)
      {
        if ((field_28_flags & idx_as_flag) != 0)
        {
          pActor->m7FnPtrs_field_2C[i](pActor, pOtStart);
        }
        idx_as_flag *= 2;
      }

    }
  }

  addPrim(pOtStart, &pActor->field_4C_drawEnv[GV_Clock_800AB920]);
}

void menuman_kill_800387E8(Actor_MenuMan *pActor)
{
    TInitKillFn *pIter = gMenuKillFns_8009E2B4;
    while (*pIter)
    {
        (*pIter)(pActor);
        pIter++;
    }

    menu_viewer_kill_80044A90(pActor);
}

void menu_init_subsystems_8003884C(Actor_MenuMan *pMenuMan)
{
    TInitKillFn *pIter;
    DRAWENV      drawEnv;

    pMenuMan->field_2A_state = 0;
    pMenuMan->field_29 = 0;
    pMenuMan->field_28_flags = 0;

    pMenuMan->field_20_otBuf = &gMenuPrimBuffer_8009E2D0;

    gMenuPrimBuffer_8009E2D0.mPrimPtrs[0] = &gPrimBackingBuffers_800B9360[0][0];
    gMenuPrimBuffer_8009E2D0.mPrimPtrs[1] = &gPrimBackingBuffers_800B9360[1][0];

    DG_Init_DrawEnv_80018384(&drawEnv, 0, 0, 320, 224);
    drawEnv.isbg = 0;
    drawEnv.tpage = 31;
    SetDrawEnv_800906B0(&pMenuMan->field_4C_drawEnv[0], &drawEnv);

    DG_Init_DrawEnv_80018384(&drawEnv, 320, 0, 320, 224);
    drawEnv.isbg = 0;
    drawEnv.tpage = 31;
    SetDrawEnv_800906B0(&pMenuMan->field_4C_drawEnv[1], &drawEnv);

    menu_rpk_init_8003DD1C(aItem);

    pIter = &gMenuInitFns_8009E290[0];
    while (*pIter)
    {
        (*pIter)(pMenuMan);
        pIter++;
    }

    menu_viewer_init_80044A70(pMenuMan);
}

void menuman_init_80038954(void)
{
    GV_SetNamedActor_8001514C(&gMenuMan_800BD360.field_0_actor, (TActorFunction)menuman_act_800386A4,
                              (TActorFunction)menuman_kill_800387E8, aMenumanC);
    menu_init_subsystems_8003884C(&gMenuMan_800BD360);
    menu_InitRadioTable_80049644();
}

void menuman_Reset_800389A8()
{
    menu_ResetCall_80042814();
    menu_ClearRadioTable_8004967C();
    menu_SetRadarScale_80038E28(4096);
    menu_SetRadarFunc_80038F30(0);
    gMenuMan_800BD360.field_1D9 = 0;
    gMenuMan_800BD360.field_1DA = 0;
    gMenuMan_800BD360.field_2B = 0;
    gMenuMan_800BD360.field_1DC_menu_left.field_12_flashingAnimationFrame = 0;
    gMenuMan_800BD360.field_1F0_menu_right.field_12_flashingAnimationFrame = 0;
    menu_bars_init_8003F7E0(&gMenuMan_800BD360);
}

void menu_ResetTexture_80038A00(void)
{
    menu_right_unknown_8003DEB0();
}

void menu_StartDeamon_80038A20(void)
{
    GV_InitActor_800150A8(1, &gMenuMan_800BD360.field_0_actor, 0);
    GV_SetNamedActor_8001514C(&gMenuMan_800BD360.field_0_actor, 0, 0, aMenumanC);
}

void menu_radio_update_helper_80038A6C(void)
{
    gMenuMan_800BD360.field_1D8_healthBarDisplayCountdown = 1;
}

void menu_radio_update_helper2_80038A7C(void)
{
    gMenuMan_800BD360.field_1D8_healthBarDisplayCountdown = 0;
}

void menu_ResetSystem_80038A88()
{
    DG_CHNL       *p;
    unsigned char *pFreeLoc = gMenuPrimBuffer_8009E2D0.mPrimBuf.mFreeLocation;
    if (gMenuPrimBuffer_8009E2D0.mPrimBuf.mOtEnd < pFreeLoc)
    {
        mts_nullsub_8_8008BB98(-1, aMenuPrimOver);
    }

    MENU_PrimUse_800AB68C =
        gMenuPrimBuffer_8009E2D0.mPrimBuf.mFreeLocation - gMenuPrimBuffer_8009E2D0.mPrimPtrs[1 - GV_Clock_800AB920];

    gMenuPrimBuffer_8009E2D0.mPrimBuf.mFreeLocation = gMenuPrimBuffer_8009E2D0.mPrimPtrs[GV_Clock_800AB920];
    gMenuPrimBuffer_8009E2D0.mPrimBuf.mOtEnd = gMenuPrimBuffer_8009E2D0.mPrimBuf.mFreeLocation + 0x2000;

    p = &DG_Chanls_800B1800[1];
    gMenuPrimBuffer_8009E2D0.mPrimBuf.mOt = (p + 1)->mOrderingTables[GV_Clock_800AB920];
    menu_Text_Init_80038B98();
}

void menu_Text_XY_Flags_80038B34(int xpos, int ypos, int flags)
{
    TextConfig *pTextConfig = &gMenuTextConfig_8009E2E4;

    pTextConfig->xpos = xpos;
    pTextConfig->ypos = ypos;
    pTextConfig->flags = flags;
}

void menu_Color_80038B4C(int r, int g, int b)
{
    unsigned int newColour;
    unsigned int unknown;
    TextConfig  *pTextConfig = &gMenuTextConfig_8009E2E4;

    if ((pTextConfig->flags & TextConfig_Flags_eSemiTransparent_20) != 0)
    {
        newColour = r | g << 8 | b << 0x10;
        unknown = 0x66000000;
    }
    else
    {
        newColour = r | g << 8 | b << 0x10;
        unknown = 0x64000000;
    }

    pTextConfig->colour = newColour | unknown;
}

void menu_Text_Init_80038B98(void)
{
    TextConfig *pTextConfig = &gMenuTextConfig_8009E2E4;
    pTextConfig->colour = 0x64808080;
    pTextConfig->flags = 0;
}

void menu_Text_PrimUnknown_80038BB4()
{
    DR_TPAGE *pPrim; // $a0

    pPrim = (DR_TPAGE*)gMenuPrimBuffer_8009E2D0.mPrimBuf.mFreeLocation;
    gMenuPrimBuffer_8009E2D0.mPrimBuf.mFreeLocation += sizeof(DR_TPAGE);
    setDrawTPage(pPrim, 1, 1, getTPage(0, gMenuTextConfig_8009E2E4.flags >> 8, 960, 256));

    addPrim(gMenuPrimBuffer_8009E2D0.mPrimBuf.mOt, pPrim);
}

int menu_Text_80038C38(const char *fmt, const char *str, int param_3, int param_4, int param_5)
{
    int          string_length;
    unsigned int free_space;
    char         string_buffer[64];

    if (gMenuPrimBuffer_8009E2D0.mPrimBuf.mFreeLocation)
    {
        sprintf_8008E878(string_buffer, fmt, str, param_3, param_4, param_5);
        free_space = gMenuPrimBuffer_8009E2D0.mPrimBuf.mOtEnd - gMenuPrimBuffer_8009E2D0.mPrimBuf.mFreeLocation;
        string_length = strlen_8008E7B8(string_buffer);
        if (string_length * 0x14 + 0x28U <= free_space)
        {
            if (gMenuTextConfig_8009E2E4.flags & 0x10U)
            {
                menu_number_draw_string2_80043220(&gMenuPrimBuffer_8009E2D0, &gMenuTextConfig_8009E2E4,
                                                  string_buffer);
            }
            else
            {
                menu_number_draw_string_80042BF4(&gMenuPrimBuffer_8009E2D0, &gMenuTextConfig_8009E2E4,
                                                 string_buffer);
            }
            menu_Text_PrimUnknown_80038BB4();
        }
    }
    return gMenuTextConfig_8009E2E4.xpos;
}

int menu_draw_num_80038D10(int number)
{
    if (!gMenuPrimBuffer_8009E2D0.mPrimBuf.mFreeLocation)
    {

        return gMenuTextConfig_8009E2E4.xpos;
    }
    menu_number_draw_80042988(&gMenuPrimBuffer_8009E2D0, &gMenuTextConfig_8009E2E4, number);
    menu_Text_PrimUnknown_80038BB4();
    return gMenuTextConfig_8009E2E4.xpos;
}

MenuGlue *menu_GetPrimInfo_80038D68(void)
{
    return &gMenuPrimBuffer_8009E2D0;
}

void menu_DrawBar_80038D74(int xpos, int ypos, int a3, int a4, BarConfig *pConfig)
{
    GM_GameStatus_800AB3CC |= 0x8000u;
    menu_8003F464(&gMenuPrimBuffer_8009E2D0, xpos, ypos, a3, a4, 1024, pConfig);
    menu_Text_PrimUnknown_80038BB4();
}

void menu_DrawBar2_80038DE0(int ypos, int a3, int a4, int a5, BarConfig *pConfig)
{
    menu_8003F408(&gMenuPrimBuffer_8009E2D0, ypos, a3, a4, a5, pConfig);
}
