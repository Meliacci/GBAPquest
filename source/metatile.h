#include <tonc.h>
#ifndef __METATILE__
#define __METATILE__

#endif // __METATILE__

bool MetaTileEquals(u16 MetaX, u16 MetaY, u16 TileID, SCR_ENTRY *se_en,  const unsigned short *MetaTiles, u16 offset);
void MetaTileLoad(u16 MetaX, u16 MetaY, u16 TileID, SCR_ENTRY *se_en, const unsigned short *MetaTiles, u16 offset);