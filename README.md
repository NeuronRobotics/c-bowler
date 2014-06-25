c-bowler
========

A library for controlling Bowler devices with C

#Theory Of Operation

The Bowler stack is based off of a pair of abstract data streams. These streams can be passed to the Bowler Server stack to be processed into packets. 

The streams are contained in the "BYTE_FIFO_STORAGE" struct. The FIFO is initialized by the stack by passing it a buffer and the size. 

```C

  static BYTE privateRXCom[comBuffSize];
  static BYTE_FIFO_STORAGE store;
  InitByteFifo(&store,privateRXCom,comBuffSize);
  
```

Once the FIFO for receiving packets is established, bytes can be added from whatever physical layer is needed:

```C

  	BYTE err;
  	BYTE b= getLatestByte();
  	FifoAddByte(&store, b, &err);
  	
```

The FiFO can be checked for a packet by calling a GetBowlerPacket(), then processed by the stack:

```C
  BowlerPacket Packet;
  if(GetBowlerPacket(&Packet,&store)){
    //Now the Packet struct contains the parsed packet data
    Process_Self_Packet(&Packet);
    // The call backs for processing the packet have been called and the Packet struct now contains the data 
    // to be sent back to the client as a response. 
  }
  
```



# Compilers

## Pic32 V 1.0.0
```bash
wget http://ww1.microchip.com/downloads/en/DeviceDoc/xc32-v1.00-linux.zip

unzip xc32-v1.00-linux.zip

sudo chmod +x xc32-v1.00-linux-installer.run

sudo ./xc32-v1.00-linux-installer.run
```

## AVR
```bash
sudo apt-get install -y --force-yes avr-libc gcc-avr git build-essential
```
