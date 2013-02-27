/**
 *
 * Copyright 2009 Neuron Robotics, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "Bowler/Bowler.h"

static NAMESPACE_SET namespace={0,{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}};
static BYTE zone;
static METHOD_HANDLER getMeth={FALSE,0};
static METHOD_HANDLER postMeth={FALSE,0};
static METHOD_HANDLER critMeth={FALSE,0};

BYTE setMethodCallback(BYTE method,BYTE ( *_callback)(BowlerPacket*)){
	if (_callback==0)
		return FALSE;
	switch (method){
	case BOWLER_GET:
		getMeth.set=TRUE;
		getMeth.callback=_callback;
		return TRUE;
	case BOWLER_POST:
		postMeth.set=TRUE;
		postMeth.callback=_callback;
		return TRUE;
	case BOWLER_CRIT:
		critMeth.set=TRUE;
		critMeth.callback=_callback;
		return TRUE;
	}
	return FALSE;
}

#if defined(USE_DYN_RPC)
	static RPC_HANDLER_SET postSet={0,{}};
	static RPC_HANDLER_SET getSet={0,{}};
	static RPC_HANDLER_SET critSet={0,{}};
#else
	//#warning NOT using the dynamic RPC functions
#endif
BYTE addRPCLoc(const char * rpc,void( *_callback)(BowlerPacket*),RPC_HANDLER_SET * set){
	BYTE index = set->numRPC;
	if(index==MAX_NUM_RPC)
		return FALSE;
	set->handlers[index].rpc=GetRPCValue((char *)rpc);
	set->handlers[index].callback=_callback;
	set->numRPC++;
	return TRUE;
}

BYTE addRPC(BYTE method,const char * rpc,void( *_callback)(BowlerPacket*)){
	if (_callback==0)
		return FALSE;
	switch (method){
#if defined(USE_DYN_RPC)
	case BOWLER_GET:
		return addRPCLoc(rpc,_callback,&getSet);
	case BOWLER_POST:
		return addRPCLoc(rpc,_callback,&postSet);
	case BOWLER_CRIT:
		return addRPCLoc(rpc,_callback,&critSet);
#endif
	}
	return FALSE;
}

BOOL runRPC(BowlerPacket* Packet, RPC_HANDLER_SET * set){
	if(set->numRPC>0){
		//println("Checking RPC list");
		BYTE i;
		for(i=0;i<set->numRPC;i++){
			if(set->handlers[i].rpc == Packet->use.head.RPC){
				//print(", Found matching RPC");
				set->handlers[i].callback(Packet);
				return TRUE;
			}
		}
		//print(", No matches found");
	}
	//println("No RPC'c regestered");
	return FALSE;

}
static float lastPacketTime;
float getLastPacketTime(){
	if(lastPacketTime > getMs())
		lastPacketTime=0;
	return lastPacketTime;
}
/*
 */

void Process_Self_Packet(BowlerPacket * Packet){
	switch (Packet->use.head.Method){
	case BOWLER_STATUS:
		StatRPCs(Packet);
		break;
	case BOWLER_GET:
		GetRPCs(Packet);
		break;
	case BOWLER_POST:
		PostRPCs(Packet);
		break;
	case BOWLER_CRIT:
		CriticalRPCs(Packet);
		break;
	default:
		ERR(Packet,0,0x7f);
	}
	Packet->use.head.ResponseFlag=1;// Set the response flag
	Packet->use.head.MessageID=0;
	lastPacketTime = getMs();
}//finish processing the Packet

void StatRPCs(BowlerPacket *Packet){
	ERR(Packet,0,0);
}

void GetRPCs(BowlerPacket *Packet){
	zone = 0;
	switch (Packet->use.head.RPC){
	case _PNG:
		Packet->use.head.Method = BOWLER_POST;
		Packet->use.head.RPC = GetRPCValue("_png");
		Packet->use.head.DataLegnth = 4;
		break;
	case _NMS:
		if(Packet->use.head.DataLegnth==4){
			Packet->use.head.DataLegnth=5;
			Packet->use.head.Method = BOWLER_POST;
			Packet->use.data[0]=namespace.numNamespaces;
			//println("Have ");
			//p_ul(namespace.numNamespaces);
			//println(" namespaces");
		}else if(Packet->use.head.DataLegnth==5){
			if(Packet->use.data[0]<namespace.numNamespaces){
				//print("\nSending: ");
				Packet->use.head.DataLegnth=4;
				Packet->use.head.Method = BOWLER_POST;
				NAMESPACE tmp = namespace.names[Packet->use.data[0]];
				//print((char *)tmp.name);
				unsigned char i=0;
				while(tmp.name[i]!='\0'){
					Packet->use.data[i]=tmp.name[i];
					Packet->use.head.DataLegnth++;
					i++;
				}
			}else{
				ERR(Packet,zone,5);
			}
		}else{
			ERR(Packet,zone,4);
		}
		break;
	default:
#if defined(USE_DYN_RPC)
		if(runRPC(Packet,&getSet)==TRUE){
			return;
		}
#endif
		if(getMeth.set){
			//println("Using callback");
			if(getMeth.callback(Packet)==FALSE){
				//println("Failed get callback");
				ERR(Packet,zone,1);
			}
			return;
		}
	}

}
void PostRPCs(BowlerPacket *Packet){
	zone = 0;
	switch (Packet->use.head.RPC){
	default:

#if defined(USE_DYN_RPC)
		if(runRPC(Packet,&postSet)==TRUE){
			return;
		}
#endif

		if(postMeth.set){
			//println("Using callback");
			if(!postMeth.callback(Packet))
				ERR(Packet,zone,2);
			return;
		}
	}
}
void CriticalRPCs(BowlerPacket *Packet){
	zone = 0;
	switch (Packet->use.head.RPC){
	default:

#if defined(USE_DYN_RPC)
		if(runRPC(Packet,&critSet)==TRUE){
			return;
		}
#endif
		if(critMeth.set){
			//println("Using callback");
			if(!critMeth.callback(Packet))
				ERR(Packet,zone,3);
			return;
		}
	}
}

void AddNamespace(unsigned char len, const unsigned char *  string){
	////println("Adding a namespace: ");
	////print((char *)string);
	int index = namespace.numNamespaces;
	namespace.names[index].len=len;
	namespace.names[index].name=string;
	namespace.numNamespaces++;
}


