package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.addons.kinematics.AbstractCartesianPositionDevice;
import com.neuronrobotics.sdk.addons.kinematics.AbstractLink;
import com.neuronrobotics.sdk.addons.kinematics.StepperPrismaticLink;
import com.neuronrobotics.sdk.dyio.DyIORegestry;

public class Fab3D extends AbstractCartesianPositionDevice {

	

	public Fab3D(AbstractLink X, AbstractLink Y, AbstractLink Z) {
		super(X, Y, Z);
	}

	// done
	// initialize functions as the calibration routine of the printer
	@Override
	public void initialize() {
		// configure DyIO for cache mode
		DyIORegestry.get().setCachedMode(true);

		// Printer calibration
		// sets all axes to home positions
		SetXAxisPosition(getX().getHome());
		SetYAxisPosition(getY().getHome());
		SetZAxisPosition(getZ().getHome());
		// execute calibration with DyIO flush *********** Kevin what should we
		// have for the time/feedrate?
		flush(100);
	}

	// done
	// for executing multiple commands at once
	@Override
	public void flush(double time) {
		// executes DyIO flush
		DyIORegestry.get().flushCache((float) time);
	}

	@Override
	public int ConvertUnitsToSteps(double units) {
		// converts the units for the project into
		// motor steps, returns steps as type int
		return 0;
	}

	// done
	@Override
	public void SetXAxisPosition(int steps) {
		getX().getChannel().setValue(steps);
	}

	// done
	@Override
	public void SetYAxisPosition(int steps) {
		getY().getChannel().setValue(steps);

	}

	// done
	@Override
	public void SetZAxisPosition(int steps) {
		getZ().getChannel().setValue(steps);

	}

	// done
	@Override
	public void ReCalibrateAbstractCartesianDevice() {
		/** sets all axes to home positions */
		SetXAxisPosition(getX().getHome());
		SetYAxisPosition(getY().getHome());
		SetZAxisPosition(getZ().getHome());
	}

}
