from json import load
from os import write


ROM=['Initializers','humanTiles', 'humanPal', 'inanimatesMetaTiles', 'bossMetaTiles','normal_enemyMetaTiles']

#Initializers Structure size is  0x0f, Total Size is 0xf0 meaning there are 0x10(16) Initializer Structs
#

# enum EInteractType   type;		//!< Interaction Type
# 	u8	        		 x, y;		//!< Position in Meta Tiles
# 	u16			        state;		//!< Initial tile
#     u16                target;
#     /*
#     * Pointer to Meta Tile Mappings (where Given index tile*4+[0-3] returns the S-tile to be used at each of the 4 Tiles that make up a Meta Tile)
#     */
# 	const u16            *MetaTiles;		//!< Tile Reference
#    u32 SCR_ENTRY *dst; //!< Screen Entry
###### For the u16 state
#     /*
#     * Target Behaviour Dependent on interaction Type:
#     * EIT_NONE
#     * 
#     * Does nothing when Interacted with
#     * 
#     * EIT_CHEST
#     * 
#     * Places Target Index from ItemMetaTiles into Coords
#     * 
#     * EIT_BUTTON
#     * 
#     * Used by Exactly 1 Tile, If Interacts,Deletes Wall at Target Coord Where 0xFF00 is X and 0x00FF is y
#     * 
#     * EIT_ENEMY
#     * 
#     * Health indicated by State
#     * Once Health Reaches 0, Replace with Target Item at Coords
#     * 
#     * EIT_BOSS
#     * 
#     * This Needs a Bit more Work. He will never have an item himself.
#     * Target indicates True Health, while State Gives out the Initial tiles for the Interactable 
#     * 
#     * EIT_WALL
#     * 
#     * State is interactable Wall Type (Breakable)
#     * Checks that Player has Target Item in Inventory
#     * If player has, Delete Wall at Coord
#     *  
#     * EIT_ITEM
#     * 
#     * Deletes tile Entry and Places Target item in Player Inventory
#     * 
#     */


InitializedRam=['PRNG_Next','Inventory','HardMode','ExtraChest','HammerMode']
UninitializedRam=['DiedThisFrame', 'InteractiveInitializers','g_CoordChecked','g_CoordLUT']

SectionsStart = ['__data_start__','__bss_start__','__data_end__','__bss_end__',]
SectionLoad=['load address']

VariableAddressDictionary={
    '.bss':UninitializedRam,
    '.data':InitializedRam,
    '.rodata':ROM,
    '.memorySections':SectionsStart,
}

def checkAddresses()->list[list]:
    with open('build/GBAPquest.map') as datafile:
        
        found = list()
        for line in datafile:
            for wordKey in VariableAddressDictionary:
                for word in VariableAddressDictionary.get(wordKey,[]):
                        if word in line and '0x' in line:
                            nulist=[blem for blem in line.strip().split(" ") if blem]
                            found.append(nulist)
            
            
    return found
def checkSections()->list[list]:
    with open('build/GBAPquest.map') as datafile:
        
        found = list()
        for line in datafile:
            for wordKey in VariableAddressDictionary:
                for word in VariableAddressDictionary.get(wordKey,[]):
                        if word in line and wordKey in line:
                            nulist=[blem for blem in line.strip().split(".") if blem]
                            found.append(nulist)
            
            
    return found

def checkStarts()->list[list]:
    dicti={
        '.LoadAtMemory':SectionLoad
    }
    with open('build/GBAPquest.map') as datafile:
        
        found = list()
        for line in datafile:
            for wordlist in dicti.values():
                for word in wordlist:
                    if word in line and '0x' in line:
                        nulist=[blem for blem in line.strip().split(" ") if blem]
                        length=nulist[2]
                        if int(length,base=16):
                            found.append(nulist)
            
    return found

with open('AddressFactory.py', "w") as writeFile:
    writeFile.write("AddressDictionary={\n")
    nuli=checkAddresses()
    for duox in nuli:
        writeFile.write(f"    '{duox[1]}':{duox[0]},\n")
    writeFile.write("}\n")

    writeFile.write("SectionDictionary={\n")
    nuli=checkSections()
    for duox in nuli:
        writeFile.write(f"    '{duox[1]}':'{duox[0]}',\n")
    writeFile.write("}\n")
    
    Oli=checkStarts()
    writeFile.write("# Section, AddressStart, Length, LoadAtMemoryROM\n")
    writeFile.write("SectionsTuple=[\n")
    for duox in Oli:
        writeFile.write(f"    ('{duox[0]}',{duox[1]},{duox[2]},{duox[5]}),\n")
    writeFile.write("]\n")
        


