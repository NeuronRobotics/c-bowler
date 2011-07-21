package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.addons.kinematics.AbstractPrismaticLink;
import com.neuronrobotics.sdk.dyio.DyIORegestry;


public class SimplePrintHead  {
	
	AbstractPrismaticLink Ext = null;

	// constructor for simple printhead, takes argument of AbstractPrismaticLink
	public SimplePrintHead(AbstractPrismaticLink ext){
		this.Ext=ext;
	}
	
	public void ExtrudeMaterial(double unit,double time) {
		Ext.setTargetEngineeringUnits(unit);
		flush(time);
		
	}


	public void SetTemperatureOfHead(int temp) {
		// TODO Auto-generated method stub
		
	}


	public void flush(double time) {
		// executes DyIO flush
		DyIORegestry.get().flushCache((float) time);
		
	}


	public void initialize() {
		// TODO Auto-generated method stub
		
	}


	public int GetCurrentTemperatureOfHead() {
		// TODO Auto-generated method stub
		return 0;
	}

	public void StartHeatingElement() {
		// TODO Auto-generated method stub
		
	}

}
