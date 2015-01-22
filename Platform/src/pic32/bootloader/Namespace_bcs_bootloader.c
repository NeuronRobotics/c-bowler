#include "Bowler/Bowler.h"
#define DYIO
//  int getHeartBeatTime();
//  boolean getHeartBeatLock();
//void setHeartBeatState(boolean hb, int time);
boolean resetFlag = false;
//char safeNSName[] = "bcs.bootloader.*;0.3;;";
uint8_t core0str[] = "pic32mx440f128h";
uint8_t core1str[] = "avr_atmegaXX4p_";
uint8_t avrID[7];

void callBootloaderReset() {
    resetFlag = true;
}

boolean getBootloaderResetFlag() {
    return resetFlag;
}

boolean bcsBootloaderAsyncEventCallback(BowlerPacket *Packet, boolean(*pidAsyncCallbackPtr)(BowlerPacket *Packet)) {
    //println_W("Async ");print_W(safeNSName);

    return false;
}

uint8_t bcsBootloaderProcessor_g(BowlerPacket *Packet) {
    int i;
    int offset = 0;
    uint8_t rev[3];
    switch (Packet->use.head.RPC) {
        case BLID:
            Packet->use.head.Method = BOWLER_POST;
            for (i = 0; i<sizeof (core0str); i++) {
                Packet->use.data[i] = core0str[i];
            }
            offset = sizeof (core0str);
            for (i = 0; i<sizeof (core1str); i++) {
                Packet->use.data[i + offset] = core1str[i];
            }
            GetAVRid(avrID);
            offset = sizeof (core0str) + sizeof (core1str);
            for (i = 0; i < 6; i++) {
                Packet->use.data[i + offset - 1] = avrID[i];
            }
            Packet->use.head.DataLegnth = sizeof (core0str) + sizeof (core1str) + 6 + 4;
            break;
        case _REV:
            //FlashGetFwRev(rev);
            for (i = 0; i < 3; i++) {
                Packet->use.data[i] = rev[i];
            }
            //FlashGetBlRev(rev);
            for (i = 0; i < 3; i++) {
                Packet->use.data[i + 3] = rev[i];
            }
            Packet->use.head.DataLegnth = 4 + 6;
            break;
    }
    return true;
}

uint8_t bcsBootloaderProcessor_c(BowlerPacket *Packet) {

    switch (Packet->use.head.RPC) {

        case PROG:
            if (Packet->use.data[0] == 0) {
                writeLine(Packet);
            } else {
#if !defined(DYIO)
                ERR(Packet, 0, 1);
                break;
#endif
            }
            if (Packet->use.data[0] == 1) {
                avrSPIProg(Packet);
            }
            READY(Packet, 0, 0);
            break;
        case ERFL:
            if (Packet->use.data[0] == 0) {
                eraseFlash();
            } else {
#if !defined(DYIO)
                ERR(Packet, 0, 2);
                break;
#endif
            }
            if (Packet->use.data[0] == 1) {
                eraseAVR();
            }
            READY(Packet, 0, 1);
            //printfDEBUG("#Erasing device");
            break;
        case REST:
            //printfDEBUG("#Resetting device");
            resetFlag = true;
            READY(Packet, 0, 3);
    }
    return true;
}

  RPC_LIST bcsBootloader_blid_g = {BOWLER_GET, // Method
    "blid", //RPC as string
    &bcsBootloaderProcessor_g, //function pointer to a packet parsinf function
     {0}, // Calling arguments
    BOWLER_POST, // response method
    {
        BOWLER_ASCII,
        BOWLER_ASCII,
        BOWLER_I08,
        BOWLER_I08,
        BOWLER_I08,
        BOWLER_I08,
        BOWLER_I08,
        BOWLER_I08,
        0}, // Calling arguments
    NULL //Termination
};

  RPC_LIST bcsBootloader__rev_g = {BOWLER_GET, // Method
    "_rev", //RPC as string
    &bcsBootloaderProcessor_g, //function pointer to a packet parsinf function
     {0}, // Calling arguments
    BOWLER_POST, // response method
     {
        BOWLER_I08,
        BOWLER_I08,
        BOWLER_I08,
        BOWLER_I08,
        BOWLER_I08,
        BOWLER_I08,
        0}, // Calling arguments
    NULL //Termination
};

  RPC_LIST bcsBootloader_prog_c = {BOWLER_CRIT, // Method
    "prog", //RPC as string
    &bcsBootloaderProcessor_c, //function pointer to a packet parsinf function
     {0}, // Calling arguments
    BOWLER_POST, // response method
     {0}, // Calling arguments
    NULL //Termination
};

  RPC_LIST bcsBootloader_erfl_c = {BOWLER_CRIT, // Method
    "erfl", //RPC as string
    &bcsBootloaderProcessor_c, //function pointer to a packet parsinf function
     {0}, // Calling arguments
    BOWLER_POST, // response method
    {0}, // Calling arguments
    NULL //Termination
};

  RPC_LIST bcsBootloader_rest_c = {BOWLER_CRIT, // Method
    "rest", //RPC as string
    &bcsBootloaderProcessor_c, //function pointer to a packet parsinf function
    {0}, // Calling arguments
    BOWLER_POST, // response method
    {0}, // Calling arguments
    NULL //Termination
};




  NAMESPACE_LIST bcsBootloader = {"bcs.bootloader.*;0.3;;", // The string defining the namespace
    NULL, // the first element in the RPC list
    &bcsBootloaderAsyncEventCallback, // async for this namespace
    NULL// no initial elements to the other namesapce field.
};

  boolean BootloadernamespcaedAdded = false;

NAMESPACE_LIST * get_bcsBootloaderNamespace() {
    if (!BootloadernamespcaedAdded) {
        //POST
        //Add the RPC structs to the namespace
        addRpcToNamespace(&bcsBootloader, & bcsBootloader_blid_g);
        addRpcToNamespace(&bcsBootloader, & bcsBootloader__rev_g);
        addRpcToNamespace(&bcsBootloader, & bcsBootloader_prog_c);
        addRpcToNamespace(&bcsBootloader, & bcsBootloader_erfl_c);
        addRpcToNamespace(&bcsBootloader, & bcsBootloader_rest_c);
        BootloadernamespcaedAdded = true;
    }

    return &bcsBootloader; //Return pointer to the struct
}





