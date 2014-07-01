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
    // The call backs for processing the packet have been called 
    // and the Packet struct now contains the data 
    // to be sent back to the client as a response. 
  }
  
  int i=0;
  for(i=0;i< GetPacketLegnth(&Packet);i++){
    //Grab the response packet one byte at a time and push it out the physical layer
    writeToPyhsicalLayerImp(Packet.stream[i]);
  }
  
```

To add new RPC's to the stack, we create a linked list element called a Namespace. 

For a detailed example of different types of functionality take a look at the PID namespace:

https://github.com/NeuronRobotics/c-bowler/blob/master/firmware/library/NR-Clib/development/BowlerStack/src/PID/Namespace_bcs_pid.c

To add a standard namespace, such as PID, to the stack, call addNamespaceToList:

```C

  addNamespaceToList((NAMESPACE_LIST * ) getBcsPidNamespace());
```

This new namespace and its call backs will be added to the Process_Self_Packet function call. 

To make a custom namespace, first you define the NAMESPACE_LIST linked list struct element. This also requires a call back function for asynchronus packets.


```C
BOOL pidAsyncEventCallbackLocal(BowlerPacket *Packet,BOOL (*pidAsyncCallbackPtr)(BowlerPacket *Packet)){

  //Run any cooperative tasks
  //Pack and send any packets, more then one is ok
}

static NAMESPACE_LIST bcsPid ={	"bcs.pid.*;1.0;;",// The string defining the namespace
                                NULL,// the first element in the RPC list
                                &pidAsyncEventCallbackLocal,// async for this namespace
                                NULL// no initial elements to the other namesapce field.
};

```

Next you define an RPC struct element. This element includes the callback for processing the specified RPC packet. 

```C


static RPC_LIST bcsPid__PID={	BOWLER_GET,
                                "_pid",
                                &processPIDGet,
                                ((const char [2]){	BOWLER_I08,//channel
                                                    0}),// Response arguments
                                BOWLER_POST,// response method
                                ((const char [3]){	BOWLER_I08,//channel
                                BOWLER_I32,//current position
                                 0}),// Response arguments
                                NULL //Termination
};

```

Finally you add the new RPC's to the namespace and the new namespace to the processing stack:

```C
  addRpcToNamespace(&bcsPid,& bcsPid__PID);
  addNamespaceToList(&bcsPid);

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
