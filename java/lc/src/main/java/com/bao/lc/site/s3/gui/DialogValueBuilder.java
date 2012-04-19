package com.bao.lc.site.s3.gui;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.JComponent;
import javax.swing.JDialog;

import com.bao.lc.common.Builder;

public class DialogValueBuilder<T> implements Builder<T>
{
	// Required
	private JComponent contentComponent = null;
	private ValueBean<T> valueBean = null;

	// Optional
	private Frame owner = null;
	private String title = null;
	private boolean modal = true;
	private Component parentComponent = null;
	private Dimension preferredSize = null;

	public DialogValueBuilder<T> content(JComponent content)
	{
		this.contentComponent = content;
		return this;
	}

	public DialogValueBuilder<T> valueBean(ValueBean<T> valueBean)
	{
		this.valueBean = valueBean;
		return this;
	}

	public DialogValueBuilder<T> owner(Frame owner)
	{
		this.owner = owner;
		return this;
	}

	public DialogValueBuilder<T> title(String title)
	{
		this.title = title;
		return this;
	}

	public DialogValueBuilder<T> modal(boolean modal)
	{
		this.modal = modal;
		return this;
	}

	public DialogValueBuilder<T> parent(Component parentComponent)
	{
		this.parentComponent = parentComponent;
		return this;
	}

	public DialogValueBuilder<T> preferredSize(Dimension size)
	{
		this.preferredSize = size;
		return this;
	}

	@Override
	public T build()
	{
		T result = null;
		try
		{
			if(contentComponent == null)
			{
				throw new IllegalArgumentException("'contentComponent' can't be null");
			}
			if(valueBean == null)
			{
				throw new IllegalArgumentException("'valueBean' can't be null");
			}
			if(preferredSize == null)
			{
				preferredSize = new Dimension(480, 320);
			}

			JDialog dialog = new JDialog(owner, title, modal);
			initDialog(dialog);

			dialog.setPreferredSize(preferredSize);
			dialog.pack();
			GUIUtils.centerInScreen(dialog);
			dialog.setVisible(true);
			dialog.dispose();

			result = valueBean.getValue();
		}
		finally
		{
			reset();
		}
		return result;
	}

	private void reset()
	{
		contentComponent = null;
		valueBean = null;
		
		owner = null;
		title = null;
		modal = true;
		parentComponent = null;
		preferredSize = null;
	}

	private void initDialog(final JDialog dialog)
	{
		Container contentPane = dialog.getContentPane();

		contentPane.setLayout(new BorderLayout());
		contentPane.add(contentComponent, BorderLayout.CENTER);
		dialog.setLocationRelativeTo(parentComponent);
		WindowAdapter adapter = new WindowAdapter()
		{
			public void windowClosing(WindowEvent we)
			{
				valueBean.setValue(null);
			}
		};
		dialog.addWindowListener(adapter);
		contentComponent.addPropertyChangeListener(new PropertyChangeListener()
		{
			public void propertyChange(PropertyChangeEvent event)
			{
				// Let the defaultCloseOperation handle the closing
				// if the user closed the window without selecting a button
				// (newValue = null in that case). Otherwise, close the dialog.
				if(dialog.isVisible() && event.getSource() == contentComponent
					&& (event.getPropertyName().equals(ValueBean.VALUE_PROPERTY)))
				{
					dialog.setVisible(false);
				}
			}
		});
	}
}
