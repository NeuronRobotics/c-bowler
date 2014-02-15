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

static INT32_UNION PID_Temp;
static float time;

void GetConfigPDVelocity(BowlerPacket * Packet){
	BYTE chan = Packet->use.data[0];
	INT32_UNION PID_K;
	PID_K.Val=getPidVelocityDataTable()[chan].K.P*100;
	Packet->use.data[1]=PID_K.byte.FB;//=Packet->use.data[4];
	Packet->use.data[2]=PID_K.byte.TB;//Packet->use.data[5];
	Packet->use.data[3]=PID_K.byte.SB;//Packet->use.data[6];
	Packet->use.data[4]=PID_K.byte.LB;//Packet->use.data[7];

	PID_K.Val=getPidVelocityDataTable()[chan].K.D*100;
	Packet->use.data[5]=PID_K.byte.FB;//Packet->use.data[8];
	Packet->use.data[6]=PID_K.byte.TB;//Packet->use.data[9];
	Packet->use.data[7]=PID_K.byte.SB;//Packet->use.data[10];
	Packet->use.data[8]=PID_K.byte.LB;//Packet->use.data[11];


	Packet->use.head.DataLegnth=4+9;
	Packet->use.head.Method=BOWLER_POST;

}

BYTE ConfigPDVelovity(BowlerPacket * Packet){
	BYTE chan = Packet->use.data[0];

	float KP=0;
	float KD=0;
	INT32_UNION PID_K;

	PID_K.byte.FB=Packet->use.data[1];
	PID_K.byte.TB=Packet->use.data[2];
	PID_K.byte.SB=Packet->use.data[3];
	PID_K.byte.LB=Packet->use.data[4];
	KP=(float)PID_K.Val;

	PID_K.byte.FB=Packet->use.data[5];
	PID_K.byte.TB=Packet->use.data[6];
	PID_K.byte.SB=Packet->use.data[7];
	PID_K.byte.LB=Packet->use.data[8];
	KD=(float)PID_K.Val;



	getPidVelocityDataTable()[chan].K.P=KP/100;
	getPidVelocityDataTable()[chan].K.D=KD/100;


	OnPidConfigure(chan);

	return TRUE;
}


void GetConfigPID(BowlerPacket * Packet){
	BYTE chan = Packet->use.data[0];
	Packet->use.data[1]=getPidGroupDataTable()[chan].Enabled;//  = ((Packet->use.data[1]==0)?0:1);
	Packet->use.data[2]=getPidGroupDataTable()[chan].Polarity;// = ((Packet->use.data[2]==0)?0:1);
	Packet->use.data[3]=getPidGroupDataTable()[chan].Async;//= ((Packet->use.data[3]==0)?0:1);

	INT32_UNION PID_K;
	PID_K.Val=getPidGroupDataTable()[chan].K.P*100;
	Packet->use.data[4]=PID_K.byte.FB;//=Packet->use.data[4];
	Packet->use.data[5]=PID_K.byte.TB;//Packet->use.data[5];
	Packet->use.data[6]=PID_K.byte.SB;//Packet->use.data[6];
	Packet->use.data[7]=PID_K.byte.LB;//Packet->use.data[7];

	PID_K.Val=getPidGroupDataTable()[chan].K.I*100;
	Packet->use.data[8]=PID_K.byte.FB;//Packet->use.data[8];
	Packet->use.data[9]=PID_K.byte.TB;//Packet->use.data[9];
	Packet->use.data[10]=PID_K.byte.SB;//Packet->use.data[10];
	Packet->use.data[11]=PID_K.byte.LB;//Packet->use.data[11];

	PID_K.Val=getPidGroupDataTable()[chan].K.D*100;
	Packet->use.data[12]=PID_K.byte.FB;//Packet->use.data[12];
	Packet->use.data[13]=PID_K.byte.TB;//Packet->use.data[13];
	Packet->use.data[14]=PID_K.byte.SB;//Packet->use.data[14];
	Packet->use.data[15]=PID_K.byte.LB;//Packet->use.data[15];

	PID_K.Val=getPidGroupDataTable()[chan].IndexLatchValue;
	Packet->use.data[16]=PID_K.byte.FB;//Packet->use.data[12];
	Packet->use.data[17]=PID_K.byte.TB;//Packet->use.data[13];
	Packet->use.data[18]=PID_K.byte.SB;//Packet->use.data[14];
	Packet->use.data[19]=PID_K.byte.LB;//Packet->use.data[15]
	//latching data
	Packet->use.data[20]=getPidGroupDataTable()[chan].useIndexLatch;//
	Packet->use.data[21]=getPidGroupDataTable()[chan].stopOnIndex;//


	Packet->use.head.DataLegnth=4+22;
	Packet->use.head.Method=BOWLER_POST;

}

BYTE ConfigPID(BowlerPacket * Packet){
	BYTE chan = Packet->use.data[0];

	getPidGroupDataTable()[chan].Polarity = ((Packet->use.data[2]==0)?0:1);
	getPidGroupDataTable()[chan].Async    = ((Packet->use.data[3]==0)?0:1);


	float KP=0;
	float KI=0;
	float KD=0;
	INT32_UNION PID_K;

	PID_K.byte.FB=Packet->use.data[4];
	PID_K.byte.TB=Packet->use.data[5];
	PID_K.byte.SB=Packet->use.data[6];
	PID_K.byte.LB=Packet->use.data[7];
	KP=(float)PID_K.Val;

	PID_K.byte.FB=Packet->use.data[8];
	PID_K.byte.TB=Packet->use.data[9];
	PID_K.byte.SB=Packet->use.data[10];
	PID_K.byte.LB=Packet->use.data[11];
	KI=(float)PID_K.Val;

	PID_K.byte.FB=Packet->use.data[12];
	PID_K.byte.TB=Packet->use.data[13];
	PID_K.byte.SB=Packet->use.data[14];
	PID_K.byte.LB=Packet->use.data[15];
	KD=(float)PID_K.Val;

	if(Packet->use.head.DataLegnth>(4+16)){
		PID_K.byte.FB=Packet->use.data[16];
		PID_K.byte.TB=Packet->use.data[17];
		PID_K.byte.SB=Packet->use.data[18];
		PID_K.byte.LB=Packet->use.data[19];

		getPidGroupDataTable()[chan].useIndexLatch= Packet->use.data[20];
		getPidGroupDataTable()[chan].stopOnIndex = Packet->use.data[21];
	}else{
		PID_K.Val=0;
		getPidGroupDataTable()[chan].useIndexLatch= TRUE;
		getPidGroupDataTable()[chan].stopOnIndex = TRUE;
	}
	getPidGroupDataTable()[chan].IndexLatchValue=(float)PID_K.Val;


	getPidGroupDataTable()[chan].K.P=KP/100;
	getPidGroupDataTable()[chan].K.I=KI/100;
	getPidGroupDataTable()[chan].K.D=KD/100;
	//println("Resetting PID channel from Config:",INFO_PRINT);printBowlerPacketDEBUG(Packet,INFO_PRINT);
	//println("From Config Current setpoint:",INFO_PRINT);p_fl(getPidGroupDataTable()[chan].SetPoint,INFO_PRINT);
	//pidReset(chan, getPidGroupDataTable()[chan].SetPoint);

	//getPidGroupDataTable()[chan].Enabled=TRUE;//Ensures output enabled to stop motors
	//getPidGroupDataTable()[chan].Output=0;
	//setOutput(chan,getPidGroupDataTable()[chan].Output);


	//getPidVelocityDataTable()[chan].enabled=FALSE;

	OnPidConfigure(chan);

	getPidGroupDataTable()[chan].Enabled  = ((Packet->use.data[1]==0)?0:1);
//	if(local_groups[chan].Enabled)
//		fail();
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
			PID_Temp.Val=GetPIDPosition(i);
			Packet->use.data[1+(i*4)]=PID_Temp.byte.FB;
			Packet->use.data[2+(i*4)]=PID_Temp.byte.TB;
			Packet->use.data[3+(i*4)]=PID_Temp.byte.SB;
			Packet->use.data[4+(i*4)]=PID_Temp.byte.LB;
			Packet->use.head.DataLegnth+=4;
		}
		Packet->use.head.Method=BOWLER_POST;
		break;
	case _PID:
		PID_Temp.Val=GetPIDPosition(Packet->use.data[0]);
		Packet->use.data[1]=PID_Temp.byte.FB;
		Packet->use.data[2]=PID_Temp.byte.TB;
		Packet->use.data[3]=PID_Temp.byte.SB;
		Packet->use.data[4]=PID_Temp.byte.LB;
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
		PID_Temp.Val=getNumberOfPidChannels();
		Packet->use.data[0]=PID_Temp.byte.FB;
		Packet->use.data[1]=PID_Temp.byte.TB;
		Packet->use.data[2]=PID_Temp.byte.SB;
		Packet->use.data[3]=PID_Temp.byte.LB;
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
	switch (Packet->use.head.RPC){
	case APID:
		PID_Temp.byte.FB=Packet->use.data[0];
		PID_Temp.byte.TB=Packet->use.data[1];
		PID_Temp.byte.SB=Packet->use.data[2];
		PID_Temp.byte.LB=Packet->use.data[3];
		time = (float)PID_Temp.Val;
		BYTE i=0;
		for(i=0;i<Packet->use.data[4];i++){
			PID_Temp.byte.FB=Packet->use.data[5+(i*4)];
			PID_Temp.byte.TB=Packet->use.data[6+(i*4)];
			PID_Temp.byte.SB=Packet->use.data[7+(i*4)];
			PID_Temp.byte.LB=Packet->use.data[8+(i*4)];
			val=PID_Temp.Val;
			SetPIDTimed(i,val,time);
		}
		READY(Packet,zone,3);
		break;
	case _VPD:
		chan = Packet->use.data[0];
		PID_Temp.byte.FB=Packet->use.data[1];
		PID_Temp.byte.TB=Packet->use.data[2];
		PID_Temp.byte.SB=Packet->use.data[3];
		PID_Temp.byte.LB=Packet->use.data[4];
		val = PID_Temp.Val;
		PID_Temp.byte.FB=Packet->use.data[5];
		PID_Temp.byte.TB=Packet->use.data[6];
		PID_Temp.byte.SB=Packet->use.data[7];
		PID_Temp.byte.LB=Packet->use.data[8];
		time=PID_Temp.Val;

		StartPDVel(chan,val,time);

		READY(Packet,zone,4);
		break;
	case _PID:
		chan = Packet->use.data[0];
		PID_Temp.byte.FB=Packet->use.data[1];
		PID_Temp.byte.TB=Packet->use.data[2];
		PID_Temp.byte.SB=Packet->use.data[3];
		PID_Temp.byte.LB=Packet->use.data[4];
		val = PID_Temp.Val;
		PID_Temp.byte.FB=Packet->use.data[5];
		PID_Temp.byte.TB=Packet->use.data[6];
		PID_Temp.byte.SB=Packet->use.data[7];
		PID_Temp.byte.LB=Packet->use.data[8];
		time=PID_Temp.Val;
		SetPIDTimed(chan,val,time);
		READY(Packet,zone,5);
		break;
	case RPID:
		chan = Packet->use.data[0];
		PID_Temp.byte.FB=Packet->use.data[1];
		PID_Temp.byte.TB=Packet->use.data[2];
		PID_Temp.byte.SB=Packet->use.data[3];
		PID_Temp.byte.LB=Packet->use.data[4];
		//println("Resetting PID channel from packet:",INFO_PRINT);printBowlerPacketDEBUG(Packet,INFO_PRINT);
		pidReset(chan, PID_Temp.Val);
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
			getPidGroupDataTable()[i].Enabled = TRUE;
			setOutput(i,0.0);
			getPidGroupDataTable()[i].Enabled = FALSE;
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


