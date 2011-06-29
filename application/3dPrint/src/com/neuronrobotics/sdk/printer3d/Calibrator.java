package com.neuronrobotics.sdk.printer3d;
import com.neuronrobotics.sdk.dyio.DyIO;
import com.neuronrobotics.sdk.dyio.DyIOChannel;
import com.neuronrobotics.sdk.dyio.peripherals.DigitalInputChannel;

/** The purpose of this class is for calibrating 
 * the Printer, and containing all objects associated
 * with the calibration routine*/
public class Calibrator {

	/** class available dyio*/
	DyIO lcl_dyio = null;
	
	/**
	 * limit switches hardware/counter controlled
	 * */
	
	/** X direction zero indication limit switch 
	 * plugged into DyIO port 1*/
	LimitSwitch X_limitZero = null;
	/** Y direction zero indication limit switch 
	 * plugged intro DyIO port 0*/
	LimitSwitch Y_limitZero = null;
	/** Z direction zero indication limit switch
	 * plugged into DyIO port 2 */
	LimitSwitch Z_limitZero = null;

	/** software controlled limits for maxes */
	/** X direction maximum indication limit switch*/
	LimitSwitch X_limitMax = null;
	/** Y direction maximum indication limit switch*/
	LimitSwitch Y_limitMax = null;
	/** Z direction maximum indication limit switch*/
	LimitSwitch Z_limitMax = null;
	
	/** length of axes to be loaded from XML file data*/
	int Z_length =0;
	int X_length=0;
	int Y_length=0;

	/** calibrator constructor method */
	public Calibrator(DyIO d) {
			/** sets the x, y , z axes to their zero positions*/
			Print3D_Statics.X_stepper.setValue(-15000);
			Print3D_Statics.Y_stepper.setValue(-15000);
			Print3D_Statics.Z_stepper.setValue(-15000);
	}

	/** method can be called to recalibrate the printer */
	public void ReCalibrate() {
			/** sets the x, y , z axes to their zero positions */
			Print3D_Statics.X_stepper.setValue(-15000);
			Print3D_Statics.Y_stepper.setValue(-15000);
			Print3D_Statics.Z_stepper.setValue(-15000);
	}// end recalibrate

}
