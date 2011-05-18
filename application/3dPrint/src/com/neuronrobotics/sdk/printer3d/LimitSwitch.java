package com.neuronrobotics.sdk.printer3d;

import com.neuronrobotics.sdk.common.ByteList;
import com.neuronrobotics.sdk.dyio.DyIOChannel;
import com.neuronrobotics.sdk.dyio.DyIOChannelEvent;
import com.neuronrobotics.sdk.dyio.IChannelEventListener;


public class LimitSwitch implements IChannelEventListener {
	//limit switch connection port
	DyIOChannel port;
	// status hit or not (true/false)
	boolean status=false;
	
	//default constructor 
	public LimitSwitch(DyIOChannel chnl){
		// holds switch DyIO channel
		port=chnl;
		// channel port number
		int i = chnl.getChannelNumber();
		chnl.addChannelEventListener(this);
	}// end constructor
	
	
	@Override
	public void onChannelEvent(DyIOChannelEvent event) {
		// TODO Auto-generated method stub
		ByteList i = event.getData();
		int j = i.get(0);
		if(j==1){
			// switch hit status = true
			status=true;
		}
		else{
			status=false;
		}
	}// end onChannelEvent
	
	
	// returns updated value of switch
	public boolean getStatus(){
		return status;
	}
}


