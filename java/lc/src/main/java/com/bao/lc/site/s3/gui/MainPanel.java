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

	private JToolBar toolBar = null;
	private JButton addPassenger = null;
	private JButton startLogin = null;
	private JButton startBook = null;
	private JButton logout = null;
	private JButton oneClikBook = null;
	private JButton stop = null;
	private JButton loginByIE = null;

	private InputInfoPanel inputInfoPanel = new InputInfoPanel();

	private JScrollPane msgWindowScrollPane = new JScrollPane();
	private JTextArea msgWindow = new JTextArea();

	private Log uiLog = null;
	private WorkerThread worker = null;

	private TdClient client = null;

	private static final int NOT_RUNNING = 0;
	private static final int RUNNING = 1;

	private WorkerState workerState = new WorkerState(NOT_RUNNING);

	public MainPanel()
	{
		initGUI();
		addListeners();

		setButtonStatus();

		uiLog = new TextAreaLog(msgWindow);
		client = new TdClient();

		WatchDog dog = new WatchDog();
		dog.start();
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
		createMenuItem(fileMenu, ResMgr.getString("td.menu.file.add_passenger"),
			new ActionListener()
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
					ResMgr.getString("td.save.parameter.result.title"),
					JOptionPane.INFORMATION_MESSAGE);
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

		logout = new JButton(ResMgr.getString("td.logout"));
		toolBar.add(logout);

		oneClikBook = new JButton(ResMgr.getString("td.main.toolbar.one_click_book"));
		toolBar.add(oneClikBook);

		stop = new JButton(ResMgr.getString("td.main.toolbar.stop"));
		toolBar.add(stop);
		
		loginByIE = new JButton(ResMgr.getString("td.main.toolbar.login_by_ie"));
		toolBar.add(loginByIE);
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
		startLogin.addActionListener(new ActionListener()
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

				Runnable target = getDecoratedRunner(new Runnable()
				{
					public void run()
					{
						client.login();
					}
				});
				worker = new WorkerThread(target);
				worker.start();
			}
		});
		startBook.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				if(worker != null && worker.isAlive())
				{
					return;
				}

				if(!client.isLogin())
				{
					JOptionPane.showMessageDialog(GUIUtils.getMainFrame(),
						ResMgr.getString("td.msg.user.not_login"));
					return;
				}

				Runnable target = getDecoratedRunner(new Runnable()
				{
					public void run()
					{
						client.bookTicket();
					}
				});

				worker = new WorkerThread(target);
				worker.start();
			}
		});

		logout.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				if(worker != null && worker.isAlive())
				{
					return;
				}

				if(!client.isLogin())
				{
					JOptionPane.showMessageDialog(GUIUtils.getMainFrame(),
						ResMgr.getString("td.msg.user.not_login"));
					return;
				}

				Runnable target = getDecoratedRunner(new Runnable()
				{
					public void run()
					{
						client.logout();
					}
				});
				worker = new WorkerThread(target);
				worker.start();
			}
		});
		oneClikBook.addActionListener(new ActionListener()
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

				Runnable target = getDecoratedRunner(new Runnable()
				{
					public void run()
					{
						client.login();
						client.bookTicket();
					}
				});
				worker = new WorkerThread(target);
				worker.start();
			}
		});

		stop.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				if(worker != null && !worker.isAlive())
				{
					return;
				}

				worker.end();
				worker = null;
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

	private void setButtonStatus()
	{
		boolean isRunning = (worker != null && worker.isAlive());
		boolean isLogin = (client != null && client.isLogin());
		
		addPassenger.setEnabled(!isRunning);
		startLogin.setEnabled(!isRunning && !isLogin);
		startBook.setEnabled(!isRunning && isLogin);
		logout.setEnabled(!isRunning && isLogin);
		oneClikBook.setEnabled(!isRunning && !isLogin);

		stop.setEnabled(isRunning);
		
		loginByIE.setEnabled(!isRunning && isLogin);
	}

	public JMenuBar getMainMenuBar()
	{
		return this.menuBar;
	}

	public Thread getAppShutdownHook()
	{
		return new SysCleanUpThread();
	}

	private Runnable getDecoratedRunner(final Runnable target)
	{
		Runnable runner = new Runnable()
		{
			public void run()
			{
				synchronized(workerState)
				{
					workerState.setState(RUNNING);
					workerState.notifyAll();
				}

				target.run();

				synchronized(workerState)
				{
					workerState.setState(NOT_RUNNING);
					workerState.notifyAll();
				}
			}
		};
		return runner;
	}

	private class WorkerThread extends Thread
	{
		public WorkerThread(Runnable target)
		{
			super(target, "BookWorker");
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
				workerState.setState(NOT_RUNNING);
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
//				client.logout();
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
						SwingUtilities.invokeLater((new Runnable()
						{
							public void run()
							{
								setButtonStatus();
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

	private static class WorkerState
	{
		private int oldState = NOT_RUNNING;
		private int newState = NOT_RUNNING;

		public WorkerState(int initState)
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
}
