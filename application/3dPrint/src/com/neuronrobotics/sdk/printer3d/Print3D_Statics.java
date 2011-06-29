package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.dyio.peripherals.CounterOutputChannel;

public class Print3D_Statics {

	// stepper motor control port/motor assignments
	// x direction stepper motor
	public static CounterOutputChannel X_stepper=null;
	// y direction stepper motor
	public static CounterOutputChannel Y_stepper=null;
	// z direction stepper motor
	public static CounterOutputChannel Z_stepper=null;
	// printhead extrusion stepper
	public static CounterOutputChannel EXT_stepper=null;
}
