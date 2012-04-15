package com.bao.lc.site.s3.gui;

import java.awt.BorderLayout;
import java.awt.Dimension;

import javax.swing.*;
import javax.swing.border.TitledBorder;

import com.bao.lc.ResMgr;

@SuppressWarnings("serial")
public class MainPanel extends JPanel
{
	private JToolBar toolBar = null;

	private InputInfoPanel inputInfoPanel = new InputInfoPanel();

	private JScrollPane msgWindowScrollPane = new JScrollPane();
	private JTextArea msgWindow = new JTextArea();

	public MainPanel()
	{
		initGUI();
	}

	private void initGUI()
	{
		initToolBar();
		initMessageWindow();
		initMainPanel();
	}

	private void initToolBar()
	{
		toolBar = new JToolBar();

		JButton addPassenger = new JButton(ResMgr.getString("td.main.toolbar.add_passenger"));
		toolBar.add(addPassenger);

		JButton startLogin = new JButton(ResMgr.getString("td.main.toolbar.start_login"));
		toolBar.add(startLogin);

		JButton startBook = new JButton(ResMgr.getString("td.main.toolbar.start_book"));
		toolBar.add(startBook);

		JButton stop = new JButton(ResMgr.getString("td.main.toolbar.stop"));
		toolBar.add(stop);
	}

	private void initMessageWindow()
	{
		msgWindowScrollPane.setViewportBorder(new TitledBorder(ResMgr
			.getString("td.message.window.title")));
		msgWindowScrollPane.getViewport().add(msgWindow);
	}

	private void initMainPanel()
	{
		// Prepare the main split panel
		JSplitPane mainSplitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT, inputInfoPanel,
			msgWindowScrollPane);
		mainSplitPane.setContinuousLayout(true);
		mainSplitPane.setOneTouchExpandable(true);

		Dimension minSize = inputInfoPanel.getMinimumSize();
		minSize.height += 100;
		inputInfoPanel.setMinimumSize(minSize);
		mainSplitPane.setDividerLocation(minSize.height + mainSplitPane.getDividerSize());

		// set the layout
		setLayout(new BorderLayout());
		add(toolBar, BorderLayout.PAGE_START);
		add(mainSplitPane, BorderLayout.CENTER);
	}
}
