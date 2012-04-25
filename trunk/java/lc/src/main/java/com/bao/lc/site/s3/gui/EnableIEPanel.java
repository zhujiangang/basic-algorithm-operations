package com.bao.lc.site.s3.gui;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;

import com.bao.lc.ResMgr;

@SuppressWarnings("serial")
public class EnableIEPanel extends JPanel implements ValueBean<String>
{
	private JPanel messagePanel = null;
	
	private JPanel confirmBtnPanel = null;
	private JButton closeBtn = null;
	
	public EnableIEPanel()
	{
		initGUI();
		addListeners();
	}
	
	private void initGUI()
	{
		initMessagePanel();
		initConfirmBtnPanel();
		
		this.setLayout(new BorderLayout());

		this.add(messagePanel, BorderLayout.CENTER);
		this.add(confirmBtnPanel, BorderLayout.SOUTH);
	}
	
	private void initMessagePanel()
	{
		messagePanel = new JPanel();
		JLabel messageLabel = new JLabel(ResMgr.getString("td.msg.enable_ie"));
		
		JTextField urlField = new JTextField(ResMgr.getString("td.msg.suggest_url"));
		urlField.setEditable(false);
		
		messagePanel.setLayout(new BorderLayout());

		messagePanel.add(messageLabel, BorderLayout.NORTH);
		messagePanel.add(urlField, BorderLayout.SOUTH);
		
		messagePanel.setBorder(new TitledBorder(ResMgr.getString("td.msg.hint")));
	}
	private void initConfirmBtnPanel()
	{
		closeBtn = new JButton(ResMgr.getString("td.close"));

		confirmBtnPanel = new JPanel();
		confirmBtnPanel.setLayout(new FlowLayout(FlowLayout.CENTER, 7, 7));
		confirmBtnPanel.add(closeBtn);
	}
	private void addListeners()
	{
		closeBtn.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				setValue(null);
			}
		});
	}
	
	@Override
	public void setValue(String newValue)
	{
		firePropertyChange(VALUE_PROPERTY, null, newValue);
	}

	@Override
	public String getValue()
	{
		return null;
	}
}
