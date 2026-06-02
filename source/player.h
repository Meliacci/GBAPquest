//
//  Sprite Link movement
//
//! \file link.h
//! \author J Vijn
//! \date 20070216 - 20070216
//
// === NOTES ===


#ifndef __PLAYER__
#define __PLAYER__


// === CONSTANTS ======================================================

enum ELookDir
{
	LOOK_DOWN= 0, LOOK_LEFT, LOOK_RIGHT, LOOK_UP,
};

#define SPR_STATE_STAND		0x0100
#define SPR_STATE_WALK		0x0200

// === CLASSES ========================================================

typedef struct TSprite
{
	u32			x, y;		//!< Position in Meta Tiles
	s16			vx,vy; 		//!< This will be A Single Up/Down toggle
	u16			state;		//!< Sprite state
	u8			dir;		//!< Look direction
	u8			objId;		//!< Object index
} TSprite;

#endif // __PLAYER__


// --------------------------------------------------------------------
// PROTOTYPES
// --------------------------------------------------------------------

void player_init(TSprite *link, u32 x, u32 y, int obj_id);
void player_input(TSprite *link);

void player_move(TSprite *link);

void player_turn(TSprite *link);

// EOF
