package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.dyio.DyIO;
import com.neuronrobotics.sdk.serial.SerialConnection;
import com.neuronrobotics.sdk.ui.ConnectionDialog;

public class TestRun {

	
public static void main (String args){
	DyIO lcldyio= new DyIO(new SerialConnection("/dev/ACM0")); 
	lcldyio.connect();
		
		Printer3d printer = new Printer3d(lcldyio);
		Calibrator cal = new Calibrator(mode.ZERO);
	}
	
	
}
