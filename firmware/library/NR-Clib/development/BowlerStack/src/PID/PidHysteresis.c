/*
 * PidHysteresis.c
 *
 *  Created on: Feb 14, 2014
 *      Author: hephaestus
 */
#include "Bowler/AbstractPID.h"
#include "Bowler/Debug.h"
#include "Bowler/Defines.h"

void incrementHistoresis(int group){
    getPidGroupDataTable()[group].calibration.upperHistoresis+=1;
    //calcCenter( group);
}
void decrementHistoresis(int group){
    getPidGroupDataTable()[group].calibration.lowerHistoresis-=1;
}


void calcCenter(int group){
    int diff = (getPidGroupDataTable()[group].calibration.upperHistoresis+getPidGroupDataTable()[group].calibration.lowerHistoresis)/2;
    getPidGroupDataTable()[group].calibration.stop = diff;
}

void checkCalibration(int group){
    if(getPidGroupDataTable()[group].calibration.calibrated != TRUE){
       getPidGroupDataTable()[group].calibration.upperHistoresis=0;
       getPidGroupDataTable()[group].calibration.lowerHistoresis=0;
       getPidGroupDataTable()[group].calibration.stop=0;
       getPidGroupDataTable()[group].calibration.calibrated = TRUE ;
    }
}

int getUpperPidHistoresis(int group){
    checkCalibration(group);
    return getPidGroupDataTable()[group].calibration.upperHistoresis;
}
int getLowerPidHistoresis(int group){
    checkCalibration(group);
    return getPidGroupDataTable()[group].calibration.lowerHistoresis;
}
int getPidStop(int group){
    checkCalibration(group);
    return getPidGroupDataTable()[group].calibration.stop;
}

void runPidHysterisisCalibration(int group){

//    println_E("\r\n\nStart calibration #");p_int_E(group);
    getPidGroupDataTable()[group].calibration.lowerHistoresis = 0;
    getPidGroupDataTable()[group].calibration.upperHistoresis = 0;
    getPidGroupDataTable()[group].calibration.stop = 0;
//    println_I("\tReset PID");
    pidReset(group,0);// Zero encoder reading
 //   println_I("\tDisable PID Output");
    SetPIDEnabled(group, TRUE);
    SetPIDCalibrateionState(group, CALIBRARTION_hysteresis);

    getPidGroupDataTable()[group].calibration.state =  backward;
  //  println_I("\tSetting slow move");
    setOutput(group, -1.0f);
    getPidGroupDataTable()[group].calibration.timer.setPoint=1000;
    getPidGroupDataTable()[group].calibration.timer.MsTime=getMs();

}

CAL_STATE pidHysterisis(int group){

    if(RunEvery(&getPidGroupDataTable()[group].calibration.timer)>0){

        float boundVal = 6.0;
        float extr=GetPIDPosition(group);
        if(getPidGroupDataTable()[group].calibration.state == forward){
            incrementHistoresis( group );
        }else if (getPidGroupDataTable()[group].calibration.state == backward){
            decrementHistoresis( group );
        }
        if( bound(0, extr, boundVal, boundVal)){// check to see if the encoder has moved
            //we have not moved
  //          println_I("NOT moved ");p_fl_I(extr);
            int historesisBound = 50;
            if( getPidGroupDataTable()[group].calibration.lowerHistoresis<-historesisBound &&
                getPidGroupDataTable()[group].calibration.state == backward){
   //             println_E("Backward Motor seems damaged, more then counts of historesis ");
                getPidGroupDataTable()[group].calibration.state = forward;
            }
            if(     getPidGroupDataTable()[group].calibration.upperHistoresis>historesisBound &&
                    getPidGroupDataTable()[group].calibration.state == forward){
   //             println_E("Forward Motor seems damaged, more then counts of historesis ");
                getPidGroupDataTable()[group].calibration.state = done;
            }
        }else{
            pidReset(group,0);
            setOutput(group, 0);
 //           println_E("Moved ");p_fl_E(extr);
            if(getPidGroupDataTable()[group].calibration.state == backward){
  //              println_I("Backward Calibrated for link# ");p_int_I(group);
                getPidGroupDataTable()[group].calibration.state = forward;
            }else{
   //             println_I("Calibration done for link# ");p_int_I(group);
                getPidGroupDataTable()[group].calibration.state = done;
                SetPIDCalibrateionState(group, CALIBRARTION_DONE);
                //calcCenter( group);
            }

        }
        if(getPidGroupDataTable()[group].calibration.state == forward){
            setOutput(group, 1.0f);
        }else if (getPidGroupDataTable()[group].calibration.state == backward){
            setOutput(group, -1.0f);
        }
    }

    return getPidGroupDataTable()[group].calibration.state;
}
