#include "tonc.h"
#include "interactions.h"
#include "inventory.h"
#include "metatile.h"
#include "hearts.h"
#include "items.h"
#include "inanimates.h"

extern bool g_CoordChecked[16][16];
void InteractWith(TInteract* Interactable){
    if (Interactable)
    {
        switch (Interactable->type)
        {
            case EIT_BUTTON:
            MetaTileLoad(Interactable->x,Interactable->y,Interactable->state+1,Interactable->dst,Interactable->MetaTiles);
            MetaTileLoad(X_EXTRACT(Interactable->target),Y_EXTRACT(Interactable->target),0x00,Interactable->dst,Interactable->MetaTiles);
            Interactable->type=EIT_NONE;
            //Needs to Check the 
            g_CoordChecked[X_EXTRACT(Interactable->target)][Y_EXTRACT(Interactable->target)]=true;
            break;
        case EIT_CHEST:
            if(Interactable->target>>8){
                Interactable->MetaTiles=heartsMetaTiles;//The meta Tile index for Upgrade should be 0x02, but the Item id is 0x0107, We Fix this manually, unfortunately
                MetaTileLoad(Interactable->x,Interactable->y,(Interactable->target&0xFF)-5,Interactable->dst,Interactable->MetaTiles);
            }else{
                Interactable->MetaTiles=itemsMetaTiles;
                MetaTileLoad(Interactable->x,Interactable->y,Interactable->target&0xFF,Interactable->dst,Interactable->MetaTiles);
            }
            Interactable->type=EIT_ITEM;
            Interactable->state=Interactable->target;
            break;
        case EIT_ENEMY:
            if(has_item(ITEM_SWORD)){//Check that player has the Sword
                Interactable->state=(Interactable->state)-1;
            }
                if(Interactable->state>0){
                    MetaTileLoad(Interactable->x,Interactable->y,Interactable->state,Interactable->dst,Interactable->MetaTiles);
                    deal_damage();
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
            break;
        case EIT_ITEM:
            Interactable->MetaTiles=inanimatesMetaTiles;
            MetaTileLoad(Interactable->x,Interactable->y,0x00,Interactable->dst,Interactable->MetaTiles);
            Interactable->type=EIT_NONE;
            add_item(Interactable->target); //Call to add Target Item into inventory
            break;
        case EIT_WALL:
            if (use_item(Interactable->target)) //Check item here
            {   
                MetaTileLoad(Interactable->x,Interactable->y,0x00,Interactable->dst,Interactable->MetaTiles);
                Interactable->type=EIT_NONE;
            }
            break;

        case EIT_NONE:
        default:
            break;
        }
        if(Interactable->type==EIT_NONE){//if after Changing Type you are None, Means that you no Longer need to be Interacted and are Checked
            g_CoordChecked[Interactable->x][Interactable->y]=true;
        }
    }
}