#include "Bowler/Bowler.h"



static const char rpcNSName[]   = "bcs.rpc.*;0.3;;";


BOOL _rpc(BowlerPacket * Packet){
	Packet->use.head.Method = BOWLER_POST;
	Packet->use.head.RPC = GetRPCValue("_rpc");
        int index = 0;
        int nsIndex = Packet->use.data[index++];
        int rpcIndex = Packet->use.data[index++];
        Packet->use.data[index++] = getNumberOfRpcs(nsIndex);
        RPC_LIST * rpc = getRpcByIndex(getNamespaceAtIndex(nsIndex),rpcIndex);
        UINT32_UNION rpcValue;
        rpcValue.Val = GetRPCValue((char*)rpc->rpc);
        int i;
        for(i=0;i<4;i++){
            Packet->use.data[index++] = rpcValue.v[i];
        }
        Packet->use.data[index++] = rpc->bowlerMethod;
        
        Packet->use.head.DataLegnth = 4+index;
	return TRUE;
}
BOOL _rpcArgs(BowlerPacket * Packet){
	Packet->use.head.Method = BOWLER_POST;
	Packet->use.head.RPC = GetRPCValue("args");
        int index = 0;
        int nsIndex = Packet->use.data[index++];
        int rpcIndex = Packet->use.data[index++];

        RPC_LIST * rpc = getRpcByIndex(getNamespaceAtIndex(nsIndex),rpcIndex);
        UINT32_UNION rpcValue;
        rpcValue.Val = GetRPCValue((char*)rpc->rpc);
        int i;
        for(i=0;i<4;i++){
            Packet->use.data[(index++)] = rpcValue.v[i];
        }

        Packet->use.data[(index++)] = rpc->bowlerMethod;

        if(rpc->arguments == NULL){
             Packet->use.data[(index++)] = 0;
        }else{
           //add non NULL arguments
            i=0;
            while(rpc->arguments[i]!=NULL){
                Packet->use.data[(index++)]= rpc->arguments[i++];
            }
        }
        Packet->use.data[(index++)] = rpc->responseMethod;
        if(rpc->responseArguments == NULL){
             Packet->use.data[(index++)] = 0;
        }else{
           //add non NULL arguments
            i=0;
            while(rpc->responseArguments[i]!=NULL){
                Packet->use.data[(index++)]= rpc->responseArguments[i++];
            }
        }

        Packet->use.head.DataLegnth = 4+index;
	return TRUE;
}
//Get RPC's
static RPC_LIST bcsRpc__RPC={	BOWLER_GET,
                                "_rpc",
                                &_rpc,
                                {BOWLER_I08,BOWLER_I08,NULL},// Calling arguments
                                BOWLER_POST,// response method
                                {BOWLER_I08,BOWLER_I08,BOWLER_I08,BOWLER_I32,BOWLER_I08,NULL},// Response arguments
                                NULL //Termination
};
//Get RPC's
static RPC_LIST bcsRpc_ARGS={	BOWLER_GET,
                                "args",
                                &_rpcArgs,
                                {BOWLER_I08,BOWLER_I08,NULL},// Calling arguments
                                BOWLER_POST,// response method
                                {BOWLER_I08,BOWLER_I08,BOWLER_I32,BOWLER_STR,BOWLER_I32,BOWLER_STR,NULL},// Response arguments
                                NULL //Termination
};


static NAMESPACE_LIST bcsRpc ={	rpcNSName,// The string defining the namespace
                                NULL,// the first element in the RPC list
                                NULL,// async for this namespace
                                NULL// no initial elements to the other namesapce field.
};



static BOOL namespcaedAdded = FALSE;

NAMESPACE_LIST * getBcsRpcNamespace(){
	if(!namespcaedAdded){
		//GET
		addRpcToNamespace(&bcsRpc,& bcsRpc__RPC);
                addRpcToNamespace(&bcsRpc,& bcsRpc_ARGS);
		namespcaedAdded =TRUE;
	}

	return &bcsRpc;
}
