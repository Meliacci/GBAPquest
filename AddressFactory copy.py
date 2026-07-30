

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

VariableAddressDictionary={
    '.bss':UninitializedRam,
    '.data':InitializedRam,
    '.rodata':ROM,
    '.memorySections':SectionsStart,
}

AddressDictionary={
    'bossMetaTiles':0x08004e40,
    'humanTiles':0x08005224,
    'humanPal':0x08005424,
    'inanimatesMetaTiles':0x08005774,
    'normal_enemyMetaTiles':0x08005be4,
    'HardModeHealthToSprite':0x0803d274,
    'Initializers':0x0803d28c,
    '__bss_start__':0x03001504,
    'g_CoordChecked':0x03001524,
    'g_CoordLUT':0x03001624,
    'InteractiveInitializers':0x03001a24,
    'InteractiveInitializers':0x03001a24,
    'DiedThisFrame':0x03001b14,
    '__bss_end__':0x03002910,
    '__data_start__':0x03002910,
    'PRNG_Next':0x03002910,
    'Inventory':0x03002934,
    'InitializersLen':0x0300294c,
    'HardMode':0x0300294e,
    'ExtraChest':0x0300294f,
    'HammerMode':0x03002950,
    '__data_end__':0x03002fd0,
}
SectionDictionary={
    'Initializers':'rodata',
    'g_CoordChecked':'bss',
    'g_CoordLUT':'bss',
    'InteractiveInitializers':'bss',
    'DiedThisFrame':'bss',
    'PRNG_Next':'data',
    'Inventory':'data',
    'HardMode':'data',
    'ExtraChest':'data',
    'HammerMode':'data',
}
# Section, AddressStart, Length, LoadAtMemoryROM
SectionsTuple=[
    ('.iwram',0x03000000,0x1504,0x0803d3a8),
    ('.data',0x03002910,0x6b8,0x0803e8ac),
    ('.init_array',0x03002fc8,0x4,0x0803ef64),
    ('.fini_array',0x03002fcc,0x4,0x0803ef68),
    ('.pad',0x02000000,0x8,0x0803ef6c),
]
