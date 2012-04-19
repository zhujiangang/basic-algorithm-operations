package com.bao.lc.site.s3.gui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;
import javax.swing.border.TitledBorder;

import org.apache.commons.logging.Log;

import com.bao.lc.ResMgr;
import com.bao.lc.site.s3.TdClient;
import com.bao.lc.site.s3.params.InputParameter;
import com.bao.lc.util.AppUtils;

@SuppressWarnings("serial")
public class MainPanel extends JPanel
{
	private JMenuBar menuBar = null;

	private JToolBar toolBar = null;
	private JButton addPassenger = null;
	private JButton startLogin = null;
	private JButton startBook = null;
	private JButton stop = null;

	private InputInfoPanel inputInfoPanel = new InputInfoPanel();

	private JScrollPane msgWindowScrollPane = new JScrollPane();
	private JTextArea msgWindow = new JTextArea();
	
	private Log uiLog = null;
	private BookThread bookThread = null;

	public MainPanel()
	{
		uiLog = new TextAreaLog(msgWindow);
		initGUI();
		addListeners();
	}

	private void initGUI()
	{
		initMenuBar();
		initToolBar();
		initMessageWindow();
		initMainPanel();
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
				addPassenger();
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
				bookActionPerformed();
			}
		});
		
		createMenuItem(fileMenu, ResMgr.getString("td.menu.file.save_param"), new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				boolean result = saveParameter();
				String message = ResMgr.getString("td.save.parameter")
					+ (result ? ResMgr.getString("td.success") : ResMgr.getString("td.failed"));
				JOptionPane.showMessageDialog(GUIUtils.getMainFrame(), message,
					ResMgr.getString("td.save.parameter.result.title"), JOptionPane.INFORMATION_MESSAGE);
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
	}
	
	private JMenuItem createMenuItem(JMenu menu, String text, ActionListener action)
	{
		JMenuItem mi = menu.add(new JMenuItem(text));
		mi.addActionListener(action);
		if(action == null)
		{
			mi.setEnabled(false);
		}

		return mi;
	}
	
	private void initToolBar()
	{
		toolBar = new JToolBar();

		addPassenger = new JButton(ResMgr.getString("td.main.toolbar.add_passenger"));
		toolBar.add(addPassenger);

		startLogin = new JButton(ResMgr.getString("td.main.toolbar.start_login"));
		toolBar.add(startLogin);

		startBook = new JButton(ResMgr.getString("td.main.toolbar.start_book"));
		toolBar.add(startBook);

		stop = new JButton(ResMgr.getString("td.main.toolbar.stop"));
		toolBar.add(stop);
	}

	private void initMessageWindow()
	{
		msgWindow.setEditable(false);
		msgWindow.setLineWrap(true);
		
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
	
	private void addListeners()
	{
		addPassenger.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				addPassenger();
			}
		});
		startBook.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				bookActionPerformed();
			}
		});
		
		stop.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				if(bookThread != null && !bookThread.isAlive())
				{
					return;
				}
				
				bookThread.end();
				bookThread = null;
			}
		});
	}

	public void addPassenger()
	{
		this.inputInfoPanel.addPassenger();
	}
	
	public boolean saveParameter()
	{
		InputParameter parameter = inputInfoPanel.getInputParam();
		return inputInfoPanel.saveData(parameter, AppUtils.getUserFilePath("auto_input.xml"));
	}
	
	private void bookActionPerformed()
	{
		if(bookThread != null && bookThread.isAlive())
		{
			return;
		}
		InputParameter param = inputInfoPanel.getInputParam();
		if(!inputInfoPanel.checkParameter(param, true))
		{
			return;
		}
		
		saveParameter();
		
		bookThread = new BookThread(param);
		bookThread.start();
	}
	
	public JMenuBar getMainMenuBar()
	{
		return this.menuBar;
	}
	
	private class BookThread extends Thread
	{
		private TdClient client = null;
		private InputParameter param = null;
		
		public BookThread(final InputParameter param)
		{
			super("BookClient");
			
			this.param = param;
		}
		public void run()
		{
			client = new TdClient();
			client.bookTicket(param, uiLog);
		}
		
		public void end()
		{
			if(client == null)
			{
				return;
			}
			
			this.interrupt();
			uiLog.info(ResMgr.getString("td.msg.user_kill_book_thread"));
		}
	}
}
