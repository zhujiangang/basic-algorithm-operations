package com.bao.lc.site.s3.gui;

import java.awt.BorderLayout;
import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import com.bao.lc.ResMgr;

public class TdClientGUI
{
	private JFrame mainFrame = null;
	private MainPanel mainPanel = null;
	
	public TdClientGUI()
	{
		initGUI();
		
		Runtime.getRuntime().addShutdownHook(mainPanel.getAppShutdownHook());
	}
	
	private void initGUI()
	{
		mainFrame = new JFrame(ResMgr.getString("td.main.window.title"));
		mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		mainFrame.setLayout(new BorderLayout());
		
		mainPanel = new MainPanel();
		mainFrame.getContentPane().add(mainPanel, BorderLayout.CENTER);
		mainFrame.setJMenuBar(mainPanel.getMainMenuBar());
	}
	
	public JFrame getMainFrame()
	{
		return this.mainFrame;
	}
	
	public void show()
	{
		SwingUtilities.invokeLater(new Runnable()
		{
			public void run()
			{
				mainFrame.setPreferredSize(new Dimension(800, 640));
				mainFrame.pack();
				GUIUtils.centerInScreen(mainFrame);
				mainFrame.setVisible(true);
			}
		});
	}
	
	public static void main(String[] args)
	{
		UIManager.put("swing.boldMetal", Boolean.FALSE);
		
		TdClientGUI gui = new TdClientGUI();
		GUIUtils.setMainFrame(gui.getMainFrame());
		
		gui.show();
	}
}
