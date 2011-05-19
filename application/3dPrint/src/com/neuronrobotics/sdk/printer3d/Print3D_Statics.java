package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.dyio.peripherals.CounterOutputChannel;

public class Print3D_Statics {

	// stepper motor control port/motor assignments
	// x direction stepper motor
	static CounterOutputChannel X_stepper=null;
	// y direction stepper motor
	static CounterOutputChannel Y_stepper=null;
	// z direction stepper motor
	static CounterOutputChannel Z_stepper=null;
	// printhead extrusion stepper
	static CounterOutputChannel EXT_stepper=null;
}
