#include "tonc.h"
#include "interactions.h"
#include "inventory.h"
#include "metatile.h"
#include "hearts.h"
#include "items.h"
#include "inanimates.h"
#include "sound.h"
//---------External------------

extern bool HardMode;
//---------

extern bool g_CoordChecked[16][16];

const int HardModeHealthToSprite[6]={1,2,3,4,5,6};
const int EasyModeHealthToSprite[6]={1,8,3,4,5,6};// SHOULDN'T be that high on Easy mode but just in case- it'll display the Health, even if it shouldn't be that high

void InteractWith(TInteract* Interactable){
    if (Interactable)
    {
        switch (Interactable->type)
        {
        case EIT_BUTTON:
            PlayEffect(EMME_LOCK);
            MetaTileLoad(Interactable->x,Interactable->y,Interactable->state+1,Interactable->dst,Interactable->MetaTiles);
            MetaTileLoad(X_EXTRACT(Interactable->target),Y_EXTRACT(Interactable->target),0x00,Interactable->dst,Interactable->MetaTiles);
            Interactable->type=EIT_NONE;
            //Needs to Check the 
            g_CoordChecked[X_EXTRACT(Interactable->target)][Y_EXTRACT(Interactable->target)]=true;
            break;
        case EIT_CHEST:
            PlayEffect(EMME_LOCK);
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
                PlayEffect(EMME_ENEMY);

            }
                if(Interactable->state>0){
                    MetaTileLoad(Interactable->x,Interactable->y,Interactable->state,Interactable->dst,Interactable->MetaTiles);
                    deal_damage();
                }else{
                    int Diff=0;
                    if(Interactable->target>>8){
                        Interactable->MetaTiles=heartsMetaTiles;
                        Diff=-5;
                    }else{
                        Interactable->MetaTiles=itemsMetaTiles;
                    }
                    if(Interactable->target&0xFF){
                        MetaTileLoad(Interactable->x,Interactable->y,(Interactable->target&0xFF)+Diff,Interactable->dst,Interactable->MetaTiles);
                        Interactable->type=EIT_ITEM;
                        Interactable->state=Interactable->target-Diff;
                    }else{
                        Interactable->MetaTiles=inanimatesMetaTiles;
                        MetaTileLoad(Interactable->x,Interactable->y,0x00,Interactable->dst,Interactable->MetaTiles);
                        Interactable->type=EIT_NONE;
                    }
                }
            break;
        case EIT_BOSS://Practically the Same Logic as EIT_ENEMY
            if(has_item(ITEM_SWORD)){//Check that player has the Sword
                Interactable->target=(Interactable->target)-1;
                PlayEffect(EMME_BOSS);
            }
                if(Interactable->target>0){//still Above 0 health
                    Interactable->state=(HardMode?HardModeHealthToSprite:EasyModeHealthToSprite)[Interactable->target-1];
                    MetaTileLoad(Interactable->x,Interactable->y,Interactable->state,Interactable->dst,Interactable->MetaTiles);
                    deal_damage();
                }else{//Boss dead
                    Interactable->MetaTiles=inanimatesMetaTiles;
                    MetaTileLoad(Interactable->x,Interactable->y,0x00,Interactable->dst,Interactable->MetaTiles);//Unload Boss
                    Interactable->type=EIT_NONE;
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
                PlayEffect(EMME_LOCK);
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