package com.neuronrobotics.sdk.printer3d;

public abstract class AbstractPrintHead {
	
	/**initialize is used for changing any settings that need to be changed at the start
	 * of a print job
	 * */
	public abstract void initialize();
	
	/** flush is for executing the DyIO's flush command
	 * @param time the feedrate for the printer task*/
	public abstract void flush(double time);
	
	/**SetTemperatureOfHead is for setting the temperature of the
	 * heating element in the printhead
	 * @param temp accounts  for the desired temoperature to be set.*/
	public abstract void SetTemperatureOfHead(int temp);
	
	/**GetCurrentTemperatureOfHead returns the temperature of heating element in
	 * the printhead as an int*/
	public abstract int GetCurrentTemperatureOfHead();
	
	/**StartHeatingElement is for starting the heating element for extrusion
	 *  this will most likely controlled by a relay/digital out */
	public abstract void StartHeatingElement();
	
	/**ExtrudeMaterial is for extruding the material from the printehead
	 * @param time accoutns for feedrate 
	 * @param unit accounts for the lengh of material to feed into the extruder*/
	public abstract void ExtrudeMaterial(double unit,double time);
	
	

}
