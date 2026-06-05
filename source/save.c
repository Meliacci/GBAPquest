#include <tonc.h>
#include "inventory.h"
#include "save.h"

extern bool g_CoordChecked[16][16];

void load_inv_from_SRAM(){
    if (sram_mem[SAVE_FLAG_OFFS] && sram_mem[SAVE_FLAG_OFFS]!=0xff){
        for(u32 i=0; i<InventoryLen;i++){
            Inventory[i].state=sram_mem[i*4]<<8;
            Inventory[i].state|=sram_mem[i*4+1] & 0xFF;
            Inventory[i].count=sram_mem[i*4+2];
            Inventory[i].used=sram_mem[i*4+3];
        }
        sram_mem[0x20]=0x11;
    }
}

void save_inv_to_SRAM(){
    sram_mem[SAVE_FLAG_OFFS]=0xBB;
    for(u32 i=0; i<InventoryLen;i++){
        sram_mem[i*4]=Inventory[i].state>>8;
        sram_mem[i*4+1]=Inventory[i].state & 0xff;
        sram_mem[i*4+2]=Inventory[i].count;
        sram_mem[i*4+3]=Inventory[i].used;
    }
    
}

void delete_inv_SRAM(){
    sram_mem[SAVE_FLAG_OFFS]=0xFF;//  we don't actually delete Anything :p
}


void load_checks_from_SRAM(){
    if (sram_mem[SAVE_FLAG_OFFS] && sram_mem[SAVE_FLAG_OFFS]!=0xff){
        for (u32 i = 0; i < 16; i++){
            for (u32 x = 0; x < 16; x++)
            {
                g_CoordChecked[i][x]=sram_mem[SAVE_CHECKS_OFFS+0x10*x+i];
            }
        }
        sram_mem[SAVE_FLAG_OFFS]=0x11;
    }
}
void save_checks_to_SRAM(){
    for (u32 i = 0; i < 16; i++){
        for (u32 x = 0; x < 16; x++)
        {
            sram_mem[SAVE_CHECKS_OFFS+0x10*x+i]=g_CoordChecked[i][x];
        }
    }
    sram_mem[SAVE_FLAG_OFFS]=0xBB;
}
void delete_checks_SRAM(){
    sram_mem[SAVE_FLAG_OFFS]=0xFF;
    for (u32 i = 0; i < 16; i++){
        for (u32 x = 0; x < 16; x++)
        {
            sram_mem[SAVE_CHECKS_OFFS+0x10*x+i]=false;
        }
    }
}