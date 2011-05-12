package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.dyio.DyIO;
import com.neuronrobotics.sdk.dyio.peripherals.CounterOutputChannel;

public class StepMotor {
	int port;
	DyIO lcldyio=null;
	public StepMotor(int chnl){
		// loads motor port into class available variable
		port=chnl;
		lcldyio=TestPrinter.getdyio();
	}
	
}
