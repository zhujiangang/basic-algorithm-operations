package com.bao.lc.site.s3.gui;

import java.awt.Insets;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.Window;

import javax.swing.JFrame;
import javax.swing.JTextField;


public class GUIUtils
{
	private static JFrame mainFrame = null;
	
	private GUIUtils()
	{
	}

	public static void centerInScreen(Window f)
	{
		Rectangle screenRect = f.getGraphicsConfiguration().getBounds();
		Insets screenInsets = Toolkit.getDefaultToolkit().getScreenInsets(
			f.getGraphicsConfiguration());

		// Make sure we don't place the demo off the screen.
		int centerWidth = screenRect.width < f.getSize().width ? screenRect.x : screenRect.x
			+ screenRect.width / 2 - f.getSize().width / 2;
		int centerHeight = screenRect.height < f.getSize().height ? screenRect.y : screenRect.y
			+ screenRect.height / 2 - f.getSize().height / 2;

		centerHeight = centerHeight < screenInsets.top ? screenInsets.top : centerHeight;

		f.setLocation(centerWidth, centerHeight);
	}
	
	public static String getFieldText(JTextField field)
	{
		String text = field.getText();
		if(text == null)
		{
			return "";
		}
		return text.trim();
	}
	
	public static JFrame getMainFrame()
	{
		return mainFrame;
	}
	
	public static void setMainFrame(JFrame frame)
	{
		mainFrame = frame;
	}
}
