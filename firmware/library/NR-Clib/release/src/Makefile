# Generated Makefile
#@ Author Kevin Harrington
SHELL := /bin/bash -e

INCLUDES	    = -I../include/
-include ../toolchain/pic32/toolchain.mk

CCP_pic32MX440F128H    		= $(GCCP) -mprocessor=32MX440F128H $(INCLUDES) -I../include/arch/pic32/  -c
CCP_pic32MX460F512L   		= $(GCCP) -mprocessor=32MX460F512L $(INCLUDES) -I../include/arch/pic32/  -c
CCP_pic32MX795F512L   		= $(GCCP) -mprocessor=32MX795F512L $(INCLUDES) -I../include/arch/pic32/  -c


CPU_SPD       	= 18432000UL
GCCA            = avr-gcc -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -DF_CPU=$(CPU_SPD) -MMD -MP $(INCLUDES) -I/usr/lib/avr/include/  -c
CCA_avratmega324p  = $(GCCA) -mmcu=atmega324p 
CCA_avratmega644p  = $(GCCA) -mmcu=atmega644p

CCNAT    		= gcc -Os -g3 -Wall -c -fmessage-length=0 -MMD -MP $(INCLUDES) -c

LIBDIR  		= ../lib
RLIB     		= libRBELib.a
ARA      		= |grep "o"|grep "."|xargs avr-ar rcs
ARPIC      		= |grep "o"|grep "."|xargs $(ARP) rcs

all: COMPILE
	echo "Done!"
clean:DIRS
	echo Cleaned!
DIRS:
	rm -rf Release
	mkdir Release
	mkdir Release/pic32MX440F128H
	mkdir Release/pic32MX460F512L
	mkdir Release/pic32MX795F512L
	mkdir Release/avratmega324p
	mkdir Release/avratmega644p
	mkdir Release/native
	cd BowlerStack
COMPILE: DIRS pic32MX440F128H AVR324p AVR644p  pic32MX460F512L pic32MX795F512L  Native
	echo OK
	
pic32MX440F128H:
	
	$(CCP_pic32MX440F128H) BowlerStack/FIFO.c 						-o Release/pic32MX440F128H/FIFO.o
	$(CCP_pic32MX440F128H) BowlerStack/Bowler_Helper.c 				-o Release/pic32MX440F128H/Bowler_Helper.o
	$(CCP_pic32MX440F128H) BowlerStack/Bowler_Server.c 				-o Release/pic32MX440F128H/Bowler_Server.o
	$(CCP_pic32MX440F128H) BowlerStack/Bowler_RPC_Process.c 		-o Release/pic32MX440F128H/Bowler_RPC_Process.o
	$(CCP_pic32MX440F128H) BowlerStack/Scheduler.c 					-o Release/pic32MX440F128H/Scheduler.o	
	$(CCP_pic32MX440F128H) BowlerStack/Debug.c 						-o Release/pic32MX440F128H/Debug_en.o
	$(CCP_pic32MX440F128H) BowlerStack/Debug_stub.c 				-o Release/pic32MX440F128H/Debug.o
	$(CCP_pic32MX440F128H) BowlerStack/AbstractPID.c 				-o Release/pic32MX440F128H/AbstractPID.o
	$(CCP_pic32MX440F128H) BowlerStack/Bowler_Transport.c 			-o Release/pic32MX440F128H/Bowler_Transport.o
	$(CCP_pic32MX440F128H) BowlerStack/Bowler_Stack_Callback.c 		-o Release/pic32MX440F128H/Bowler_Stack_Callback.o
	
	
	$(CCP_pic32MX440F128H) arch/pic32/HAL_P32.c 					-o Release/pic32MX440F128H/HAL_P32.o
	$(CCP_pic32MX440F128H) arch/pic32/Bowler_HAL.c 					-o Release/pic32MX440F128H/Bowler_HAL.o
	$(CCP_pic32MX440F128H) arch/pic32/Bowler_UART_HAL.c 			-o Release/pic32MX440F128H/Bowler_UART_HAL.o
	$(CCP_pic32MX440F128H) arch/pic32/Bowler_USB_HAL.c 				-o Release/pic32MX440F128H/Bowler_USB_HAL.o
	$(CCP_pic32MX440F128H) arch/pic32/Delay.c 						-o Release/pic32MX440F128H/Delay.o
	$(CCP_pic32MX440F128H) arch/pic32/LED.c 						-o Release/pic32MX440F128H/LED.o
	$(CCP_pic32MX440F128H) arch/pic32/Tick.c 						-o Release/pic32MX440F128H/Tick.o
	$(CCP_pic32MX440F128H) arch/pic32/UART.c 						-o Release/pic32MX440F128H/UART.o
	$(CCP_pic32MX440F128H) arch/pic32/FlashStorage.c 				-o Release/pic32MX440F128H/FlashStorage.o
	
	$(CCP_pic32MX440F128H) arch/pic32/usb/usb_callback.c 			-o Release/pic32MX440F128H/usb_callback.o
	$(CCP_pic32MX440F128H) arch/pic32/usb/usb_descriptors.c 		-o Release/pic32MX440F128H/usb_descriptors.o
	$(CCP_pic32MX440F128H) arch/pic32/usb/usb_device.c 				-o Release/pic32MX440F128H/usb_device.o
	$(CCP_pic32MX440F128H) arch/pic32/usb/usb_fifo.c 				-o Release/pic32MX440F128H/usb_fifo.o
	$(CCP_pic32MX440F128H) arch/pic32/usb/usb_function_cdc.c 		-o Release/pic32MX440F128H/usb_function_cdc.o
	$(CCP_pic32MX440F128H) arch/pic32/usb/usb_interrupt.c 			-o Release/pic32MX440F128H/usb_interrupt.o
	
	find Release/pic32MX440F128H/|grep -v Debug_en.o$(ARPIC) 	$(LIBDIR)/PIC32/32MX440F128H/libNRLIB.a
	find Release/pic32MX440F128H/|grep -v Debug.o$(ARPIC) 	$(LIBDIR)/PIC32/32MX440F128H/libNRLIB_debug.a
	
	
pic32MX460F512L:
	$(CCP_pic32MX460F512L) BowlerStack/FIFO.c 						-o Release/pic32MX460F512L/FIFO.o
	$(CCP_pic32MX460F512L) BowlerStack/Bowler_Helper.c 				-o Release/pic32MX460F512L/Bowler_Helper.o
	$(CCP_pic32MX460F512L) BowlerStack/Bowler_Server.c 				-o Release/pic32MX460F512L/Bowler_Server.o
	$(CCP_pic32MX460F512L) BowlerStack/Bowler_RPC_Process.c 		-o Release/pic32MX460F512L/Bowler_RPC_Process.o
	$(CCP_pic32MX460F512L) BowlerStack/Scheduler.c 					-o Release/pic32MX460F512L/Scheduler.o	
	$(CCP_pic32MX460F512L) BowlerStack/Debug.c 						-o Release/pic32MX460F512L/Debug_en.o
	$(CCP_pic32MX460F512L) BowlerStack/Debug_stub.c 				-o Release/pic32MX460F512L/Debug.o
	$(CCP_pic32MX460F512L) BowlerStack/AbstractPID.c 				-o Release/pic32MX460F512L/AbstractPID.o
	$(CCP_pic32MX460F512L) BowlerStack/Bowler_Transport.c 			-o Release/pic32MX460F512L/Bowler_Transport.o
	$(CCP_pic32MX460F512L) BowlerStack/Bowler_Stack_Callback.c 		-o Release/pic32MX460F512L/Bowler_Stack_Callback.o
	
	$(CCP_pic32MX460F512L) arch/pic32/HAL_P32.c 					-o Release/pic32MX460F512L/HAL_P32.o
	$(CCP_pic32MX460F512L) arch/pic32/Bowler_HAL.c 					-o Release/pic32MX460F512L/Bowler_HAL.o
	$(CCP_pic32MX460F512L) arch/pic32/Bowler_UART_HAL.c 			-o Release/pic32MX460F512L/Bowler_UART_HAL.o
	$(CCP_pic32MX460F512L) arch/pic32/Bowler_USB_HAL.c 				-o Release/pic32MX460F512L/Bowler_USB_HAL.o
	$(CCP_pic32MX460F512L) arch/pic32/Delay.c 						-o Release/pic32MX460F512L/Delay.o
	$(CCP_pic32MX460F512L) arch/pic32/LED.c 						-o Release/pic32MX460F512L/LED.o
	$(CCP_pic32MX460F512L) arch/pic32/Tick.c 						-o Release/pic32MX460F512L/Tick.o
	$(CCP_pic32MX460F512L) arch/pic32/UART.c 						-o Release/pic32MX460F512L/UART.o
	$(CCP_pic32MX460F512L) arch/pic32/FlashStorage.c 				-o Release/pic32MX460F512L/FlashStorage.o
	
	$(CCP_pic32MX460F512L) arch/pic32/usb/usb_callback.c 			-o Release/pic32MX460F512L/usb_callback.o
	$(CCP_pic32MX460F512L) arch/pic32/usb/usb_descriptors.c 		-o Release/pic32MX460F512L/usb_descriptors.o
	$(CCP_pic32MX460F512L) arch/pic32/usb/usb_device.c 				-o Release/pic32MX460F512L/usb_device.o
	$(CCP_pic32MX460F512L) arch/pic32/usb/usb_fifo.c 				-o Release/pic32MX460F512L/usb_fifo.o
	$(CCP_pic32MX460F512L) arch/pic32/usb/usb_function_cdc.c 		-o Release/pic32MX460F512L/usb_function_cdc.o
	$(CCP_pic32MX460F512L) arch/pic32/usb/usb_interrupt.c 		-o Release/pic32MX460F512L/usb_interrupt.o
	
	find Release/pic32MX460F512L/|grep -v Debug_en.o$(ARPIC) 	$(LIBDIR)/PIC32/32MX460F512L/libNRLIB.a
	find Release/pic32MX460F512L/|grep -v Debug.o$(ARPIC) 	$(LIBDIR)/PIC32/32MX460F512L/libNRLIB_debug.a
	
	
pic32MX795F512L:
	$(CCP_pic32MX795F512L) BowlerStack/FIFO.c 						-o Release/pic32MX795F512L/FIFO.o
	$(CCP_pic32MX795F512L) BowlerStack/Bowler_Helper.c 				-o Release/pic32MX795F512L/Bowler_Helper.o
	$(CCP_pic32MX795F512L) BowlerStack/Bowler_Server.c 				-o Release/pic32MX795F512L/Bowler_Server.o
	$(CCP_pic32MX795F512L) BowlerStack/Bowler_RPC_Process.c 		-o Release/pic32MX795F512L/Bowler_RPC_Process.o
	$(CCP_pic32MX795F512L) BowlerStack/Scheduler.c 					-o Release/pic32MX795F512L/Scheduler.o	
	$(CCP_pic32MX795F512L) BowlerStack/Debug.c 						-o Release/pic32MX795F512L/Debug_en.o
	$(CCP_pic32MX795F512L) BowlerStack/Debug_stub.c 				-o Release/pic32MX795F512L/Debug.o
	$(CCP_pic32MX795F512L) BowlerStack/AbstractPID.c 				-o Release/pic32MX795F512L/AbstractPID.o
	$(CCP_pic32MX795F512L) BowlerStack/Bowler_Transport.c 			-o Release/pic32MX795F512L/Bowler_Transport.o
	$(CCP_pic32MX795F512L) BowlerStack/Bowler_Stack_Callback.c 		-o Release/pic32MX795F512L/Bowler_Stack_Callback.o
	
	$(CCP_pic32MX795F512L) arch/pic32/HAL_P32.c 					-o Release/pic32MX795F512L/HAL_P32.o
	$(CCP_pic32MX795F512L) arch/pic32/Bowler_HAL.c 					-o Release/pic32MX795F512L/Bowler_HAL.o
	$(CCP_pic32MX795F512L) arch/pic32/Bowler_UART_HAL.c 			-o Release/pic32MX795F512L/Bowler_UART_HAL.o
	$(CCP_pic32MX795F512L) arch/pic32/Bowler_USB_HAL.c 				-o Release/pic32MX795F512L/Bowler_USB_HAL.o
	$(CCP_pic32MX795F512L) arch/pic32/Delay.c 						-o Release/pic32MX795F512L/Delay.o
	$(CCP_pic32MX795F512L) arch/pic32/LED.c 						-o Release/pic32MX795F512L/LED.o
	$(CCP_pic32MX795F512L) arch/pic32/Tick.c 						-o Release/pic32MX795F512L/Tick.o
	$(CCP_pic32MX795F512L) arch/pic32/UART.c 						-o Release/pic32MX795F512L/UART.o
	$(CCP_pic32MX795F512L) arch/pic32/FlashStorage.c 				-o Release/pic32MX795F512L/FlashStorage.o
	
	$(CCP_pic32MX795F512L) arch/pic32/usb/usb_callback.c 			-o Release/pic32MX795F512L/usb_callback.o
	$(CCP_pic32MX795F512L) arch/pic32/usb/usb_descriptors.c 		-o Release/pic32MX795F512L/usb_descriptors.o
	$(CCP_pic32MX795F512L) arch/pic32/usb/usb_device.c 				-o Release/pic32MX795F512L/usb_device.o
	$(CCP_pic32MX795F512L) arch/pic32/usb/usb_fifo.c 				-o Release/pic32MX795F512L/usb_fifo.o
	$(CCP_pic32MX795F512L) arch/pic32/usb/usb_function_cdc.c 		-o Release/pic32MX795F512L/usb_function_cdc.o
	$(CCP_pic32MX795F512L) arch/pic32/usb/usb_interrupt.c 			-o Release/pic32MX795F512L/usb_interrupt.o
	
	find Release/pic32MX795F512L/|grep -v Debug_en.o$(ARPIC) 	$(LIBDIR)/PIC32/32MX795F512L/libNRLIB.a
	find Release/pic32MX795F512L/|grep -v Debug.o$(ARPIC) 	$(LIBDIR)/PIC32/32MX795F512L/libNRLIB_debug.a
	
AVR644p:
	$(CCA_avratmega644p) BowlerStack/FIFO.c 						-o Release/avratmega644p/FIFO.o
	$(CCA_avratmega644p) BowlerStack/Bowler_Helper.c 				-o Release/avratmega644p/Bowler_Helper.o
	$(CCA_avratmega644p) BowlerStack/Bowler_Server.c 				-o Release/avratmega644p/Bowler_Server.o
	$(CCA_avratmega644p) BowlerStack/Bowler_RPC_Process.c 			-o Release/avratmega644p/Bowler_RPC_Process.o
	$(CCA_avratmega644p) BowlerStack/Debug.c 						-o Release/avratmega644p/Debug_en.o
	$(CCA_avratmega644p) BowlerStack/Debug_stub.c 					-o Release/avratmega644p/Debug.o
	$(CCA_avratmega644p) BowlerStack/Scheduler.c 					-o Release/avratmega644p/Scheduler.o
	$(CCA_avratmega644p) BowlerStack/AbstractPID.c 					-o Release/avratmega644p/AbstractPID.o
	$(CCA_avratmega644p) BowlerStack/Bowler_Transport.c 			-o Release/avratmega644p/Bowler_Transport.o
	$(CCA_avratmega644p) BowlerStack/Bowler_Stack_Callback.c 		-o Release/avratmega644p/Bowler_Stack_Callback.o
	
	$(CCA_avratmega644p) arch/avr/HAL_xxx4p.c 						-o Release/avratmega644p/HAL_xxx4p.o	
	
	find Release/avratmega644p/|grep -v Debug_en.o$(ARA) 	$(LIBDIR)/AVR/atmega644p/libNRLIB.a
	find Release/avratmega644p/|grep -v Debug.o$(ARA) 		$(LIBDIR)/AVR/atmega644p/libNRLIB_debug.a
	
AVR324p:
	$(CCA_avratmega324p) BowlerStack/FIFO.c 						-o Release/avratmega324p/FIFO.o
	$(CCA_avratmega324p) BowlerStack/Bowler_Helper.c 				-o Release/avratmega324p/Bowler_Helper.o
	$(CCA_avratmega324p) BowlerStack/Bowler_Server.c 				-o Release/avratmega324p/Bowler_Server.o
	$(CCA_avratmega324p) BowlerStack/Bowler_RPC_Process.c 			-o Release/avratmega324p/Bowler_RPC_Process.o
	$(CCA_avratmega324p) BowlerStack/Debug.c 						-o Release/avratmega324p/Debug_en.o
	$(CCA_avratmega324p) BowlerStack/Debug_stub.c 					-o Release/avratmega324p/Debug.o
	$(CCA_avratmega324p) BowlerStack/Scheduler.c 					-o Release/avratmega324p/Scheduler.o
	$(CCA_avratmega324p) BowlerStack/AbstractPID.c 					-o Release/avratmega324p/AbstractPID.o
	$(CCA_avratmega324p) BowlerStack/Bowler_Transport.c 			-o Release/avratmega324p/Bowler_Transport.o
	$(CCA_avratmega324p) BowlerStack/Bowler_Stack_Callback.c 		-o Release/avratmega324p/Bowler_Stack_Callback.o
	
	$(CCA_avratmega324p) arch/avr/HAL_xxx4p.c 						-o Release/avratmega324p/HAL_xxx4p.o	
	
	find Release/avratmega324p/|grep -v Debug_en.o$(ARA) 	$(LIBDIR)/AVR/atmega324p/libNRLIB.a
	find Release/avratmega324p/|grep -v Debug.o$(ARA) 		$(LIBDIR)/AVR/atmega324p/libNRLIB_debug.a
	
Native:
	$(CCNAT) BowlerStack/FIFO.c 					-o Release/native/FIFO.o
	$(CCNAT) BowlerStack/Bowler_Helper.c 			-o Release/native/Bowler_Helper.o
	$(CCNAT) BowlerStack/Bowler_Server.c 			-o Release/native/Bowler_Server.o
	$(CCNAT) BowlerStack/Bowler_RPC_Process.c 		-o Release/native/Bowler_RPC_Process.o
	$(CCNAT) BowlerStack/Debug.c 					-o Release/native/Debug.o
	$(CCNAT) BowlerStack/Scheduler.c 				-o Release/native/Scheduler.o
	$(CCNAT) BowlerStack/AbstractPID.c 				-o Release/native/AbstractPID.o
	$(CCNAT) BowlerStack/Bowler_Transport.c 		-o Release/native/Bowler_Transport.o
	$(CCNAT) BowlerStack/Bowler_Stack_Callback.c 	-o Release/native/Bowler_Stack_Callback.o
	
	$(CCNAT) arch/native/Bowler_HAL_Native.c 			-o Release/native/Bowler_HAL_Native.o
	
	find Release/native/|grep ".o"|xargs ar rcs $(LIBDIR)/native/libNRLIB.a


