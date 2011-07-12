package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.addons.kinematics.AbstractCartesianPositionDevice;
import com.neuronrobotics.sdk.addons.kinematics.StepperPrismaticLink;
import com.neuronrobotics.sdk.dyio.DyIORegestry;

public class Fab3D extends AbstractCartesianPositionDevice {
	StepperPrismaticLink X = null;
	StepperPrismaticLink Y = null;
	StepperPrismaticLink Z = null;

	public Fab3D(StepperPrismaticLink x2, StepperPrismaticLink y2,
			StepperPrismaticLink z2) {
		// class available objects loaded with constructor arguments
		X = x2;
		Y = y2;
		Z = z2;

	}

	// Setters and Getters for Links in Fab3D AbstractCartesianDevice
	// setter for x link
	public void setX(StepperPrismaticLink X) {
		this.X = X;
	}

	// getter for x link
	public StepperPrismaticLink getX() {
		return X;
	}

	// setter for Y link
	public void setY(StepperPrismaticLink Y) {
		this.Y = Y;
	}

	// getter for Y link
	public StepperPrismaticLink getY() {
		return Y;
	}

	// setter for Z link
	public void setZ(StepperPrismaticLink Z) {
		this.Z = Z;
	}

	// getter for Z link
	public StepperPrismaticLink getZ() {
		return Z;
	}

	// end Setters and Getters

	// done
	// initialize functions as the calibration routine of the printer
	@Override
	public void initialize() {
		// configure DyIO for cache mode
		DyIORegestry.get().setCachedMode(true);

		// Printer calibration
		// sets all axes to home positions
		SetXAxisPosition(X.getHome());
		SetYAxisPosition(Y.getHome());
		SetZAxisPosition(Z.getHome());
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
		X.getChannel().setValue(steps);
	}

	// done
	@Override
	public void SetYAxisPosition(int steps) {
		Y.getChannel().setValue(steps);

	}

	// done
	@Override
	public void SetZAxisPosition(int steps) {
		Z.getChannel().setValue(steps);

	}

	// done
	@Override
	public void ReCalibrateAbstractCartesianDevice() {
		/** sets all axes to home positions */
		SetXAxisPosition(X.getHome());
		SetYAxisPosition(Y.getHome());
		SetZAxisPosition(Z.getHome());

	}

}
