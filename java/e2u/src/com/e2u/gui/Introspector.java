package com.e2u.gui;

import java.awt.BorderLayout;
import java.awt.Insets;
import java.lang.reflect.Field;
import java.util.HashSet;
import java.util.Set;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;

public class Introspector
{
    private static Set<Class> leafNodeClassSet = new HashSet<Class>();
    static
    {
        leafNodeClassSet.add(getClass("java.lang.Void"));
        leafNodeClassSet.add(getClass("java.lang.Character"));
        leafNodeClassSet.add(getClass("java.lang.Short"));
        leafNodeClassSet.add(getClass("java.lang.Integer"));
        leafNodeClassSet.add(getClass("java.lang.Long"));
        leafNodeClassSet.add(getClass("java.lang.Float"));
        leafNodeClassSet.add(getClass("java.lang.Double"));
        leafNodeClassSet.add(getClass("java.lang.String"));
    }
        
    private Class clazz = null;
    private JTree tree = null;
    private JPanel treePanel = null;
    
    private Set<Class> visitedSet = new HashSet<Class>();

    public Introspector(String className)
    {
        clazz = getClass(className);

        visitedSet.clear();
        DefaultMutableTreeNode root = introspect(clazz, "root");
        visitedSet.clear();
        tree = new JTree(root)
        {
            public Insets getInsets()
            {
                return new Insets(5, 5, 5, 5);
            }
        };

        tree.setEditable(true);
        JScrollPane scrollPane = new JScrollPane(tree);
        
        treePanel = new JPanel();
        treePanel.setLayout(new BorderLayout());
        treePanel.add(scrollPane, BorderLayout.CENTER);
    }
    public JTree getTree()
    {
        return tree;
    }
    public JPanel getTreePanel()
    {
        return treePanel;
    }

    private DefaultMutableTreeNode introspect(Class cls, String name)
    {
        DefaultMutableTreeNode node = null;
        if(cls.isArray())
        {
            node = new DefaultMutableTreeNode(name + " -- " + cls.getComponentType() + "[]");
            return node;
        }
        node = new DefaultMutableTreeNode(name + " -- " + cls.getName());
        
        if(leafNodeClassSet.contains(cls))
        {
            return node;
        }
        if(cls.isPrimitive() || cls.isInterface() || cls.isAnnotation() || cls.isEnum())
        {
            return node;
        }
        
        if(visitedSet.contains(cls))
        {
            System.out.println(cls.getName() + " visited");
            return null;
        }
        visitedSet.add(cls);

        Field[] fields = null;
        // Super class is complex type
        if (cls.getSuperclass() != null && cls.getSuperclass() != Object.class)
        {
            fields = cls.getSuperclass().getDeclaredFields();
            for (int i = 0; i < fields.length; i++)
            {
                addChild(node, introspect(fields[i].getType(), fields[i].getName()));
            }
        }
        fields = cls.getDeclaredFields();
        for (int i = 0; i < fields.length; i++)
        {
            addChild(node, introspect(fields[i].getType(), fields[i].getName()));
        }
        return node;
    }
    
    private static void addChild(DefaultMutableTreeNode parent, DefaultMutableTreeNode child)
    {
        if(child == null)
        {
            return;
        }
        parent.add(child);
    }

    private static Class getClass(String className)
    {
        Class cls = null;
        try
        {
            cls = Class.forName(className);
        }
        catch (ClassNotFoundException e)
        {
            cls = null;
            e.printStackTrace();
        }
        return cls;
    }
}
