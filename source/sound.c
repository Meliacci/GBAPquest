#include <maxmod.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include "sound.h"

mm_sound_effect boom = {
		{ SFX_BOOM } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		127,	// panning
	};

mm_sound_effect boss = {
		{ SFX_BOSSHIT } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		127,	// panning
    };

mm_sound_effect enemy = {
        { SFX_ENEMYHIT } ,			// id
        (int)(1.0f * (1<<10)),	// rate
        0,		// handle
        255,	// volume
        127,	// panning
    };

mm_sound_effect lock = {
        { SFX_RECLICK } ,			// id
        (int)(1.0f * (1<<10)),	// rate
        0,		// handle
        255,	// volume
        127,	// panning
    };

void PlayEffect(enum EMaxModEffectEx EffectIndex){
    switch (EffectIndex)
    {
    case EMME_BOOM:
        mmEffectEx(&boom);
        break;
    case EMME_BOSS:
        mmEffectEx(&boss);
        break;
    case EMME_ENEMY:
        mmEffectEx(&enemy);
        break;
    case EMME_LOCK:
        mmEffectEx(&lock);
        break;
    default:
        break;
    }
}