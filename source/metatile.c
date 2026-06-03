#include <tonc.h>
#include "inanimates.h"

bool MetaTileEquals(u16 MetaX, u16 MetaY, u16 TileID, SCR_ENTRY *se_en){
	MetaX*=2;
	MetaY*=2;
	TileID*=4;
	return 
	(se_en[MetaY*32+MetaX]==(inanimatesMetaTiles[TileID]))&&
	(se_en[MetaY*32+MetaX+1]==(inanimatesMetaTiles[TileID+1]))&&
	(se_en[(MetaY+1)*32+MetaX]==(inanimatesMetaTiles[TileID+2]))&&
	(se_en[(MetaY+1)*32+MetaX+1]==(inanimatesMetaTiles[TileID+3]));
}
void MetaTileLoad(u16 MetaX, u16 MetaY, u16 TileID, SCR_ENTRY *se_en){
	MetaX*=2;
	MetaY*=2;
	TileID*=4;
	se_en[MetaY*32+MetaX]=inanimatesMetaTiles[TileID];
	se_en[MetaY*32+MetaX+1]=inanimatesMetaTiles[TileID+1];
	se_en[(MetaY+1)*32+MetaX]=inanimatesMetaTiles[TileID+2];
	se_en[(MetaY+1)*32+MetaX+1]=inanimatesMetaTiles[TileID+3];
}