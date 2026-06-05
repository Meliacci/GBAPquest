#include <tonc.h>
#ifndef __INVENTORY__
#define __INVENTORY__

#define ITEM_NOTHING 0x00
#define ITEM_ARCHIPELAGO 0x01
#define ITEM_CONFETTI 0x02
#define ITEM_HAMMER 0x03
#define ITEM_KEY 0x04
#define ITEM_SHIELD 0x05
#define ITEM_SWORD 0x06
#define ITEM_HEALTH 0x0107

typedef struct TItem
{

    /*
    * in the Form 0x00AA
    * If Health; 
    * 0x0107
    * 
    * Else;
    * 
    * 0x00 nothing (should remain unused)
    * 0x01 Archipelago Item (Should Remain unused)
    * 0x02 Confetti Cannon
    * 0x03 Hammer
    * 0x04 Key
    * 0x05 Shield
    * 0x06 Sword
    *
    * */
	u16			state;		
	u8			count;		//!< Inventory Count of item (Confetti and Health Upgrades)
	u8			used;		//!< Inventory Count of How many times it has been used (Confetti and Damage Receive)

}ALIGN(4) TItem;

extern TItem Inventory[6];
#define InventoryLen 6

#endif // __INVENTORY__


void add_item(u32 itemID);
u32 has_item(u32 itemID);
bool use_item(u32 itemID);

bool is_dead();
void full_heal();
void deal_damage();


void load_inv_from_SRAM();
void save_inv_to_SRAM();