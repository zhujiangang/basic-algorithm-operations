package com.bao.examples.tree.nma;

import java.awt.Dimension;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTree;

public class DataProducer
{
	private static final String SPLIT_PREFIX = "__";
	private static final String SPLIT_TAG = "SPLIT";
	private static final String SPLIT_STR = ".";
	
	public void produce(List<String> dataList)
	{
		String splitter = null;
		for(int i = 0; i < 3; i++)
		{
			//add splitter
			splitter = String.format("%s%s", SPLIT_PREFIX, SPLIT_TAG);
			dataList.add(splitter);
			
			//some other data
			dataList.add(String.format("[%d]", i + 1));
			
			for(int j = 0; j < 4; j++)
			{
				//add splitter
				splitter = String.format("%s%s%s%s", SPLIT_PREFIX, SPLIT_TAG, SPLIT_STR, SPLIT_TAG);
				dataList.add(splitter);
				
				//some other data
				dataList.add(String.format("[%d-%d]", i + 1, j + 1));
				
				for(int k = 0; k < 5; k++)
				{
					//add splitter
					splitter = String.format("%s%s%s%s%s%s", SPLIT_PREFIX, SPLIT_TAG, SPLIT_STR, SPLIT_TAG, SPLIT_STR, SPLIT_TAG);
					dataList.add(splitter);
					
					//some other data
					dataList.add(String.format("[%d-%d-%d]", i + 1, j + 1, k + 1));			
				}
			}
		}
	}
	
	public TreeLocator createTree(List<String> dataList)
	{
		TreeLocator treeLocator = new TreeLocator();
		
		String data;
		for(int i = 0, size = dataList.size(); i < size; i++)
		{
			data = dataList.get(i);
			if(data.startsWith(SPLIT_PREFIX))
			{
				treeLocator.addNewNode(data.substring(SPLIT_PREFIX.length()));
			}
			else
			{
				treeLocator.setCurrNodeData(data);
			}
		}
		
		//NMATreeNode treeNode = (NMATreeNode)treeLocator.getTreeModel().getRoot();
		
		return treeLocator;
	}
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		JFrame frame = new JFrame("TestTree");
		
		DataProducer producer = new DataProducer();
		
		List<String> dataList = new ArrayList<String>();
		producer.produce(dataList);
		TreeLocator treeLocator = producer.createTree(dataList);
		
		JTree tree = new JTree(treeLocator.getTreeModel());
		JScrollPane pane = new JScrollPane(tree);
		frame.getContentPane().add(pane);
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		Dimension dim = new Dimension(900, 500);
        frame.setPreferredSize(dim);
        frame.pack();
        
        frame.setVisible(true);
	}

}
