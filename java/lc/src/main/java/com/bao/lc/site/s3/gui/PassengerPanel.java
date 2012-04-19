package com.bao.lc.site.s3.gui;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;

import com.bao.lc.ResMgr;
import com.bao.lc.site.s3.TdUtils;
import com.bao.lc.site.s3.bean.GenericBean;
import com.bao.lc.site.s3.bean.PassengerInfo;

@SuppressWarnings("serial")
public class PassengerPanel extends JPanel implements ValueBean<PassengerInfo>
{
	private static final Insets DEFAULT_INSECT_LABEL = new Insets(2, 5, 3, 5);
	private static final Insets DEFAULT_INSECT_FIELD = new Insets(2, 10, 3, 10);

	private PassengerInfo rawPassenger = null;
	private PassengerInfo resultPassenger = null;

	private JPanel fieldsPanel = new JPanel();

	private JTextField nameField = new JTextField();
	private JComboBox cardTypeComboBox = new JComboBox();
	private JTextField cardnoField = new JTextField();
	private JComboBox ticketTypeComboBox = new JComboBox();
	private JTextField phoneField = new JTextField();
	private JComboBox saveTypeComboBox = new JComboBox();

	private JPanel confirmBtnPanel = null;
	private JButton okBtn = null;
	private JButton closeBtn = null;

	public PassengerPanel(PassengerInfo passenger)
	{
		this.rawPassenger = passenger;

		initGUI();
		addListeners();
		initData();
	}

	private void initGUI()
	{
		initFieldsPanel();
		initConfirmBtnPanel();

		this.setLayout(new BorderLayout());

		this.add(fieldsPanel, BorderLayout.NORTH);
		this.add(confirmBtnPanel, BorderLayout.SOUTH);
	}

	private void initFieldsPanel()
	{
		fieldsPanel.setLayout(new GridBagLayout());

		int row = 0;
		// Name
		JLabel nameLabel = new JLabel(ResMgr.getString("td.passenger.name"));
		addPair(row++, nameLabel, nameField);

		// CardType
		List<GenericBean> cardTypes = TdUtils.getCardTypes();
		for(GenericBean bean : cardTypes)
		{
			cardTypeComboBox.addItem(bean);
		}
		JLabel cardTypeLabel = new JLabel(ResMgr.getString("td.passenger.cardtype"));
		addPair(row++, cardTypeLabel, cardTypeComboBox);

		// CardNo
		JLabel cardnoLabel = new JLabel(ResMgr.getString("td.passenger.cardno"));
		addPair(row++, cardnoLabel, cardnoField);

		// TicketType
		List<GenericBean> ticketTypes = TdUtils.getTicketTypes();
		for(GenericBean bean : ticketTypes)
		{
			ticketTypeComboBox.addItem(bean);
		}
		JLabel ticketTypeLabel = new JLabel(ResMgr.getString("td.passenger.tickettype"));
		addPair(row++, ticketTypeLabel, ticketTypeComboBox);

		// Phone
		JLabel phoneLabel = new JLabel(ResMgr.getString("td.passenger.phone"));
		addPair(row++, phoneLabel, phoneField);

		// Save?
		List<GenericBean> saveList = TdUtils.getSaveList();
		for(GenericBean bean : saveList)
		{
			saveTypeComboBox.addItem(bean);
		}
		JLabel saveLabel = new JLabel(ResMgr.getString("td.passenger.issave"));
		addPair(row++, saveLabel, saveTypeComboBox);

		fieldsPanel.setBorder(new TitledBorder(ResMgr.getString("td.passenger.info")));
	}

	private void addPair(int row, JLabel label, JComponent component)
	{
		fieldsPanel.add(label, new GridBagConstraints(0, row, 1, 1, 0.0, 0.0,
			GridBagConstraints.NORTHEAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		fieldsPanel.add(component, new GridBagConstraints(1, row, 1, 1, 1.0, 0.0,
			GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD, 0, 0));
	}

	private void initConfirmBtnPanel()
	{
		okBtn = new JButton(ResMgr.getString("td.ok"));
		closeBtn = new JButton(ResMgr.getString("td.close"));

		confirmBtnPanel = new JPanel();
		confirmBtnPanel.setLayout(new FlowLayout(FlowLayout.CENTER, 7, 7));
		confirmBtnPanel.add(okBtn);
		confirmBtnPanel.add(closeBtn);
	}

	private void addListeners()
	{
		okBtn.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				okBtnActionPerformed();
			}
		});

		closeBtn.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				setValue(null);
			}
		});
	}

	private void okBtnActionPerformed()
	{
		String name = GUIUtils.getFieldText(nameField);
		if(name.isEmpty())
		{
			showWarningDialog(ResMgr.getString("td.passenger.warning.name.empty"));
			return;
		}

		String cardno = GUIUtils.getFieldText(cardnoField);
		if(cardno.isEmpty())
		{
			showWarningDialog(ResMgr.getString("td.passenger.warning.cardno.empty"));
			return;
		}

		String phone = GUIUtils.getFieldText(phoneField);
		if(phone.isEmpty())
		{
			showWarningDialog(ResMgr.getString("td.passenger.warning.phone.empty"));
			return;
		}

		PassengerInfo passenger = new PassengerInfo();
		passenger.name = name;
		passenger.cardType = getSelectedItemValue(cardTypeComboBox, 1);
		passenger.cardNo = cardno;
		passenger.tiketType = getSelectedItemValue(ticketTypeComboBox, 1);
		passenger.phone = phone;
		passenger.isSave = getSelectedItemValue(saveTypeComboBox, 1);

		setValue(passenger);
	}
	
	private void showWarningDialog(String message)
	{
		JOptionPane.showMessageDialog(this, message,
			ResMgr.getString("td.passenger.warning.dialog.title"), JOptionPane.WARNING_MESSAGE);
	}

	private void initData()
	{
		if(this.rawPassenger == null)
		{
			return;
		}

		nameField.setText(rawPassenger.name);

		// CardType
		setComboBoxSelected(cardTypeComboBox, rawPassenger.cardType, 1, TdUtils.getCardTypes());
		cardnoField.setText(rawPassenger.cardNo);

		// TicketType
		setComboBoxSelected(ticketTypeComboBox, rawPassenger.tiketType, 1, TdUtils.getTicketTypes());

		phoneField.setText(rawPassenger.phone);

		// Save Type
		setComboBoxSelected(saveTypeComboBox, rawPassenger.isSave, 1, TdUtils.getSaveList());
	}

	private void setComboBoxSelected(JComboBox comboBox, String key, int keyIndex,
		List<GenericBean> beans)
	{
		GenericBean bean = GenericBean.get(key, keyIndex, beans);
		if(bean != null)
		{
			comboBox.setSelectedItem(bean);
		}
		else
		{
			comboBox.setSelectedIndex(0);
		}
	}

	private String getSelectedItemValue(JComboBox comboBox, int keyIndex)
	{
		GenericBean bean = (GenericBean) comboBox.getSelectedItem();
		if(bean == null)
		{
			return null;
		}
		return bean.getField(keyIndex);
	}

	@Override
	public void setValue(PassengerInfo newValue)
	{
		PassengerInfo oldValue = resultPassenger;
		resultPassenger = newValue;
		firePropertyChange(VALUE_PROPERTY, oldValue, resultPassenger);
	}

	@Override
	public PassengerInfo getValue()
	{
		return resultPassenger;
	}
}
