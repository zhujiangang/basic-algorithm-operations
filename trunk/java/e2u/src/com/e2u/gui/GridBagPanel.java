package com.e2u.gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class GridBagPanel extends JPanel
{
	private GridLocator locator = new GridLocator(4);
    private JLabel serverLabel = new JLabel("Server:");
    private JTextField serverField = new JTextField();
    private JLabel serverDescriptionLabel = new JLabel("Server Address");

    private JLabel portLabel = new JLabel("Port:");
    private JTextField portField = new JTextField();
    private JLabel portDescriptionLabel = new JLabel("Port Number");

    private JButton submitBtn = new JButton("Submit");
    private JButton resetBtn = new JButton("Reset");
    private JButton defaultBtn = new JButton("Default");

    public GridBagPanel()
    {
        initGUI();
        addListeners();
        setDefaultValues();
    }

    private void initGUI()
    {
        JPanel inputPanel = new JPanel();
        inputPanel.setLayout(new GridBagLayout());
        
        inputPanel.add(serverLabel, new GridBagConstraints(locator.getGridX(), locator.getGridY(), 1, 1, 0.0, 0.0,
            GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(10, 5, 0, 5), 0, 0));
        inputPanel.add(createRedStarLabel(), new GridBagConstraints(locator.getGridX(), locator.getGridY(), 1, 1, 0.0, 0.0,
            GridBagConstraints.EAST, GridBagConstraints.HORIZONTAL, new Insets(10, 15, 0, 5), 0, 0)); 
        inputPanel.add(serverField, new GridBagConstraints(locator.getGridX(), locator.getGridY(), 1, 1, 1.0, 0.0,
            GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(10, 5, 0, 5), 0, 0));        
        inputPanel.add(serverDescriptionLabel, new GridBagConstraints(locator.getGridX(), locator.getGridY(), 1, 1, 0.0, 0.0,
            GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(10, 0, 0, 15), 0, 0));
        
        inputPanel.add(portLabel, new GridBagConstraints(locator.getGridX(), locator.getGridY(), 1, 1, 0.0, 0.0,
            GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(5, 5, 0, 5), 0, 0));
        inputPanel.add(createRedStarLabel(), new GridBagConstraints(locator.getGridX(), locator.getGridY(), 1, 1, 0.0, 0.0,
            GridBagConstraints.EAST, GridBagConstraints.HORIZONTAL, new Insets(5, 15, 0, 5), 0, 0));
        inputPanel.add(portField, new GridBagConstraints(locator.getGridX(), locator.getGridY(), 1, 1, 1.0, 0.0,
            GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(5, 5, 0, 5), 0, 0));
        inputPanel.add(portDescriptionLabel, new GridBagConstraints(locator.getGridX(), locator.getGridY(), 1, 1, 0.0, 0.0,
            GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(5, 0, 0, 15), 0, 0));
        
        JPanel btnPanel = new JPanel();
        btnPanel.setLayout(new FlowLayout());
        btnPanel.add(submitBtn, new FlowLayout(FlowLayout.CENTER, 7, 7));
        btnPanel.add(resetBtn, new FlowLayout(FlowLayout.CENTER, 7, 7));
        btnPanel.add(defaultBtn, new FlowLayout(FlowLayout.CENTER, 7, 7));
        
        this.setLayout(new BorderLayout());
        this.add(inputPanel, BorderLayout.NORTH);
        this.add(btnPanel, BorderLayout.SOUTH);
    }
    

    private void addListeners()
    {

    }

    private void setDefaultValues()
    {

    }
    
    private JLabel createRedStarLabel()
    {
        JLabel starLabel = new JLabel("*");
        Font defaultFont = starLabel.getFont();
        Font font = new Font(defaultFont.getName(), Font.BOLD, defaultFont.getSize() + 5);
        starLabel.setFont(font);
        starLabel.setForeground(Color.RED);
        return starLabel;
    }
}
