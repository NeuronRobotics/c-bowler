package com.neuronrobotics.sdk.printer3d;

public abstract class AbstractPrintHead {
	
	/**initialize is used for changing any settings that need to be changed at the start
	 * of a print job
	 * */
	public abstract void initialize();
	
	/** flush is for executing the DyIO's flush command
	 * @param time the feedrate for the printer task*/
	public abstract void flush(double time);
	
	public abstract void SetTemperatureofHead(int temp);
	
	public abstract int GetCurrentTemperatureOfHead();
	
	public abstract void StartHeatingElement();
	
	/**ExtrudeMaterial is for extruding the material from the printehead
	 * @param time accoutns for feedrate 
	 * @param unit accounts for the lengh of material to feed into the extruder*/
	public abstract void ExtrudeMaterial(double unit,double time);
	
	

}
