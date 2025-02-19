#ifndef _RADAR_H_
#define _RADAR_H_

typedef struct radar_sprt_params_8009E30C
{
    char u0;
    char v0;
    char w;
    char h;

    int  unknown;
} radar_sprt_params_8009E30C;

void menu_radar_update_8003B350(struct Actor_MenuMan* pActor, unsigned char * pOt);
void menu_init_radar_helper_8003ADAC(void);
void menu_radar_helper_8003ADD8(struct Actor_MenuMan *a1, int a2);
void menu_SetRadarFunc_80038F30(int param_1);

void draw_radar_8003AEC0(Actor_MenuMan *pActor, unsigned char * pOt);

#endif // _RADAR_H_
