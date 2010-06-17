package com.e2u.misc.image;

/**
 * <p>Title:ImageDialog </p>
 * <p>Copyright:Copyright (c) 2005</p>
 * <p>Company:www.b9527.net </p>
 * @author robertb9527
 * @version 1.0
 */

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class ImageDialog
    extends JDialog {
  JPanel jPanel = new JPanel();
  JLabel jLabel;
  ImageIcon imageURL = new ImageIcon();
  BorderLayout theBorderLayout = new BorderLayout();

  public ImageDialog(Frame frame, String title, boolean modal, String name) {
    super(frame, title, modal);
    this.setResizable(true);
    try {
      this.imageURL = new ImageIcon(name);
      jbInit();
      pack();
    }
    catch (Exception ex) {
      ex.printStackTrace();
    }
  }

  public ImageDialog() {
    this(null, "", false, null);
  }

  public ImageDialog(String name) {
    this(null, "", false, name);
  }

  public ImageDialog(ImageIcon example) {
    super();
    this.imageURL = example;
    try {
      jbInit();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
    pack();
  }

  private void jbInit() throws Exception {
    jPanel.setLayout(theBorderLayout);
    this.getContentPane().add(jPanel);
    jLabel = new JLabel();
    jLabel.setIcon(imageURL);
    jPanel.add(jLabel, BorderLayout.NORTH);
  }

  public ImageIcon getImage() {
    return this.imageURL;
  }

  public void setImage(ImageIcon example) {
    this.imageURL = example;
  }

  protected void processWindowEvent(WindowEvent e) {
    if (e.getID() == WindowEvent.WINDOW_CLOSING) {
      cancel();
    }
    super.processWindowEvent(e);
  }

  void cancel() {
    dispose();
  }
}
