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
import com.bao.lc.proxy.HttpProxyServer;
import com.bao.lc.proxy.IEProxy;
import com.bao.lc.proxy.IEProxy.ProxySettings;
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
	private JMenuItem saveParamMenuItem = null;

	private JToolBar toolBar = null;
	private JButton addPassengerBtn = null;
	private JButton loginBtn = null;
	private JButton bookBtn = null;
	private JButton logoutBtn = null;
	private JButton oneClickBookBtn = null;
	private JButton stopBtn = null;
	private JCheckBox enableIEBtn = null;

	private InputInfoPanel inputInfoPanel = new InputInfoPanel();

	private JScrollPane msgWindowScrollPane = new JScrollPane();
	private JTextArea msgWindow = new JTextArea();

	private Log uiLog = null;
	private WorkerThread worker = null;

	private TdClient client = null;
	private HttpProxyServer proxyServer = null;
	private ProxySettings origProxySettings = null;

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
		
		startProxyServer();
	}

	public JMenuBar getMainMenuBar()
	{
		return this.menuBar;
	}

	public Thread getAppShutdownHook()
	{
		return new SysCleanUpThread();
	}
	
	public void resoreProxySetting()
	{
		if(this.origProxySettings != null)
		{
			IEProxy.setProxySettings(this.origProxySettings);
			this.origProxySettings = null;
		}
	}
	
	private void startProxyServer()
	{
		if(!TdOptions.enableOpenIE)
		{
			log.debug("Didn't start proxy server because 'enableLoginByIE' is disabled");
			return;
		}
		String parameter = "-localport " + String.valueOf(TdOptions.proxyPort);
		proxyServer = new HttpProxyServer(parameter);
		proxyServer.start();
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
		enableIEBtn = new JCheckBox(ResMgr.getString("td.main.toolbar.enable_ie"));
		
		toolBar.add(addPassengerBtn);
		toolBar.add(oneClickBookBtn);
		toolBar.add(stopBtn);

		toolBar.addSeparator();

		toolBar.add(loginBtn);
		toolBar.add(bookBtn);
		toolBar.add(logoutBtn);

		if(TdOptions.enableOpenIE)
		{
			toolBar.addSeparator();
			toolBar.add(enableIEBtn);
		}
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

		addPassengerMenuItem.addActionListener(new AddPassengerListener());
		loginMenuItem.addActionListener(new LoginListener());
		bookMenuItem.addActionListener(new BookListener());
		logoutMenuItem.addActionListener(new LogoutListener());
		oneClickBookMenuItem.addActionListener(new OneClickBookListener());
		stopMenuItem.addActionListener(new StopListener());
		saveParamMenuItem.addActionListener(new SaveParameterListener());
		
		if(TdOptions.enableOpenIE)
		{			
			enableIEBtn.addActionListener(new EnableIEListener());
		}
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
		
		enableIEBtn.setEnabled(!isRunning && isLogin);

		saveParamMenuItem.setEnabled(!isRunning);
	}
	
	private void login()
	{
		if(client != null)
		{
			client.login();
			
			System.setProperty("RequestCookieFilter.host", TdOptions.tdHost);
			System.setProperty("RequestCookieFilter.port", String.valueOf(TdOptions.tdPort));
			System.setProperty("RequestCookieFilter.cookie", client.getCookie());
		}
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
					login();
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
					InputParameter parameter = inputInfoPanel.getInputParam();
					client.updateParameter(parameter, false);
					
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
					resoreProxySetting();
					enableIEBtn.setSelected(false);
					
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
					login();
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
	
	private class EnableIEListener implements ActionListener
	{
		public void actionPerformed(ActionEvent e)
		{
			//Enabled
			if(enableIEBtn.isSelected())
			{
				if(origProxySettings != null)
				{
					JOptionPane.showMessageDialog(GUIUtils.getMainFrame(),
						ResMgr.getString("td.msg.warning.already_enable_ie"),
						ResMgr.getString("td.main.window.title"), JOptionPane.WARNING_MESSAGE);
					return;
				}
				
				//Save original proxy setting
				origProxySettings = IEProxy.getProxySettings();
				
				//Set the new setting
				ProxySettings currSetting = new ProxySettings();
				currSetting.proxyServer = "127.0.0.1:" + TdOptions.proxyPort;
				currSetting.proxyBypass = TdOptions.proxyByPass;
				currSetting.setProxy(true);
				currSetting.setDirect(true);
				IEProxy.setProxySettings(currSetting);
				
				EnableIEPanel enableIEPanel = new EnableIEPanel();
				DialogValueBuilder<String> builder = new DialogValueBuilder<String>();
				builder.content(enableIEPanel).valueBean(enableIEPanel).owner(GUIUtils.getMainFrame());
				builder.title(ResMgr.getString("td.main.window.title"));
				builder.preferredSize(new Dimension(360, 200));
				builder.parent(GUIUtils.getMainFrame());
				builder.build();
			}
			//Disabled
			else
			{
				if(origProxySettings == null)
				{
					JOptionPane.showMessageDialog(GUIUtils.getMainFrame(),
						ResMgr.getString("td.msg.warning.not_enable_ie"),
						ResMgr.getString("td.main.window.title"), JOptionPane.WARNING_MESSAGE);
					return;
				}
				
				resoreProxySetting();
				
				JOptionPane.showMessageDialog(GUIUtils.getMainFrame(),
					ResMgr.getString("td.msg.ie_login_disabled"),
					ResMgr.getString("td.main.window.title"), JOptionPane.PLAIN_MESSAGE);
			}
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
