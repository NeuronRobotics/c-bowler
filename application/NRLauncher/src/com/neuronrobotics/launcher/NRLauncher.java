package com.neuronrobotics.launcher;

import java.awt.Dimension;
import java.awt.Frame;
import java.io.File;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

public class NRLauncher {
	public NRLauncher(String args) throws InterruptedException{
		File f = new File(args);
		if(!f.exists() || !f.isDirectory()){
			throw new RuntimeException("Directory does not exist!"+f.getAbsolutePath());
		}
		System.out.println("Starting with jar directory: "+f.getAbsolutePath());
		LauncherWindow window = new LauncherWindow(args);
		//window.setSize(new Dimension(300,200));
		//window.setExtendedState(Frame.MAXIMIZED_BOTH);
		window.pack();
		reSize(window);
		//window.setUndecorated(true); 
		window.setVisible(true);
		window.repaint();
		window.refresh();
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

	}
	private void reSize(JFrame f){
		//f.setExtendedState(Frame.MAXIMIZED_BOTH);
		//f.setSize(new Dimension(600,400));
	}
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try{
			if(args.length ==1)
				new NRLauncher(args[0]);
			else
				throw new RuntimeException("Need the path to the jar directory!");
		}catch (Exception ex){
			ex.printStackTrace();
			System.exit(0);
		}

	}

}
