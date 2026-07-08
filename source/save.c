#include <tonc.h>
#include "inventory.h"
#include "save.h"

extern bool g_CoordChecked[16][16];
extern bool HammerMode;
extern bool WallMode;
extern bool ExtraChest;
extern bool HardMode;

void load_inv_from_SRAM(){
    if (sram_mem[SAVE_FLAG_OFFS] && sram_mem[SAVE_FLAG_OFFS]!=0xff){
        for(u32 i=0; i<InventoryLen;i++){
            Inventory[i].state=sram_mem[i*4]<<8;
            Inventory[i].state|=sram_mem[i*4+1] & 0xFF;
            Inventory[i].count=sram_mem[i*4+2];
            Inventory[i].used=sram_mem[i*4+3];
        }
    }
}

void save_inv_to_SRAM(){
    for(u32 i=0; i<InventoryLen;i++){
        sram_mem[i*4]=Inventory[i].state>>8;
        sram_mem[i*4+1]=Inventory[i].state & 0xff;
        sram_mem[i*4+2]=Inventory[i].count;
        sram_mem[i*4+3]=Inventory[i].used;
    }
    
}

void delete_inv_SRAM(){    
    for(u32 i=0; i<InventoryLen;i++){
        sram_mem[i*4]=0xFF;
        sram_mem[i*4+1]=0xFF;
        sram_mem[i*4+2]=0xFF;
        sram_mem[i*4+3]=0xFF;
    }
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
    volatile u8 Fill=0xff;//Volatile to make The Compiler be a Dummy and Optimize this to a Int Access loop (i hate having to do this but it's the only way i could think of right now)
    for (u32 i = 0; i < 16; i++){
        for (u32 x = 0; x < 16; x++)
        {
            sram_mem[SAVE_CHECKS_OFFS+0x10*x+i]=Fill;
        }
    }
}
void load_Flags(){
    if(sram_mem[SAVE_FLAG_OFFS] && sram_mem[SAVE_FLAG_OFFS]!=0xff){
        sram_mem[SAVE_FLAG_OFFS]=0x11;
    }
}

void save_Flags(){
    sram_mem[SAVE_FLAG_OFFS]=0xBB;
}

void delete_Flags(){
    sram_mem[SAVE_FLAG_OFFS]=0xFF;
}


void loadSave(){
    load_inv_from_SRAM();
    load_checks_from_SRAM();
    load_Flags();
}
void save(){
    save_inv_to_SRAM();
    save_checks_to_SRAM();
    save_Flags();
}
void deleteSave(){
    delete_inv_SRAM();
    delete_checks_SRAM();
    delete_Flags();
}