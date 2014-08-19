/*
 * PidVelocity.c
 *
 *  Created on: Feb 14, 2014
 *      Author: hephaestus
 */
#include "Bowler/Bowler.h"

void RunVel(void){
	uint8_t i;
	for (i=0;i<getNumberOfPidChannels();i++){
		//println_I("Checking velocity on ");p_int_I(i);
		if(!getPidGroupDataTable(i)->config.Enabled){
			RunPDVel(i);
		}
	}
}

float runPdVelocityFromPointer(PD_VEL* vel, float currentState,float KP, float KD){

    		float currentTime = getMs();
		float timeMsDiff =  (currentTime -vel->lastTime);
		float timeDiff =  timeMsDiff/1000;
		float posDiff=currentState -vel->lastPosition;
		float currentVelocity = posDiff/timeDiff;
		//float velocityDiff = currentVelocity-vel->lastVelocity;
		float velocityDiff=0;
		float proportional =  currentVelocity-vel->unitsPerSeCond;
		float set = (proportional*KP)+(velocityDiff*KD)*timeMsDiff;
		vel->currentOutputVel-=(set);

		if (vel->currentOutputVel>200){
			vel->currentOutputVel=200;
                }else if(vel->currentOutputVel<-200){
			vel->currentOutputVel=-200;
                }

		println_I("\t Velocity: set=   ");p_fl_I(vel->unitsPerSeCond );print_I(" ticks/seCond" );
                println_I("\t current state=   ");p_fl_I(currentState );print_I(" ticks" );
                println_I("\t last state=      ");p_fl_I(vel->lastPosition );print_I(" ticks" );
		println_I("\t position diff=   ");p_fl_I(posDiff );print_I(" ticks" );
		println_I("\t MS diff=         ");p_fl_I(timeMsDiff );
		println_I("\t current=         ");p_fl_I(currentVelocity );print_I(" ticks/seCond" );
		println_I("\t Velocity offset= ");p_fl_I(set );
		println_I("\t Velocity set=    ");p_fl_I(vel->currentOutputVel );

		//cleanup
		vel->lastPosition=currentState;
		vel->lastVelocity=currentVelocity;
		vel->lastTime=currentTime;
                return vel->currentOutputVel;
}

void RunPDVel(uint8_t chan){
	//println_I("Running PID vel");
	if(getPidVelocityDataTable(chan)->enabled==true) {
         

		getPidGroupDataTable(chan)->Output=runPdVelocityFromPointer(getPidVelocityDataTable(chan),
                        getPidGroupDataTable(chan)->CurrentState,
                        getPidGroupDataTable(chan)->config.V.P,
                        getPidGroupDataTable(chan)->config.V.D
                        );

                if(GetPIDCalibrateionState(chan)<=CALIBRARTION_DONE)
                    setOutput(chan,getPidGroupDataTable(chan)->Output);
	}
}

void StartPDVel(uint8_t chan,int32_t unitsPerSeCond,float ms){

        if(ms<.1){
            //println_I("Starting Velocity");
            getPidVelocityDataTable(chan)->enabled=true; 
            getPidGroupDataTable(chan)->config.Enabled=false; 
            getPidVelocityDataTable(chan)->lastPosition=GetPIDPosition(chan);
            getPidVelocityDataTable(chan)->lastTime=getMs();
            getPidVelocityDataTable(chan)->unitsPerSeCond=unitsPerSeCond;
            getPidVelocityDataTable(chan)->currentOutputVel =0;
        }else{
            //println_I("Starting Velocity Timed");
            float seConds = ms/1000;
            int32_t dist = (INT32)unitsPerSeCond*(INT32)seConds;
            int32_t delt = ((INT32) (GetPIDPosition(chan))-dist);
            SetPIDTimed(chan, delt, ms);
        }


}


