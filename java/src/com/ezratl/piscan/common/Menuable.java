package com.ezratl.piscan.common;

import java.util.ArrayList;

public interface Menuable {
	public boolean				addKey(Menuable value);
	public String				getDescriptor();
	public Menuable				getKey(int index);
	public ArrayList<String>	getKeyDescriptors();
	public ArrayList<Menuable>	getKeys();
	public Menuable				getSelectedKey();
	public int					getSelectedKeyIndex();
	public boolean				isMenu();
	public boolean				isModifiable();
	public boolean				isSelectable();
	public int					numberOfKeys();
	public Menuable				removeKey(int index);
	public boolean				selectKey(int index);
	public boolean				setKey(int index, Menuable value);
}
