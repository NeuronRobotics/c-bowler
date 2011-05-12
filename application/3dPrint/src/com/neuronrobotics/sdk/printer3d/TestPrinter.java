package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.dyio.DyIO;
import com.neuronrobotics.sdk.serial.SerialConnection;
import com.neuronrobotics.sdk.ui.ConnectionDialog;

public class TestPrinter {
	
	static DyIO project_dyio =null;
	
	public TestPrinter(){
		// instantiates DyIO object and generates DyIO moduale connection dialog
		project_dyio = new DyIO(new SerialConnection("/dev/tty.usbmodemfd131"));
		if (!ConnectionDialog.getBowlerDevice(project_dyio)) {
			System.exit(1);
		}
		
	}
	
	public static DyIO getdyio(){
		return project_dyio;
	}

}
