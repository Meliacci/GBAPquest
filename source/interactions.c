#include "tonc.h"
#include "interactions.h"
#include "metatile.h"
#include "hearts.h"
#include "items.h"
#include "inanimates.h"

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
            if(Interactable->target>>8){
                Interactable->MetaTiles=heartsMetaTiles;
            }else{
                Interactable->MetaTiles=itemsMetaTiles;
            }
            MetaTileLoad(Interactable->x,Interactable->y,Interactable->target&0xFF,Interactable->dst,Interactable->MetaTiles);
            Interactable->type=EIT_ITEM;
            Interactable->state=Interactable->target;
            break;
        case EIT_ENEMY:
            if(true){//Check that player has the Sword
                Interactable->state=(Interactable->state)-1;
                if(Interactable->state>0){
                    MetaTileLoad(Interactable->x,Interactable->y,Interactable->state,Interactable->dst,Interactable->MetaTiles);
                }else{
                    if(Interactable->target>>8){
                        Interactable->MetaTiles=heartsMetaTiles;
                    }else{
                        Interactable->MetaTiles=itemsMetaTiles;
                    }
                    if(Interactable->target&0xFF){
                        MetaTileLoad(Interactable->x,Interactable->y,Interactable->target&0xFF,Interactable->dst,Interactable->MetaTiles);
                        Interactable->type=EIT_ITEM;
                        Interactable->state=Interactable->target;
                    }else{
                        Interactable->MetaTiles=inanimatesMetaTiles;
                        MetaTileLoad(Interactable->x,Interactable->y,0x00,Interactable->dst,Interactable->MetaTiles);
                        Interactable->type=EIT_NONE;
                    }
                }
            }
            break;
        case EIT_ITEM:
            Interactable->MetaTiles=inanimatesMetaTiles;
            MetaTileLoad(Interactable->x,Interactable->y,0x00,Interactable->dst,Interactable->MetaTiles);
            Interactable->type=EIT_NONE;
            Interactable->target; //Call to add Target Item into inventory
            break;
        case EIT_WALL:
            if (true) //Check item here
            {   
                MetaTileLoad(Interactable->x,Interactable->y,0x00,Interactable->dst,Interactable->MetaTiles);
                Interactable->type=EIT_NONE;
            }
            break;

        case EIT_NONE:
        default:
            break;
        }
    }
}