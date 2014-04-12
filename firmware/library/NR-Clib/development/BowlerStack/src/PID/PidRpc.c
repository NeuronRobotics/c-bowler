/*
 * PidRpc.c
 *
 *  Created on: Feb 14, 2014
 *      Author: hephaestus
 */


/**
 * @file AbstractPID.c
 *
 * Created on: Apr 9, 2010
 * @author Kevin Harrington
 */


#include "Bowler/AbstractPID.h"
#include "Bowler/Debug.h"
#include "Bowler/Defines.h"

//static INT32_UNION PID_Temp;
//static float time;


void GetConfigPDVelocity(BowlerPacket * Packet){
//   2700	      4	      8	   2712	    a98	output/o/PidRpc.o

	set32bit(Packet,getPidVelocityDataTable()[Packet->use.data[0]].K.P*100, 1);
	set32bit(Packet,getPidVelocityDataTable()[Packet->use.data[0]].K.D*100, 5);

	Packet->use.head.DataLegnth=4+9;
	Packet->use.head.Method=BOWLER_POST;

}

BYTE ConfigPDVelovity(BowlerPacket * Packet){
	BYTE chan = Packet->use.data[0];

	float KP=0;
	float KD=0;

	KP=(float)get32bit(Packet,1);
	KD=(float)get32bit(Packet,5);

	getPidVelocityDataTable()[chan].K.P=KP/100.0;
	getPidVelocityDataTable()[chan].K.D=KD/100.0;

	OnPidConfigure(chan);
	return TRUE;
}


void GetConfigPID(BowlerPacket * Packet){
	BYTE chan = Packet->use.data[0];
	Packet->use.data[1]=getPidGroupDataTable()[chan].config.Enabled;//  = ((Packet->use.data[1]==0)?0:1);
	Packet->use.data[2]=getPidGroupDataTable()[chan].config.Polarity;// = ((Packet->use.data[2]==0)?0:1);
	Packet->use.data[3]=getPidGroupDataTable()[chan].config.Async;//= ((Packet->use.data[3]==0)?0:1);

	set32bit(Packet,getPidGroupDataTable()[chan].config.K.P*100,4);
	set32bit(Packet,getPidGroupDataTable()[chan].config.K.I*100,8);
	set32bit(Packet,getPidGroupDataTable()[chan].config.K.D*100,12);
	set32bit(Packet,getPidGroupDataTable()[chan].config.IndexLatchValue,16);

	//latching data
	Packet->use.data[20]=getPidGroupDataTable()[chan].config.useIndexLatch;//
	Packet->use.data[21]=getPidGroupDataTable()[chan].config.stopOnIndex;//


	Packet->use.head.DataLegnth=4+22;
	Packet->use.head.Method=BOWLER_POST;

}

BYTE ConfigPID(BowlerPacket * Packet){
	BYTE chan = Packet->use.data[0];

	getPidGroupDataTable()[chan].config.Polarity = ((Packet->use.data[2]==0)?0:1);
	getPidGroupDataTable()[chan].config.Async    = ((Packet->use.data[3]==0)?0:1);


	float KP=0;
	float KI=0;
	float KD=0;
	float temp=0;

	KP=(float)get32bit(Packet,4);
	KI=(float)get32bit(Packet,8);
	KD=(float)get32bit(Packet,12);

	if(Packet->use.head.DataLegnth>(4+16)){

		temp=(float)get32bit(Packet,16);

		getPidGroupDataTable()[chan].config.useIndexLatch= Packet->use.data[20];
		getPidGroupDataTable()[chan].config.stopOnIndex = Packet->use.data[21];
	}else{
		temp=0;
		getPidGroupDataTable()[chan].config.useIndexLatch= TRUE;
		getPidGroupDataTable()[chan].config.stopOnIndex = TRUE;
	}
	getPidGroupDataTable()[chan].config.IndexLatchValue=(float)temp;


	getPidGroupDataTable()[chan].config.K.P=KP/100;
	getPidGroupDataTable()[chan].config.K.I=KI/100;
	getPidGroupDataTable()[chan].config.K.D=KD/100;
	//println("Resetting PID channel from Config:",INFO_PRINT);printBowlerPacketDEBUG(Packet,INFO_PRINT);
	//println("From Config Current setpoint:",INFO_PRINT);p_fl(getPidGroupDataTable()[chan].SetPoint,INFO_PRINT);

	OnPidConfigure(chan);

	getPidGroupDataTable()[chan].config.Enabled  = ((Packet->use.data[1]==0)?0:1);

	return TRUE;
}


int zone = 66;
BOOL processPIDGet(BowlerPacket * Packet){
	int i;
	switch (Packet->use.head.RPC){
	case APID:
		Packet->use.head.DataLegnth=5;
		Packet->use.data[0]=getNumberOfPidChannels();
		for(i=0;i<getNumberOfPidChannels();i++){
			set32bit(Packet,GetPIDPosition(i),1+(i*4));
			Packet->use.head.DataLegnth+=4;
		}
		Packet->use.head.Method=BOWLER_POST;
		break;
	case _PID:
		set32bit(Packet,GetPIDPosition(Packet->use.data[0]),1);

		Packet->use.head.DataLegnth=4+1+4;
		Packet->use.head.Method=BOWLER_POST;
		break;
	case CPID:
		GetConfigPID(Packet);
		break;
	case CPDV:
		GetConfigPDVelocity(Packet);
		break;
	case GPDC:
		set32bit(Packet,getNumberOfPidChannels(),0);
		Packet->use.head.DataLegnth=4+4;
		Packet->use.head.Method=BOWLER_POST;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL processPIDPost(BowlerPacket * Packet){
    int chan, val;
    float time;
	switch (Packet->use.head.RPC){
	case APID:

		time = (float)get32bit(Packet,0);
		BYTE i=0;
		for(i=0;i<Packet->use.data[4];i++){

			SetPIDTimed(i,get32bit(Packet,5+(i*4)),time);
		}
		READY(Packet,zone,3);
		break;
	case _VPD:
		chan = Packet->use.data[0];
		val = get32bit(Packet,1);
		time=get32bit(Packet,5);
		StartPDVel(chan,val,time);

		READY(Packet,zone,4);
		break;
	case _PID:
		chan = Packet->use.data[0];
		val = get32bit(Packet,1);
		time=get32bit(Packet,5);
		SetPIDTimed(chan,val,time);
		READY(Packet,zone,5);
		break;
	case RPID:
		chan = Packet->use.data[0];
		//println("Resetting PID channel from packet:",INFO_PRINT);printBowlerPacketDEBUG(Packet,INFO_PRINT);
		pidReset(chan, get32bit(Packet,1));
		READY(Packet,zone,6);
		break;
        default:
		return FALSE;
	}
	return TRUE;
}
BOOL processPIDCrit(BowlerPacket * Packet){
	BYTE i=0;
	switch (Packet->use.head.RPC){
	case KPID:
		for(i=0;i<getNumberOfPidChannels();i++){
			getPidGroupDataTable()[i].config.Enabled = TRUE;
			setOutput(i,0.0);
			getPidGroupDataTable()[i].config.Enabled = FALSE;
			getPidVelocityDataTable()[i].enabled=FALSE;
			getPidGroupDataTable()[i].Output=0.0;
		}
		READY(Packet,zone,0);
		break;
	case CPID:
		if(ConfigPID(Packet)){
			READY(Packet,zone,1);
		}else
			ERR(Packet,zone,1);
		break;
	case CPDV:
		if(ConfigPDVelovity(Packet)){
			READY(Packet,zone,1);
		}else
			ERR(Packet,zone,1);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
/**
 * Handle a PID packet.
 * @return True if the packet was processed, False if it was not  PID packet
 */
BOOL ProcessPIDPacket(BowlerPacket * Packet){
	switch(Packet->use.head.Method){
	case BOWLER_GET:
		return processPIDGet(Packet);
	case BOWLER_POST:
		return processPIDPost(Packet);
	case BOWLER_CRIT:
		return processPIDCrit(Packet);
	default:
		return FALSE;
	}
}


