#include <tonc.h>
#include "confettiCannon.h"
#include "sound.h"
#include "confetti.h"
#include "player.h"
extern OBJ_ATTR obj_buffer[];

u32 ShootingConfetti=0;
OBJ_ATTR Confettis[]={
    {ATTR0_HIDE,ATTR1_SIZE_16,ATTR2_ID(0x10) |ATTR2_PALBANK(1),0},
    {ATTR0_HIDE,ATTR1_SIZE_16,ATTR2_ID(0x14) |ATTR2_PALBANK(1),0},
    {ATTR0_HIDE,ATTR1_SIZE_16,ATTR2_ID(0x18) |ATTR2_PALBANK(1),0},
    {ATTR0_HIDE,ATTR1_SIZE_16,ATTR2_ID(0x1C) |ATTR2_PALBANK(1),0},
};

u32 Next=0xBA5EBA11;

u32 PRNG(){

    //LCG: X * a + c: Where 
    Next = Next * 1103515243 + 12345;
    return (u32)(Next >> 16) & 0x7fff;
}


void ShootConfetti(){
    if(ShootingConfetti!=0){
        if(ShootingConfetti==CONFETTI_TIMER_MAX){
            PlayEffect(EMME_BOOM);
            for (u32 i = 0; i < 4; i++)
            {
                obj_buffer[1+i].attr0=(obj_buffer[0].attr0&ATTR0_Y_MASK);
                obj_buffer[1+i].attr1=(obj_buffer[1+i].attr1&~ATTR1_X_MASK)|(obj_buffer[0].attr1&ATTR1_X_MASK);
            }        
        }else{
            for (u32 i = 0; i < 4; i++)
            {
                obj_buffer[1+i].attr0=((((obj_buffer[1+i].attr0>>ATTR0_Y_SHIFT)+((i<=1?CONFETTI_SPEED:-CONFETTI_SPEED)))<<ATTR0_Y_SHIFT))&ATTR0_Y_MASK;
                obj_buffer[1+i].attr1=(obj_buffer[1+i].attr1&~ATTR1_X_MASK)|(((obj_buffer[1+i].attr1>>ATTR1_X_SHIFT)+(i%2?CONFETTI_SPEED:-CONFETTI_SPEED))&ATTR1_X_MASK);
            }        
        }
        ShootingConfetti--;

    }
}

void confetti_init(){


    GRIT_CPY(pal_obj_mem+16, confettiPal);
	GRIT_CPY(tile_mem[4]+16, confettiTiles);
    oam_copy(&obj_buffer[1], &Confettis[0], 1);
    oam_copy(&obj_buffer[2], &Confettis[1], 1);
    oam_copy(&obj_buffer[3], &Confettis[2], 1);
    oam_copy(&obj_buffer[4], &Confettis[3], 1);
}