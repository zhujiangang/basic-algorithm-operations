package com.e2u.gui;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class GUIFramework
{
    public static void show(JPanel panel)
    {
        JFrame frame = new JFrame("show");
        frame.getContentPane().add(panel);
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        frame.setSize(600, 450);
        frame.setVisible(true);
    }
    /*
     * @param args
     */
    public static void main(String[] args)
    {
        show(new GridBagPanel());
    }
}