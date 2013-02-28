# Generated Makefile
#@ Author Kevin Harrington
SHELL := /bin/bash -e

-include ../toolchain/pic32/toolchain.mk
-include ../toolchain/avr/toolchain.mk

CCP_pic32MX440F128H    		= $(GCCP) -mprocessor=32MX440F128H  
CCP_pic32MX460F512L   		= $(GCCP) -mprocessor=32MX460F512L 
CCP_pic32MX795F512L   		= $(GCCP) -mprocessor=32MX795F512L


CPU_SPD       	= 18432000UL
GCCA_full       = $(GCCA) -DF_CPU=$(CPU_SPD) -I/usr/lib/avr/include/  
CCA_avratmega324p  = $(GCCA_full) -mmcu=atmega324p 
CCA_avratmega644p  = $(GCCA_full) -mmcu=atmega644p

CCNAT    		= gcc -Os -g3 -Wall -c -fmessage-length=0 -MMD -MP

LIBDIR  		= ../lib


AVR_CFILES :=$(wildcard src/avr/*.c) $(wildcard ../BowlerStack/src/*.c)
AVR_OFILES := $(AVR_CFILES:%.c=%.o)

PIC_CFILES :=$(wildcard src/pic32/*.c) $(wildcard src/pic32/usb/*.c) $(wildcard ../BowlerStack/src/*.c)
PIC_OFILES := $(PIC_CFILES:%.c=%.o)

NATIVE_CFILES :=$(wildcard src/native/*.c) $(wildcard ../BowlerStack/src/*.c)
NATIVE_OFILES := $(NATIVE_CFILES:%.c=%.o)

all: COMPILE
	echo "Done!"
clean:
	echo Cleaned!

COMPILE: Native pic32MX440F128H AVR324p AVR644p  pic32MX460F512L pic32MX795F512L  
	echo OK
	
Native:
	make -C src/native/  CC_Bowler_Arch="$(CCNAT)" 
	make -C ../BowlerStack/ CC_Bowler_Arch="$(CCNAT) -DNO_PRINTING -I../Platform/include/" 
	$(AR) rcs  $(LIBDIR)/native/linux/libNRLIB.a $(NATIVE_OFILES)
	make -C ../BowlerStack/ CC_Bowler_Arch="$(CCNAT) -I../Platform/include/" 
	$(AR) rcs  $(LIBDIR)/native/linux/libNRLIB_debug.a $(NATIVE_OFILES)

AVR644p:
	make -C src/avr/  CC_Bowler_Arch="$(CCA_avratmega644p)" 
	make -C ../BowlerStack/ CC_Bowler_Arch="$(CCA_avratmega644p) -DNO_PRINTING -I../Platform/include/" 
	$(ARAVR) rcs  $(LIBDIR)/AVR/atmega644p/libNRLIB.a $(AVR_OFILES)
	make -C ../BowlerStack/ CC_Bowler_Arch="$(CCA_avratmega644p) -I../Platform/include/" 
	$(ARAVR) rcs  $(LIBDIR)/AVR/atmega644p/libNRLIB_debug.a $(AVR_OFILES)

AVR324p:
	make -C src/avr/  CC_Bowler_Arch="$(CCA_avratmega324p)" 
	make -C ../BowlerStack/ CC_Bowler_Arch="$(CCA_avratmega324p) -DNO_PRINTING -I../Platform/include/" 
	$(ARAVR) rcs  $(LIBDIR)/AVR/atmega324p/libNRLIB.a $(AVR_OFILES)
	make -C ../BowlerStack/ CC_Bowler_Arch="$(CCA_avratmega324p) -I../Platform/include/" 
	$(ARAVR) rcs  $(LIBDIR)/AVR/atmega324p/libNRLIB_debug.a $(AVR_OFILES)
	
pic32MX440F128H:
	make -C src/pic32/  CC_Bowler_Arch="$(CCP_pic32MX440F128H)" 
	make -C ../BowlerStack/ CC_Bowler_Arch="$(CCP_pic32MX440F128H) -DNO_PRINTING -I../Platform/include/" 
	$(ARP) rcs  $(LIBDIR)/PIC32/32MX440F128H/libNRLIB.a $(PIC_OFILES)
	make -C ../BowlerStack/ CC_Bowler_Arch="$(CCP_pic32MX440F128H) -I../Platform/include/" 
	$(ARP) rcs  $(LIBDIR)/PIC32/32MX440F128H/libNRLIB_debug.a $(PIC_OFILES)

pic32MX460F512L:
	make -C src/pic32/  CC_Bowler_Arch="$(CCP_pic32MX460F512L)" 
	make -C ../BowlerStack/ CC_Bowler_Arch="$(CCP_pic32MX460F512L) -DNO_PRINTING -I../Platform/include/" 
	$(ARP) rcs  $(LIBDIR)/PIC32/32MX460F512L/libNRLIB.a $(PIC_OFILES)
	make -C ../BowlerStack/ CC_Bowler_Arch="$(CCP_pic32MX460F512L) -I../Platform/include/" 
	$(ARP) rcs  $(LIBDIR)/PIC32/32MX460F512L/libNRLIB_debug.a $(PIC_OFILES)
	
	
pic32MX795F512L:
	make -C src/pic32/  CC_Bowler_Arch="$(CCP_pic32MX795F512L)" 
	make -C ../BowlerStack/ CC_Bowler_Arch="$(CCP_pic32MX795F512L) -DNO_PRINTING -I../Platform/include/" 
	$(ARP) rcs  $(LIBDIR)/PIC32/32MX795F512L/libNRLIB.a $(PIC_OFILES)
	make -C ../BowlerStack/ CC_Bowler_Arch="$(CCP_pic32MX795F512L) -I../Platform/include/" 
	$(ARP) rcs  $(LIBDIR)/PIC32/32MX795F512L/libNRLIB_debug.a $(PIC_OFILES)
	
	
	
#	$(CCP_pic32MX795F512L) BowlerStack/FIFO.c 						-o Release/pic32MX795F512L/FIFO.o
#	$(CCP_pic32MX795F512L) BowlerStack/Bowler_Helper.c 				-o Release/pic32MX795F512L/Bowler_Helper.o
#	$(CCP_pic32MX795F512L) BowlerStack/Bowler_Server.c 				-o Release/pic32MX795F512L/Bowler_Server.o
#	$(CCP_pic32MX795F512L) BowlerStack/Bowler_RPC_Process.c 		-o Release/pic32MX795F512L/Bowler_RPC_Process.o
#	$(CCP_pic32MX795F512L) BowlerStack/Scheduler.c 					-o Release/pic32MX795F512L/Scheduler.o	
#	$(CCP_pic32MX795F512L) BowlerStack/Debug.c 						-o Release/pic32MX795F512L/Debug_en.o
#	$(CCP_pic32MX795F512L) BowlerStack/Debug.c -D NO_PRINTING				-o Release/pic32MX795F512L/Debug.o
#	$(CCP_pic32MX795F512L) BowlerStack/AbstractPID.c 				-o Release/pic32MX795F512L/AbstractPID.o
#	$(CCP_pic32MX795F512L) BowlerStack/Bowler_Transport.c 			-o Release/pic32MX795F512L/Bowler_Transport.o
#	$(CCP_pic32MX795F512L) BowlerStack/Bowler_Stack_Callback.c 		-o Release/pic32MX795F512L/Bowler_Stack_Callback.o
#	
#	$(CCP_pic32MX795F512L) arch/pic32/HAL_P32.c 					-o Release/pic32MX795F512L/HAL_P32.o
#	$(CCP_pic32MX795F512L) arch/pic32/Bowler_HAL.c 					-o Release/pic32MX795F512L/Bowler_HAL.o
#	$(CCP_pic32MX795F512L) arch/pic32/Bowler_UART_HAL.c 			-o Release/pic32MX795F512L/Bowler_UART_HAL.o
#	$(CCP_pic32MX795F512L) arch/pic32/Bowler_USB_HAL.c 				-o Release/pic32MX795F512L/Bowler_USB_HAL.o
#	$(CCP_pic32MX795F512L) arch/pic32/Delay.c 						-o Release/pic32MX795F512L/Delay.o
#	$(CCP_pic32MX795F512L) arch/pic32/LED.c 						-o Release/pic32MX795F512L/LED.o
#	$(CCP_pic32MX795F512L) arch/pic32/Tick.c 						-o Release/pic32MX795F512L/Tick.o
#	$(CCP_pic32MX795F512L) arch/pic32/UART.c 						-o Release/pic32MX795F512L/UART.o
#	$(CCP_pic32MX795F512L) arch/pic32/FlashStorage.c 				-o Release/pic32MX795F512L/FlashStorage.o
#	$(CCP_pic32MX795F512L) arch/pic32/ADC.c 						-o Release/pic32MX795F512L/ADC.o
#	
#	$(CCP_pic32MX795F512L) arch/pic32/usb/usb_callback.c 			-o Release/pic32MX795F512L/usb_callback.o
#	$(CCP_pic32MX795F512L) arch/pic32/usb/usb_descriptors.c 		-o Release/pic32MX795F512L/usb_descriptors.o
#	$(CCP_pic32MX795F512L) arch/pic32/usb/usb_device.c 				-o Release/pic32MX795F512L/usb_device.o
#	$(CCP_pic32MX795F512L) arch/pic32/usb/usb_fifo.c 				-o Release/pic32MX795F512L/usb_fifo.o
#	$(CCP_pic32MX795F512L) arch/pic32/usb/usb_function_cdc.c 		-o Release/pic32MX795F512L/usb_function_cdc.o
#	$(CCP_pic32MX795F512L) arch/pic32/usb/usb_interrupt.c 			-o Release/pic32MX795F512L/usb_interrupt.o
#	
#	find Release/pic32MX795F512L/|grep -v Debug_en.o$(ARPIC) 	$(LIBDIR)/PIC32/32MX795F512L/libNRLIB.a
#	find Release/pic32MX795F512L/|grep -v Debug.o$(ARPIC) 	$(LIBDIR)/PIC32/32MX795F512L/libNRLIB_debug.a
	
	

	



