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

static float lastPacketTime;

float getLastPacketTime(){
	if(lastPacketTime > getMs())
		lastPacketTime=0;
	return lastPacketTime;
}


void Process_Self_Packet(BowlerPacket * Packet){
	int namespaceIndex = 0;
	int foundRpc = 0;
	int currentNamespaceIndexForPacket = namespaceIndex;
	NAMESPACE_LIST * tmp = getBcsCoreNamespace();
	// First locate all Namespaces for the given RPC
	do{
		//Start the list with the first one
		RPC_LIST * rpc = getRpcByID(tmp,Packet->use.head.RPC, Packet->use.head.Method );
		//null check
		if(rpc !=NULL){
			//Found matching RPC and Method to parse
			foundRpc++;
			currentNamespaceIndexForPacket =  namespaceIndex;
			//println_I("Rpc found in namespace: ");print_I(tmp->namespaceString);
		}
		//Null check and move to next namespace
		tmp = tmp->next;
		namespaceIndex++;
	}while(tmp != NULL);
	// Now the namespace should have been found or not
       
	if(foundRpc == 0){
                Print_Level l = getPrintLevel();
                setPrintLevelErrorPrint();
		println_E("##ERROR Rpc not found!");
                setPrintLevel(l);
		ERR(Packet,0,0);
		return;
	}else if(foundRpc > 0 ){
                //println_I("Namespace found: ");print_I(getNamespaceAtIndex(currentNamespaceIndexForPacket)->namespaceString);
		if(foundRpc > 1){
			if(Packet->use.head.MessageID == 0){
				//RPC overlap but no resolution defined
				println_E("##ERROR Rpc not resolved! Multiple implementations");
				printPacket(Packet,ERROR_PRINT);
				ERR(Packet,0,1);
				return;
			}else{
				//RPC resolution is specified
				currentNamespaceIndexForPacket = Packet->use.head.MessageID;
				//println_I("Rpc resolved to: ");print_I(getNamespaceAtIndex(currentNamespaceIndexForPacket)->namespaceString);
			}
		}

		RPC_LIST * rpc = getRpcByID(getNamespaceAtIndex(currentNamespaceIndexForPacket),
						Packet->use.head.RPC,
						Packet->use.head.Method );

		if(rpc !=NULL){
                     //println_I("RPC found: ");print_I(rpc->rpc);
                     rpc->callback(Packet);
		}
		Packet->use.head.MessageID = currentNamespaceIndexForPacket;
		Packet->use.head.ResponseFlag=1;
		FixPacket(Packet);
		lastPacketTime = getMs();
	}


}//finish processing the Packet


