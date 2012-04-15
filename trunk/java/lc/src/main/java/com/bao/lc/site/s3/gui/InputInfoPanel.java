package com.bao.lc.site.s3.gui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableColumn;

import com.bao.lc.ResMgr;

@SuppressWarnings("rawtypes")
public class InputInfoPanel extends JPanel
{
	private static final Insets DEFAULT_INSECT_LABEL = new Insets(2, 5, 3, 5);
	private static final Insets DEFAULT_INSECT_FIELD = new Insets(2, 10, 3, 10);

	private JTextField userTextField = new JTextField();
	private JTextField pwdTextField = new JTextField();

	private JPanel ticketCondPanel = new JPanel();
	private JTextField fromStation = new JTextField(80);
	private JTextField toStation = new JTextField(80);
	private JTextField ticketDate = new JTextField(80);

	private JTextField ticketNo = new JTextField(80);
	private JTextField seatClass = new JTextField(80);
	private JButton seatSelectBtn = new JButton(ResMgr.getString("td.seat_select"));

	private JTextField trainClass = new JTextField(80);

	private JScrollPane passengerInfoPanel = new JScrollPane();
	private DefaultTableModel passengerTableModel = null;
	private JTable passengerTable = null;

	public InputInfoPanel()
	{
		initGUI();
		addListeners();
	}

	private void initPassengerInfoPanel()
	{
		Vector columnNames = new Vector();
		columnNames.add(ResMgr.getString("td.passenger.select"));
		columnNames.add(ResMgr.getString("td.passenger.name"));
		columnNames.add(ResMgr.getString("td.passenger.cardtype"));
		columnNames.add(ResMgr.getString("td.passenger.cardno"));
		columnNames.add(ResMgr.getString("td.passenger.phone"));
		columnNames.add(ResMgr.getString("td.passenger.tickettype"));
		columnNames.add(ResMgr.getString("td.passenger.issave"));

		int[] columnWidths = { 200, 100, 100, 150, 125, 200, 100 };

		passengerTableModel = new DefaultTableModel((Vector)null, (Vector)columnNames);
		passengerTable = new JTable(passengerTableModel);
		passengerTable.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);

		TableColumn column = null;
		for(int i = 0, columnCount = passengerTable.getColumnCount(); i < columnCount; i++)
		{
			column = passengerTable.getColumnModel().getColumn(i);

			if(i < columnWidths.length)
			{
				column.setPreferredWidth(columnWidths[i]);
			}
		}
		passengerInfoPanel.getViewport().add(passengerTable);
	}

	private void initTicketCondPanel()
	{
		ticketCondPanel.setLayout(new GridBagLayout());

		JLabel userLabel = new JLabel(ResMgr.getString("td.user_name"));
		ticketCondPanel.add(userLabel, new GridBagConstraints(0, 0, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		ticketCondPanel.add(userTextField, new GridBagConstraints(1, 0, 1, 1, 1.0, 1.0,
			GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD, 0, 0));

		JLabel pwdLabel = new JLabel(ResMgr.getString("td.password"));
		ticketCondPanel.add(pwdLabel, new GridBagConstraints(2, 0, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		ticketCondPanel.add(pwdTextField, new GridBagConstraints(3, 0, 1, 1, 1.0, 1.0,
			GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD, 0, 0));

		JLabel fromStationLabel = new JLabel(ResMgr.getString("td.label.from_station"));
		ticketCondPanel.add(fromStationLabel, new GridBagConstraints(0, 1, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		ticketCondPanel.add(fromStation, new GridBagConstraints(1, 1, 1, 1, 1.0, 1.0,
			GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD, 0, 0));

		JLabel toStationLabel = new JLabel(ResMgr.getString("td.label.to_station"));
		ticketCondPanel.add(toStationLabel, new GridBagConstraints(2, 1, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		ticketCondPanel.add(toStation, new GridBagConstraints(3, 1, 1, 1, 1.0, 1.0,
			GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD, 0, 0));

		JLabel ticketDateLabel = new JLabel(ResMgr.getString("td.label.ticket_date"));
		ticketCondPanel.add(ticketDateLabel, new GridBagConstraints(4, 1, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		ticketCondPanel.add(ticketDate, new GridBagConstraints(5, 1, 1, 1, 1.0, 1.0,
			GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD, 0, 0));

		JLabel ticketNoLabel = new JLabel(ResMgr.getString("td.label.ticket_no"));
		ticketCondPanel.add(ticketNoLabel, new GridBagConstraints(0, 2, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		ticketCondPanel.add(ticketNo, new GridBagConstraints(1, 2, GridBagConstraints.REMAINDER, 1,
			1.0, 1.0, GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD,
			0, 0));

		JLabel seatClassLabel = new JLabel(ResMgr.getString("td.label.seat_class"));
		ticketCondPanel.add(seatClassLabel, new GridBagConstraints(0, 3, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		ticketCondPanel.add(seatClass, new GridBagConstraints(1, 3, GridBagConstraints.RELATIVE, 1,
			1.0, 1.0, GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD,
			0, 0));
		ticketCondPanel.add(seatSelectBtn, new GridBagConstraints(5, 3,
			GridBagConstraints.REMAINDER, 1, 0.0, 1.0, GridBagConstraints.WEST,
			GridBagConstraints.NONE, DEFAULT_INSECT_FIELD, 0, 0));

		JLabel trainClassLabel = new JLabel(ResMgr.getString("td.label.train_class"));
		ticketCondPanel.add(trainClassLabel, new GridBagConstraints(0, 4, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		ticketCondPanel.add(trainClass, new GridBagConstraints(1, 4, GridBagConstraints.REMAINDER,
			1, 1.0, 1.0, GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL,
			DEFAULT_INSECT_FIELD, 0, 0));
		
		seatSelectBtn.setSize(seatSelectBtn.getWidth(), seatClass.getHeight());
	}

	private void initGUI()
	{
		initTicketCondPanel();
		initPassengerInfoPanel();

		this.setLayout(new BorderLayout());
		this.add(ticketCondPanel, BorderLayout.NORTH);
		this.add(passengerInfoPanel, BorderLayout.CENTER);

		this.setBorder(new TitledBorder(ResMgr.getString("td.user_info.border")));
	}

	private void addListeners()
	{
		seatSelectBtn.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				List<String> seatClassList = new ArrayList<String>();
				
				for(int i = 0; i < 10; i++)
				{
					seatClassList.add(String.valueOf("高级座" + i));
				}
				
				SelectDialog selDlg = new SelectDialog(null, "选座", true, seatClassList);
				selDlg.setPreferredSize(new Dimension(640, 320));
				selDlg.pack();
				GUIUtils.centerInScreen(selDlg);
				selDlg.setVisible(true);
				
				List<?> selectedItems = selDlg.getSelects();
				if(selectedItems != null)
				{
					seatClass.setText(selectedItems.toString());
				}
			}
		});
	}
}
