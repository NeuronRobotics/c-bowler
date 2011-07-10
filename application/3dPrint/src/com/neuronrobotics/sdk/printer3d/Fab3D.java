package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.addons.kinematics.AbstractCartesianPositionDevice;
import com.neuronrobotics.sdk.addons.kinematics.StepperPrismaticLink;
import com.neuronrobotics.sdk.dyio.DyIORegestry;

public class Fab3D extends  AbstractCartesianPositionDevice {
	StepperPrismaticLink X= null;
	StepperPrismaticLink Y= null;
	StepperPrismaticLink Z= null;
	
	
	public Fab3D(StepperPrismaticLink x2, StepperPrismaticLink y2, StepperPrismaticLink z2) {
		// class available objects loaded with constructor arguments
		X=x2;
		Y=y2;
		Z=z2;
		
	}
	
	// Setters and Getters for Links in Fab3D AbstractCartesianDevice
	// setter for x link
	public void setX(StepperPrismaticLink X){
		this.X=X;
	}
	// getter for x link
	public StepperPrismaticLink getX(){
		return X;
	}
	// setter for Y link
	public void setY(StepperPrismaticLink Y){
		this.Y=Y;
	}
	// getter for Y link
	public StepperPrismaticLink getY(){
		return Y;
	}
	// setter for Z link
	public void setZ(StepperPrismaticLink Z){
		this.Z=Z;
	}
	// getter for Z link
	public StepperPrismaticLink getZ(){
		return Z;
	}
	// end Setters and Getters
	
	
	@Override
	public void initialize() {
		// configure DyIO for cache mode
		DyIORegestry.get().setCachedMode(true);
		
	}


	@Override
	public void flush(double time) {
		// executes DyIO flush
		DyIORegestry.get().flushCache((float)time);
	}

	
	@Override
	public int ConvertUnitsToSteps(double units) {
		// converts the units for the project into
		// motor steps, returns steps as type int
		return 0;
	}

	@Override
	public void SetXAxisPosition(int steps) {
		X.getChannel().setValue(steps);
	}

	@Override
	public void SetYAxisPosition(int steps) {
		Y.getChannel().setValue(steps);
		
	}

	@Override
	public void SetZAxisPosition(int steps) {
		Z.getChannel().setValue(steps);
		
	}
	
}
