package com.bao.lc.site.s3.gui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;
import javax.swing.border.TitledBorder;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.ResMgr;
import com.bao.lc.site.s3.TdClient;
import com.bao.lc.site.s3.params.InputParameter;
import com.bao.lc.util.AppUtils;

@SuppressWarnings("serial")
public class MainPanel extends JPanel
{
	private static Log log = LogFactory.getLog(MainPanel.class);

	private JMenuBar menuBar = null;
	private JMenuItem addPassengerMenuItem = null;
	private JMenuItem loginMenuItem = null;
	private JMenuItem bookMenuItem = null;
	private JMenuItem logoutMenuItem = null;
	private JMenuItem oneClickBookMenuItem = null;
	private JMenuItem stopMenuItem = null;
	private JMenuItem loginByIEMenuItem = null;
	private JMenuItem saveParamMenuItem = null;

	private JToolBar toolBar = null;
	private JButton addPassengerBtn = null;
	private JButton loginBtn = null;
	private JButton bookBtn = null;
	private JButton logoutBtn = null;
	private JButton oneClickBookBtn = null;
	private JButton stopBtn = null;
	private JButton loginByIEBtn = null;

	private InputInfoPanel inputInfoPanel = new InputInfoPanel();

	private JScrollPane msgWindowScrollPane = new JScrollPane();
	private JTextArea msgWindow = new JTextArea();

	private Log uiLog = null;
	private WorkerThread worker = null;

	private TdClient client = null;

	private StateChange workerState = new StateChange(StateChange.THREAD_NOT_RUNNING);

	public MainPanel()
	{
		initGUI();
		addListeners();

		setButtonStatus(false);

		uiLog = new TextAreaLog(msgWindow);
		client = new TdClient();

		WatchDog dog = new WatchDog();
		dog.start();
	}

	public JMenuBar getMainMenuBar()
	{
		return this.menuBar;
	}

	public Thread getAppShutdownHook()
	{
		return new SysCleanUpThread();
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
		addPassengerMenuItem = createMenuItem(fileMenu,
			ResMgr.getString("td.menu.file.add_passenger"));
		oneClickBookMenuItem = createMenuItem(fileMenu,
			ResMgr.getString("td.main.toolbar.one_click_book"));
		stopMenuItem = createMenuItem(fileMenu, ResMgr.getString("td.main.toolbar.stop"));
		fileMenu.addSeparator();
		loginMenuItem = createMenuItem(fileMenu, ResMgr.getString("td.menu.file.start_login"));
		bookMenuItem = createMenuItem(fileMenu, ResMgr.getString("td.menu.file.start_book"));
		logoutMenuItem = createMenuItem(fileMenu, ResMgr.getString("td.logout"));
		fileMenu.addSeparator();
		loginByIEMenuItem = createMenuItem(fileMenu,
			ResMgr.getString("td.main.toolbar.login_by_ie"));
		fileMenu.addSeparator();
		saveParamMenuItem = createMenuItem(fileMenu, ResMgr.getString("td.menu.file.save_param"));

		// ***** create Tools menu
		JMenu toolsMenu = menuBar.add(new JMenu(ResMgr.getString("td.menu.tools")));
		createMenuItem(toolsMenu, ResMgr.getString("td.menu.tools.options"));

		// ***** create Help menu
		JMenu helpMenu = menuBar.add(new JMenu(ResMgr.getString("td.menu.help")));
		createMenuItem(helpMenu, ResMgr.getString("td.menu.help.about"));
	}

	private JMenuItem createMenuItem(JMenu menu, String text)
	{
		JMenuItem mi = menu.add(new JMenuItem(text));
		return mi;
	}

	private void initToolBar()
	{
		toolBar = new JToolBar();

		addPassengerBtn = new JButton(ResMgr.getString("td.main.toolbar.add_passenger"));
		loginBtn = new JButton(ResMgr.getString("td.main.toolbar.start_login"));
		bookBtn = new JButton(ResMgr.getString("td.main.toolbar.start_book"));
		logoutBtn = new JButton(ResMgr.getString("td.logout"));
		oneClickBookBtn = new JButton(ResMgr.getString("td.main.toolbar.one_click_book"));
		stopBtn = new JButton(ResMgr.getString("td.main.toolbar.stop"));
		loginByIEBtn = new JButton(ResMgr.getString("td.main.toolbar.login_by_ie"));

		toolBar.add(addPassengerBtn);
		toolBar.add(oneClickBookBtn);
		toolBar.add(stopBtn);

		toolBar.addSeparator();

		toolBar.add(loginBtn);
		toolBar.add(bookBtn);
		toolBar.add(logoutBtn);

		toolBar.addSeparator();

		toolBar.add(loginByIEBtn);
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
		minSize.height += 80;
		inputInfoPanel.setMinimumSize(minSize);
		mainSplitPane.setDividerLocation(minSize.height + mainSplitPane.getDividerSize());

		// set the layout
		setLayout(new BorderLayout());
		add(toolBar, BorderLayout.PAGE_START);
		add(mainSplitPane, BorderLayout.CENTER);
	}

	private void addListeners()
	{
		addPassengerBtn.addActionListener(new AddPassengerListener());
		loginBtn.addActionListener(new LoginListener());
		bookBtn.addActionListener(new BookListener());
		logoutBtn.addActionListener(new LogoutListener());
		oneClickBookBtn.addActionListener(new OneClickBookListener());
		stopBtn.addActionListener(new StopListener());
		loginByIEBtn.addActionListener(new LoginByIEListener());

		addPassengerMenuItem.addActionListener(new AddPassengerListener());
		loginMenuItem.addActionListener(new LoginListener());
		bookMenuItem.addActionListener(new BookListener());
		logoutMenuItem.addActionListener(new LogoutListener());
		oneClickBookMenuItem.addActionListener(new OneClickBookListener());
		stopMenuItem.addActionListener(new StopListener());
		loginByIEMenuItem.addActionListener(new LoginByIEListener());
		saveParamMenuItem.addActionListener(new SaveParameterListener());
	}

	private void addPassenger()
	{
		this.inputInfoPanel.addPassenger();
	}

	private boolean saveParameter()
	{
		InputParameter parameter = inputInfoPanel.getInputParam();
		return inputInfoPanel.saveData(parameter, AppUtils.getUserFilePath("auto_input.xml"));
	}

	private void setButtonStatus(boolean isRunning)
	{
		// boolean isRunning = (worker != null && worker.isAlive());
		boolean isLogin = (client != null && client.isLogin());

		addPassengerBtn.setEnabled(!isRunning);
		addPassengerMenuItem.setEnabled(!isRunning);

		loginBtn.setEnabled(!isRunning && !isLogin);
		loginMenuItem.setEnabled(!isRunning && !isLogin);

		bookBtn.setEnabled(!isRunning && isLogin);
		bookMenuItem.setEnabled(!isRunning && isLogin);

		logoutBtn.setEnabled(!isRunning && isLogin);
		logoutMenuItem.setEnabled(!isRunning && isLogin);

		oneClickBookBtn.setEnabled(!isRunning && !isLogin);
		oneClickBookMenuItem.setEnabled(!isRunning && !isLogin);

		stopBtn.setEnabled(isRunning);
		stopMenuItem.setEnabled(isRunning);

		loginByIEBtn.setEnabled(!isRunning && isLogin);
		loginByIEMenuItem.setEnabled(!isRunning && isLogin);

		saveParamMenuItem.setEnabled(!isRunning);
	}

	private class AddPassengerListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			if(worker != null && worker.isAlive())
			{
				log.error("Assert failure: worker is alive.");
				return;
			}
			addPassenger();
		}
	}

	private class LoginListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			if(worker != null && worker.isAlive())
			{
				log.error("Assert failure: worker is alive.");
				return;
			}

			if(client.isLogin())
			{
				JOptionPane.showMessageDialog(GUIUtils.getMainFrame(),
					ResMgr.getString("td.msg.user.already_login"));
				return;
			}

			InputParameter param = inputInfoPanel.getInputParam();
			if(!inputInfoPanel.checkParameter(param, true))
			{
				return;
			}
			saveParameter();

			client.initContext(param, uiLog);

			Runnable target = new Runnable()
			{
				public void run()
				{
					client.login();
				}
			};
			worker = new WorkerThread(target);
			worker.start();
		}
	}

	private class BookListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			if(worker != null && worker.isAlive())
			{
				log.error("Assert failure: worker is alive.");
				return;
			}

			if(!client.isLogin())
			{
				JOptionPane.showMessageDialog(GUIUtils.getMainFrame(),
					ResMgr.getString("td.msg.user.not_login"));
				return;
			}

			Runnable target = new Runnable()
			{
				public void run()
				{
					client.bookTicket();
				}
			};

			worker = new WorkerThread(target);
			worker.start();
		}
	}

	private class LogoutListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			if(worker != null && worker.isAlive())
			{
				log.error("Assert failure: worker is alive.");
				return;
			}

			if(!client.isLogin())
			{
				JOptionPane.showMessageDialog(GUIUtils.getMainFrame(),
					ResMgr.getString("td.msg.user.not_login"));
				return;
			}

			Runnable target = new Runnable()
			{
				public void run()
				{
					client.logout();
				}
			};
			worker = new WorkerThread(target);
			worker.start();
		}
	}

	private class OneClickBookListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			if(worker != null && worker.isAlive())
			{
				log.error("Assert failure: worker is alive.");
				return;
			}

			if(client.isLogin())
			{
				JOptionPane.showMessageDialog(GUIUtils.getMainFrame(),
					ResMgr.getString("td.msg.user.already_login"));
				return;
			}

			InputParameter param = inputInfoPanel.getInputParam();
			if(!inputInfoPanel.checkParameter(param, true))
			{
				return;
			}

			saveParameter();

			client.initContext(param, uiLog);

			Runnable target = new Runnable()
			{
				public void run()
				{
					client.login();
					client.bookTicket();
				}
			};
			worker = new WorkerThread(target);
			worker.start();
		}
	}

	private class StopListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			synchronized(workerState)
			{
				workerState.setState(StateChange.THREAD_NOT_RUNNING);
				workerState.notifyAll();
			}
			if(worker != null && !worker.isAlive())
			{
				log.error("Assert failure: worker is not alive.");
				return;
			}

			worker.end();
			worker = null;
		}
	}

	private class SaveParameterListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			boolean result = saveParameter();
			String message = ResMgr.getString("td.save.parameter")
				+ (result ? ResMgr.getString("td.success") : ResMgr.getString("td.failed"));
			JOptionPane
				.showMessageDialog(GUIUtils.getMainFrame(), message,
					ResMgr.getString("td.save.parameter.result.title"),
					JOptionPane.INFORMATION_MESSAGE);
		}
	}

	private class LoginByIEListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			// TODO
		}
	}

	private class WorkerThread extends Thread
	{
		public WorkerThread(Runnable target)
		{
			super(target, "BookWorker");
		}

		public void run()
		{
			synchronized(workerState)
			{
				workerState.setState(StateChange.THREAD_RUNNING);
				workerState.notifyAll();
			}

			super.run();

			synchronized(workerState)
			{
				workerState.setState(StateChange.THREAD_NOT_RUNNING);
				workerState.notifyAll();
			}
		}

		public void end()
		{
			if(!this.isAlive())
			{
				return;
			}
			this.interrupt();

			synchronized(workerState)
			{
				workerState.setState(StateChange.THREAD_NOT_RUNNING);
				workerState.notifyAll();
			}

			uiLog.info(ResMgr.getString("td.msg.user_kill_book_thread"));
		}
	}

	private class SysCleanUpThread extends Thread
	{
		public SysCleanUpThread()
		{
			super("SysCleanUpThread");
		}

		public void run()
		{
			if(client != null)
			{
				// client.logout();
				client.shutdown();
			}
		}
	}

	private class WatchDog extends Thread
	{
		public WatchDog()
		{
			super("WatchDog");
			setDaemon(true);
		}

		public void run()
		{
			while(true)
			{
				synchronized(workerState)
				{
					while(workerState.changed())
					{
						final boolean running = (workerState.getNewState() == StateChange.THREAD_RUNNING);
						SwingUtilities.invokeLater((new Runnable()
						{
							public void run()
							{
								setButtonStatus(running);
							}
						}));
						workerState.reset();
					}

					try
					{
						workerState.wait();
					}
					catch(InterruptedException e)
					{
						log.warn("The thread is interrupted.", e);
						Thread.currentThread().interrupt(); // very important
					}
				}
			}
		}
	}
}

class StateChange
{
	public static final int THREAD_NOT_RUNNING = 0;
	public static final int THREAD_RUNNING = 1;

	private int oldState = THREAD_NOT_RUNNING;
	private int newState = THREAD_NOT_RUNNING;

	public StateChange()
	{
		this(THREAD_NOT_RUNNING);
	}

	public StateChange(int initState)
	{
		this.oldState = initState;
		this.newState = initState;
	}

	public int getNewState()
	{
		return newState;
	}

	public boolean changed()
	{
		return oldState != newState;
	}

	public void reset()
	{
		this.oldState = this.newState;
	}

	public void setState(int state)
	{
		this.oldState = newState;
		this.newState = state;
	}
}
