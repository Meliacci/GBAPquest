#include <tonc.h>

#include "BG.h"
#include "player.h"

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
extern VIEWPORT g_vp;
extern OBJ_ATTR obj_buffer[];

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
bool MetaTileEquals(u16 MetaX, u16 MetaY, u16 TileID, SCR_ENTRY *se_en){
	MetaX*=2;
	MetaY*=2;
	TileID*=4;
	return 
	(se_en[MetaY*32+MetaX]==(BGMetaTiles[TileID]&0x1F))&&
	(se_en[MetaY*32+MetaX+1]==(BGMetaTiles[TileID+1]&0x1F))&&
	(se_en[(MetaY+1)*32+MetaX]==(BGMetaTiles[TileID+2]&0x1F))&&
	(se_en[(MetaY+1)*32+MetaX+1]==(BGMetaTiles[TileID+3]&0x1F));
}

bool can_move_target(TSprite *link){
	POINT pt= { (fx2int(link->x+link->vx))/16, (fx2int(link->y+link->vy))/16};
	return  !(pt.x<0 ||pt.x>15 || pt.y<0 || pt.y>15)&&( MetaTileEquals(pt.x,pt.y, 0x0001, g_bg.dstMap) || MetaTileEquals(pt.x, pt.y, 0x0003, g_bg.dstMap));
}

void player_init(TSprite *link, u32 x, u32 y, int obj_id)
{
	link->x= x;
	link->y= y;
	link->vx= link->vy= 0;

	link->dir= LOOK_DOWN;
	link->objId= obj_id;

	oam_copy(&obj_buffer[obj_id], &cLinkObjs, 1);
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
}

void player_move(TSprite *link)
{
	// TODO : collision testing here?
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
	obj[0].attr2=dir*4;

	
}

// EOF
