package vLCD;

import javax.swing.JFrame;
import java.awt.BorderLayout;
import java.awt.image.BufferedImage;
import java.io.EOFException;
import java.io.IOException;
import java.io.RandomAccessFile;

import com.sun.prism.paint.Color;

import java.awt.Component;

import javax.swing.Box;
import javax.swing.JLabel;
import javax.swing.ImageIcon;
import javax.swing.SwingConstants;

public class DisplayWindow {

	private static DisplayWindow window;
	private JFrame frame;
	private JLabel displayDrawPanel;
	
	private static int height;
	private static int width;
	private static final String defaultPipePath = "../../graphic_lcd.fifo";
	private static RandomAccessFile imagePipe;
	private static byte imageByteBuffer[];
	private static BufferedImage displayImageBuffer;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) throws Exception {
		window = null;
		String pipePath = null;
		
		if(args.length == 0) {
			height = 64;
			width = 240;
			pipePath = defaultPipePath;
		}
		else if(args.length > 1){
			width = Integer.parseInt(args[0]);
			height = Integer.parseInt(args[1]);
			if(args.length > 2)
				pipePath = new String(args[2]);
			else
				pipePath = defaultPipePath;
		}
		else
			throw new Exception("Not enough arguments");
		
		try {
			imagePipe = new RandomAccessFile(pipePath, "r");
		}
		catch(IOException e) {
			e.printStackTrace();
		}
		
		imageByteBuffer = new byte[width * height];
		displayImageBuffer = new BufferedImage(width * 2, height * 2, BufferedImage.TYPE_BYTE_BINARY);
		
		/*EventQueue.invokeLater(new Runnable() {
			public void run() {*/
				try {
					window = new DisplayWindow();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			/*}
		});*/
		
		//Image splashimg = ImageIO.read(new File("LCDsplash.bmp"));
		//window.displayDrawPanel.setIcon(new ImageIcon(splashimg));
		window.displayDrawPanel.setIcon(new ImageIcon(displayImageBuffer));
		window.frame.pack();
		//main loop
		int readb = 0;
		while(true) {
			try {
				readb = imagePipe.read(imageByteBuffer);
				
			} catch (EOFException e) {
				e.printStackTrace();
			} catch (Exception e) {
				
			}
			//System.out.print(".");
			if (readb > 0) {
				//render image
				int x, y;
				System.out.println("Data received. size: " + readb);
				for(int i = 0; i < displayImageBuffer.getWidth() * displayImageBuffer.getHeight() && i < readb; i++) {
					
					x = /*displayImageBuffer.getWidth() - */2*((1 + i) % width);
					y = 2 * (int) Math.floor( i / width);
					//System.out.print(i + " " + x + " " + y + " " + displayImageBuffer.getWidth() + " " + ((i+1)%displayImageBuffer.getWidth()) + " ");
					if(imageByteBuffer[i] != 0) {
						//System.out.println("B");
						displayImageBuffer.setRGB(x, y, 0);
						displayImageBuffer.setRGB(x + 1, y, 0);
						displayImageBuffer.setRGB(x, y + 1, 0);
						displayImageBuffer.setRGB(x + 1, y + 1, 0);
					}
					else {
						//System.out.println("W");
						displayImageBuffer.setRGB(x, y, Color.WHITE.getIntArgbPre());
						displayImageBuffer.setRGB(x + 1, y, Color.WHITE.getIntArgbPre());
						displayImageBuffer.setRGB(x, y + 1, Color.WHITE.getIntArgbPre());
						displayImageBuffer.setRGB(x + 1, y + 1, Color.WHITE.getIntArgbPre());
					}
				}
				readb = 0;
				System.out.println("Update");
				window.displayDrawPanel.setIcon(new ImageIcon(displayImageBuffer));
			}
			Thread.sleep(100);
		}
	}

	/**
	 * Create the application.
	 */
	public DisplayWindow() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frame = new JFrame();
		frame.setBounds(100, 100, 500, 500);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(new BorderLayout(0, 0));
		
		Component horizontalStrut = Box.createHorizontalStrut(20);
		frame.getContentPane().add(horizontalStrut, BorderLayout.WEST);
		
		Component horizontalStrut_1 = Box.createHorizontalStrut(20);
		frame.getContentPane().add(horizontalStrut_1, BorderLayout.EAST);
		
		Component verticalStrut = Box.createVerticalStrut(20);
		frame.getContentPane().add(verticalStrut, BorderLayout.NORTH);
		
		Component verticalStrut_1 = Box.createVerticalStrut(20);
		frame.getContentPane().add(verticalStrut_1, BorderLayout.SOUTH);
		
		JLabel lblNewLabel = new JLabel();
		lblNewLabel.setHorizontalAlignment(SwingConstants.CENTER);
		displayDrawPanel = lblNewLabel;
		lblNewLabel.setIcon(new ImageIcon(DisplayWindow.class.getResource("/javax/swing/plaf/basic/icons/image-delayed.png")));
		frame.getContentPane().add(lblNewLabel, BorderLayout.CENTER);
	}
}
