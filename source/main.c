//
// brin_demo.c
// Essential tilemap loading: the start of brinstar from metroid 1
//
// (20060221 - 20070216, cearn)

#include <string.h>

#include <tonc.h>
#include "BG.h"

#define MAX_X_SCROLL 16
#define MAX_Y_SCROLL 96


void MetaTileLoad(u16 MetaX, u16 MetaY, u16 TileID);

int main()
{
	// Init interrupts and VBlank irq.
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	// Load palette
	memcpy16(pal_bg_mem, BGPal, BGPalLen / sizeof(u16));
	// Load tiles into CBB 0
	memcpy32(&tile_mem[0][0], BGTiles, BGTilesLen / sizeof(u32));
	
	// Load map into SBB 30
	for (u16 i = 0; i < 16; i++)
	{
		for (u16 x = 0; x < 16; x++)
		{
			/* code */
			MetaTileLoad(x,i,BGMetaMap[i*16+x]);
		}
		
	}
	

	// set up BG0 for a 4bpp 32x32t(256bx256b) map, using
	//   using charblock 0 and screenblock 31
	REG_BG0CNT= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_32x32;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0;

	// Scroll around some
	int x= 0, y=0;
	bool HammerMode=true;
	while(1)
	{
		VBlankIntrWait();
		key_poll();

		x += key_tri_horz();
		y += key_tri_vert();
		if(key_hit(KEY_A)){
			HammerMode=!HammerMode;
			if(HammerMode){
				MetaTileLoad(6,5,0x01);
				MetaTileLoad(8,5,0x01);
			}else{
				MetaTileLoad(6,5,0x02);
				MetaTileLoad(8,5,0x02);
			}
		}
		if(x<0){x=0;}
		if(x>MAX_X_SCROLL){x=MAX_X_SCROLL;}
		if(y<0){y=0;}
		if(y>MAX_Y_SCROLL){y=MAX_Y_SCROLL;}
		REG_BG0HOFS= x;
		REG_BG0VOFS= y;
	}

	return 0;
}


void MetaTileLoad(u16 MetaX, u16 MetaY, u16 TileID){
	MetaX*=2;
	MetaY*=2;
	TileID*=4;
	se_mem[30][MetaY*32+MetaX]=BGMetaTiles[TileID]&0x1F;
	se_mem[30][MetaY*32+MetaX+1]=BGMetaTiles[TileID+1]&0x1F;
	se_mem[30][(MetaY+1)*32+MetaX]=BGMetaTiles[TileID+2]&0x1F;
	se_mem[30][(MetaY+1)*32+MetaX+1]=BGMetaTiles[TileID+3]&0x1F;
}