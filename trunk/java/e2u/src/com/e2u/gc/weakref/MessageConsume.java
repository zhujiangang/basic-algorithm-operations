package com.e2u.gc.weakref;

import java.awt.BorderLayout;

import javax.swing.border.TitledBorder;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;

public class MessageConsume extends JPanel
{

	private String id = null;
	private JTextArea textArea = new JTextArea();

	// just for consume big memory
	private double[] data = new double[1000000];

	private MessageListener l = new MessageListener()
	{
		public void recevied(final String message)
		{
			SwingUtilities.invokeLater(new Runnable()
			{
				public void run()
				{
					textArea.append(message + "\n");
				}
			});
		}
	};

	public MessageConsume(String id)
	{
		this.id = id;
		this.setBorder(new TitledBorder(id));
		this.setLayout(new BorderLayout());
		this.add(textArea, BorderLayout.CENTER);

		MessagePublisher.addMessageListener(l);
	}

	public void finalize()
	{
		System.out.println(id + " is finalized");
	}
}
