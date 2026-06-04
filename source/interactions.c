#include "interactions.h"

void InteractWith(TInteract* Interactable){
    switch (Interactable->type)
    {
    case EIT_BUTTON:
        /* code */
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