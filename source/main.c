//
// brin_demo.c
// Essential tilemap loading: the start of brinstar from metroid 1
//
// (20060221 - 20070216, cearn)

#include <string.h>

#include <tonc.h>
#include "BG.h"
#include "inanimates.h"
#include "human.h"

#include "metatile.h"
#include "player.h"

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

OBJ_ATTR obj_buffer[128];
TSprite g_link;


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
	
	for (u32 i = 0; i < map_height; i++){
		for (u32 x = 0; x < map_width; x++)
		{
			MetaTileLoad(x,i,0,dst);
		}
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
			MetaTileLoad(x,i,src[i*tileSize+x],dst);
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

void init_textbox(int bgnr, int left, int top, int right, int bottom)
{
	tte_set_margins(left, top, right, bottom);

	REG_DISPCNT |= DCNT_WIN0;

	REG_WIN0H= left<<8 | right;
	REG_WIN0V= top<<8 | bottom;
	REG_WIN0CNT= WIN_ALL | WIN_BLD;
	REG_WINOUTCNT= WIN_ALL;

	REG_BLDCNT= (BLD_ALL&~BIT(bgnr)) | BLD_BLACK;
	REG_BLDY= 5;
}

int main()
{
	// Init interrupts and VBlank irq.
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
	oam_init(obj_buffer, 128);
	// Load palette
	GRIT_CPY(pal_bg_mem, inanimatesPal);
	// Load tiles into CBB 0
	memcpy32(&tile_mem[0][0], inanimatesTiles, inanimatesTilesLen / sizeof(u32));
	
	bgt_meta_init(&g_bg, 2, BG_CBB(0)|BG_SBB(30) | BG_4BPP | BG_REG_32x32, BGMetaMap, 16,
		16, 16);
	
	wallt_meta_init(&g_walls, 1, BG_CBB(0)|BG_SBB(26) | BG_4BPP | BG_REG_32x32, 16,
		16, 16);
	
	GRIT_CPY(pal_obj_mem, humanPal);
	GRIT_CPY(tile_mem[4], humanTiles);

	player_init(&g_link, int2fx(96), int2fx(176), 0);
	//# NOTE: erasing and rendering text flows over into the VDRAW period.
	//# Using the ASM renderer and placing the text at the bottom limits its effects.
	tte_init_chr4c_b4_default(0, BG_CBB(2)|BG_SBB(28));
	tte_set_drawg(chr4c_drawg_b4cts_fast);
	tte_init_con();

	//init_textbox(0, 8, 8, SCR_W-8, 8+2*12);

	init_textbox(0, 8, SCR_H-(8+2*12), SCR_W-8, SCR_H-8);
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_OBJ |
		DCNT_OBJ_1D | DCNT_WIN0;

	// Scroll around some
	int x= 0, y=0;
	bool HammerMode=true;
	bool WallMode=true;
	bool ChestClosed=true;
	while(1)
	{
		VBlankIntrWait();
		key_poll();
		
		
		// Player Logic & stuff
		player_input(&g_link);
		player_turn(&g_link);
		player_move(&g_link);
		
		//Screen View Stuff
		x= fx2int(g_link.x), y= fx2int(g_link.y);

		vp_center(&g_vp, x, y);
		oam_copy(oam_mem, obj_buffer, 128);
		
		bgt_update(&g_bg, &g_vp);
		bgt_update(&g_walls, &g_vp);

		//Debug Stuff, Needs to be deleted After finishing

		if(key_hit(KEY_START)){
			WallMode=!WallMode;
			if(WallMode){
				MetaTileLoad(3,3,0x08, g_walls.dstMap);
				MetaTileLoad(3,7,0x07, g_walls.dstMap);
				MetaTileLoad(8,11,0x06, g_walls.dstMap);
				MetaTileLoad(11,5,0x0A, g_walls.dstMap);
				if(HammerMode){
					MetaTileLoad(7,5,0x04, g_walls.dstMap);
				}else{
					MetaTileLoad(7,5,0x00, g_walls.dstMap);
				}
			}else{
				MetaTileLoad(3,3,0x09, g_walls.dstMap);
				MetaTileLoad(3,7,0x00, g_walls.dstMap);
				MetaTileLoad(8,11,0x00, g_walls.dstMap);
				MetaTileLoad(11,5,0x00, g_walls.dstMap);
				MetaTileLoad(7,5,0x00, g_walls.dstMap);
			}
		}
		if(key_hit(KEY_SELECT)){
			HammerMode=!HammerMode;
			if(!HammerMode){
				MetaTileLoad(6,5,0x01, g_bg.dstMap);
				MetaTileLoad(8,5,0x01, g_bg.dstMap);
				MetaTileLoad(7,5,0x00, g_walls.dstMap);
			}else{
				MetaTileLoad(6,5,0x02, g_bg.dstMap);
				MetaTileLoad(8,5,0x02, g_bg.dstMap);
				if(WallMode){
					MetaTileLoad(7,5,0x04, g_walls.dstMap);
				}else{
					MetaTileLoad(7,5,0x00, g_walls.dstMap);
				}
			}
		}
		if(key_hit(KEY_A)){
			ChestClosed=!ChestClosed;
			if(ChestClosed){
				MetaTileLoad(3,5,0x05, g_walls.dstMap);
				MetaTileLoad(3,9,0x05, g_walls.dstMap);
				MetaTileLoad(3,11,0x05, g_walls.dstMap);
				MetaTileLoad(7,3,0x05, g_walls.dstMap);
				MetaTileLoad(10,11,0x05, g_walls.dstMap);
				MetaTileLoad(11,11,0x05, g_walls.dstMap);
			}else{
				MetaTileLoad(3,5,0x00, g_walls.dstMap);
				MetaTileLoad(3,9,0x00, g_walls.dstMap);
				MetaTileLoad(3,11,0x00, g_walls.dstMap);
				MetaTileLoad(7,3,0x00, g_walls.dstMap);
				MetaTileLoad(10,11,0x00, g_walls.dstMap);
				MetaTileLoad(11,11,0x00, g_walls.dstMap);
			}
		}
		
		


		tte_printf("#{es;P}( x, y) = (%d,%d)\n(vx,vy) = (%d,%d)",
			x, y, g_vp.x, g_vp.y);
	}

	return 0;
}