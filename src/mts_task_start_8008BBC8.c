#include "linker.h"
#include "mts_new.h"

extern void mg_printf_8008BBA0(const char*, ...);

void sub_8009953C(void);
void sub_8008B51C(void);
void sub_8008B77C(void);

extern const char aTaskStartDX[]; // = "TASK START: %d %X\n";
extern const char aAssertionFaled[]; // = "assertion faled : %s line %d : Task %d\n";
extern const char aMtsNewC[]; // = "mts_new.c";
extern const char aTaskStartBody[]; // = "task_start_body";
extern const char asc_80013E2C[]; // = "\n";

typedef void(*TMtsFn)(void);

typedef struct mts_unknown1
{
    TMtsFn field_0_fn;
    int field_4;
    int field_8;
    int field_C;
    int unknown[4]; // TODO: Really not sure how this structure is supposed to look yet
} mts_unknown1;

int SECTION(".gTaskIdx_800C0DB0") gTaskIdx_800C0DB0 = 0;
mts_unknown1 SECTION(".gTasks_800C0C38") gTasks_800C0C38[] = {};

void mts_task_start_8008BBC8(void)
{
    sub_8009953C();
    mg_printf_8008BBA0(aTaskStartDX, gTaskIdx_800C0DB0, gTasks_800C0C38[gTaskIdx_800C0DB0].field_0_fn);
    gTasks_800C0C38[gTaskIdx_800C0DB0].field_0_fn();
    sub_8008B51C();
    mg_printf_8008BBA0(aAssertionFaled, aMtsNewC, 421, gTaskIdx_800C0DB0);
    mg_printf_8008BBA0(aTaskStartBody);
    mg_printf_8008BBA0(asc_80013E2C);
    sub_8008B77C();
}
