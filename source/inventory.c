#include "tonc.h"
#include "inventory.h"

TItem Inventory[]={
    {ITEM_CONFETTI,0x00,0},
    {ITEM_HAMMER,0x00,0},
    {ITEM_KEY,0,0},
    {ITEM_SHIELD,0,0},
    {ITEM_SWORD,0,0},
    {ITEM_HEALTH,1,0},
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
            entry = &Inventory[itemID-2];
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
