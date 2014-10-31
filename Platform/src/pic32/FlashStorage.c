/*
 * FlashStorage.c
 *
 *  Created on: Jul 15, 2010
 *      Author: hephaestus
 */
#include "arch/pic32/BowlerConfig.h"
#include "Bowler/Bowler.h"
//#include <stddef.h>
  FlashStorageStruct flash;

  uint32_t * externalStream=0;
  uint32_t streamSize=0;
  uint32_t * stream;

  uint8_t  defMac[]  ={0x74,0xf7,0x26,0x00,0x00,0x00} ;

  uint32_t MEMORY_BASE =DefaultStartStorePhysical;
  uint32_t VirtualBase = DefaultStartStorePhysical+VirtualAddress;

  boolean disableFlash =true;

void enableFlashStorage(boolean enabled){
	disableFlash = enabled?false:true;
}


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
    externalStream=s;
    println_W("Setting external flash data ");p_int_W(size);
}

void FlashLoad(void){
    if(disableFlash == true)
    	return;

    
    if (FLASHSTORE % 4) {
        println_E("BAD FLASH size = ");
        p_int_W(FLASHSTORE % 4);
        while (1);
    }
    int i;
    stream = (uint32_t *) &flash;
    for (i=0;i<FLASHSTORE/4;i++){
            stream[i]=*((uint32_t *)(VirtualBase +(i*4)));
    }
    if(externalStream != 0 && streamSize!=0){
		for (i=0;i<streamSize;i++){
				externalStream[i]=*((uint32_t *)(VirtualBase +((i+FLASHSTORE)*4)));
		}
    }else{
           println_E("External storage not availible!"); 
    }
}

void FlashSync(void){
	if(disableFlash == true)
	    	return;
	//return;
	uint32_t i;
        uint32_t data=0, read=0,addr=0;

	println_I("Erasing Storage page");
        if(disableFlash==false)
            NVMErasePage( (uint32_t *) MEMORY_BASE);
	println_I("Writing new data Storage page");
        stream = (uint32_t *) &flash;
	for (i=0;i<FLASHSTORE/4;i++){
            if(disableFlash==false)
		NVMWriteWord((uint32_t *)(VirtualBase +(i*4)), stream[i]);
	}
	if(externalStream != 0 && streamSize!=0){
            for (i=0;i<streamSize;i++){
                    data = externalStream[i];
                    addr = (VirtualBase +((i+FLASHSTORE)*4));
                    if(disableFlash==false){
                        NVMWriteWord((uint32_t *)(addr), data );
                        read=*((uint32_t *)(addr));
                        if(data != read){
                            println_E("Data write failed! ");prHEX32(read,ERROR_PRINT);
                            print_E(" expected ");prHEX32(data,ERROR_PRINT);
                            print_E(" at ");prHEX32(addr,ERROR_PRINT);
                        }
                    }
            }
	}else{
          println_E("External storage not availible!");
        }
	println_I("Storage synced");
}

uint8_t FlashSetMac(uint8_t * mac){
	FlashLoad();
	if(flash.lock==LOCKBYTE){
		return false; 
	}
	int i;
	for (i=0;i<6;i++){
		flash.mac[i]=mac[i];
	}
	flash.lock=LOCKBYTE;
	FlashSync();
	return true; 
}

void FlashSetName(char * name){
	FlashLoad();
	int i;
	for (i=0;i<17;i++){
		flash.name[i]=name[i];
	}
	FlashSync();
}

void FlashGetMac(uint8_t * mac){
	int i;
	FlashLoad();
	if(flash.lock == 0xff){
		for (i=0;i<6;i++){
			mac[i]=defMac[i];
		}
		return;
	}
	for (i=0;i<6;i++){
		mac[i]=flash.mac[i];
	}
}

void FlashGetName(char * name){
	FlashLoad();
	int i;
	for (i=0;i<17;i++){
		name[i]=flash.name[i];
	}
}

void FlashGetBlRev(uint8_t * mac){
	int i;
	FlashLoad();
	for (i=0;i<3;i++){
		mac[i]=flash.bl[i];
	}
}
void FlashGetFwRev(uint8_t * mac){
	int i;
	FlashLoad();
	for (i=0;i<3;i++){
		mac[i]=flash.fw[i];
	}
}

uint8_t FlashSetFwRev(uint8_t * mac){
	//println("Loading fw from flash");
	FlashLoad();
	//println("Loading fw  new data");
	int i;
	boolean sync=false; 
	for (i=0;i<3;i++){
		if(flash.fw[i]!=mac[i]){
			flash.fw[i]=mac[i];
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
		if(flash.bl[i]!=mac[i]){
			flash.bl[i]=mac[i];
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
