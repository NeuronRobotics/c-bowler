package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.dyio.DyIOChannel;

import com.neuronrobotics.sdk.dyio.peripherals.DigitalInputChannel;

// The purpose of this class is for calibrating 
public class Calibrator {
	
	// limit switches
	// X direction zero indication limit switch
	DyIOChannel X_limitZero =null;// 1
	// Y direction zero indication limit switch
	DyIOChannel  Y_limitZero =null;// 0
	// Z direction zero indication limit switch
	DyIOChannel  Z_limitZero =null; // port 2
	// X direction maximum indication limit switch
	DigitalInputChannel X_limitMax = null;
	// Y direction maximum indication limit switch
	DigitalInputChannel Y_limitMax = null;
	// Z direction maximum indication limit switch
	
	
	// constructor
	public Calibrator(){
		
	}



}
