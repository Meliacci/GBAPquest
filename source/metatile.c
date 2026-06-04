#include <tonc.h>
#include "inanimates.h"

bool OffsetMetaTileEquals(u16 MetaX, u16 MetaY, u16 TileID, SCR_ENTRY *se_en,  const unsigned short *MetaTiles, u16 offset){
	MetaX*=2;
	MetaY*=2;
	TileID*=4;
	return 
	(se_en[MetaY*32+MetaX]==(MetaTiles[TileID]+offset))&&
	(se_en[MetaY*32+MetaX+1]==(MetaTiles[TileID+1]+offset))&&
	(se_en[(MetaY+1)*32+MetaX]==(MetaTiles[TileID+2]+offset))&&
	(se_en[(MetaY+1)*32+MetaX+1]==(MetaTiles[TileID+3]+offset));
}
void OffsetMetaTileLoad(u16 MetaX, u16 MetaY, u16 TileID, SCR_ENTRY *se_en, const unsigned short *MetaTiles, u16 offset){
	MetaX*=2;
	MetaY*=2;
	TileID*=4;
	se_en[MetaY*32+MetaX]=MetaTiles[TileID]+offset;
	se_en[MetaY*32+MetaX+1]=MetaTiles[TileID+1]+offset;
	se_en[(MetaY+1)*32+MetaX]=MetaTiles[TileID+2]+offset;
	se_en[(MetaY+1)*32+MetaX+1]=MetaTiles[TileID+3]+offset;
}


bool MetaTileEquals(u16 MetaX, u16 MetaY, u16 TileID, SCR_ENTRY *se_en,  const unsigned short *MetaTiles){
	return OffsetMetaTileEquals(MetaX, MetaY, TileID, se_en, MetaTiles, 0);
}
void MetaTileLoad(u16 MetaX, u16 MetaY, u16 TileID, SCR_ENTRY *se_en, const unsigned short *MetaTiles){
	return OffsetMetaTileLoad(MetaX, MetaY, TileID, se_en, MetaTiles, 0);
}