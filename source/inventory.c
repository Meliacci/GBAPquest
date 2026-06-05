#include "tonc.h"
#include "inventory.h"

TItem Inventory[]={
    {ITEM_HEALTH(1),1},
    {ITEM_CONFETTI,0xDC,0},
    {ITEM_HAMMER,0xBA},
    {ITEM_KEY,0},
    {ITEM_SHIELD,0},
    {ITEM_SWORD,0},
};

void load_inv_from_SRAM(){
    if (sram_mem[20]==0xBB){
        for(u32 i=0; i<InventoryLen;i++){
            Inventory[i].state=sram_mem[i*4]<<8;
            Inventory[i].state|=sram_mem[i*4+1] & 0xFF;
            Inventory[i].count=sram_mem[i*4+2];
            Inventory[i].used=sram_mem[i*4+3];
        }
    }
}

void save_inv_to_SRAM(){
    sram_mem[0x20]=0xBB;
    for(u32 i=0; i<InventoryLen;i++){
        sram_mem[i*4]=Inventory[i].state>>8;
        sram_mem[i*4+1]=Inventory[i].state & 0xff;
        sram_mem[i*4+2]=Inventory[i].count;
        sram_mem[i*4+3]=Inventory[i].used;
    }
    
}