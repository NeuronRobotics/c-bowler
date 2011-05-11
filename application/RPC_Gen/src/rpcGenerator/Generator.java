/**
 * 
 */
package rpcGenerator;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
//import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

/**
 * @author Kevin Harrington
 *
 */
public class Generator implements ActionListener{
	private static JFrame frame;
	private static JTextField rpc = new JTextField(4);
	private static JTextField output = new JTextField(45);
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		new Generator();
	}
	public Generator(){
		initGUI();
	}
	private void initGUI(){
		frame = new JFrame("Bowler RPC Generator");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setLocationRelativeTo(null);
		frame.setResizable(false);
		JButton goBtn = new JButton("Convert");
		goBtn.addActionListener(this);
		rpc.addActionListener(this);
		goBtn.setActionCommand("GO");
		
		JPanel pannel = new JPanel();
		pannel.add(rpc);
		pannel.add(goBtn);
		pannel.add(output);
		
		
		frame.add(pannel);
		frame.pack();
		frame.setVisible(true);
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		String RPC = rpc.getText();
		String txtOut;
		String hex;
		if (RPC.length()== 4){
			hex = rpcToHex(RPC);
			txtOut = "#define "+RPC.toUpperCase()+"\t\t\t"+hex+" // '"+RPC+"'  Comment";
			output.setText(txtOut);
		}else
			output.setText("BAD RPC!!");
		
	}
	private String rpcToHex(String rpc){
		String hex="0x";
		for (int i=0;i<4;i++){
			hex +=charToHex(rpc.charAt(3-i));
		}
		return hex;
	}
	private String charToHex(char c){
		String hex;
		int b = (int) c;
		hex = Integer.toString( ( b & 0xff ) + 0x100, 16 /* radix */ ).substring( 1 );
		return hex;
	}
}
