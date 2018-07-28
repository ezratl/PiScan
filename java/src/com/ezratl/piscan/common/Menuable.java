package com.ezratl.piscan.common;

import java.util.List;

public interface Menuable {
	public String			getDescriptor();
	public Menuable			getKey(int index);
	public List<String>		getKeyDescriptors();
	public List<Menuable>	getKeys();
	public Menuable			getSelectedKey();
	public boolean			isMenu();
	public boolean			isModifiable();
	public boolean			isSelectable();
	public int				numberOfKeys();
	public boolean			selectKey(int index);
	public boolean			setKey(int index, Menuable value);
}
