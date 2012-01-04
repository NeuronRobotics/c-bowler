/**
 * @file FIFO.c
 *
 * Created on: May 27, 2010
 * @author hephaestus
 */

#include "Bowler/Bowler.h"
void clearByteFifo(BYTE_FIFO_STORAGE * fifo);
static UINT32 i;
//const char * fifoinit = "Initializing FIFO, Buffer Size: ";
//const char * error = "@##ERROR fifo is overflown! Size, buffer: ";

void printFiFoState(BYTE_FIFO_STORAGE * fifo, BYTE * buffer){
	int i;
	print("\nFifo state: \n\tBytes:");p_ul(FifoGetByteCount(fifo));print("\n\tData [ ");
	FifoReadByteStream(	buffer,
						FifoGetByteCount(fifo)+1,
						fifo);
	for(i=0;i<FifoGetByteCount(fifo);i++){
		p_ul(buffer[i]);print(" ");
	}
	print(" ]\n");
}

BOOL lockFifo(BYTE_FIFO_STORAGE * fifo){
//	if(fifo->mutex != FALSE){
//		return FALSE;
//	}
//	fifo->mutex=TRUE;
	StartCritical();
	return TRUE;
}
BOOL unLockFifo(BYTE_FIFO_STORAGE * fifo){
	//fifo->mutex=FALSE;
	EndCritical();
	return TRUE;
}
void InitByteFifo(BYTE_FIFO_STORAGE * fifo,BYTE * buff,UINT32 size){
	if(fifo == 0 ||  buff == 0){
		println("@#@#FIFO FAILED TO INITIALIZE");p_ul(size);
	}
	fifo->buffer=buff;
	fifo->bufferSize=size;
	fifo->readPointer=0;
	fifo->writePointer=0;
	//fifo->byteCount=0;
	fifo->mutex=FALSE;
	clearByteFifo(fifo);
	//println(fifoinit);p_ul(size);
}
void clearByteFifo(BYTE_FIFO_STORAGE * fifo){
	//StartCritical();
	for (i=0;i < fifo->bufferSize;i++){
		fifo->buffer[i]=0;
	}
	//EndCritical();
}

BYTE ReadFirstByte(BYTE_FIFO_STORAGE * fifo){
	return fifo->buffer[fifo->readPointer];
}

UINT32 calcByteCount(BYTE_FIFO_STORAGE * fifo){
	int w =fifo->writePointer;
	int r = fifo->readPointer;
	if(w==r){
		fifo->byteCount= 0;
	}
	if(w>r){
		fifo->byteCount= w-r;
	}else if(w==r){
		fifo->byteCount= 0;
	}else{
		fifo->byteCount= (w+fifo->bufferSize)-r;
	}
	return fifo->byteCount;
}

UINT32 FifoGetByteCount(BYTE_FIFO_STORAGE * fifo){
	return fifo->byteCount;

}

UINT32 FifoAddByte(BYTE_FIFO_STORAGE * fifo,BYTE b, BYTE * errorCode){
	if(FifoGetByteCount(fifo) >= (fifo->bufferSize-2)){
		//println(error);p_ul(fifo->bufferSize);print(",");p_ul(fifo->byteCount);
		errorCode[0]=FIFO_OVERFLOW;
		return 0;
	}
	if(lockFifo(fifo)==FALSE){
		errorCode[0]=FIFO_FAILED_TO_SET;
		//return 0;
	}
	fifo->buffer[fifo->writePointer]=b;
	fifo->writePointer++;
	if (fifo->writePointer == fifo->bufferSize){
		fifo->writePointer=0;
	}
	//fifo->byteCount++;
	calcByteCount(fifo);
	errorCode[0]=FIFO_OK;
	unLockFifo(fifo);
	return (FifoGetByteCount(fifo)<=fifo->bufferSize);
}

BYTE getByte(BYTE_FIFO_STORAGE * fifo, BYTE * errorCode){
	if(lockFifo(fifo)==FALSE){
		errorCode[0]=FIFO_FAILED_TO_GET;
		return 0;
	}
	BYTE b = fifo->buffer[fifo->readPointer];
	fifo->readPointer++;
	if (fifo->readPointer==fifo->bufferSize){
		fifo->readPointer=0;
	}
	errorCode[0]=FIFO_OK;
	calcByteCount(fifo);
	unLockFifo(fifo);
	return b;
}

UINT32 FifoGetByteStream(BYTE_FIFO_STORAGE * fifo,BYTE *packet,UINT32 size){
	BYTE err;
	int count = FifoGetByteCount(fifo);
	for (i=0;i<size;i++){
		if(count > 0){
			BYTE b;
			do{
				b = getByte(fifo,&err);
			}while(err != FIFO_OK);
			packet[i] = b;
		}else{
			packet[i]=0;
		}
	}
	return i;
}

UINT32 FifoReadByteStream(BYTE *packet,UINT32 size,BYTE_FIFO_STORAGE * fifo){
	UINT32 read=fifo->readPointer;
	int count = FifoGetByteCount(fifo);
	for (i=0;i<size;i++){
		if(count > 0){
			packet[i] = fifo->buffer[read];
			read++;
			if (read==fifo->bufferSize){
				read=0;
			}
		}else{
			packet[i]=0;
		}
	}
	return i;
}

//void clearUINT32Fifo(UINT32_FIFO_STORAGE * fifo);
//
//void InitUINT32Fifo(UINT32_FIFO_STORAGE * fifo,UINT32 * buff,UINT32 size){
//	fifo->buffer=buff;
//	fifo->bufferSize=size;
//	fifo->readPointer=0;
//	fifo->writePointer=0;
//	fifo->count=0;
//	clearUINT32Fifo(fifo);
//}
//void clearUINT32Fifo(UINT32_FIFO_STORAGE * fifo){
//	//StartCritical();
//	for (i=0;i < fifo->bufferSize;i++){
//		fifo->buffer[i]=0;
//	}
//	//EndCritical();
//}
//
//UINT32 FifoAddUINT32(UINT32_FIFO_STORAGE * fifo,UINT32 b){
//	fifo->buffer[fifo->writePointer]=b;
//	StartCritical();
//	fifo->writePointer++;
//	if (fifo->writePointer == fifo->bufferSize){
//		fifo->writePointer=0;
//	}
//	fifo->count++;
//	EndCritical();
//	return (fifo->count<=fifo->bufferSize);
//}
//
//UINT32 FifoGetUINT32Stream(UINT32_FIFO_STORAGE * fifo,UINT32 *packet,UINT32 size){
//
//	for (i=0;i<size;i++){
//		if(fifo->count>0){
//
//			packet[i] = fifo->buffer[fifo->readPointer];
//			StartCritical();
//			fifo->readPointer++;
//			if (fifo->readPointer==fifo->bufferSize){
//				fifo->readPointer=0;
//			}
//			//StartCritical();
//			fifo->count--;
//			EndCritical();
//			//EndCritical();
//		}else{
//			packet[i]=0x0;
//		}
//	}
//
//	return i;
//}
