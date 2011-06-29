package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.addons.kinematics.AbstractCartesianPositionDevice;
import com.neuronrobotics.sdk.dyio.DyIORegestry;

public class Fab3D extends  AbstractCartesianPositionDevice {
	/**
	 * this class should have NO public methods that are not defined in AbstractCartesianPositionDevice
	 */

	@Override
	public void initialize() {
		DyIORegestry.get().setCachedMode(true);
		
	}

	@Override
	public void flush(double time) {
		// TODO Auto-generated method stub
		
	}
	
}
