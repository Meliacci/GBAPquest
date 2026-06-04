#include <tonc.h>
#ifndef __TILEINFO__
#define __TILEINFO__


// === MACRO ======================================================

#define XY_MIXER(x,y) ((((x<<2)&0xFF00))|((y)&0x00FF))
#define X_EXTRACT(xy) (((xy>>2)&0x00FF))
#define Y_EXTRACT(xy) ((xy&0x00FF))

// === CONSTANTS ======================================================

enum EInteractType
{
	EIT_NONE= 0, EIT_CHEST, EIT_BUTTON, EIT_ENEMY, EIT_WALL, EIT_ITEM
};

// === CLASSES ========================================================
// interact{
// type(enemy|chest|wall), coordx, coordy, InitMetaTileIndex, target , MetaTiles*
// }
/*
* 
*/
typedef struct TInteract
{
	enum EInteractType   type;		//!< Interaction Type
	u8	        		 x, y;		//!< Position in Meta Tiles
	u16			        state;		//!< Initial tile
    /*
    * Target Behaviour Dependent on interaction Type:
    * EIT_NONE
    * 
    * Does nothing when Interacted with
    * 
    * EIT_CHEST
    * 
    * Places Target Index from ItemMetaTiles into Coords
    * 
    * EIT_BUTTON
    * 
    * Used by Exactly 1 Tile, If Interacts,Deletes Wall at Target Coord Where 0xFF00 is X and 0x00FF is y
    * 
    * EIT_ENEMY
    * 
    * Health indicated by State
    * Once Health Reaches 0, Replace with Target Item at Coords
    * 
    * EIT_WALL
    * 
    * State is interactable Wall Type (Breakable)
    * Checks that Player has Target Item in Inventory
    * If player has, Delete Wall at Coord
    *  
    * EIT_ITEM
    * 
    * Deletes tile Entry and Places Target item in Player Inventory
    * 
    */
    u16                target;
    /*
    * Pointer to Meta Tile Mappings (where Given index tile*4+[0-3] returns the S-tile to be used at each of the 4 Tiles that make up a Meta Tile)
    */
	const u16            *MetaTiles;		//!< Tile Reference
} TInteract;

#endif // __TILEINFO__

void InteractWith(TInteract* Interactable);