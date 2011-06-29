package com.neuronrobotics.sdk.printer3d;
import com.neuronrobotics.sdk.dyio.DyIO;
import com.neuronrobotics.sdk.dyio.DyIOChannel;
import com.neuronrobotics.sdk.dyio.peripherals.DigitalInputChannel;

// The purpose of this class is for calibrating 
public class Calibrator {

	// class available dyio
	DyIO lcl_dyio = null;
	
	// limit switches
	// hardware/counter controlled
	// X direction zero indication limit switch
	LimitSwitch X_limitZero = null;// 1
	// Y direction zero indication limit switch
	LimitSwitch Y_limitZero = null;// 0
	// Z direction zero indication limit switch
	LimitSwitch Z_limitZero = null; // port 2

	// software controlled limits for maxes
	// X direction maximum indication limit switch
	LimitSwitch X_limitMax = null;
	// Y direction maximum indication limit switch
	LimitSwitch Y_limitMax = null;
	// Z direction maximum indication limit switch
	LimitSwitch Z_limitMax = null;
	
	// length of axes to be loaded from XML file data
	int Z_length =0;
	int X_length=0;
	int Y_length=0;

	// constructor
	public Calibrator(DyIO d) {
			// sets the x, y , z axes to their zero positions
			Print3D_Statics.X_stepper.setValue(-15000);
			Print3D_Statics.Y_stepper.setValue(-15000);
			Print3D_Statics.Z_stepper.setValue(-15000);
	}

	// method can be called to recalibrate the printer
	public void ReCalibrate() {
			// sets the x, y , z axes to their zero positions
			Print3D_Statics.X_stepper.setValue(-15000);
			Print3D_Statics.Y_stepper.setValue(-15000);
			Print3D_Statics.Z_stepper.setValue(-15000);
	}// end recalibrate

}
