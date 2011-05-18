package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.dyio.DyIOChannel;

import com.neuronrobotics.sdk.dyio.peripherals.DigitalInputChannel;

// The purpose of this class is for calibrating 
public class Calibrator {
	
	// limit switches
	// hardware/counter controlled
	// X direction zero indication limit switch
	LimitSwitch X_limitZero =null;// 1
	// Y direction zero indication limit switch
	LimitSwitch  Y_limitZero =null;// 0
	// Z direction zero indication limit switch
	LimitSwitch  Z_limitZero =null; // port 2
	
	// software controlled limits for maxes
	// X direction maximum indication limit switch
	LimitSwitch X_limitMax = null;
	// Y direction maximum indication limit switch
	LimitSwitch Y_limitMax = null;
	// Z direction maximum indication limit switch
	LimitSwitch Z_limitMax = null;
	
	
	// constructor
	public Calibrator(){
		// sets the x, y , z axes to their zero positions
		Printer3d.X_stepper.setValue(-15000);
		Printer3d.Y_stepper.setValue(-15000);
		Printer3d.Z_stepper.setValue(-15000);
	}



}
