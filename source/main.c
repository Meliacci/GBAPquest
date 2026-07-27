//
// brin_demo.c
// Essential tilemap loading: the start of brinstar from metroid 1
//
// (20060221 - 20070216, cearn)

#include <string.h>
#include <maxmod.h>
#include "soundbank.h"
#include "soundbank_bin.h"

#include <tonc.h>
#include "BG.h"
#include "inanimates.h"
#include "items.h"
#include "human.h"
#include "confetti.h"
#include "boss.h"
#include "normal_enemy.h"
#include "hearts.h"

#include "metatile.h"
#include "player.h"
#include "confettiCannon.h"
#include "interactions.h"
#include "inventory.h"
#include "save.h"
#include "sound.h"

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
bool HammerMode=false;
bool WallMode=true;
bool DiedThisFrame=false;
bool ExtraChest=true;
bool HardMode=true; //Will probably Need to Be `Volatile`d?? haven't tested this yet
extern u32 ShootingConfetti;

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

const TInteract Initializers[]={//No need To Make these Change as it's the Default Configure
	//Buttons 0
	{EIT_BUTTON, 3,3, 0x08, XY_MIXER(11,5), inanimatesMetaTiles},
	//Chests 1-6
	{EIT_CHEST, 3,5, 0x05, ITEM_HEALTH, inanimatesMetaTiles},
	{EIT_CHEST, 3,9, 0x05, ITEM_CONFETTI, inanimatesMetaTiles},//0x02 Confetti Extra Chest
	{EIT_CHEST, 3,11, 0x05, ITEM_SWORD, inanimatesMetaTiles},
	{EIT_CHEST, 7,3, 0x05, ITEM_HEALTH, inanimatesMetaTiles},
	{EIT_CHEST, 10,11, 0x05, ITEM_SHIELD, inanimatesMetaTiles},
	{EIT_CHEST, 11,11, 0x05, ITEM_HAMMER, inanimatesMetaTiles},//0x06, Hammer Turns into Confetti when not in hammer mode
	//Enemies 7-8
	{EIT_BOSS, 11,3, 0x08, 2, bossMetaTiles},//Let's start this one at EasyMode
	{EIT_ENEMY, 11,7, 0x01, ITEM_KEY, normal_enemyMetaTiles},//this one is also Easy mode, Hardmode Starts at 2
	//Interaction  Walls 9-10
	{EIT_WALL, 3,7, 0x07, ITEM_KEY, inanimatesMetaTiles},
	{EIT_WALL, 8,11, 0x06, ITEM_SWORD, inanimatesMetaTiles},
	//Gate 11
	{EIT_NONE, 11,5, 0x0A, ITEM_NOTHING, inanimatesMetaTiles},
	//if Hammer Mode 12-14
	{EIT_WALL, 7,5, 0x04, ITEM_HAMMER, inanimatesMetaTiles},
	{EIT_NONE, 6,5, 0x02, ITEM_NOTHING, inanimatesMetaTiles},
	{EIT_NONE, 8,5, 0x02, ITEM_NOTHING, inanimatesMetaTiles},
};
TInteract InteractiveInitializers[15];
u16 InitializersLen = 15;

TInteract* g_CoordLUT[16][16];
bool g_CoordChecked[16][16];
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
	REG_BG_OFS[bgnr].x= 8;
	REG_BG_OFS[bgnr].y= 8;

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
	
	for (u16 i = 0; i < InitializersLen; i++)
	{
		InteractiveInitializers[i]=Initializers[i];
	}
	u32 copyLen=InitializersLen;
	if(!HammerMode){
		copyLen-=3;
		InteractiveInitializers[6].target=ITEM_CONFETTI;
	}else{
		InteractiveInitializers[6].target=ITEM_HAMMER;
	}
	if(!HardMode){
		InteractiveInitializers[7]=(TInteract){EIT_BOSS, 11,3, 0x08, 2, bossMetaTiles};
		InteractiveInitializers[8].state=0x01;
	}else{
		InteractiveInitializers[7]=(TInteract){EIT_BOSS, 11,3, 0x05, 5, bossMetaTiles};
		InteractiveInitializers[8].state=0x02;
	}
	if(!ExtraChest){
		InteractiveInitializers[2].state=0x00;
	}else{
		InteractiveInitializers[2].state=0x05;//0x05 is chest
	}
	for (u16 i = 0; i < InitializersLen; i++)
	{
		if (i<copyLen)
		{		
			TInteract* tempInit=&InteractiveInitializers[i];
			MetaTileLoad(tempInit->x,tempInit->y,tempInit->state,dst,tempInit->MetaTiles);
			tempInit->dst=dst;
			g_CoordLUT[tempInit->x][tempInit->y]=tempInit;
			g_CoordChecked[tempInit->x][tempInit->y]=false;
		}else{//If it's not hammer mode this will be the last 3 Elements, which have the Blocking Walls, Remove them
			TInteract* tempInit=&InteractiveInitializers[i];
			MetaTileLoad(tempInit->x,tempInit->y,0x00,dst,tempInit->MetaTiles);
			tempInit->dst=dst;
			g_CoordLUT[tempInit->x][tempInit->y]=tempInit;
			g_CoordChecked[tempInit->x][tempInit->y]=false;
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

void wallt_meta_reload_room(TMapInfo *bgt){

	SCR_ENTRY *dst= bgt->dstMap;
	for (u16 i = 0; i < InitializersLen; i++)
	{
		if(!g_CoordChecked[Initializers[i].x][Initializers[i].y]){
			InteractiveInitializers[i]=Initializers[i];
			
			if(i==2){
				if(!ExtraChest){
					InteractiveInitializers[i].state=0x00;
				}else{
					InteractiveInitializers[i].state=0x05;//0x05 is chest
				}
			}else if(i==6){
				if (HammerMode)
				{
					InteractiveInitializers[6].target=ITEM_HAMMER;
				}else{
					InteractiveInitializers[6].target=ITEM_CONFETTI;
				}
				
			}else if (i==7)//HardCodedEnemyAndBossIndex
			{
				if(!HardMode){
					InteractiveInitializers[i]=(TInteract){EIT_BOSS, 11,3, 0x08, 2, bossMetaTiles};
				}else{
					InteractiveInitializers[i]=(TInteract){EIT_BOSS, 11,3, 0x05, 5, bossMetaTiles};
				}
			}else if(i==8){
				if(!HardMode){
					InteractiveInitializers[i].state=0x01;
				}else{
					InteractiveInitializers[i].state=0x02;
				}
			}

		}else{
			if (Initializers[i].type==EIT_BUTTON)
			{
				InteractiveInitializers[i]=Initializers[i];
				InteractiveInitializers[i].state=Initializers[i].state+1;
			}else{
				const TInteract Blank={EIT_NONE, Initializers[i].x,Initializers[i].y, 0x00, ITEM_NOTHING, inanimatesMetaTiles};
				InteractiveInitializers[i]=Blank;
			}
			
		}
	}
	u32 copyLen=InitializersLen;
	if(!HammerMode){
		copyLen-=3;
	}
	
	for (u16 i = 0; i < InitializersLen; i++)
	{
		if (i<copyLen)
		{		
			TInteract* tempInit=&InteractiveInitializers[i];
			MetaTileLoad(tempInit->x,tempInit->y,tempInit->state,dst,tempInit->MetaTiles);
			tempInit->dst=dst;
			g_CoordLUT[tempInit->x][tempInit->y]=tempInit;
		}else{//If it's not hammer mode this will be the last 3 Elements, which have the Blocking Walls, Remove them
			TInteract* tempInit=&InteractiveInitializers[i];
			MetaTileLoad(tempInit->x,tempInit->y,0x00,dst,tempInit->MetaTiles);
			tempInit->dst=dst;
			g_CoordLUT[tempInit->x][tempInit->y]=tempInit;
		}
	}
}

void ui_update(TMapInfo *bgt){
	for (u32 i = 1; i < 8; i++) //Annoyingly, i do have to "optimize" this
	{
		MetaTileLoad(i,1,0,bgt->dstMap, inanimatesMetaTiles);
		MetaTileLoad(0x0F - i,9,0,bgt->dstMap, inanimatesMetaTiles);
	}
	s32 offset=0;

	//Health at LeftMost Top
	u32 HealthIndex=InventoryLen-1;
	u32 damage=Inventory[HealthIndex].used;//We know it's the last element of the Inventory
	//Calculate Offset from Left top (1,1)
	s32 RemainingHealth=Inventory[HealthIndex].count*2-damage;
	for(s32 y=0; y<Inventory[HealthIndex].count ;y++){//Go Right to Left to update state
		if(RemainingHealth-2>=0){
			MetaTileLoad(offset+1,1,0x02,bgt->dstMap,heartsMetaTiles);
			RemainingHealth-=2;
		}else if (RemainingHealth-1>=0)
		{
			MetaTileLoad(offset+1,1,0x03,bgt->dstMap,heartsMetaTiles);
			RemainingHealth-=1;
		}else{
			MetaTileLoad(offset+1,1,0x01,bgt->dstMap,heartsMetaTiles);
		}
		offset++;
	}
	//Items at Rightmost Bottom?]
	offset=0;
	for(s32 i=InventoryLen-1; i>=0;i--){//We Walk back to Draw from the right and Reduce the Offset
		if(!(Inventory[i].state>>8)){
			for(u32 y=0; y<Inventory[i].count-Inventory[i].used;y++){
				//Let's Abuse this ""Bug"" where You wrap and Step Over One Non-Meta S-Tile when Overflowing X by 0x10 (Every 0x20 you Go down a full meta-tile)
				MetaTileLoad(0x0D +offset+1,9,Inventory[i].state,bgt->dstMap,itemsMetaTiles);
				offset--;
			}
		}
    }
	//load_inv_from_SRAM(); Load only if prompted?
}

int main()
{
	// Init interrupts and VBlank irq.
	irq_init(NULL);
	irq_add(II_VBLANK, mmVBlank);
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
	memcpy32(&tile_mem[0][85], normal_enemyTiles, normal_enemyTilesLen / sizeof(u32));
	
	bgt_meta_init(&g_bg, 3, BG_CBB(0)|BG_SBB(30) | BG_4BPP | BG_REG_32x32|BG_PRIO(2), BGMetaMap, 16,
		16, 16);
	
	wallt_meta_init(&g_walls, 2, BG_CBB(0)|BG_SBB(26) | BG_4BPP | BG_REG_32x32|BG_PRIO(2), 16,
		16, 16);
		
	ui_meta_init(&g_ui,1,BG_CBB(0)|BG_SBB(28)| BG_4BPP | BG_REG_32x32 | BG_PRIO(0), 16, 16, 16);
	GRIT_CPY(pal_obj_mem, humanPal);
	GRIT_CPY(tile_mem[4], humanTiles);
	
	confetti_init();

	player_init(&g_link, int2fx(96), int2fx(176), 0);

	REG_DISPCNT= DCNT_MODE0 | DCNT_BG1 | DCNT_BG2 | DCNT_BG3 | DCNT_OBJ |
		DCNT_OBJ_1D;
	ui_update(&g_ui);
	// Scroll around some

	mmInitDefault( (mm_addr)soundbank_bin, 8 );
	mmSetModuleVolume(512);
	mmStart( MOD_BGM, MM_PLAY_LOOP );
	int x= 0, y=0;
	while(1)
	{
		VBlankIntrWait();
		mmFrame();
		ui_update(&g_ui);
		key_poll();
		
		
		// Player Logic & stuff
		player_input(&g_link);
		player_turn(&g_link);
		player_move(&g_link);
		if(DiedThisFrame){
			PlayEffect(EMME_BOOM);
			DiedThisFrame=false;
			full_heal();
			player_tp(&g_link, int2fx(96), int2fx(176)); //Spawn at Start, as He failed
			wallt_meta_reload_room(&g_walls);
		}
		if (key_hit(KEY_SELECT))
		{
			if(key_is_down(KEY_LEFT)){
				loadSave();
				DiedThisFrame=true;
			}else if(key_is_down(KEY_RIGHT)){
				deleteSave();
				DiedThisFrame=true;
			}else if(!(key_tri_vert()||key_tri_horz())){
				save();
			}
			
		}
		if (key_hit(KEY_START))
		{
			if(key_is_down(KEY_DOWN)){
				HardMode=!HardMode;
				DiedThisFrame=true;
			}else if(key_is_down(KEY_RIGHT)){
				ExtraChest=!ExtraChest;
				DiedThisFrame=true;
			}
			else{
				HammerMode=!HammerMode;
				DiedThisFrame=true;
			}
			
			wallt_meta_init(&g_walls, 2, BG_CBB(0)|BG_SBB(26) | BG_4BPP | BG_REG_32x32|BG_PRIO(2), 16,
				16, 16);
			reset_inventory();

		}
		if(key_hit(KEY_A)){
			if(use_item(ITEM_CONFETTI)){
				ShootingConfetti=CONFETTI_TIMER_MAX;
			}
		}
		//Screen View Stuff
		x= fx2int(g_link.x), y= fx2int(g_link.y);
		ShootConfetti();
		vp_center(&g_vp, x, y);
		oam_copy(oam_mem, obj_buffer, 128);
		bgt_update(&g_bg, &g_vp);
		bgt_update(&g_walls, &g_vp);
	}

	return 0;
}