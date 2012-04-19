package com.bao.lc.site.s3.gui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import com.bao.lc.ResMgr;

@SuppressWarnings("serial")
public class MainFrame extends JFrame
{
	private JMenuBar menuBar = null;
	private MainPanel mainPanel = null;

	public MainFrame()
	{
		super(ResMgr.getString("td.main.window.title"));
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		// set the layout
		setLayout(new BorderLayout());
		
		mainPanel = new MainPanel();
		getContentPane().add(mainPanel, BorderLayout.CENTER);
		initMenuBar();
	}

	public void showInScreenCenter()
	{
		// Show the demo. Must do this on the GUI thread using invokeLater.
		SwingUtilities.invokeLater(new Runnable()
		{
			public void run()
			{
				JFrame f = MainFrame.this;
				f.pack();

				Rectangle screenRect = f.getGraphicsConfiguration().getBounds();
				Insets screenInsets = Toolkit.getDefaultToolkit().getScreenInsets(
					f.getGraphicsConfiguration());

				// Make sure we don't place the demo off the screen.
				int centerWidth = screenRect.width < f.getSize().width ? screenRect.x
					: screenRect.x + screenRect.width / 2 - f.getSize().width / 2;
				int centerHeight = screenRect.height < f.getSize().height ? screenRect.y
					: screenRect.y + screenRect.height / 2 - f.getSize().height / 2;

				centerHeight = centerHeight < screenInsets.top ? screenInsets.top : centerHeight;

				f.setLocation(centerWidth, centerHeight);
				f.setVisible(true);
			}
		});

	}

	private void initMenuBar()
	{
		menuBar = new JMenuBar();

		// ***** create File menu
		JMenu fileMenu = menuBar.add(new JMenu(ResMgr.getString("td.menu.file")));
		createMenuItem(fileMenu, ResMgr.getString("td.menu.file.add_passenger"), new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				mainPanel.addPassenger();
			}
		});
		createMenuItem(fileMenu, ResMgr.getString("td.menu.file.start_login"), new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				
			}
		});
		createMenuItem(fileMenu, ResMgr.getString("td.menu.file.start_book"), new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				
			}
		});
		
		createMenuItem(fileMenu, ResMgr.getString("td.menu.file.save_param"), new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				boolean result = mainPanel.saveParameter();
				String message = ResMgr.getString("td.save.parameter")
					+ (result ? ResMgr.getString("td.success") : ResMgr.getString("td.failed"));
				JOptionPane.showMessageDialog(getParent(), message,
					ResMgr.getString("td.save.parameter.result.title"), JOptionPane.PLAIN_MESSAGE);
			}
		});

		// ***** create Tools menu
		JMenu toolsMenu = menuBar.add(new JMenu(ResMgr.getString("td.menu.tools")));
		createMenuItem(toolsMenu, ResMgr.getString("td.menu.tools.options"), new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				
			}
		});

		// ***** create Help menu
		JMenu helpMenu = menuBar.add(new JMenu(ResMgr.getString("td.menu.help")));
		createMenuItem(helpMenu, ResMgr.getString("td.menu.help.about"), new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				
			}
		});

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
		UIManager.put("swing.boldMetal", Boolean.FALSE);
		final MainFrame mainFrame = new MainFrame();
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
}
