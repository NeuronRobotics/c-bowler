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

#define __WASP2_C

const MAC_ADDR Broadcast={{{0,0,0,0,0,0}}};
MAC_ADDR MyMAC ={{{0x74,0xf7,0x26,0x01,0x01,0x01}}} ;
static BYTE i;
const unsigned char coreName[] = "bcs.core.*;0.3;;";

void Bowler_Init(void){
	startScheduler();
	Bowler_HAL_Init();
	AddNamespace(sizeof(coreName),coreName);
}

BYTE Bowler_Server(BowlerPacket * Packet, BOOL debug){
	//StartCritical();
	BOOL back = GetBowlerPacket_arch(Packet);
	//EndCritical();
	if (back){
		SetColor(0,1,0);
		//if(debug){
			if(Packet->use.head.RPC != 0x7277705f){//Ignore Power Packet
				println("Got:",INFO_PRINT);printPacket(Packet,INFO_PRINT);
			}
		//}
		if ( (CheckAddress(MyMAC.v,Packet->use.head.MAC.v) == TRUE) || ((CheckAddress((BYTE *)Broadcast.v,(BYTE *)Packet->use.head.MAC.v) == TRUE) )) {
			Process_Self_Packet(Packet);
			for (i=0;i<6;i++){
				Packet->use.head.MAC.v[i]=MyMAC.v[i];
			}
			SetCRC(Packet);
			PutBowlerPacket(Packet);
			if(debug){
				if(Packet->use.head.RPC != 0x7277705f){//Ignore Power Packet
					println("Response:",INFO_PRINT);printPacket(Packet,INFO_PRINT);
				}
			}
		}else{
			//if(debug){
				println("Packet not addressed to me: ",ERROR_PRINT);
				printByteArray(Packet->use.head.MAC.v,6,ERROR_PRINT);
				print(" is not mine: ",ERROR_PRINT);
				printByteArray(MyMAC.v,6,ERROR_PRINT);
			//}
		}
		SetColor(0,0,1);
		return TRUE;
	}//Have a packet

	return FALSE;
}







