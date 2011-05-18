package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.dyio.DyIO;
import com.neuronrobotics.sdk.dyio.peripherals.CounterOutputChannel;

public class Printer3d extends DyIO {
	// stepper motor control port/motor assigments
	// x direction stepper motor
	static CounterOutputChannel X_stepper=null;
	// y direction stepper motor
	static CounterOutputChannel Y_stepper=null;
	// z direction stepper motor
	static CounterOutputChannel Z_stepper=null;
	// printhead extrusion stepper
	static CounterOutputChannel EXT_stepper=null;
	
	// constructor
	private Printer3d(){
		// ports assigned to motors
		// assigns X_stepper to channel 21
		X_stepper=new CounterOutputChannel(getChannel(21));
		// assigns Y_stepper to channel 23
		Y_stepper=new CounterOutputChannel(getChannel(23));
		// assigns Z_stepper to channel 19
		Z_stepper=new CounterOutputChannel(getChannel(19));	
	}
	

private void gotoXY(int x,int y){
	
	// holds the steps calculated from the x and y dimentions
	int x_steps=0;
	int y_steos=0;
	
	// math that converts x and y coordinates into motor steps
}

private void linearLine(int x, int y,int x2,int y2){
	
	
}


	
	
	
	
	
	
}
