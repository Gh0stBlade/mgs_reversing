#include "linker.h"
#include "psyq.h"
#include "Game/game.h"
#include "Game/linkvarbuf.h"

extern GCL_Vars gGcl_vars_800B3CC8;
extern GCL_Vars gGcl_memVars_800b4588;

extern char  gStageName_800B4D88[16];
extern short gGcl_gameStateVars_800B44C8[0x60];

void GCL_SaveVar_80021314(void)
{
    memcpy(gGcl_gameStateVars_800B44C8, linkvarbuf, 0xC0);
    gGcl_memVars_800b4588 = gGcl_vars_800B3CC8;
    strcpy_8008E768(gStageName_800B4D88, GM_GetArea_8002A880(0));
}
