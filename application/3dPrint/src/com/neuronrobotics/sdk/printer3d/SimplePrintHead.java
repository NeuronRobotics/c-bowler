package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.addons.kinematics.AbstractPrismaticLink;
import com.neuronrobotics.sdk.dyio.DyIORegestry;


public class SimplePrintHead extends AbstractPrintHead {
	
	AbstractPrismaticLink Ext = null;

	// constructor for simple printhead, takes argument of AbstractPrismaticLink
	public SimplePrintHead(AbstractPrismaticLink ext){
		this.Ext=ext;
	}
	
	@Override
	public void ExtrudeMaterial(double unit,double time) {
		Ext.setTargetEngineeringUnits(unit);
		flush(time);
		
	}

	@Override
	public void SetTemperatureOfHead(int temp) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void flush(double time) {
		// executes DyIO flush
		DyIORegestry.get().flushCache((float) time);
		
	}

	@Override
	public void initialize() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public int GetCurrentTemperatureOfHead() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public void StartHeatingElement() {
		// TODO Auto-generated method stub
		
	}

}
