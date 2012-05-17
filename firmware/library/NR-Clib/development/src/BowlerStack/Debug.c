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

Print_Level level=NO_PRINT;

//static BOOL DebugFlag=TRUE;
static BOOL DebugINIT = FALSE;

Print_Level getPrintLevel(){
	return level;
}

Print_Level setPrintLevel(Print_Level l){
#if defined(NO_PRINTING)
	level=NO_PRINT;
#else
	level=l;
#endif
	return getPrintLevel();
}

BOOL okToPrint(Print_Level l){
	if(getPrintLevel()>=l){
		if(DebugINIT == FALSE){
			DebugINIT = TRUE;
			EnableDebugTerminal();
		}
		return TRUE;
	}
	return FALSE;
}
//
//void enableDebug(void){
//	setPrintLevel(INFO_PRINT);
//}
//void disableDebug(void){
//	setPrintLevel(NO_PRINT);
//}

const char AsciiHex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

char GetLowNib(BYTE b){
	return AsciiHex[b&0x0f];
}
char GetHighNib(BYTE b){
	return AsciiHex[((b&0xF0)>>4)];
}

void printfDEBUG(const char *str,Print_Level l){
	if(!okToPrint(l)){
		return;
	}

	int x;
	x=0;
	putCharDebug('\n');
	putCharDebug('\r');
	while(str[x]!='\0'){
		putCharDebug(str[x++]);
	}

}
void printfDEBUG_BYTE(char b,Print_Level l){
	if(!okToPrint(l)){
		return;
	}
	 putCharDebug(b);

}
void printfDEBUG_NNL(const char *str,Print_Level l)
{
	if(!okToPrint(l)){
		return;
	}
	int x;
	x=0;
	while(str[x]!='\0'){
		 putCharDebug(str[x++]);
	}


}
void printfDEBUG_UL(UINT32 val,Print_Level l){
	printfDEBUG_SL(val,l);
}

void printfDEBUG_SL(INT32 val,Print_Level l){
	if(!okToPrint(l)){
		return;
	}
	BYTE byteStr[11];
	int x=0;
	if (val<0){
		val *=-1;
		putCharDebug('-');
	}
	ultoaMINE(val,byteStr);
	// putCharDebug(',');
	while(byteStr[x] != '\0'){
		putCharDebug(byteStr[x++]);
	}

}

void printfDEBUG_FL(float f,Print_Level l){
	if(!okToPrint(l)){
		return;
	}
	INT32 upper = (INT32)f;
	INT32 shift =(INT32)(f*1000);
	INT32 clip  = upper*1000;
	printfDEBUG_SL(upper,l);
	putCharDebug('.');
	INT32 dec =shift-clip;
	if (dec<0)
		dec*=-1;
	if(dec<100)
		putCharDebug('0');
	if(dec<10)
		putCharDebug('0');
	printfDEBUG_UL(dec,l);
}

#if defined(BOWLERSTRUCTDEF_H_)


static const char  packet[] = "\tPacket = \t";
static const char get[] = "Get";
static const char post[]="Post ";
static const char stat[]= "Status";
static const char crit[]="Critical";
static const char unknown[] = "Unknown";
static const char sync[] = "(Synchronous)";
static const char async[] = "(Asynchronous)";
static const char ver[] ="\tVersion = \t";
static const char mac[] = "\tMAC = \t\t";
static const char meth[] = "\tMethod = \t";
static const char id[] = "\tAsync ID = \t";
static const char dataSise[] ="\tData Size = \t";
static const char crcval []= "\tCRC Value = \t";
static const char dval[] = "\tData = \t\t";
static const char rpc []="\tRPC code = \t";
static const char nodata[] = " no data";

void printBowlerPacketDEBUG(BowlerPacket * Packet,Print_Level l){
    
	if(!okToPrint(l)){
		return;
	}
		int i;
		BYTE s;
		printfDEBUG(packet,l);
		s = BowlerHeaderSize+Packet->stream[DataSizeIndex];
		printfDEBUG_BYTE('[',l);
		for (i=0;i<s;i++){
			printfDEBUG_UL(Packet->stream[i],l);
			if (i<s-1)
				printfDEBUG_BYTE(',',l);
		}
		printfDEBUG_BYTE(']',l);
		printfDEBUG(ver,l);
		printfDEBUG_UL(Packet->stream[0],l);
		printfDEBUG(mac,l);
		for (i=0;i<6;i++){
			printfDEBUG_UL(Packet->stream[1+i],l);
			if (i<5)
				printfDEBUG_BYTE(':',l);
		}
		printfDEBUG(meth,l);
		switch (Packet->stream[MethodIndex]){
		case BOWLER_STATUS:
			printfDEBUG_NNL(stat,l);
			break;
		case BOWLER_GET:
			printfDEBUG_NNL(get,l);
			break;
		case BOWLER_POST:
			printfDEBUG_NNL(post,l);
			break;
		case BOWLER_CRIT:
			printfDEBUG_NNL(crit,l);
			break;
		default:
			printfDEBUG(unknown,l);
			printfDEBUG_UL(Packet->stream[MethodIndex],l);
		}
		printfDEBUG(id,l);
		printfDEBUG_UL((Packet->stream[SessionIDIndex]&0x7f),l);
		if(!(Packet->stream[SessionIDIndex]&0x7f)){
			printfDEBUG_NNL(sync,l);
		}else{
			printfDEBUG_NNL(async,l);
		}
		printfDEBUG(dataSise,l);
		printfDEBUG_UL((Packet->stream[DataSizeIndex]),l);
		printfDEBUG(crcval,l);
		printfDEBUG_UL((Packet->stream[CRCIndex]),l);
		if(Packet->use.head.DataLegnth>=4){
			printfDEBUG(rpc,l);
			for (i=0;i<4;i++){
				printfDEBUG_BYTE(Packet->stream[RPCCodeStart+i],l);
			}
		}
		if(Packet->use.head.DataLegnth>4){
			s= (Packet->use.head.DataLegnth-4);
			printfDEBUG(dval,l);
			for (i=0;i<s;i++){
				printfDEBUG_UL(Packet->use.data[i],l);
				if (i<(s-1))
					printfDEBUG_BYTE(',',l);
			}
		}else{
			printfDEBUG(nodata,l);
		}
		printfDEBUG("\n",l);
}
#endif
static const char streamsize[] = " Stream: size=";
void printByteArray(BYTE * stream,UINT16 len,Print_Level l){
	if(!okToPrint(l)){
		return;
	}
	UINT16 i;
	printfDEBUG_NNL(streamsize,l);
	printfDEBUG_UL(len,l);
	printfDEBUG_NNL(" [",l);
	for (i=0;i<len;i++){
		printfDEBUG_UL(stream[i],l);
		if (i<(len-1))
			printfDEBUG_BYTE(',',l);
	}
	printfDEBUG_NNL("]",l);
}

void ultoaMINE(UINT32 Value, BYTE* Buffer)
{
	BYTE i;
	UINT32 Digit;
	UINT32 Divisor;
	BOOL Printed = FALSE;

	if(Value)
	{
		for(i = 0, Divisor = 1000000000; i < 10; i++)
		{
			Digit = Value/Divisor;
			if(Digit || Printed)
			{
				*Buffer++ = '0' + Digit;
				Value -= Digit*Divisor;
				Printed = TRUE;
			}
			Divisor /= 10;
		}
	}
	else
	{
		*Buffer++ = '0';
	}

	*Buffer = 0;
}
