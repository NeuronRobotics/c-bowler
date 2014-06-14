/*
 * PidHysteresis.c
 *
 *  Created on: Feb 14, 2014
 *      Author: hephaestus
 */
#include "Bowler/Bowler.h"

void incrementHistoresis(int group){
    getPidGroupDataTable()[group].config.upperHistoresis+=1;
    //calcCenter( group);
}
void decrementHistoresis(int group){
    getPidGroupDataTable()[group].config.lowerHistoresis-=1;
}


void calcCenter(int group){
    int diff = (getPidGroupDataTable()[group].config.upperHistoresis+getPidGroupDataTable()[group].config.lowerHistoresis)/2;
    getPidGroupDataTable()[group].config.stop = diff;
}

void checkCalibration(int group){
    if(getPidGroupDataTable()[group].calibration.calibrated != TRUE){
       getPidGroupDataTable()[group].config.upperHistoresis=0;
       getPidGroupDataTable()[group].config.lowerHistoresis=0;
       getPidGroupDataTable()[group].config.stop=0;
       getPidGroupDataTable()[group].calibration.calibrated = TRUE ;
    }
}

int getUpperPidHistoresis(int group){
    checkCalibration(group);
    return getPidGroupDataTable()[group].config.upperHistoresis;
}
int getLowerPidHistoresis(int group){
    checkCalibration(group);
    return getPidGroupDataTable()[group].config.lowerHistoresis;
}
int getPidStop(int group){
    checkCalibration(group);
    return getPidGroupDataTable()[group].config.stop;
}
BOOL processRunAutoCal(BowlerPacket * Packet){
    int group = Packet->use.data[0];

    runPidHysterisisCalibration(group);

    READY(Packet,0,0);
    return TRUE;
}

void runPidHysterisisCalibration(int group){

    if(!getPidGroupDataTable()[group].config.Enabled){
        println_E("Axis disabled for calibration #");p_int_E(group);
        getPidGroupDataTable()[group].config.Enabled=TRUE;
    }
    getPidGroupDataTable()[group].config.lowerHistoresis = 0;
    getPidGroupDataTable()[group].config.upperHistoresis = 0;
    getPidGroupDataTable()[group].config.stop = 0;
//    println_I("\tReset PID");
    pidReset(group,0);// Zero encoder reading
 //   println_I("\tDisable PID Output");
    SetPIDEnabled(group, TRUE);
    SetPIDCalibrateionState(group, CALIBRARTION_hysteresis);

    getPidGroupDataTable()[group].calibration.state =  backward;
  //  println_I("\tSetting slow move");
    setOutput(group, -1.0f);
    getPidGroupDataTable()[group].timer.setPoint=2000;
    getPidGroupDataTable()[group].timer.MsTime=getMs();

}

CAL_STATE pidHysterisis(int group){

    if(RunEvery(&getPidGroupDataTable()[group].timer)>0){
        Print_Level l = getPrintLevel();
        setPrintLevelInfoPrint();
        float boundVal = 15.0;
        float extr=GetPIDPosition(group);
        if( bound(0, extr, boundVal, boundVal)){// check to see if the encoder has moved
            //we have not moved
  //          println_I("NOT moved ");p_fl_I(extr);
            if(getPidGroupDataTable()[group].calibration.state == forward){
                incrementHistoresis( group );
            }else if (getPidGroupDataTable()[group].calibration.state == backward){
                decrementHistoresis( group );
            }
            int historesisBound = 25;
            if( getPidGroupDataTable()[group].config.lowerHistoresis<(-historesisBound) &&
                getPidGroupDataTable()[group].calibration.state == backward){
                println_E("Backward Motor seems damaged, more then counts of historesis #");p_int_I(group);
                getPidGroupDataTable()[group].calibration.state = forward;
            }
            if(     getPidGroupDataTable()[group].config.upperHistoresis>(historesisBound) &&
                    getPidGroupDataTable()[group].calibration.state == forward){
                println_E("Forward Motor seems damaged, more then counts of historesis #");p_int_I(group);
                getPidGroupDataTable()[group].calibration.state = done;
            }
        }else{
            pidReset(group,0);
            setOutput(group, 0);
            println_E("Moved ");p_fl_E(extr);
            if(getPidGroupDataTable()[group].calibration.state == backward){
                println_I("Backward Calibrated for link# ");p_int_I(group);
                getPidGroupDataTable()[group].calibration.state = forward;
            }else{
                println_I("Calibration done for link# ");p_int_I(group);
                getPidGroupDataTable()[group].calibration.state = done;
               
                getPidGroupDataTable()[group].config.lowerHistoresis *=.7;
                getPidGroupDataTable()[group].config.upperHistoresis *=.7;
                calcCenter( group);
            }

        }
        if(getPidGroupDataTable()[group].calibration.state == forward){
            setOutput(group, 1.0f);
        }else if (getPidGroupDataTable()[group].calibration.state == backward){
            setOutput(group, -1.0f);
        }
        setPrintLevel(l);
    }
    if(getPidGroupDataTable()[group].calibration.state == done)
        SetPIDCalibrateionState(group, CALIBRARTION_DONE);
    return getPidGroupDataTable()[group].calibration.state;
}


void startHomingLink(int group, PidCalibrationType type){
    float speed=20.0;
    if(type == CALIBRARTION_home_up)
       speed*=1.0;
    else if (type == CALIBRARTION_home_down)
        speed*=-1.0;
    else{
        println_E("Invalid homing type");
        return;
    }
    SetPIDCalibrateionState(group, type);
    setOutput(group, speed);
    getPidGroupDataTable()[group].timer.MsTime=getMs();
    getPidGroupDataTable()[group].timer.setPoint = 1000;
    getPidGroupDataTable()[group].homing.homingStallBound = 2;
    getPidGroupDataTable()[group].homing.previousValue = GetPIDPosition(group);
}

void checkLinkHomingStatus(int group){
    if(!(   GetPIDCalibrateionState(group)==CALIBRARTION_home_down ||
            GetPIDCalibrateionState(group)==CALIBRARTION_home_up)
            ){
        return;//Calibration is not running
    }
    if(RunEvery(&getPidGroupDataTable()[group].timer)>0){
            float boundVal = getPidGroupDataTable()[group].homing.homingStallBound;

            if( bound(  getPidGroupDataTable()[group].homing.previousValue,
                        GetPIDPosition(group),
                        boundVal,
                        boundVal
                    )
                ){
                pidReset(group,0);
                println_W("Homing Done for group ");p_int_W(group);
                SetPIDCalibrateionState(group, CALIBRARTION_DONE);
                OnPidConfigure(group);
            }else{

                getPidGroupDataTable()[group].homing.previousValue = GetPIDPosition(group);
            }
        }
}
