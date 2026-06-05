#include "tonc.h"
#include "inventory.h"

TItem Inventory[]={
    {ITEM_CONFETTI,0x1,0},
    {ITEM_HAMMER,0x01,0},
    {ITEM_KEY,1,0},
    {ITEM_SHIELD,1,0},
    {ITEM_SWORD,1,0},
    {ITEM_HEALTH,1,1},
};



void add_item(u32 itemID){
    TItem *entry;
    switch (itemID)
        {
        case ITEM_HEALTH:
            full_heal();
        case ITEM_CONFETTI:
        case ITEM_HAMMER:
        case ITEM_KEY:
        case ITEM_SHIELD:
        case ITEM_SWORD:
        /* code */
            itemID&=0xff;
            entry = &Inventory[itemID-1];
            entry->count++;
        break;
        case ITEM_ARCHIPELAGO:
        case ITEM_NOTHING:
        
        default:
            break;
        }
}
u32 has_item(u32 itemID){
    switch (itemID)
        {
        case ITEM_HEALTH:
        case ITEM_CONFETTI:
        case ITEM_HAMMER:
        case ITEM_KEY:
        case ITEM_SHIELD:
        case ITEM_SWORD:
            /* code */
            itemID&=0xff;
            TItem *entry = &Inventory[itemID-2];
            return entry->count;
            break;
        case ITEM_ARCHIPELAGO:
        case ITEM_NOTHING:
        
        default:
            break;
        }
        return 0;
}
bool use_item(u32 itemID){
    TItem *entry;
    switch (itemID){
        case ITEM_HAMMER:
        case ITEM_SHIELD:
        case ITEM_SWORD:
        entry = &Inventory[itemID-2];
        return entry->count; //Use count is Irrelevant to these items, but you can "use" them, IF you have at least 1
        break;
        case ITEM_KEY:
        case ITEM_CONFETTI:
        entry = &Inventory[itemID-2];
        if(entry->count>entry->used){
            entry->used++;
            return true;
        }
        break;
        case ITEM_HEALTH:// WHY would you "USE" Health????? You cannot Use Health, blocked
        case ITEM_ARCHIPELAGO:
        case ITEM_NOTHING:
        
        default:
            break;
    }
    return false;
}

bool is_dead(){
    TItem *HealthEntry = &Inventory[(ITEM_HEALTH&0xff)-2]; //the Health Entry
    return HealthEntry->count*2<=HealthEntry->used;//Each healthUpgrade absorbs like 2 Points of damage
}
void full_heal(){
    TItem *HealthEntry = &Inventory[(ITEM_HEALTH&0xff)-2]; //the Health Entry
    HealthEntry->used=0;
}
void deal_damage(){
    TItem *HealthEntry = &Inventory[(ITEM_HEALTH&0xff)-2]; //the Health Entry
    HealthEntry->used++;
    if (!has_item(ITEM_SHIELD))//The Shield Absorbs 1 Damage
    {
        HealthEntry->used++;
    }
    
}



void load_inv_from_SRAM(){
    if (sram_mem[0x20] && sram_mem[0x20]!=0xff){
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
    sram_mem[0x20]=0xBB;
    for(u32 i=0; i<InventoryLen;i++){
        sram_mem[i*4]=Inventory[i].state>>8;
        sram_mem[i*4+1]=Inventory[i].state & 0xff;
        sram_mem[i*4+2]=Inventory[i].count;
        sram_mem[i*4+3]=Inventory[i].used;
    }
    
}