/*
 * FlashStorage.c
 *
 *  Created on: Jul 15, 2010
 *      Author: hephaestus
 */
#include "arch/pic32/BowlerConfig.h"
#include "Bowler/Bowler.h"
FLASH_STORE flash;

uint32_t * stream;
uint32_t streamSize=0;

uint8_t  defMac[]  ={0x74,0xf7,0x26,0x00,0x00,0x00} ;

uint32_t MEMORY_BASE =DefaultStartStorePhysical;
uint32_t VirtualBase = DefaultStartStorePhysical+VirtualAddress;

void FlashSwitchMemoryToBootloader(){
    VirtualBase = BootloaderStartStorePhysical+VirtualAddress;
    MEMORY_BASE = BootloaderStartStorePhysical;
}




void SetFlashData(uint32_t * s,uint32_t size){
    if(((size*4)+FLASHSTORE) > FLASH_PAGE_SIZE ){
        println_E("This data page is too big for the flash page!");
        SoftReset();
    }
    streamSize=size;
    stream=s;
    
}

void FlashLoad(void){
	int i;
	for (i=0;i<FLASHSTORE;i++){
		flash.stream[i]=*((uint32_t *)(VirtualBase +(i*4)));
	}
        for (i=FLASHSTORE;i<FLASHSTORE+streamSize;i++){
		stream[i-FLASHSTORE]=*((uint32_t *)(VirtualBase +(i*4)));
	}
}

void FlashSync(void){
	uint32_t i;
        uint32_t data=0, read=0,addr=0;

	println_I("Erasing Storage page");
	NVMErasePage( (uint32_t *) MEMORY_BASE);
	println_I("Writing new data Storage page");
	for (i=0;i<FLASHSTORE;i++){
		NVMWriteWord((uint32_t *)(VirtualBase +(i*4)), flash.stream[i]);
	}
        for (i=FLASHSTORE;i<FLASHSTORE+streamSize;i++){
                data = stream[i-FLASHSTORE];
                addr = (VirtualBase +(i*4));
                NVMWriteWord((uint32_t *)(addr), data );
                read=*((uint32_t *)(addr));
                if(data != read){
                    println_E("Data write failed! ");prHEX32(read,ERROR_PRINT);
                    print_E(" expected ");prHEX32(data,ERROR_PRINT);
                    print_E(" at ");prHEX32(addr,ERROR_PRINT);
                }
        }
	println_I("Storage synced");
}

uint8_t FlashSetMac(uint8_t * mac){
	FlashLoad();
	if(flash.data.lock==LOCKBYTE){
		return false; 
	}
	int i;
	for (i=0;i<6;i++){
		flash.data.mac[i]=mac[i];
	}
	flash.data.lock=LOCKBYTE;
	FlashSync();
	return true; 
}

void FlashSetName(char * name){
	FlashLoad();
	int i;
	for (i=0;i<17;i++){
		flash.data.name[i]=name[i];
	}
	FlashSync();
}

void FlashGetMac(uint8_t * mac){
	int i;
	FlashLoad();
	if(flash.data.lock == 0xff){
		for (i=0;i<6;i++){
			mac[i]=defMac[i];
		}
		return;
	}
	for (i=0;i<6;i++){
		mac[i]=flash.data.mac[i];
	}
}

void FlashGetName(char * name){
	FlashLoad();
	int i;
	for (i=0;i<17;i++){
		name[i]=flash.data.name[i];
	}
}

void FlashGetBlRev(uint8_t * mac){
	int i;
	FlashLoad();
	for (i=0;i<3;i++){
		mac[i]=flash.data.bl[i];
	}
}
void FlashGetFwRev(uint8_t * mac){
	int i;
	FlashLoad();
	for (i=0;i<3;i++){
		mac[i]=flash.data.fw[i];
	}
}

uint8_t FlashSetFwRev(uint8_t * mac){
	//println("Loading fw from flash");
	FlashLoad();
	//println("Loading fw  new data");
	int i;
	boolean sync=false; 
	for (i=0;i<3;i++){
		if(flash.data.fw[i]!=mac[i]){
			flash.data.fw[i]=mac[i];
			sync=true; 
		}
	}
	if(sync){
		//println("Syncing fw ");
		FlashSync();
		//println("Syncing fw  done");
	}
	return true; 
}

uint8_t FlashSetBlRev(uint8_t * mac){
	//println("Loading bl flash page");
	FlashLoad();
	//println("Loading bl  new data");
	int i;
	boolean sync=false; 
	for (i=0;i<3;i++){
		if(flash.data.bl[i]!=mac[i]){
			flash.data.bl[i]=mac[i];
			sync=true; 
		}
	}
	if(sync){
		//println("Syncing bl ");
		FlashSync();
		//println("Syncing bl  done");
	}
	return true; 
}
