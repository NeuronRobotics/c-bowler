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
	// setter for x link
	public void setX(StepperPrismaticLink X){
		this.X=X;
	}
	// getter for x link
	public StepperPrismaticLink getX(){
		return X;
	}
	
	public void setY(StepperPrismaticLink Y){
		this.Y=Y;
	}
	
	public StepperPrismaticLink getY(){
		return Y;
	}
	
	public void setZ(StepperPrismaticLink Z){
		this.Z=Z;
	}
	
	public StepperPrismaticLink getZ(){
		return Z;
	}

	@Override
	public void initialize() {
		DyIORegestry.get().setCachedMode(true);
		
	}


	@Override
	public void flush(double time) {
		DyIORegestry.get().flushCache((float)time);
		
	}

	@Override
	public int ConvertUnitsToSteps(double units) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public void SetXAxisPosition(int steps) {
		
		
	}

	@Override
	public void SetYAxisPosition(int steps) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void SetZAxisPosition(int steps) {
		// TODO Auto-generated method stub
		
	}
	
}
