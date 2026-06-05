#include <tonc.h>
#ifndef __SAVE__
#define __SAVE__
#define SAVE_FLAG_OFFS 0x20
#define SAVE_CHECKS_OFFS 0x30
#endif // __SAVE__
void load_inv_from_SRAM();
void save_inv_to_SRAM();
void delete_inv_SRAM();

void load_checks_from_SRAM();
void save_checks_to_SRAM();
void delete_checks_SRAM();