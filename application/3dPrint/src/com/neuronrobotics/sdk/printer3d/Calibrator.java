package com.neuronrobotics.sdk.printer3d;
import com.neuronrobotics.sdk.dyio.DyIOChannel;
import com.neuronrobotics.sdk.dyio.peripherals.DigitalInputChannel;

// The purpose of this class is for calibrating 
public class Calibrator {

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

	// classwide varibale for holding calibration mode
	mode classMode = null;

	// constructor
	public Calibrator(mode m) {

		classMode = m;

		if (classMode == mode.MAX) {

		} else {

			// sets the x, y , z axes to their zero positions
			Print3D_Statics.X_stepper.setValue(-15000);
			Print3D_Statics.Y_stepper.setValue(-15000);
			Print3D_Statics.Z_stepper.setValue(-15000);
		}
	}

	// method can be called to recalibrate the printer
	public void ReCalibrate() {
		if (classMode == mode.MAX) {
			while (X_limitMax.getStatus() != true) {
				Print3D_Statics.X_stepper.setValue(15000);
			}
			Print3D_Statics.X_stepper.setValue(0);
			while (Y_limitMax.getStatus() != true) {
				Print3D_Statics.Y_stepper.setValue(15000);
			}
			Print3D_Statics.Y_stepper.setValue(0);
			while (Z_limitMax.getStatus() != true) {
				Print3D_Statics.Z_stepper.setValue(15000);
			}
			Print3D_Statics.Z_stepper.setValue(0);
			
			// code that retrieves the counter value from MAX to zero and loads
			// it into variables, as zero positions are achieved
			Print3D_Statics.X_stepper.setValue(-15000);
			Print3D_Statics.Y_stepper.setValue(-15000);
			Print3D_Statics.Z_stepper.setValue(-15000);

		} else {
			// sets the x, y , z axes to their zero positions
			Print3D_Statics.X_stepper.setValue(-15000);
			Print3D_Statics.Y_stepper.setValue(-15000);
			Print3D_Statics.Z_stepper.setValue(-15000);
		}
	}// end recalibrate

}

// inner enum class for determining functionality and mode of calibration
// ZERO assumes there are only limit switches for the zero positions
// MAX assumes that there are switches for the max and zero positions
enum mode {
	ZERO, MAX
}