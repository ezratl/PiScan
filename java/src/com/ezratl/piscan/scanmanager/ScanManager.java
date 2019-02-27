/*GUI configuration/database manager for PiScan software
 * Written by Ezra Taimuty-Loomis
 * <date>
 * */

package com.ezratl.piscan.scanmanager;

import java.io.*;
import java.awt.*;
import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import java.util.*;
import java.awt.event.*;
import javax.swing.table.*;

import com.ezratl.piscan.backend.config.SystemConfiguration;
import com.ezratl.piscan.backend.scanner.*;

public class ScanManager {

	public static void main(String[] args) {
		File conPath = new File(System.getProperty("user.dir"));
		File conFile = new File("config.piscan");
		if (!conFile.exists()) {
			int choice = JOptionPane.showOptionDialog(null, "Welcome to PiScan Manager", "PiScan Manager", 0, 0, null,
					new String[] { "Load configuration", "New configuration" }, null);
			// File conPath = null;
			JFileChooser fc = new JFileChooser();
			fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
			switch (choice) {
			case -1:// exit
				return;
			case 0:// load config
				fc.showOpenDialog(null);
				conPath = fc.getSelectedFile();
				conFile = new File(conPath + "/config.piscan");
				if (!conFile.exists()) {
					JOptionPane.showMessageDialog(null, "Main configuration file not found, will create new one");
				}
				break;
			case 1:// new config
				//conPath = new File(System.getProperty("user.home") + "/NewPiScanConfig");
				while (true) {
					try {
						conPath.mkdir();
						break;
					} catch (SecurityException e) {
						JOptionPane.showMessageDialog(null,
								"Error: directory access restricted. Select a new directory.");
						fc.showOpenDialog(null);
						conPath = new File(fc.getSelectedFile() + "/NewPiScanConfig");
					}
				}
				JOptionPane.showMessageDialog(null, "New configuration directory was created at " + conPath);
			}
		}
		System.out.println(conPath);
		new ScanManager(conPath);
	}
	
	//begin main class
	private File configPath, configFile, systemsPath, systemFiles[];
	private ArrayList<RadioSystem> systems;
	private JFrame mainWindow;
	private JPanel leftPanel, rightPanel;
	private JButton editConfButton, addSysButton, editSysButton, delSysButton, upButton, downButton, saveButton, importButton;
	private JTable systemTable;
	private DefaultTableModel theTable;
	private SystemConfiguration config;
	private RadioSystem selectedSystem;
	private int modifyIndex;
	
	public ScanManager(File conPath) {
		//initialize instance vars
		configPath = conPath;
		configFile = new File(configPath + "/config.piscan");
		systemsPath = new File(configPath + "/systems");
		systemsPath.mkdir(); //does not affect existing folder
		systems = new ArrayList<RadioSystem>();
		modifyIndex = -1;
		
		//locate system files
		systemFiles = systemsPath.listFiles(new FilenameFilter() {
			public boolean accept(File dir, String name) {
				return name.toLowerCase().endsWith(".system");
			}
		});
		
		//load files
		config = SystemConfiguration.getConfig(configFile);
		
		for(File sysFile: systemFiles) {
			System.out.println(sysFile);
			RadioSystem nextSys = RadioSystem.getSystem(sysFile);
			if(nextSys != null)
				systems.add(nextSys);
		}
		
		//begin GUI initialize
		mainWindow = new JFrame("PiScan Manager");
		mainWindow.setLayout(new FlowLayout());
		mainWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		leftPanel = new JPanel(); rightPanel = new JPanel(new GridLayout(8,1));
		mainWindow.add(leftPanel); mainWindow.add(rightPanel);
		
		//button setup
		editConfButton = new JButton("Edit settings"); addSysButton = new JButton("Add new system"); 
		editSysButton = new JButton("Edit system"); delSysButton = new JButton("Delete system");
		upButton = new JButton("Move up"); downButton = new JButton("Move down"); saveButton = new JButton("Save config");
		importButton = new JButton("Import");
		editSysButton.setEnabled(false); delSysButton.setEnabled(false);
		upButton.setEnabled(false); downButton.setEnabled(false);
		MainButtonListener mainListener = new MainButtonListener();
		editConfButton.addActionListener(mainListener); addSysButton.addActionListener(mainListener); editSysButton.addActionListener(mainListener); delSysButton.addActionListener(mainListener);
		upButton.addActionListener(mainListener); downButton.addActionListener(mainListener); saveButton.addActionListener(mainListener);
		importButton.addActionListener(mainListener);
		rightPanel.add(editConfButton); rightPanel.add(addSysButton); rightPanel.add(editSysButton); rightPanel.add(delSysButton);
		rightPanel.add(upButton); rightPanel.add(downButton); rightPanel.add(importButton); rightPanel.add(saveButton);
		
		
		systemTable = new JTable();
		updateTable();
		systemTable.getSelectionModel().addListSelectionListener(new SystemListListener());
		leftPanel.add(new JScrollPane(systemTable));
		mainWindow.pack();
		mainWindow.setLocationRelativeTo(null);
		mainWindow.setVisible(true);
	}
	
	private void updateTable() {
		theTable = new DefaultTableModel();
		theTable.addColumn("Index"); theTable.addColumn("Name"); theTable.addColumn("Type"); theTable.addColumn("Lockout");
		for (int i = 0; i < systems.size(); i++)
			theTable.addRow(new String[] {String.valueOf(i), systems.get(i).toString(), systems.get(i).getSystemType(), String.valueOf(systems.get(i).lockedOut())});
		systemTable.setModel(theTable);
		if(modifyIndex >= 0 && modifyIndex < systems.size())
			systemTable.changeSelection(modifyIndex, 0, false, false);
	}
	
	
	
	//GUI classes
	
	public class EditConfigDialog extends JDialog{
		private JComboBox<String> displaySelect, radioSelect, audioSelect, interfaceSelect;
		private JButton saveConfigButton, cancelButton;
		
		public EditConfigDialog() {
			super(mainWindow, "Edit settings");
			
			displaySelect = new JComboBox<String>(new String[] {"None (Console only)", "GUI", "Matrix Orbital", "HD44780"});
			displaySelect.setSelectedItem(config.displayDriver);
			
			radioSelect = new JComboBox<String>(new String[] {"RTL"});
			radioSelect.setSelectedItem(config.tunerDriver);
			
			audioSelect = new JComboBox<String>(new String[] {"System Default"});
			audioSelect.setSelectedItem(config.audioDriver);
			
			interfaceSelect = new JComboBox<String>(new String[] {"Disabled", "Enabled"});
			if(config.hasPhysicalInterface)
				interfaceSelect.setSelectedItem("Enabled");
			
			GridLayout layout = new GridLayout(5,2);
			layout.setHgap(5); layout.setVgap(5);
			setLayout(layout);
			add(new JLabel("Display driver:"));
			add(displaySelect);
			add(new JLabel("Tuner driver:"));
			add(radioSelect);
			add(new JLabel("Audio driver:"));
			add(audioSelect);
			add(new JLabel("Physical controls:"));
			add(interfaceSelect);
			
			saveConfigButton = new JButton("Save"); cancelButton = new JButton("Cancel");
			saveConfigButton.addActionListener(new ConfirmListener()); cancelButton.addActionListener(new ConfirmListener());
			add(saveConfigButton); add(cancelButton);
			pack();
			setVisible(true);
		}
		
		public class ConfirmListener implements ActionListener{
			public void actionPerformed(ActionEvent e) {
				if(e.getSource() == saveConfigButton) {
					config.displayDriver = (String) displaySelect.getSelectedItem();
					config.tunerDriver = (String) radioSelect.getSelectedItem();
					config.audioDriver = (String) audioSelect.getSelectedItem();
					config.hasPhysicalInterface = interfaceSelect.getSelectedItem().equals("Enabled");
				}
				setVisible(false);
			}
		}
	}
	
	public class NewSystemDialog extends JDialog {
		private JButton addButton, cancelButton;
		private JComboBox<String> newSysType;
		private JTextField newSysName;
		
		public NewSystemDialog() {
			super(mainWindow, "Add new system");
			dialogInit();
			String options[] = {"Analog"};
			newSysName = new JTextField();
			newSysType = new JComboBox<String>(options);
			GridLayout layout = new GridLayout(3,2);
			layout.setHgap(5); layout.setVgap(5);
			setLayout(layout);
			add(new JLabel("System name:"));
			add(newSysName);
			add(new JLabel("System type:"));
			add(newSysType);
			
			addButton = new JButton("Add system"); cancelButton = new JButton("Cancel");
			addButton.addActionListener(new ConfirmListener()); cancelButton.addActionListener(new ConfirmListener());
			add(addButton); add(cancelButton);
			pack();
			setVisible(true);
		}
		
		public class ConfirmListener implements ActionListener{
			public void actionPerformed(ActionEvent e) {
				if(e.getSource() == addButton) {
					RadioSystem newSys = null;
					int newType = newSysType.getSelectedIndex();
					switch(newType) {
					case 0:
					default:
						if(!newSysName.getText().equals(""))
							newSys = new AnalogSystem(newSysName.getText()); 
					}
					if(newSys != null) {
						systems.add(newSys);
						theTable.addRow(new String[] {String.valueOf(systems.size() - 1), newSys.toString(), newSys.getSystemType(), String.valueOf(newSys.lockedOut())});
						setVisible(false);
					}
				}
				else
					setVisible(false);
			}
		}
	}
	
	abstract class EditSystemDialog extends JDialog {
		protected JPanel systemPanel;
		private JPanel topPanel, bottomPanel;
		private JTextField nameField;
		private ButtonGroup lockoutButtons;
		private JRadioButton trueButton, falseButton;
		protected JButton saveConfigButton, cancelButton;		
		public EditSystemDialog() {
			super(mainWindow, "Edit system " + selectedSystem);
			setLayout(new GridLayout(3,1));
			systemPanel = new JPanel(); topPanel = new JPanel(new GridLayout(2,3));
			topPanel.add(new JLabel("Name:")); topPanel.add(new JLabel("Lockout:")); topPanel.add(new JLabel("Type:"));
			trueButton = new JRadioButton("True"); falseButton = new JRadioButton("False");
			lockoutButtons = new ButtonGroup(); lockoutButtons.add(trueButton); lockoutButtons.add(falseButton);
			nameField = new JTextField(selectedSystem.toString());
			JPanel lockoutGroup = new JPanel(); lockoutGroup.add(trueButton); lockoutGroup.add(falseButton);
			topPanel.add(nameField);  topPanel.add(lockoutGroup); topPanel.add(new JLabel(selectedSystem.getSystemType()));
			
			if(selectedSystem.lockedOut())
				trueButton.setSelected(true);
			else
				falseButton.setSelected(true);
			
			bottomPanel = new JPanel();
			saveConfigButton = new JButton("Save"); cancelButton = new JButton("Cancel");
			//saveConfigButton.addActionListener(new ConfirmListener()); cancelButton.addActionListener(new ConfirmListener());
			bottomPanel.add(saveConfigButton); bottomPanel.add(cancelButton);
			add(topPanel); add(systemPanel); add(bottomPanel);
		}
		
		abstract class ConfirmListener implements ActionListener {
			public abstract void actionPerformed(ActionEvent e);
			protected void commonHandler() {
				//common handler
				selectedSystem.setName(nameField.getText());
				/*if(trueButton.isSelected())
					selectedSystem.lockOut();
				else
					selectedSystem.unLock();*/
				
				setVisible(false);
			}
		}
		
	}
	
	public class EditAnalogSystemDialog extends EditSystemDialog {
		public EditAnalogSystemDialog() {
			super();
			
			saveConfigButton.addActionListener(new AnalogConfirmListener());
			cancelButton.addActionListener(new AnalogConfirmListener());
			pack();
			setVisible(true);
		}
		
		class AnalogConfirmListener extends ConfirmListener {
			public void actionPerformed(ActionEvent e) {
				if(e.getSource() != cancelButton) {
					commonHandler();
				}
				else
					setVisible(false);
			}
		}
	}
	
	//action listeners
	public class MainButtonListener implements ActionListener{
		public void actionPerformed(ActionEvent e) {
			Object theButton = e.getSource();
			if(theButton == editConfButton)
				new EditConfigDialog();
			else if(theButton == addSysButton)
				new NewSystemDialog();
			else if(theButton == editSysButton) {
				System.out.println(selectedSystem);
				if(selectedSystem.getSystemType().equals("Analog"))
					new EditAnalogSystemDialog();
			}
			else if(theButton == delSysButton){
				if(JOptionPane.showConfirmDialog(mainWindow, "Are you sure you want to delete this system?") == 0) {
					//theTable.removeRow(systemTable.getSelectedRow());
					systems.remove(modifyIndex);
				}
			}
			else if(theButton == upButton) {
				RadioSystem temp = systems.get(modifyIndex - 1);
				systems.set(modifyIndex - 1, systems.get(modifyIndex));
				systems.set(modifyIndex, temp);
				modifyIndex--;
			}
			else if(theButton == downButton) {
				RadioSystem temp = systems.get(modifyIndex + 1);
				systems.set(modifyIndex + 1, systems.get(modifyIndex));
				systems.set(modifyIndex, temp);
				modifyIndex++;
			}
			else if(theButton == saveButton){
				//save all
				for(File sysFile: systemFiles)
					if(sysFile.delete())
						System.out.println("Deleted " + sysFile);
				if(SystemConfiguration.saveConfig(configPath, config) == 1)
					System.out.println("Config saved successfully");
				for(int i = 0; i < systems.size(); i++) {
					if(RadioSystem.saveSystem(systemsPath, systems.get(i), i) == 1)
						System.out.println("System " + i + ": " + systems.get(i) + " saved successfully");
				}
			}
			else if(theButton == importButton) {
				//new ImportDatabaseDialog();
				ArrayList<RadioSystem> newSystems = null;
				JFileChooser fc = new JFileChooser();
				fc.showOpenDialog(mainWindow);
				File convertFile = fc.getSelectedFile();
				if(convertFile != null && convertFile.toString().endsWith(".hpd")) 
					newSystems = (new SentinelConverter(convertFile)).getConvertedSystems();
				else if(convertFile != null)
					JOptionPane.showMessageDialog(mainWindow, "Unsupported file format");
				if(newSystems != null)
					systems.addAll(newSystems);
			}
			updateTable();
		}
	}
	
	public class SystemListListener implements ListSelectionListener{
		public void valueChanged(ListSelectionEvent arg0) {
			//System.out.println(systemTable.getSelectedRow());
			if(systemTable.getSelectedRow() >= 0)
				modifyIndex = systemTable.getSelectedRow();
			editSysButton.setEnabled(true); delSysButton.setEnabled(true);
			upButton.setEnabled(true); downButton.setEnabled(true);
			if(systemTable.getSelectedRow() >= 0)
				selectedSystem = systems.get(systemTable.getSelectedRow());
			else {
				editSysButton.setEnabled(false); delSysButton.setEnabled(false);
				upButton.setEnabled(false); downButton.setEnabled(false);
			}
			if(modifyIndex == 0)
				upButton.setEnabled(false);
			if(modifyIndex == systems.size() - 1)
				downButton.setEnabled(false);
		}
		
	}
}
