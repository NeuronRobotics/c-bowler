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

#ifndef DEBUG_H_
#define DEBUG_H_

typedef enum _Print_Level{
	NO_PRINT=0,
	ERROR_PRINT=1,
	WARN_PRINT=2,
	INFO_PRINT=3
}Print_Level;

#define setPrintLevelNoPrint() setPrintLevel(NO_PRINT)
#define setPrintLevelErrorPrint() setPrintLevel(ERROR_PRINT)
#define setPrintLevelWarningPrint() setPrintLevel(WARN_PRINT)
#define setPrintLevelInfoPrint() setPrintLevel(INFO_PRINT)

Print_Level setPrintLevel(Print_Level l);

Print_Level getPrintLevel();

void EnableDebugTerminal(void);
/**
 * enable printing (Defaults to enabled)
 */
void enableDebug(void);
/**
 * disable printing (Defaults to enabled)
 */
void disableDebug(void);
/**
 * print a given value as a 16 bit hex value
 */
#define prHEX(A,B) 		prHEX16(A,B)
#define prHEX8(A,B) 	printfDEBUG_BYTE(GetHighNib(A&0x000000ff),B);		printfDEBUG_BYTE(GetLowNib(A&0x000000ff),B)
#define prHEX16(A,B)  	printfDEBUG_BYTE(GetHighNib((A&0x0000ff00)>>8),B);	printfDEBUG_BYTE(GetLowNib((A&0x0000ff00)>>8),B); 	prHEX8(A,B);
#define prHEX24(A,B)  	printfDEBUG_BYTE(GetHighNib((A&0x00ff0000)>>16),B);	printfDEBUG_BYTE(GetLowNib((A&0x00ff0000)>>16),B);	prHEX16(A,B);
#define prHEX32(A,B)  	printfDEBUG_BYTE(GetHighNib((A&0xff000000)>>24),B);	printfDEBUG_BYTE(GetLowNib((A&0xff000000)>>24),B); 	prHEX24(A,B);

/**
 * print the ascii of a float. No new line
 */
#define p_fl(A,B) printfDEBUG_FL(A,B)

/**
 * print the ascii of an unsigned long/int. No new line
 */
#define p_ul(A,B)  p_sl(A,B)

/**
 * print the ascii of a signed long/int. No new line
 */
#define p_sl(A,B) printfDEBUG_SL(A,B)

/**
 * print the null terminated string with no new lines
 */
#define print(A,B) printfDEBUG_NNL(A,B)

/**
 * print the null terminated string with a newline inserted at the begining of the string
 */
#define println(A,B) printfDEBUG(A,B)

#define printStream(A,B,C) printByteArray(A,B,C);

/**
 * sends the charrector to the serial port if it is ascii, if it is not ascii, it is converted to a number then sent
 */
void printfDEBUG_BYTE(char b,Print_Level l);

/**
 * print the null terminated string with a newline inserted at the begining of the string
 */
void printfDEBUG(const char *str,Print_Level l);

/**
 * print the null terminated string with no new lines
 */
void printfDEBUG_NNL(const char *str,Print_Level l);

/**
 * print the ascii of an unsigned long/int. No new line
 */
void printfDEBUG_UL(unsigned long val,Print_Level l);

/**
 * print the ascii of a signed long/int. No new line
 */
void printfDEBUG_SL(long val,Print_Level l);

/**
 * convert a long into an ascii string and place the string into the Buffer
 */
void ultoaMINE(unsigned long Value, unsigned char* Buffer);
/**
 * print all the bytes in a byte array. The legnth of the array must be correct
 */
void printByteArray(unsigned char * stream,unsigned short int len,Print_Level l);
/**
 * convert a float into an ascii string and place the string into the outbuf
 */
void ftoa(float f, char * outbuf);
/**
 * print the ascii of a float. No new line
 */
void printfDEBUG_FL(float f,Print_Level l);

/**
 * return the char of the hex value of the low 4 bits of the given byte
 */
char GetLowNib(unsigned char b);
/**
 * return the char of the hex value of the high 4 bits of the given byte
 */
char GetHighNib(unsigned char b);


//Bowler Stack Specific:
#if defined(BOWLERSTRUCTDEF_H_)
	#define printPacket printBowlerPacketDEBUG
	void printBowlerPacketDEBUG(BowlerPacket * packet,Print_Level l);
	void printfDEBUG_BYTE(char b,Print_Level l);
#endif
#endif /* DEBUG_H_ */
