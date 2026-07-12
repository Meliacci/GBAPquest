#include <tonc.h>

#include "BG.h"
#include "player.h"
#include "inanimates.h"
#include "metatile.h"
#include "interactions.h"
#include "inventory.h"

#include "sound.h"

#define LINK_SPEED	0x1000

// --------------------------------------------------------------------
// EXTERNAL
// --------------------------------------------------------------------
typedef struct TMapInfo
{
	union
	{
		u32 state;			//!< Background state
		struct
		{
			u16 flags;
			u16 cnt;
		};
	};
	// Destination data
	SCR_ENTRY *dstMap;		//!< Screenblock pointer
	// Source data
	SCR_ENTRY *srcMap;		//!< Source map address
	u32 srcMapWidth;		//!< Source map width
	u32 srcMapHeight;		//!< Source map height
	FIXED mapX;			//!< X-coord on map (.8f)
	FIXED mapY;			//!< Y-coord on map (.8f)
} TMapInfo;

typedef struct VIEWPORT
{
	int x, xmin, xmax, xpage;
	int y, ymin, ymax, ypage;
} VIEWPORT;

extern TMapInfo g_bg;
extern TMapInfo g_walls;
extern VIEWPORT g_vp;
extern OBJ_ATTR obj_buffer[];
extern TInteract* g_CoordLUT[16][16];
extern bool DiedThisFrame;
// --------------------------------------------------------------------
// DECLARATIONS
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// LUTS
// --------------------------------------------------------------------

const u8 cLookDirs[4]= { 0, 1, 3, 2 };
// Default link attributes
const OBJ_ATTR cLinkObjs=
{	
	0, 
	ATTR1_SIZE_16,
	4,
	0 //Don't Touch  Field 3		
};


// --------------------------------------------------------------------
// FUNCTIONS
// --------------------------------------------------------------------

bool can_move_target(TSprite *link){
	POINT pt= { (fx2int(link->x+link->vx))/16, (fx2int(link->y+link->vy))/16};
	bool inBounds=!(pt.x<0 ||pt.x>15 || pt.y<0 || pt.y>15);
	bool WallNotThere=false;
	bool GroundIsSteppable=false;
	if(inBounds){
		GroundIsSteppable=( MetaTileEquals(pt.x,pt.y, 0x0001, g_bg.dstMap, inanimatesMetaTiles) || MetaTileEquals(pt.x, pt.y, 0x0003, g_bg.dstMap, inanimatesMetaTiles));
		WallNotThere=MetaTileEquals(pt.x,pt.y, 0x0000, g_walls.dstMap, inanimatesMetaTiles);

	}
	return  inBounds&&GroundIsSteppable&&WallNotThere;
}

POINT player_target_tile_coord(TSprite *link){
	int vx=0, vy=0;
	switch (link->dir)
	{
	case LOOK_RIGHT:
		vx= LINK_SPEED;
		break;
	case LOOK_LEFT:
		vx= -LINK_SPEED;
		break;
	case LOOK_DOWN:
		vy= LINK_SPEED;
		break;
	case LOOK_UP:
		vy= -LINK_SPEED;
		break;
	default:
		break;
	}
	POINT pt= { (fx2int(link->x+vx))/16, (fx2int(link->y+vy))/16};

	return pt;
}

void player_init(TSprite *link, u32 x, u32 y, int obj_id)
{
	link->x= x;
	link->y= y;
	link->vx= link->vy= 0;
	link->state = SPR_STATE_STAND;
	link->dir= LOOK_DOWN;
	link->objId= obj_id;

	oam_copy(&obj_buffer[obj_id], &cLinkObjs, 1);
}
void player_tp(TSprite *link, u32 x, u32 y){
	link->x= x;
	link->y= y;
	link->vx= link->vy= 0;
	link->state = SPR_STATE_STAND;
	link->dir= LOOK_DOWN;
}
void player_input(TSprite *link)
{
	link->vx= link->vy= 0;

	if( key_hit(KEY_RIGHT) )
	{
		link->vx= LINK_SPEED;
		link->dir= LOOK_RIGHT;
	}
	else if( key_hit(KEY_LEFT) )
	{
		link->vx= -LINK_SPEED;
		link->dir= LOOK_LEFT;
	}

	if( key_hit(KEY_DOWN) )
	{
		link->vy= LINK_SPEED;
		link->dir= LOOK_DOWN;
	}
	else if( key_hit(KEY_UP) )
	{
		link->vy= -LINK_SPEED;
		link->dir= LOOK_UP;
	}
	if(key_hit(KEY_B)){
		link->state = SPR_STATE_SWING;
	}else{
		link->state = SPR_STATE_STAND;
	}
}

void player_move(TSprite *link)
{
	POINT pt=player_target_tile_coord(link);
	if(link->state==SPR_STATE_SWING){
		InteractWith(g_CoordLUT[pt.x][pt.y]);
		if (is_dead())
		{
			DiedThisFrame=true;
			return; //early return as Move Logic No longer applies
		}
		
	}
	if(can_move_target(link)){
		link->x += link->vx;
		link->y += link->vy;
	}
}

void player_turn(TSprite *link){
	POINT pt= { fx2int(link->x) - g_vp.x, fx2int(link->y) - g_vp.y };
	OBJ_ATTR *obj= &obj_buffer[link->objId];
	int dir= link->dir;

	BFN_SET(obj[0].attr0, pt.y, ATTR0_Y);

	obj[0].attr1 &= ~(ATTR1_HFLIP | ATTR1_X_MASK);
	obj[0].attr1 |= BFN_PREP(pt.x, ATTR1_X);
	obj[0].attr2=dir*4|ATTR2_PRIO(1);

	
}

// EOF
