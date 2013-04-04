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

    RunPIDComs();
    return FALSE;
}

//Get RPC's
static RPC_LIST bcsPid_APID={	BOWLER_GET,
                                "apid",
                                &processPIDGet,
                                NULL //Termination
};
static RPC_LIST bcsPid__PID={	BOWLER_GET,
                                "_pid",
                                &processPIDGet,
                                NULL//Termination
};
static RPC_LIST bcsPid_CPID={	BOWLER_GET,
                                "cpid",
                                &processPIDGet,
                                NULL//Termination
};
static RPC_LIST bcsPid_CPDV={	BOWLER_GET,
                                "cpdv",
                                &processPIDGet,
                                NULL//Termination
};
static RPC_LIST bcsPid_GPDC={	BOWLER_GET,
                                "gpdc",
                                &processPIDGet,
                                NULL//Termination
};

//Post RPC's
static RPC_LIST bcsPid_APID_p={	BOWLER_POST,
                                "apid",
                                &processPIDPost,
                                NULL //Termination
};
static RPC_LIST bcsPid__PID_p={	BOWLER_POST,
                                "_pid",
                                &processPIDPost,
                                NULL//Termination
};
static RPC_LIST bcsPid__VPD={	BOWLER_POST,
                                "apid",
                                &processPIDPost,
                                NULL //Termination
};
static RPC_LIST bcsPid_RPID={	BOWLER_POST,
                                "rpid",
                                &processPIDPost,
                                NULL//Termination
};

//Critical
static RPC_LIST bcsPid_KPID={	BOWLER_CRIT,
                                "kpid",
                                &processPIDCrit,
                                NULL//Termination
};
static RPC_LIST bcsPid_CPID_c={	BOWLER_CRIT,
                                "cpid",
                                &processPIDCrit,
                                NULL//Termination
};
static RPC_LIST bcsPid_CPDV_c={	BOWLER_CRIT,
                                "cpdv",
                                &processPIDCrit,
                                NULL//Termination
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
