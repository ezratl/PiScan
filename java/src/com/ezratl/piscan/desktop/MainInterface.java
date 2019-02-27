package com.ezratl.piscan.desktop;

import java.awt.EventQueue;

import javax.swing.JFrame;
import java.awt.Window.Type;
import java.awt.CardLayout;
import javax.swing.JPanel;
import java.awt.BorderLayout;

public class MainInterface {

	private JFrame interfaceFrame;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					MainInterface window = new MainInterface();
					window.interfaceFrame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public MainInterface() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		interfaceFrame = new JFrame();
		//interfaceFrame.setUndecorated(true);
		interfaceFrame.setBounds(100, 100, 450, 300);
		interfaceFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		interfaceFrame.getContentPane().setLayout(new BorderLayout(0, 0));
		
		JPanel topInfoBar = new JPanel();
		interfaceFrame.getContentPane().add(topInfoBar, BorderLayout.NORTH);
		
		JPanel primaryButtonPanel = new JPanel();
		interfaceFrame.getContentPane().add(primaryButtonPanel, BorderLayout.SOUTH);
	}

}
