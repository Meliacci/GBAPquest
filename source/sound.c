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

void PlayEffect(enum EMaxModEffectEx EffectIndex){
    switch (EffectIndex)
    {
    case EMME_BOOM:
        mmEffectEx(&boom);
        break;
    
    default:
        break;
    }
}