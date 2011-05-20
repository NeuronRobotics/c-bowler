package com.neuronrobotics.sdk.printer3d;
import com.neuronrobotics.sdk.dyio.DyIO;
import com.neuronrobotics.sdk.dyio.peripherals.CounterOutputChannel;

public class Printer3d{

	// DyIO object available to the whole class
	DyIO cls_dyio = null;
	
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
	}

	// passed an amount of steps in both X and Y directions to travel
	private void gotoXY(int x, int y) {

		// holds the steps calculated from the x and y dimentions
		int x_steps = 0;
		int y_steos = 0;

		// math that converts x and y coordinates into motor steps
	}

	private void linearLine(int x, int y, int x2, int y2) {

	}

}
