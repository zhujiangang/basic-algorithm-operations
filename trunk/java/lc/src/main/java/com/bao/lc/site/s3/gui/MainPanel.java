package com.bao.lc.site.s3.gui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JToggleButton;
import javax.swing.JToolBar;

import com.bao.lc.ResMgr;

@SuppressWarnings("serial")
public class MainPanel extends JPanel
{

	public MainPanel()
	{

		// set the layout
		setLayout(new BorderLayout());

		initGUI();
	}

	private void initGUI()
	{
		initToolBar();
	}

	private void initToolBar()
	{
		JToolBar toolbar = new JToolBar();
		addButtons(toolbar);

		add(toolbar, BorderLayout.PAGE_START);
	}

	protected void addButtons(JToolBar toolBar) {
		JButton button = new JButton("Add Passenger");
	    toolBar.add(button);

	    JButton btn2 = new JButton("Start Login");
	    toolBar.add(btn2);
	}
}
