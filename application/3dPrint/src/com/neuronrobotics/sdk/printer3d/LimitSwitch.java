package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.dyio.DyIO;
import com.neuronrobotics.sdk.dyio.DyIOChannelEvent;
import com.neuronrobotics.sdk.dyio.IChannelEventListener;
import com.neuronrobotics.sdk.dyio.peripherals.DigitalInputChannel;

public class LimitSwitch implements IChannelEventListener {
	int port;
	DyIO lcldyio=null;
	public LimitSwitch(int chnl){
		port=chnl;
		
		
	}
	
	@Override
	public void onChannelEvent(DyIOChannelEvent e) {
		// TODO Auto-generated method stub
		
	}
}
