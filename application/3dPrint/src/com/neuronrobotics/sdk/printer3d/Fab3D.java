package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.addons.kinematics.AbstractCartesianPositionDevice;
import com.neuronrobotics.sdk.addons.kinematics.AbstractPrismaticLink;
import com.neuronrobotics.sdk.dyio.DyIORegestry;

public class Fab3D extends AbstractCartesianPositionDevice {

	// links that belong to the AbstractCartesianPositionDevice
	AbstractPrismaticLink X = null;
	AbstractPrismaticLink Y = null;
	AbstractPrismaticLink Z = null;

	public Fab3D(AbstractPrismaticLink X, AbstractPrismaticLink Y,
			AbstractPrismaticLink Z) {
		this.X = X;
		this.Y = Y;
		this.Z = Z;
	}

	@Override
	public void initialize() {

	}

	// done
	// for executing multiple commands at once
	@Override
	public void flush(double time) {
		// executes DyIO flush
		DyIORegestry.get().flushCache((float) time);
	}

	@Override
	public void SetXAxisPosition(double unit, double time) {
		X.setTargetEngineeringUnits(unit);
		flush(time);
	}

	@Override
	public void SetYAxisPosition(double unit, double time) {
		Y.setTargetEngineeringUnits(unit);
		flush(time);
	}

	@Override
	public void SetZAxisPosition(double unit, double time) {
		Z.setTargetEngineeringUnits(unit);
		flush(time);
	}

	@Override
	public void SetAllAxisPosition(double xUnit, double yUnit, double zUnit,
			double time) {
		X.setTargetEngineeringUnits(xUnit);
		Y.setTargetEngineeringUnits(yUnit);
		Z.setTargetEngineeringUnits(zUnit);
		flush(time);
	}

	// Setters and Getters for Links in Fab3D AbstractCartesianDevice
	// setter for x link
	public void setX(AbstractPrismaticLink X) {
		this.X = X;
	}

	// getter for x link
	public AbstractPrismaticLink getX() {
		return X;
	}

	// setter for Y link
	public void setY(AbstractPrismaticLink Y) {
		this.Y = Y;
	}

	// getter for Y link
	public AbstractPrismaticLink getY() {
		return Y;
	}

	// setter for Z link
	public void setZ(AbstractPrismaticLink Z) {
		this.Z = Z;
	}

	// getter for Z link
	public AbstractPrismaticLink getZ() {
		return Z;
	}

	// end Setters and Getters

	@Override
	public void SetAllAxisToHome(double time) {
		X.setHome(X.getHome());
		Y.setHome(Y.getHome());
		Z.setHome(Z.getHome());
		flush(time);

	}

	@Override
	public void SetXToHome(double time) {
		X.setHome(X.getHome());
		flush(time);
	}

	@Override
	public void SetYToHome(double time) {
		Y.setHome(Y.getHome());
		flush(time);
	}

	@Override
	public void SetZToHome(double time) {
		Z.setHome(Z.getHome());
		flush(time);
	}

}
