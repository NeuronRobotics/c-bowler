package com.neuronrobotics.sdk.printer3d;
import com.neuronrobotics.sdk.dyio.DyIO;
import com.neuronrobotics.sdk.dyio.peripherals.CounterOutputChannel;

public class Printer3d{

	// DyIO object available to the whole class
	DyIO cls_dyio = null;
	
	// A Calibrator object to calibrate the printer in the constructor
	Calibrator cls_clbtr;
	
	// constructor
	Printer3d(DyIO d) {	
		cls_dyio=d;
		// ports assigned to motors
		// assigns X_stepper to channel 21
		Print3D_Statics.X_stepper = new CounterOutputChannel(cls_dyio.getChannel(21));
		// assigns Y_stepper to channel 23
		Print3D_Statics.Y_stepper = new CounterOutputChannel(cls_dyio.getChannel(23));
		// assigns Z_stepper to channel 19
		Print3D_Statics.Z_stepper = new CounterOutputChannel(cls_dyio.getChannel(19));
		//configures DyIO for Cache mode
		cls_dyio.setCachedMode(true);
		
		// configuration utility
		cls_clbtr = new Calibrator(d);
	}

	// passed an amount of steps in both X and Y directions to travel
	private void gotoXY(int x, int y) {
		Print3D_Statics.X_stepper.setValue(x);
		Print3D_Statics.Y_stepper.setValue(y);
	}

	// passed the starting point coordinates in motor steps
	// and the end point coordinates in motor steps
	private void linearLine(int x, int y, int x2, int y2) {
		// goes to the starting point of the line
		Print3D_Statics.X_stepper.setValue(x);
		Print3D_Statics.Y_stepper.setValue(y);
		//cls_dyio.flushCache(feedrate);
		
		// moves in a line to the second point
		Print3D_Statics.X_stepper.setValue(x2);
		Print3D_Statics.Y_stepper.setValue(y2);
		//cls_dyio.flushCache(feedrate);
	}
	
	// will be used for moving the platform of the printer up and down
	// takes the amount of steps int he Z direction as argument
	private void moveZ(int z){
		Print3D_Statics.Z_stepper.setValue(z);
	}

	// function that will invoke non linear motion code on DyIO
	private void moveCurve(){
		// just a stub ATM arguments to be determined
		
	}
}
