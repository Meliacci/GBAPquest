#include "tonc.h"
#include "interactions.h"
#include "metatile.h"
#include "hearts.h"

void InteractWith(TInteract* Interactable){
    if (Interactable)
    {
        switch (Interactable->type)
        {
            case EIT_BUTTON:
            MetaTileLoad(Interactable->x,Interactable->y,Interactable->state+1,Interactable->dst,Interactable->MetaTiles);
            MetaTileLoad(X_EXTRACT(Interactable->target),Y_EXTRACT(Interactable->target),0x00,Interactable->dst,Interactable->MetaTiles);
            Interactable->type=EIT_NONE;
            break;
        case EIT_CHEST:
            /* code */
            break;
        case EIT_ENEMY:
            /* code */
            break;
        case EIT_ITEM:
            /* code */
            break;
        case EIT_WALL:
            /* code */
            break;

        case EIT_NONE:
        default:
            break;
        }
    }
}