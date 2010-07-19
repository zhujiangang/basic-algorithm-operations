package com.e2u.gui;
import java.awt.Component;

import javax.swing.JFrame;
import javax.swing.JTabbedPane;

public class GUIFramework
{
    public static void show(Component panel)
    {
        JFrame frame = new JFrame("show");
        frame.getContentPane().add(panel);
        frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        frame.setSize(600, 450);
        frame.setVisible(true);
    }
    
    public static void show()
    {
        JTabbedPane tabPane = new JTabbedPane();
        tabPane.add("1", new GridBagPanel());
        tabPane.add("2", new Introspector("com.e2u.gui.GridBagPanel").getTreePanel());
        
        show(tabPane);
    }   
    
    private static class Base
    {
        String a;
        int[] b;
        Integer c;
    }
    private static class D1
    {
        String cc;
        Base[] dd;
        Short ss;
        char[] ee;
    }
    private static class D2 extends Base
    {
        D1 rr;
        short vfr;
    }
    /*
     * @param args
     */
    public static void main(String[] args)
    {
        show();
    }
}