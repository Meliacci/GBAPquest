# Static Modes

At address 0x08:00:0f30

DisableDebugModes
    Originally: 
0x08:00:0fe4 
    Originally:If(key_hit(KEY_START)); b8 d5: bpl LAB_08000f58

    if(false){}:    b8 e7: b LAB_08000f58
## Should no longer be reachable but JUST in case Disable the Modes to the correct Ones
On Reset:
    Originally: HammerMode=HammerMode^1; 53 40: eor r3, r2 (Where previously mov r2, #0x1)
    HammerMode=1;    01 23: mov r3, #0x01
    HammerMode=0;    00 23: mov r3, #0x01


At address 0x08:00:1026

On Reset:
    Originally: *PTRToHardmodeRam = *PTRToHardmodeRam ^ 1; 53 40: eor r3, r2 (Where previously mov r2, #0x1)
    *PTRToHardmodeRam=1;    01 23: mov r3, #0x01
    *PTRToHardmodeRam=0;    00 23: mov r3, #0x01

At address 0x08:00:0ff4

On Reset:
    Originally: *PTRTO_EXTRACHEST = *PTRTO_EXTRACHEST ^ 1; 53 40: eor r3, r2 (Where previously mov r2, #0x1)
    *PTRTO_EXTRACHEST=1;    01 23: mov r3, #0x01
    *PTRTO_EXTRACHEST=0;    00 23: mov r3, #0x01

