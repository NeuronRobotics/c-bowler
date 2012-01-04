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

void enableDebug(void){

}
void disableDebug(void){

}
const char AsciiHex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char GetLowNib(BYTE b){
	return AsciiHex[b&0x0f];
}
char GetHighNib(BYTE b){
	return AsciiHex[((b&0xF0)>>4)];
}

void printfDEBUG(const char *str){

}
void printfDEBUG_BYTE(char b){
}
void printfDEBUG_NNL(const char *str)
{

}
void printfDEBUG_UL(UINT32 val){

}
void printfDEBUG_SL(INT32 val){

}

void printBowlerPacketDEBUG(BowlerPacket * packet){

}
void printByteArray(BYTE * stream,UINT16 len){

}

void printfDEBUG_FL(float f){

}
//void ftoa(float f, char * outbuf){
//
//}

