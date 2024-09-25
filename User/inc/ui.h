#ifndef __UI_H
#define __UI_H	


#include "bsp.h"
#include <stdbool.h>



extern void UI_Level_0(void);
extern void UI_Level_1(void);
extern void UI_Level_2(void);
extern void UI_Level_3(void);
extern void UI_Level_4(void);
extern void UI_Level_5(void);
extern void UI_Level_6(void);
extern void Menu_Title(void);
extern void Menu_Infobar(void);
extern void Menu_Refresh(void);
extern void Menu_1(void);
extern void Menu_4(void);
extern void UI_Init(void);
extern void MenuTrig_Up(void);
extern void MenuTrig_Down(void);
extern void MenuTrig_OK(void);
extern void MenuTrig_Cancel(void);
extern bool Menu_2_x_OK(unsigned char CodeNumber);
extern void Menu_0_1(bool Show_Mode);
extern void Menu_0_2(bool Show_Mode);
extern void Menu_0_3(bool Show_Mode);
extern void Menu_0_4(bool Show_Mode);
extern void Menu_0_5(bool Show_Mode);
extern void Menu_0_6(bool Show_Mode);
extern void Menu_2_x(bool Show_Mode, unsigned char line_number);
#endif
