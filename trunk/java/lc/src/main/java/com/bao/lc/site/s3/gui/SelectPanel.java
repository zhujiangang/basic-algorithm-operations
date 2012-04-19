package com.bao.lc.site.s3.gui;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.ResMgr;
import com.bao.lc.site.s3.TdUtils;

@SuppressWarnings("serial")
public class SelectPanel extends JPanel implements ValueBean<List>
{
	private static Log log = LogFactory.getLog(SelectPanel.class);
	
	private List<?> rawData = null;
	private List<Integer> checkedList = null;
	
	private List selData = new ArrayList();
	private List currValue = null;

	private JPanel selectedItemPanel = null;
	private JTextField selectedField = null;

	private JPanel optionsPanel = null;

	private List<JCheckBox> checkboxs = null;
	private JButton selectAllBtn = null;
	private JButton unselectAllBtn = null;

	private JPanel confirmBtnPanel = null;
	private JButton okBtn = null;
	private JButton closeBtn = null;

	public SelectPanel(List<?> dataList, List<Integer> checkedList)
	{
		this.rawData = dataList;
		this.checkedList = checkedList;
		
		initGUI();
		addListeners();
		initData();
	}

	private void initGUI()
	{
		initSelectedItemPanel();
		initOptionsPanel();
		initConfirmBtnPanel();

		this.setLayout(new BorderLayout());

		this.add(selectedItemPanel, BorderLayout.NORTH);
		this.add(optionsPanel, BorderLayout.CENTER);
		this.add(confirmBtnPanel, BorderLayout.SOUTH);
	}

	private void initSelectedItemPanel()
	{
		selectedField = new JTextField();
		selectedField.setEditable(false);

		selectedItemPanel = new JPanel();
		selectedItemPanel.setLayout(new GridBagLayout());

		JLabel selItemsLabel = new JLabel(ResMgr.getString("td.selected_items"));
		selectedItemPanel.add(selItemsLabel, new GridBagConstraints(0, 0, 1, 1, 0.0, 1.0,
			GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(2, 5, 3, 5), 0, 0));
		selectedItemPanel
			.add(selectedField, new GridBagConstraints(1, 0, 1, 1, 1.0, 1.0,
				GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(2, 10, 3, 10),
				0, 0));

		selectedItemPanel.setBorder(new TitledBorder(ResMgr.getString("td.selected_items")));
	}

	private void initOptionsPanel()
	{
		JPanel checkboxPanel = new JPanel();
		checkboxPanel.setLayout(new FlowLayout(FlowLayout.LEFT, 7, 7));

		checkboxs = new ArrayList<JCheckBox>(rawData.size());
		for(Object option : rawData)
		{
			String text = option.toString();

			JCheckBox checkbox = new JCheckBox(text);
			checkboxPanel.add(checkbox);

			checkboxs.add(checkbox);
		}

		JPanel selectBtnPanel = new JPanel();
		selectAllBtn = new JButton(ResMgr.getString("td.select_all"));
		unselectAllBtn = new JButton(ResMgr.getString("td.unselect_all"));

		selectBtnPanel.setLayout(new FlowLayout(FlowLayout.LEFT, 7, 7));
		selectBtnPanel.add(selectAllBtn);
		selectBtnPanel.add(unselectAllBtn);

		optionsPanel = new JPanel();
		optionsPanel.setLayout(new BorderLayout());
		optionsPanel.add(selectBtnPanel, BorderLayout.NORTH);
		optionsPanel.add(checkboxPanel, BorderLayout.CENTER);

		optionsPanel.setBorder(new TitledBorder(ResMgr.getString("td.options")));
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
		for(int i = 0, size = checkboxs.size(); i < size; i++)
		{
			final int index = i;
			final JCheckBox checkbox = checkboxs.get(i);

			checkbox.addActionListener(new ActionListener()
			{
				public void actionPerformed(ActionEvent e)
				{
					if(checkbox.isSelected())
					{
						selData.add(rawData.get(index));
					}
					else
					{
						selData.remove(rawData.get(index));
					}
					
					updateSelectedField();
				}
			});
		}
		
		selectAllBtn.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				for(int i = 0, size = checkboxs.size(); i < size; i++)
				{
					final JCheckBox checkbox = checkboxs.get(i);
					
					if(!checkbox.isSelected())
					{
						checkbox.setSelected(true);
						
						selData.add(rawData.get(i));
					}
				}
				updateSelectedField();
			}
		});
		unselectAllBtn.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{	
				selData.clear();
				
				for(int i = 0, size = checkboxs.size(); i < size; i++)
				{
					final JCheckBox checkbox = checkboxs.get(i);
					checkbox.setSelected(false);
				}
				
				updateSelectedField();
			}
		});
		
		okBtn.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				List newValue = new ArrayList(selData.size());
				newValue.addAll(selData);
				
				setValue(newValue);
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
	
	private void initData()
	{
		if(checkedList != null)
		{
			for(int i : checkedList)
			{
				if(i < 0 || i >= checkboxs.size())
				{
					log.error("checkedList contains invalid index: " + i);
					continue;
				}
				checkboxs.get(i).setSelected(true);
				
				selData.add(rawData.get(i));
			}
			updateSelectedField();
		}
	}
	
	private void updateSelectedField()
	{		
		//Then update the model data to field of the view
		if(selData.isEmpty())
		{
			selectedField.setText("");
			return;
		}
		selectedField.setText(TdUtils.toString(selData, ","));
	}
	
	@Override
	public void setValue(List newValue)
	{
		List oldValue = currValue;
		currValue = newValue;
		firePropertyChange(VALUE_PROPERTY, oldValue, currValue);
	}

	@Override
	public List getValue()
	{
		return currValue;
	}
}
