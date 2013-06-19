/*
 * Namespace_bcs_pid.c
 *
 *  Created on: Mar 7, 2013
 *      Author: hephaestus
 */

#include "Bowler/Bowler.h"
#include "Bowler/AbstractPID.h"


static const char pidNSName[]   = "bcs.pid.*;0.3;;";


BOOL pidAsyncEventCallbackLocal(BowlerPacket *Packet,BOOL (*pidAsyncCallbackPtr)(BowlerPacket *Packet)){

//    println_I("\n");
//    printPIDvals(0);

    RunPIDComs(Packet,pidAsyncCallbackPtr);
    return FALSE;
}

//Get RPC's
static RPC_LIST bcsPid_APID={	BOWLER_GET,
                                "apid",
                                &processPIDGet,
                                NULL,// Calling arguments
                                BOWLER_POST,// response method
                                ( (const char [2]){	BOWLER_I32STR,//All current values
													0}),// Response arguments
                                NULL //Termination
};
static RPC_LIST bcsPid__PID={	BOWLER_GET,
                                "_pid",
                                &processPIDGet,
                                ((const char [2]){	BOWLER_I08,//channel
                                                    0}),// Response arguments
                                BOWLER_POST,// response method
                                ((const char [3]){	BOWLER_I08,//channel
							BOWLER_I32,//current position
                                			0}),// Response arguments
                                NULL //Termination
};
static RPC_LIST bcsPid_CPID={	BOWLER_GET,
                                "cpid",
                                &processPIDGet,
                                ((const char [2]){	BOWLER_I08,//channel
                                                             0}),// Response arguments
                                BOWLER_POST,// response method
                                ((const char [11]){	BOWLER_I08,//channel
                                                        BOWLER_I08,//enabled
                                                        BOWLER_I08,//Polarity
                                                        BOWLER_I08,//async
                                                        BOWLER_FIXED100,//Kp
                                                        BOWLER_FIXED100,//Ki
                                                        BOWLER_FIXED100,//Kd
                                                        BOWLER_I32,//Latch value
                                                        BOWLER_I08,//Use index latch
                                                        BOWLER_I08,//Stop On latch
                                                   0}),// Response arguments
                                NULL //Termination
};
static RPC_LIST bcsPid_CPDV={	BOWLER_GET,
                                "cpdv",
                                &processPIDGet,
                                ((const char [2]){	BOWLER_I08,//channel
                                                             0}),// Response arguments
                                BOWLER_POST,// response method
                                ((const char [4]){	BOWLER_I08,//channel
                                                        BOWLER_FIXED100,//Kp
                                                        BOWLER_FIXED100,//Kd
                                                   0}),// Response arguments
                                NULL //Termination
};
static RPC_LIST bcsPid_GPDC={	BOWLER_GET,
                                "gpdc",
                                &processPIDGet,
                                NULL,// Calling arguments
                                BOWLER_POST,// response method
                                ((const char [2]){	BOWLER_I08,//channel
                                                             0}),// Response arguments
                                NULL //Termination
};

//Post RPC's
static RPC_LIST bcsPid_APID_p={	BOWLER_POST,
                                "apid",
                                &processPIDPost,
                                NULL,// Calling arguments
                                BOWLER_POST,// response method
                                NULL,// Response arguments
                                NULL //Termination
};
static RPC_LIST bcsPid__PID_p={	BOWLER_POST,
                                "_pid",
                                &processPIDPost,
                                NULL,// Calling arguments
                                BOWLER_POST,// response method
                                NULL,// Response arguments
                                NULL //Termination
};
static RPC_LIST bcsPid__VPD={	BOWLER_POST,
                                "_vpd",
                                &processPIDPost,
                                NULL,// Calling arguments
                                BOWLER_POST,// response method
                                NULL,// Response arguments
                                NULL //Termination
};
static RPC_LIST bcsPid_RPID={	BOWLER_POST,
                                "rpid",
                                &processPIDPost,
                                NULL,// Calling arguments
                                BOWLER_POST,// response method
                                NULL,// Response arguments
                                NULL //Termination
};

//Critical
static RPC_LIST bcsPid_KPID={	BOWLER_CRIT,
                                "kpid",
                                &processPIDCrit,
                                NULL,// Calling arguments
                                BOWLER_POST,// response method
                                NULL,// Response arguments
                                NULL //Termination
};
static RPC_LIST bcsPid_CPID_c={	BOWLER_CRIT,
                                "cpid",
                                &processPIDCrit,
                                ((const char [11]){	BOWLER_I08,//channel
                                                        BOWLER_I08,//enabled
                                                        BOWLER_I08,//Polarity
                                                        BOWLER_I08,//async
                                                        BOWLER_FIXED100,//Kp
                                                        BOWLER_FIXED100,//Ki
                                                        BOWLER_FIXED100,//Kd
                                                        BOWLER_I32,//Latch value
                                                        BOWLER_I08,//Use index latch
                                                        BOWLER_I08,//Stop On latch
                                                   0}),// Response arguments
                                BOWLER_STATUS,// response method
                                NULL,// Response arguments
                                NULL //Termination
};
static RPC_LIST bcsPid_CPDV_c={	BOWLER_CRIT,
                                "cpdv",
                                &processPIDCrit,
                                NULL,// Calling arguments
                                BOWLER_POST,// response method
                                NULL,// Response arguments
                                NULL //Termination
};



static NAMESPACE_LIST bcsPid ={	pidNSName,// The string defining the namespace
                                NULL,// the first element in the RPC list
                                &pidAsyncEventCallbackLocal,// async for this namespace
                                NULL// no initial elements to the other namesapce field.
};



static BOOL namespcaedAdded = FALSE;

NAMESPACE_LIST * getBcsPidNamespace(){
	if(!namespcaedAdded){
		//GET
		addRpcToNamespace(&bcsPid,& bcsPid_APID);
		addRpcToNamespace(&bcsPid,& bcsPid__PID);
		addRpcToNamespace(&bcsPid,& bcsPid_CPID);
		addRpcToNamespace(&bcsPid,& bcsPid_CPDV);
		addRpcToNamespace(&bcsPid,& bcsPid_GPDC);
		//Post
		addRpcToNamespace(&bcsPid,& bcsPid_APID_p);
		addRpcToNamespace(&bcsPid,& bcsPid__PID_p);
		addRpcToNamespace(&bcsPid,& bcsPid__VPD);
		addRpcToNamespace(&bcsPid,& bcsPid_RPID);

		//Critical
		addRpcToNamespace(&bcsPid,& bcsPid_KPID);
		addRpcToNamespace(&bcsPid,& bcsPid_CPID_c);
		addRpcToNamespace(&bcsPid,& bcsPid_CPDV_c);
		namespcaedAdded =TRUE;
	}

	return &bcsPid;
}
