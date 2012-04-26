package com.bao.lc.site.s3.gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.Properties;
import java.util.Vector;

import javax.swing.DefaultCellEditor;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.border.TitledBorder;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableColumn;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.ResMgr;
import com.bao.lc.site.s3.TdUtils;
import com.bao.lc.site.s3.bean.GenericBean;
import com.bao.lc.site.s3.bean.PassengerInfo;
import com.bao.lc.site.s3.bean.TicketFilterCondition;
import com.bao.lc.site.s3.params.InputParameter;
import com.bao.lc.util.AppUtils;
import com.toedter.calendar.JDateChooser;

@SuppressWarnings({ "rawtypes", "serial" })
public class InputInfoPanel extends JPanel
{
	private static Log log = LogFactory.getLog(InputInfoPanel.class);
	
	private static final DateFormat ticketDateFormat = new SimpleDateFormat("yyyy-MM-dd");
	
	private static final Insets DEFAULT_INSECT_LABEL = new Insets(2, 5, 3, 5);
	private static final Insets DEFAULT_INSECT_FIELD = new Insets(2, 10, 3, 10);

	private JPanel requiredPanel = new JPanel();
	private JTextField userTextField = new JTextField();
	private JTextField pwdTextField = new JPasswordField();

	private JPanel ticketCondPanel = new JPanel();
	private JTextField fromStation = new JTextField();
	private JTextField toStation = new JTextField();
//	private JTextField ticketDate = null;
	private JDateChooser ticketDateChooser = null;

	private JTextField seatClass = new JTextField(80);
	private JButton seatSelectBtn = new JButton(ResMgr.getString("td.seat_select"));

	private JPanel optionalPanel = new JPanel();
	private JTextField trainClass = new JTextField(80);
	private JButton trainClassSelectBtn = new JButton(ResMgr.getString("td.train_class_select"));

	private JTextField ticketNo = new JTextField(80);
	private JCheckBox ticketNoFixCheckBox = new JCheckBox(ResMgr.getString("td.fix_train_no"));

	private JScrollPane passengerInfoPanel = new JScrollPane();
	private DefaultTableModel passengerTableModel = null;
	private JTable passengerTable = null;
	
	private JPopupMenu tableRightMenu = null;
	private JMenuItem tableRemoveMenuItem = null;

	private static final int COL_PASSENGER_INFO = 0;
	private static final int COL_SELECT = 1;
	private static final int COL_CARD_TYPE = 3;
	private static final int COL_TICKET_TYPE = 6;
	private static final int COL_TICKET_SAVE = 7;

	public InputInfoPanel()
	{
		initGUI();
		addListeners();
		initData();
	}

	private void initPassengerInfoPanel()
	{
		Vector columnNames = new Vector();
		columnNames.add("passenger.info");
		columnNames.add(ResMgr.getString("td.passenger.select"));
		columnNames.add(ResMgr.getString("td.passenger.name"));
		columnNames.add(ResMgr.getString("td.passenger.cardtype"));
		columnNames.add(ResMgr.getString("td.passenger.cardno"));
		columnNames.add(ResMgr.getString("td.passenger.phone"));
		columnNames.add(ResMgr.getString("td.passenger.tickettype"));
		columnNames.add(ResMgr.getString("td.passenger.issave"));

		int[] columnWidths = { 0, 50, 100, 100, 180, 125, 100, 110 };

		passengerTableModel = new DefaultTableModel((Vector) null, (Vector) columnNames)
		{
			@Override
			public boolean isCellEditable(int row, int col)
			{
				return (col == COL_SELECT);
			}
		};
		passengerTable = new JTable(passengerTableModel);
		passengerTable.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
		passengerTable.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
		
		// Show colors by rendering them in their own color.
		DefaultTableCellRenderer defaultRenderer = new DefaultTableCellRenderer()
		{
			public Component getTableCellRendererComponent(JTable table, Object value,
				boolean isSelected, boolean hasFocus, int row, int column)
			{
				Component c = super.getTableCellRendererComponent(table, value, isSelected,
					hasFocus, row, column);
				if(value == null)
				{
					return c;
				}
				// Convert column from view to model
				column = table.convertColumnIndexToModel(column);
				if(column == COL_CARD_TYPE)
				{
					List<GenericBean> cardTypes = TdUtils.getCardTypes();
					String cardTypeName = GenericBean.getField(String.valueOf(value), 1, 0,
						cardTypes);
					setValue(cardTypeName);
				}
				else if(column == COL_TICKET_TYPE)
				{
					List<GenericBean> ticketTypes = TdUtils.getTicketTypes();
					String ticketTypeName = GenericBean.getField(String.valueOf(value), 1, 0,
						ticketTypes);
					setValue(ticketTypeName);
				}
				else if(column == COL_TICKET_SAVE)
				{
					List<GenericBean> saveList = TdUtils.getSaveList();
					String saveName = GenericBean.getField(String.valueOf(value), 1, 0, saveList);
					setValue(saveName);
				}
				return c;
			}
		};
		
		CheckBoxCellRenderer selectRenderer = new CheckBoxCellRenderer();
		DefaultCellEditor selectCellEditor = new DefaultCellEditor(new JCheckBox());
		TableColumn column = null;
		for(int i = 0, columnCount = passengerTable.getColumnCount(); i < columnCount; i++)
		{
			column = passengerTable.getColumnModel().getColumn(i);

			if(i < columnWidths.length)
			{
				column.setPreferredWidth(columnWidths[i]);
			}
			 if(i == COL_SELECT)
			 {
				 column.setCellRenderer(selectRenderer);
				 column.setCellEditor(selectCellEditor);
			 }
			 else
			 {
				 column.setCellRenderer(defaultRenderer);
			 }
		}

		column = passengerTable.getColumnModel().getColumn(COL_PASSENGER_INFO);
		passengerTable.removeColumn(column);

		passengerInfoPanel.getViewport().add(passengerTable);
		
		tableRightMenu = new JPopupMenu();
		tableRemoveMenuItem = new JMenuItem(ResMgr.getString("td.remove"));
		tableRightMenu.add(tableRemoveMenuItem);
	}

	private void initTicketCondPanel()
	{
		ticketCondPanel.setLayout(new GridBagLayout());

		requiredPanel.setLayout(new GridBagLayout());
		// user and password
		JLabel userLabel = new JLabel(ResMgr.getString("td.user_name"));
		requiredPanel.add(userLabel, new GridBagConstraints(0, 0, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		requiredPanel.add(userTextField, new GridBagConstraints(1, 0, 1, 1, 1.0, 1.0,
			GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD, 0, 0));

		JLabel pwdLabel = new JLabel(ResMgr.getString("td.password"));
		requiredPanel.add(pwdLabel, new GridBagConstraints(2, 0, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		requiredPanel.add(pwdTextField, new GridBagConstraints(3, 0, 1, 1, 1.0, 1.0,
			GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD, 0, 0));

		// requiredPanel.add(new JLabel(""), new GridBagConstraints(4, 0, 2, 1,
		// 1.0, 1.0,
		// GridBagConstraints.EAST, GridBagConstraints.HORIZONTAL,
		// DEFAULT_INSECT_LABEL, 0, 0));

		// from/to
		JLabel fromStationLabel = new JLabel(ResMgr.getString("td.label.from_station"));
		requiredPanel.add(fromStationLabel, new GridBagConstraints(0, 1, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		requiredPanel.add(fromStation, new GridBagConstraints(1, 1, 1, 1, 1.0, 1.0,
			GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD, 0, 0));

		JLabel toStationLabel = new JLabel(ResMgr.getString("td.label.to_station"));
		requiredPanel.add(toStationLabel, new GridBagConstraints(2, 1, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		requiredPanel.add(toStation, new GridBagConstraints(3, 1, 1, 1, 1.0, 1.0,
			GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD, 0, 0));

		//Date
		JLabel ticketDateLabel = new JLabel(ResMgr.getString("td.label.ticket_date"));
		requiredPanel.add(ticketDateLabel, new GridBagConstraints(4, 1, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		/*
		 * Replaced by JDateChooser
		MaskFormatter mf = null;
		try
		{
			mf = new MaskFormatter("####-##-##");
		}
		catch(ParseException e)
		{
			log.error("MaskFormatter init failed.", e);
		}
		if(mf != null)
		{
			ticketDate = new JFormattedTextField(mf);
		}
		else
		{
			ticketDate = new JTextField();
		}
		requiredPanel.add(ticketDate, new GridBagConstraints(5, 1, 1, 1, 1.0, 1.0,
			GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD, 0, 0));
		*/
		ticketDateChooser = new JDateChooser();
		ticketDateChooser.setLocale(Locale.SIMPLIFIED_CHINESE);
		ticketDateChooser.setDateFormatString("yyyy-MM-dd");
		requiredPanel.add(ticketDateChooser, new GridBagConstraints(5, 1, 1, 1, 1.0, 1.0,
			GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD, 0, 0));
		
		// seat class
		JLabel seatClassLabel = new JLabel(ResMgr.getString("td.label.seat_class"));
		requiredPanel.add(seatClassLabel, new GridBagConstraints(0, 2, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		requiredPanel.add(seatClass, new GridBagConstraints(1, 2, GridBagConstraints.RELATIVE, 1,
			1.0, 1.0, GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD,
			0, 0));
		requiredPanel.add(seatSelectBtn, new GridBagConstraints(5, 2, GridBagConstraints.REMAINDER,
			1, 0.0, 1.0, GridBagConstraints.WEST, GridBagConstraints.VERTICAL,
			DEFAULT_INSECT_FIELD, 0, 0));
		requiredPanel.setBorder(new TitledBorder(null, ResMgr.getString("td.required_parameter"),
			TitledBorder.LEADING, TitledBorder.DEFAULT_POSITION, null, Color.RED));

		optionalPanel.setLayout(new GridBagLayout());
		// train class
		JLabel trainClassLabel = new JLabel(ResMgr.getString("td.label.train_class"));
		optionalPanel.add(trainClassLabel, new GridBagConstraints(0, 0, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		optionalPanel.add(trainClass, new GridBagConstraints(1, 0, GridBagConstraints.RELATIVE, 1,
			1.0, 1.0, GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD,
			0, 0));
		optionalPanel.add(trainClassSelectBtn, new GridBagConstraints(5, 0,
			GridBagConstraints.REMAINDER, 1, 0.0, 1.0, GridBagConstraints.WEST,
			GridBagConstraints.VERTICAL, DEFAULT_INSECT_FIELD, 0, 0));

		// train no
		JLabel ticketNoLabel = new JLabel(ResMgr.getString("td.label.ticket_no"));
		optionalPanel.add(ticketNoLabel, new GridBagConstraints(0, 1, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, DEFAULT_INSECT_LABEL, 0, 0));
		optionalPanel.add(ticketNo, new GridBagConstraints(1, 1, GridBagConstraints.RELATIVE, 1,
			1.0, 1.0, GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, DEFAULT_INSECT_FIELD,
			0, 0));
		optionalPanel.add(ticketNoFixCheckBox, new GridBagConstraints(5, 1,
			GridBagConstraints.REMAINDER, 1, 0.0, 1.0, GridBagConstraints.WEST,
			GridBagConstraints.VERTICAL, DEFAULT_INSECT_FIELD, 0, 0));
		optionalPanel.setBorder(new TitledBorder(null, ResMgr.getString("td.optional_parameter"),
			TitledBorder.LEADING, TitledBorder.DEFAULT_POSITION, null, Color.BLUE));

		ticketCondPanel.add(requiredPanel, new GridBagConstraints(0, 0, 1, 1, 1.0, 1.0,
			GridBagConstraints.WEST, GridBagConstraints.BOTH, DEFAULT_INSECT_LABEL, 0, 0));
		ticketCondPanel.add(optionalPanel, new GridBagConstraints(0, 1, 1, 1, 1.0, 1.0,
			GridBagConstraints.WEST, GridBagConstraints.BOTH, DEFAULT_INSECT_LABEL, 0, 0));
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
				List<String> seatClassList = TdUtils.getSeatClassNameList();
				List<Integer> selectedIndexList = TdUtils.toIndexList(seatClass.getText(), ",",
					seatClassList);

				SelectPanel contentPanel = new SelectPanel(seatClassList, selectedIndexList);

				DialogValueBuilder<List> builder = new DialogValueBuilder<List>();
				builder.content(contentPanel).valueBean(contentPanel).owner(GUIUtils.getMainFrame());
				builder.title(ResMgr.getString("td.seat_select"));
				builder.preferredSize(new Dimension(640, 320));
				builder.parent(GUIUtils.getMainFrame());

				List selectedItems = builder.build();
				if(selectedItems != null)
				{
					seatClass.setText(TdUtils.toString(selectedItems, ","));
				}
			}
		});

		trainClassSelectBtn.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				List<String> trainClassList = TdUtils.getTrainClassNameList();

				List<Integer> selectedIndexList = TdUtils.toIndexList(trainClass.getText(), ",",
					trainClassList);

				SelectPanel contentPanel = new SelectPanel(trainClassList, selectedIndexList);

				DialogValueBuilder<List> builder = new DialogValueBuilder<List>();
				builder.content(contentPanel).valueBean(contentPanel).owner(GUIUtils.getMainFrame());
				builder.title(ResMgr.getString("td.train_class_select"));
				builder.preferredSize(new Dimension(640, 320));
				builder.parent(GUIUtils.getMainFrame());

				List selectedItems = builder.build();
				if(selectedItems != null)
				{
					trainClass.setText(TdUtils.toString(selectedItems, ","));
				}
			}
		});
		
		passengerTable.addMouseListener(new MouseAdapter()
		{
			public void mouseClicked(MouseEvent e)
			{
				if(!SwingUtilities.isRightMouseButton(e))
				{
					return;
				}
				if(passengerTable.getSelectedRowCount() > 0)
				{
					tableRightMenu.show(passengerTable, e.getX(), e.getY());
				}
			}
		});
		
		tableRemoveMenuItem.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				int[] rows = passengerTable.getSelectedRows();
				if(rows.length <= 0)
				{
					return;
				}
				
				Vector dataVec = passengerTableModel.getDataVector();
				for(int i = rows.length - 1; i >= 0; i--)
				{
					dataVec.remove(rows[i]);
				}
				
				passengerTableModel.fireTableDataChanged();
			}
		});
	}

	private void initData()
	{
		seatClass.setEditable(false);
		trainClass.setEditable(false);
		
		this.loadData(AppUtils.getUserFilePath("auto_input.xml"));
	}

	public void addPassenger()
	{
		PassengerPanel pane = new PassengerPanel(null);

		DialogValueBuilder<PassengerInfo> pb = new DialogValueBuilder<PassengerInfo>();

		pb.content(pane).valueBean(pane).owner(GUIUtils.getMainFrame());
		pb.title(ResMgr.getString("td.main.toolbar.add_passenger"));
		pb.preferredSize(new Dimension(480, 320));
		pb.parent(GUIUtils.getMainFrame());

		PassengerInfo passenger = pb.build();
		if(passenger == null)
		{
			return;
		}
		Vector rowData = addPassengerToTable(passenger);
		
		Vector dataVec = passengerTableModel.getDataVector();
		dataVec.add(rowData);

		passengerTableModel.fireTableDataChanged();
	}

	private Vector addPassengerToTable(PassengerInfo passenger)
	{
		Vector rowData = new Vector();
		rowData.add(passenger);
		rowData.add(Boolean.TRUE);
		rowData.add(passenger.name);
		rowData.add(passenger.cardType);
		rowData.add(passenger.cardNo);
		rowData.add(passenger.phone);
		rowData.add(passenger.tiketType);
		rowData.add(passenger.isSave);

		return rowData;
	}
	
	public InputParameter getInputParam()
	{
		InputParameter param = new InputParameter();
		param.user = GUIUtils.getFieldText(userTextField);
		param.pwd = GUIUtils.getFieldText(pwdTextField);
		param.fromStation = GUIUtils.getFieldText(fromStation);
		param.toStation = GUIUtils.getFieldText(toStation);
		param.ticketDate = getTicketDateStr();
		
		param.passengers = getPassengerList();
		param.filterCond = getFilterCondition();
		
		return param;
	}
	
	public boolean checkParameter(InputParameter param, boolean showWarningDlg)
	{
		boolean result = false;
		do
		{
			result = checkField(StringUtils.isEmpty(param.user),
				ResMgr.getString("td.input.warning.user_empty"), showWarningDlg);
			if(!result)
			{
				break;
			}

			result = checkField(StringUtils.isEmpty(param.pwd),
				ResMgr.getString("td.input.warning.pwd_empty"), showWarningDlg);
			if(!result)
			{
				break;
			}

			result = checkField(StringUtils.isEmpty(param.fromStation),
				ResMgr.getString("td.input.warning.from_station_empty"), showWarningDlg);
			if(!result)
			{
				break;
			}

			result = checkField(StringUtils.isEmpty(param.toStation),
				ResMgr.getString("td.input.warning.to_station_empty"), showWarningDlg);
			if(!result)
			{
				break;
			}

			result = checkField(StringUtils.isEmpty(param.ticketDate),
				ResMgr.getString("td.input.warning.ticket_date_empty"), showWarningDlg);
			if(!result)
			{
				break;
			}

			result = checkField(param.passengers == null || param.passengers.isEmpty(),
				ResMgr.getString("td.input.warning.passenger_empty"), showWarningDlg);
			if(!result)
			{
				break;
			}

			// Seat Class
			boolean isFilterCond = param.filterCond == null
				|| param.filterCond.seatClassList == null
				|| param.filterCond.seatClassList.isEmpty();
			result = checkField(isFilterCond,
				ResMgr.getString("td.input.warning.seat_class_empty"), showWarningDlg);
			if(!result)
			{
				break;
			}

			result = true;
		}
		while(false);

		return result;
	}

	private boolean checkField(boolean invalid, String message, boolean showWarningDlg)
	{
		if(invalid && showWarningDlg)
		{
			JOptionPane.showMessageDialog(this.getParent(), message,
				ResMgr.getString("td.input.warning.dialog.title"), JOptionPane.WARNING_MESSAGE);
		}
		return !invalid;
	}
	
	public boolean saveData(InputParameter parameter, String fileName)
	{
		Properties prop = new Properties();
		parameter.save(prop);
		
		try
		{
			prop.storeToXML(new FileOutputStream(fileName), "Input Parameters", "UTF-8");
		}
		catch(Exception e)
		{
			log.error("Failed to save file: " + fileName, e);
			return false;
		}
		return true;
	}
	public void loadData(String fileName)
	{
		File file = new File(fileName);
		if(!file.exists())
		{
			log.info("The file doesn't exist: " + fileName);
			return;
		}
		Properties prop = new Properties();
		try
		{
			prop.loadFromXML(new FileInputStream(file));
		}
		catch(Exception e)
		{
			log.error("Failed to load file: " + fileName, e);
			return;
		}
		
		InputParameter parameter = new InputParameter();
		parameter.load(prop);
		
		udpateView(parameter);
	}
	
	private void udpateView(final InputParameter parameter)
	{
		Runnable runner = new Runnable()
		{
			public void run()
			{
				userTextField.setText(parameter.user);
				pwdTextField.setText(parameter.pwd);
				fromStation.setText(parameter.fromStation);
				toStation.setText(parameter.toStation);
				setTicketDateStr(parameter.ticketDate);
				
				if(parameter.passengers != null && !parameter.passengers.isEmpty())
				{
					Vector dataVec = passengerTableModel.getDataVector();
					for(PassengerInfo passenger : parameter.passengers)
					{
						Vector rowData = addPassengerToTable(passenger);
						dataVec.add(rowData);
					}
					passengerTableModel.fireTableDataChanged();
				}
				
				if(parameter.filterCond == null)
				{
					return;
				}
				
				//Seat Class
				if(parameter.filterCond.seatClassList != null)
				{
					List<GenericBean> seatClassList = TdUtils.getSeatClasses();
					List<String> seatClassNameList = new ArrayList<String>(parameter.filterCond.seatClassList.size());
					for(String seatClassIndex : parameter.filterCond.seatClassList)
					{
						String seatClassName = GenericBean.getField(seatClassIndex, 0, 1, seatClassList);
						seatClassNameList.add(seatClassName);
					}
					seatClass.setText(TdUtils.toString(seatClassNameList, TdUtils.DEFAULT_FIELD_DELIM));			
				}
				
				//Train Class
				if(parameter.filterCond.trainClassList != null)
				{
					List<GenericBean> trainClassList = TdUtils.getTrainClasses();
					List<String> trainClassNameList = new ArrayList<String>(parameter.filterCond.trainClassList.size());
					for(String trainClassValue : parameter.filterCond.trainClassList)
					{
						String trainClassName = GenericBean.getField(trainClassValue, 1, 0, trainClassList);
						trainClassNameList.add(trainClassName);
					}
					trainClass.setText(TdUtils.toString(trainClassNameList, TdUtils.DEFAULT_FIELD_DELIM));	
				}
				
				if(parameter.filterCond.trainNoList != null)
				{
					ticketNo.setText(TdUtils.toString(parameter.filterCond.trainNoList, TdUtils.DEFAULT_FIELD_DELIM));
				}
				
				ticketNoFixCheckBox.setSelected(parameter.filterCond.fixedTrainNo);
			}
		};
		
		runner.run();
//		if(SwingUtilities.isEventDispatchThread())
//		{
//			
//		}
//		else
//		{
//			SwingUtilities.invokeLater(runner);
//		}
	}
	
	private TicketFilterCondition getFilterCondition()
	{
		TicketFilterCondition cond = new TicketFilterCondition();

		//Seat Class
		List<GenericBean> seatClassList = TdUtils.getSeatClasses();
		List<String> seatClassNameList = TdUtils.toList(GUIUtils.getFieldText(seatClass), TdUtils.DEFAULT_FIELD_DELIM);
		List<String> seatClassIndexList = new ArrayList<String>(seatClassNameList.size());
		for(String seatClassName : seatClassNameList)
		{
			String index = GenericBean.getField(seatClassName, 1, 0, seatClassList);
			seatClassIndexList.add(index);
		}
		cond.seatClassList = seatClassIndexList;
		
		//Train Class
		List<GenericBean> trainClassList = TdUtils.getTrainClasses();
		List<String> trainClassNameList = TdUtils.toList(GUIUtils.getFieldText(trainClass), TdUtils.DEFAULT_FIELD_DELIM);
		List<String> trainClassValueList = new ArrayList<String>(trainClassNameList.size());
		for(String trainClassName : trainClassNameList)
		{
			String trainClassValue = GenericBean.getField(trainClassName, 0, 1, trainClassList);
			trainClassValueList.add(trainClassValue);
		}
		cond.trainClassList = trainClassValueList;
		
		//Train No List
		cond.trainNoList = TdUtils.toList(GUIUtils.getFieldText(ticketNo), TdUtils.DEFAULT_FIELD_DELIM);
		
		//Fixed train no
		cond.fixedTrainNo = ticketNoFixCheckBox.isSelected();
		
		return cond;
	}
	
	private List<PassengerInfo> getPassengerList()
	{
		List<PassengerInfo> passengers = new ArrayList<PassengerInfo>(5);
		
		Vector dataVec = passengerTableModel.getDataVector();
		
		for(int i = 0, size = dataVec.size(); i < size; i++)
		{
			Vector rowData = (Vector)dataVec.get(i);
			Boolean isSelected = (Boolean)rowData.get(COL_SELECT);
			if(isSelected == null)
			{
				log.error("The value of column [" + COL_SELECT + "] is null");
				continue;
			}
			if(isSelected.booleanValue())
			{
				passengers.add((PassengerInfo)rowData.get(COL_PASSENGER_INFO));
			}
		}
		
		return passengers;
	}
	
	private String getTicketDateStr()
	{
		Calendar calendar = ticketDateChooser.getCalendar();
		if(calendar == null)
		{
			return "";
		}
		
		return ticketDateFormat.format(calendar.getTime());
	}
	private void setTicketDateStr(String str)
	{
		if(str == null)
		{
			return;
		}
		
		Date date = null;
		try
		{
			date = ticketDateFormat.parse(str);
		}
		catch(ParseException e)
		{
			log.warn("Failed to parse the str to date: " + str, e);
		}
		if(date == null)
		{
			return;
		}
		
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		ticketDateChooser.setCalendar(calendar);
	}

	private static class CheckBoxCellRenderer extends JCheckBox implements TableCellRenderer
	{
		public Component getTableCellRendererComponent(JTable table, Object value,
			boolean isSelected, boolean hasFocus, int row, int column)
		{
			if(value instanceof Boolean)
			{
				this.setSelected((Boolean)value);
			}
			return this;
		}
	}
}
