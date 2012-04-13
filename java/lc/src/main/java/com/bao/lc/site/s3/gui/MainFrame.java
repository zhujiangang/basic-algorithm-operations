package com.bao.lc.site.s3.gui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.event.ActionListener;

import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.SwingUtilities;

import com.bao.lc.ResMgr;

@SuppressWarnings("serial")
public class MainFrame extends JFrame
{
	private JMenuBar menuBar = null;

	public MainFrame()
	{
		super(ResMgr.getString("Td.main.window.title"));
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		// set the layout
		setLayout(new BorderLayout());
		getContentPane().add(new MainPanel(), BorderLayout.CENTER);
		initMenuBar();

		// set the preferred size of the demo
		setPreferredSize(new Dimension(720, 640));
		// Show the demo. Must do this on the GUI thread using invokeLater.
		SwingUtilities.invokeLater(new Runnable()
		{
			public void run()
			{
				showMainFrame();
			}
		});

	}

	public void showMainFrame()
	{
		JFrame f = this;
		f.pack();

		Rectangle screenRect = f.getGraphicsConfiguration().getBounds();
		Insets screenInsets = Toolkit.getDefaultToolkit().getScreenInsets(
			f.getGraphicsConfiguration());

		// Make sure we don't place the demo off the screen.
		int centerWidth = screenRect.width < f.getSize().width ? screenRect.x : screenRect.x
			+ screenRect.width / 2 - f.getSize().width / 2;
		int centerHeight = screenRect.height < f.getSize().height ? screenRect.y : screenRect.y
			+ screenRect.height / 2 - f.getSize().height / 2;

		centerHeight = centerHeight < screenInsets.top ? screenInsets.top : centerHeight;

		f.setLocation(centerWidth, centerHeight);
		f.setVisible(true);
	}

	private void initMenuBar()
	{
		menuBar = new JMenuBar();

		// ***** create File menu
		JMenu fileMenu = menuBar.add(new JMenu(ResMgr.getString("FileMenu.file_label")));
		createMenuItem(fileMenu, ResMgr.getString("FileMenu.add_user"), null);
		createMenuItem(fileMenu, ResMgr.getString("FileMenu.add_user"), null);
		createMenuItem(fileMenu, ResMgr.getString("FileMenu.add_user"), null);
		createMenuItem(fileMenu, ResMgr.getString("FileMenu.add_user"), null);

		// ***** create Help menu
		JMenu helpMenu = menuBar.add(new JMenu(ResMgr.getString("FileMenu.file_label")));
		createMenuItem(helpMenu, ResMgr.getString("FileMenu.add_user"), null);
		createMenuItem(helpMenu, ResMgr.getString("FileMenu.add_user"), null);
		createMenuItem(helpMenu, ResMgr.getString("FileMenu.add_user"), null);
		createMenuItem(helpMenu, ResMgr.getString("FileMenu.add_user"), null);

		setJMenuBar(menuBar);
	}

	public JMenuItem createMenuItem(JMenu menu, String text, ActionListener action)
	{
		JMenuItem mi = menu.add(new JMenuItem(text));
		mi.addActionListener(action);
		if(action == null)
		{
			mi.setEnabled(false);
		}

		return mi;
	}

	public static void main(String[] args)
	{
		MainFrame mainFrame = new MainFrame();
	}
}
