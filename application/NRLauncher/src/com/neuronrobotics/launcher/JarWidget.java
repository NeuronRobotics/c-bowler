package com.neuronrobotics.launcher;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class JarWidget extends JPanel {

	/**
	 * 
	 */
	private static final long serialVersionUID = -8298819301680080776L;
	RunnerWidget runner;
	private JCheckBox selected = new JCheckBox();
	private JLabel fileNameLabel = new JLabel("");
	private File myFile;
	public JarWidget(RunnerWidget wid, String fileName){
		runner=wid;
		myFile = new File(fileName);
		fileNameLabel.setText(myFile.getName());
		selected.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if(selected.isSelected()){
					runner.clearFile();
					selected.setSelected(true);
					runner.setFile(myFile);
				}else{
					runner.clearFile();
				}
			}
		});
		add(selected);
		add(fileNameLabel);
	}
	@Override
	public void setVisible(boolean b){
		selected.setVisible(b);
		fileNameLabel.setVisible(b);
		super.setVisible(b);
	}
	public void selsect(boolean set){
		if(selected.isSelected() != set){
			selected.setSelected(set);
		}
	}
}
