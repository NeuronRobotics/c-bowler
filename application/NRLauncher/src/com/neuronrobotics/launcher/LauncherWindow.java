package com.neuronrobotics.launcher;


import java.awt.Dimension;
import java.awt.event.HierarchyBoundsListener;
import java.awt.event.HierarchyEvent;
import javax.swing.JFrame;
import javax.swing.JScrollPane;

public class LauncherWindow extends JFrame {

	/**
	 * 
	 */
	private static final long serialVersionUID = -5668856972512694750L;
	private RunnerWidget runner;
	private JScrollPane scrollPanel;
	public LauncherWindow (String launchDir){
		
		runner = new RunnerWidget(this,launchDir);
		getContentPane().addHierarchyBoundsListener(new HierarchyBoundsListener(){
			@Override
			public void ancestorMoved(HierarchyEvent arg0) {
				
			}
			@Override
			public void ancestorResized(HierarchyEvent arg0) {
				runner.setSize(getWidth(), getHeight());
			}			
		});
		//add(runner);
		scrollPanel = new JScrollPane(runner);
		scrollPanel.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		scrollPanel.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		
		add(scrollPanel);
	}
	@Override 
	public void setSize(int width,int height){
		super.setSize(width,height);	
		scrollPanel.setPreferredSize(new Dimension(width,600));
		//scrollPanel.setSize(new Dimension(width,height-100));
	}
	public void refresh() {
		runner.refreshJars();
	}
}
