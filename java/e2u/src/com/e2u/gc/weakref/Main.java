package com.e2u.gc.weakref;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

//From:
//http://twaver.servasoft.com/?p=418
public class Main
{

	public static int consumeId = 0;

	public static void main(String[] args)
	{
		final JFrame frame = new JFrame("sailing8036");
		frame.setSize(300, 500);
		frame.setVisible(true);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		Thread thread = new Thread(new Runnable()
		{
			public void run()
			{
				while(true)
				{
					try
					{
						SwingUtilities.invokeAndWait(new Runnable()
						{
							public void run()
							{
								String id = "Consume " + consumeId++;
								MessageConsume consume = new MessageConsume(id);
								frame.setContentPane(consume);
							}
						});
					}
					catch(Exception e)
					{
						e.printStackTrace();
					}
					try
					{
						Thread.sleep(1000);
					}
					catch(Exception e)
					{
						e.printStackTrace();
					}
				}
			}
		});
		thread.setPriority(Thread.MAX_PRIORITY);
		thread.setDaemon(true);
		thread.start();
	}

}
