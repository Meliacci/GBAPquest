//
// brin_demo.c
// Essential tilemap loading: the start of brinstar from metroid 1
//
// (20060221 - 20070216, cearn)

#include <string.h>

#include <tonc.h>
#include "BG.h"
#include "inanimates.h"
#include "items.h"
#include "human.h"
#include "boss.h"
#include "normal_enemy.h"
#include "hearts.h"

#include "metatile.h"
#include "player.h"
#include "interactions.h"
#include "inventory.h"

#define MAX_X_SCROLL 257 //set the  Size in Pixels of the Map
#define MAX_Y_SCROLL 257


typedef struct VIEWPORT
{
	int x, xmin, xmax, xpage; //Xpage is the how many pixels from the Right you  are allowed to scroll before blocking
	int y, ymin, ymax, ypage; //Ypages is how many Pixels From the Bottom you  are allowed to be
} VIEWPORT;

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

VIEWPORT g_vp=
{
	0, 0, MAX_X_SCROLL, 240,
	0, 0, MAX_Y_SCROLL, 160,
};

TMapInfo g_bg;
TMapInfo g_walls;
TMapInfo g_ui;

OBJ_ATTR obj_buffer[128];
TSprite g_link;
bool HammerMode=true;
bool WallMode=true;
bool ChestClosed=true;

/*
				Buttons
				MetaTileLoad(3,3,0x08, g_walls.dstMap, inanimatesMetaTiles);
				Walls
				MetaTileLoad(3,9,0x05, g_walls.dstMap, inanimatesMetaTiles);
				MetaTileLoad(3,11,0x05, g_walls.dstMap, inanimatesMetaTiles);
				MetaTileLoad(7,3,0x05, g_walls.dstMap, inanimatesMetaTiles);
				MetaTileLoad(10,11,0x05, g_walls.dstMap, inanimatesMetaTiles);
				MetaTileLoad(11,11,0x05, g_walls.dstMap, inanimatesMetaTiles);
				Enemy
				13,1,0x05, g_walls.dstMap, bossMetaTiles);
				MetaTileLoad(11,7
				Walls
				MetaTileLoad(3,7,0x07, g_walls.dstMap, inanimatesMetaTiles);
				MetaTileLoad(8,11,0x06, g_walls.dstMap, inanimatesMetaTiles);
				MetaTileLoad(11,5,0x0A, g_walls.dstMap, inanimatesMetaTiles);
*/

TInteract Initializers[]={
	//Buttons
	{EIT_BUTTON, 3,3, 0x08, XY_MIXER(11,5), inanimatesMetaTiles},
	//Chests
	{EIT_CHEST, 3,5, 0x05, 0x0102, inanimatesMetaTiles},
	{EIT_CHEST, 3,9, 0x05, 0x02, inanimatesMetaTiles},
	{EIT_CHEST, 3,11, 0x05, 0x06, inanimatesMetaTiles},
	{EIT_CHEST, 7,3, 0x05, 0x0102, inanimatesMetaTiles},
	{EIT_CHEST, 10,11, 0x05, 0x05, inanimatesMetaTiles},
	{EIT_CHEST, 11,11, 0x05, 0x03, inanimatesMetaTiles},
	//Enemies
	{EIT_ENEMY, 11,3, 0x03, 0x00, bossMetaTiles},
	{EIT_ENEMY, 11,7, 0x01, 0x04, normal_enemyMetaTiles},
	//Interaction  Walls
	{EIT_WALL, 3,7, 0x07, 0x00, inanimatesMetaTiles},
	{EIT_WALL, 8,11, 0x06, 0x00, inanimatesMetaTiles},
	//Gate
	{EIT_NONE, 11,5, 0x0A, 0x00, inanimatesMetaTiles},
	//if Hammer Mode
	{EIT_WALL, 7,5, 0x04, 0x00, inanimatesMetaTiles},
	{EIT_NONE, 6,5, 0x02, 0x00, inanimatesMetaTiles},
	{EIT_NONE, 8,5, 0x02, 0x00, inanimatesMetaTiles},
};

u16 InitializersLen = 15;

TInteract* g_CoordLUT[16][16];
// === PROTOTYPES =====================================================

INLINE void vp_center(VIEWPORT *vp, int x, int y);
void vp_set_pos(VIEWPORT *vp, int x, int y);

// === MACROS =========================================================
// === INLINES=========================================================
// === FUNCTIONS ======================================================


// --- VIEWPORT ---

INLINE void vp_center(VIEWPORT *vp, int x, int y)
{	vp_set_pos(vp, x - vp->xpage/2, y - vp->ypage/2);	}


void vp_set_pos(VIEWPORT *vp, int x, int y)
{
	vp->x= clamp(x, vp->xmin, vp->xmax - vp->xpage);
	vp->y= clamp(y, vp->ymin, vp->ymax - vp->ypage);
}

// --- BACKGROUND???? ---

void ui_meta_init(TMapInfo *bgt, int bgnr, u32 ctrl,
	u16 tileSize ,u32 map_width, u32 map_height)
{
	memset(bgt, 0, sizeof(TMapInfo));

	bgt->flags= bgnr;
	bgt->cnt= ctrl;
	bgt->dstMap= se_mem[BFN_GET(ctrl, BG_SBB)];

	REG_BGCNT[bgnr]= ctrl;
	REG_BG_OFS[bgnr].x= 0;
	REG_BG_OFS[bgnr].y= 0;

	bgt->srcMapWidth= map_width;
	bgt->srcMapHeight= map_height;
	//SCR_ENTRY *dst= bgt->dstMap;
	
	
}

void wallt_meta_init(TMapInfo *bgt, int bgnr, u32 ctrl,
	u16 tileSize ,u32 map_width, u32 map_height)
{
	memset(bgt, 0, sizeof(TMapInfo));

	bgt->flags= bgnr;
	bgt->cnt= ctrl;
	bgt->dstMap= se_mem[BFN_GET(ctrl, BG_SBB)];

	REG_BGCNT[bgnr]= ctrl;
	REG_BG_OFS[bgnr].x= 0;
	REG_BG_OFS[bgnr].y= 0;

	bgt->srcMapWidth= map_width;
	bgt->srcMapHeight= map_height;
	SCR_ENTRY *dst= bgt->dstMap;
	if(!HammerMode){
		InitializersLen-=3;
	}
	for (u16 i = 0; i < InitializersLen; i++)
	{
		TInteract* tempInit=&Initializers[i];
		MetaTileLoad(tempInit->x,tempInit->y,tempInit->state,dst,tempInit->MetaTiles);
		tempInit->dst=dst;
		g_CoordLUT[tempInit->x][tempInit->y]=tempInit;
	}
}

void bgt_meta_init(TMapInfo *bgt, int bgnr, u32 ctrl,
	const void *metamap, u16 tileSize ,u32 map_width, u32 map_height)
{
	memset(bgt, 0, sizeof(TMapInfo));

	bgt->flags= bgnr;
	bgt->cnt= ctrl;
	bgt->dstMap= se_mem[BFN_GET(ctrl, BG_SBB)];

	REG_BGCNT[bgnr]= ctrl;
	REG_BG_OFS[bgnr].x= 0;
	REG_BG_OFS[bgnr].y= 0;


	bgt->srcMap= (SCR_ENTRY*)metamap;
	bgt->srcMapWidth= map_width;
	bgt->srcMapHeight= map_height;
	SCR_ENTRY *dst= bgt->dstMap, *src= bgt->srcMap;
	
	for (u32 i = 0; i < map_height; i++){
		for (u32 x = 0; x < map_width; x++)
		{
			MetaTileLoad(x,i,src[i*tileSize+x],dst, inanimatesMetaTiles);
		}
	}
}

void bgt_update(TMapInfo *bgt, VIEWPORT *vp)
{
	int vx= vp->x, vy= vp->y;
	// Update TMapInfo and reg-offsets
	int bgnr= bgt->flags;
	REG_BG_OFS[bgnr].x= bgt->mapX= vx;
	REG_BG_OFS[bgnr].y= bgt->mapY= vy;
}

void ui_update(TMapInfo *bgt){
	//load_inv_from_SRAM(); Load only if prompted?
}

int main()
{
	// Init interrupts and VBlank irq.
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
	oam_init(obj_buffer, 128);
	// Load palette
	memcpy16(&pal_bg_mem[0], inanimatesPal, inanimatesPalLen/sizeof(u16));
	memcpy16(&pal_bg_mem[16], itemsPal, inanimatesPalLen/sizeof(u16));
	memcpy16(&pal_bg_mem[32], heartsPal, heartsPalLen/sizeof(u16));
	memcpy16(&pal_bg_mem[48], bossPal, bossPalLen/sizeof(u16));
	memcpy16(&pal_bg_mem[64], normal_enemyPal, normal_enemyPalLen/sizeof(u16));
	// Load tiles into CBB 0
	memcpy32(&tile_mem[0][0], inanimatesTiles, inanimatesTilesLen / sizeof(u32));
	
	memcpy32(&tile_mem[0][32], itemsTiles, itemsTilesLen / sizeof(u32));
	memcpy32(&tile_mem[0][53], heartsTiles, heartsTilesLen / sizeof(u32));
	memcpy32(&tile_mem[0][64], bossTiles, bossTilesLen / sizeof(u32));
	memcpy32(&tile_mem[0][84], normal_enemyTiles, normal_enemyTilesLen / sizeof(u32));
	
	bgt_meta_init(&g_bg, 2, BG_CBB(0)|BG_SBB(30) | BG_4BPP | BG_REG_32x32, BGMetaMap, 16,
		16, 16);
	
	wallt_meta_init(&g_walls, 1, BG_CBB(0)|BG_SBB(26) | BG_4BPP | BG_REG_32x32, 16,
		16, 16);
		
	ui_meta_init(&g_ui,0,BG_CBB(0)|BG_SBB(28)| BG_4BPP | BG_REG_32x32, 16, 16, 16);
	GRIT_CPY(pal_obj_mem, humanPal);
	GRIT_CPY(tile_mem[4], humanTiles);

	player_init(&g_link, int2fx(96), int2fx(176), 0);

	REG_DISPCNT= DCNT_MODE0 | DCNT_BG1 | DCNT_BG2 | DCNT_OBJ |
		DCNT_OBJ_1D;

	// Scroll around some
	int x= 0, y=0;
	while(1)
	{
		VBlankIntrWait();
		key_poll();
		
		
		// Player Logic & stuff
		player_input(&g_link);
		player_turn(&g_link);
		player_move(&g_link);
		if (key_hit(KEY_SELECT))
		{
			save_inv_to_SRAM();
		}
		if (key_hit(KEY_START))
		{
			load_inv_from_SRAM();
		}
		
		//Screen View Stuff
		x= fx2int(g_link.x), y= fx2int(g_link.y);

		vp_center(&g_vp, x, y);
		oam_copy(oam_mem, obj_buffer, 128);
		
		bgt_update(&g_bg, &g_vp);
		bgt_update(&g_walls, &g_vp);
	}

	return 0;
}