package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.addons.kinematics.AbstractCartesianPositionDevice;
import com.neuronrobotics.sdk.addons.kinematics.StepperPrismaticLink;
import com.neuronrobotics.sdk.dyio.DyIORegestry;

public class Fab3D extends  AbstractCartesianPositionDevice {
	
	public Fab3D(StepperPrismaticLink x, StepperPrismaticLink y, StepperPrismaticLink z) {
		
	}

	@Override
	public void initialize() {
		DyIORegestry.get().setCachedMode(true);
		
	}


	@Override
	public void flush(double time) {
		// TODO Auto-generated method stub
		
	}
	
}
