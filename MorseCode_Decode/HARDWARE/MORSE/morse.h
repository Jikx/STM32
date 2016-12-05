#ifndef _MORSE_H_
#define _MORSE_H_

#include "sys.h"
#include "delay.h"
#include "oled.h"

int Morse_Judge(u16 now_counter,u16 *last_counter,u32 *last_val,u32 now_val);		//Äª¶ûË¹ÒëÂë
int Morse_Disp(void);
#endif
