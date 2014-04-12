/*
 * PidVelocity.c
 *
 *  Created on: Feb 14, 2014
 *      Author: hephaestus
 */
#include "Bowler/AbstractPID.h"
#include "Bowler/Debug.h"
#include "Bowler/Defines.h"

void RunVel(void){
	BYTE i;
	for (i=0;i<getNumberOfPidChannels();i++){
		println_I("Checking velocity on ");p_int_I(i);
		if(!getPidGroupDataTable()[i].config.Enabled){
			RunPDVel(i);
		}
	}
}

void RunPDVel(BYTE chan){
	//println_I("Running PID vel");
	if(getPidVelocityDataTable()[chan].enabled==TRUE){
		getPidGroupDataTable()[chan].CurrentState = GetPIDPosition(chan);
		float currentTime = getMs();
		float timeMsDiff =  (currentTime -getPidVelocityDataTable()[chan].lastTime);
		float timeDiff =  timeMsDiff/1000;
		int posDiff=getPidGroupDataTable()[chan].CurrentState -getPidVelocityDataTable()[chan].lastPosition;
		float currentVelocity = posDiff/timeDiff;
		//float velocityDiff = currentVelocity-getPidVelocityDataTable()[chan].lastVelocity;
		float velocityDiff=0;
		float proportional =  currentVelocity-getPidVelocityDataTable()[chan].unitsPerSeCond;
		float set = (getPidVelocityDataTable()[chan].currentOutputVel+(proportional*getPidVelocityDataTable()[chan].K.P)+(velocityDiff*getPidVelocityDataTable()[chan].K.D))/-10;
		getPidVelocityDataTable()[chan].currentOutputVel+=set;

		if (getPidVelocityDataTable()[chan].currentOutputVel>100)
			getPidVelocityDataTable()[chan].currentOutputVel=100;
		if(getPidVelocityDataTable()[chan].currentOutputVel<-100)
			getPidVelocityDataTable()[chan].currentOutputVel=-100;

		println("Velocity set=",INFO_PRINT);p_fl(getPidVelocityDataTable()[chan].unitsPerSeCond,INFO_PRINT);print(" ticks/seCond",INFO_PRINT);
		println("Velocity position diff=",INFO_PRINT);p_int(posDiff,INFO_PRINT);print(" ticks",INFO_PRINT);
		println("Velocity time diff=",INFO_PRINT);p_fl(timeDiff,INFO_PRINT);print(" seConds",INFO_PRINT);
		println("Velocity time diff=",INFO_PRINT);p_fl(timeMsDiff,INFO_PRINT);print(" ms",INFO_PRINT);
		println("Velocity current=",INFO_PRINT);p_fl(currentVelocity,INFO_PRINT);print(" ticks/seCond",INFO_PRINT);
		println("Velocity offset=",INFO_PRINT);p_fl(set,INFO_PRINT);print("\n",INFO_PRINT);
		println("Velocity set=",INFO_PRINT);p_fl(getPidVelocityDataTable()[chan].currentOutputVel,INFO_PRINT);print("\n",INFO_PRINT);

		getPidGroupDataTable()[chan].Output=getPidVelocityDataTable()[chan].currentOutputVel;

                if(GetPIDCalibrateionState(chan)<=CALIBRARTION_DONE)
                    setOutput(chan,getPidGroupDataTable()[chan].Output);

		//cleanup
		getPidVelocityDataTable()[chan].lastPosition=getPidGroupDataTable()[chan].CurrentState;
		getPidVelocityDataTable()[chan].lastVelocity=currentVelocity;
		getPidVelocityDataTable()[chan].lastTime=currentTime;
	}
}

void StartPDVel(BYTE chan,INT32 unitsPerSeCond,float ms){

        if(ms<.1){
            //println_I("Starting Velocity");
            getPidVelocityDataTable()[chan].enabled=TRUE;
            getPidGroupDataTable()[chan].config.Enabled=FALSE;
            getPidVelocityDataTable()[chan].lastPosition=GetPIDPosition(chan);
            getPidVelocityDataTable()[chan].lastTime=getMs();
            getPidVelocityDataTable()[chan].unitsPerSeCond=unitsPerSeCond;
            getPidVelocityDataTable()[chan].currentOutputVel =0;
        }else{
            //println_I("Starting Velocity Timed");
            float seConds = ms/1000;
            INT32 dist = (INT32)unitsPerSeCond*(INT32)seConds;
            INT32 delt = ((INT32) (GetPIDPosition(chan))-dist);
            SetPIDTimed(chan, delt, ms);
        }


}


