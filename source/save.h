#include <tonc.h>
#ifndef __SAVE__
#define __SAVE__
#define SAVE_FLAG_OFFS 0x20
#define SAVE_CHECKS_OFFS 0x30
#endif // __SAVE__

void loadSave();
void save();
void deleteSave();