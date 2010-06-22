package com.e2u.misc.image;

/**
 * <p>Title:BmpReader</p>
 * <p>Copyright:Copyright (c) 2005</p>
 * <p>Company:www.b9527.net</p>
 * @author robertb9527
 * @version 1.0
 */

import java.io.*;
import java.awt.*;
import java.awt.image.*;
import javax.swing.*;
import com.sun.image.codec.jpeg.*;

public class BmpReader {
	public void reader(String file) {
		try {
			FileInputStream in = new FileInputStream(file);
			Image TheImage = read(in);
			int wideth = TheImage.getWidth(null);
			int height = TheImage.getHeight(null);
			BufferedImage tag = new BufferedImage(wideth / 2, height / 2,
					BufferedImage.TYPE_INT_RGB);
			tag.getGraphics().drawImage(TheImage, 0, 0, wideth / 2, height / 2,
					null);
			FileOutputStream out = new FileOutputStream(file + ".jpg");
			JPEGImageEncoder encoder = JPEGCodec.createJPEGEncoder(out);
			encoder.encode(tag);
			out.close();
		} catch (Exception e) {
			System.out.println(e);
		}
	}

	public static int constructInt(byte[] in, int offset) {
		int ret = ((int) in[offset + 3] & 0xff);
		ret = (ret << 8) | ((int) in[offset + 2] & 0xff);
		ret = (ret << 8) | ((int) in[offset + 1] & 0xff);
		ret = (ret << 8) | ((int) in[offset + 0] & 0xff);
		return (ret);
	}

	public static int constructInt3(byte[] in, int offset) {
		int ret = 0xff;
		ret = (ret << 8) | ((int) in[offset + 2] & 0xff);
		ret = (ret << 8) | ((int) in[offset + 1] & 0xff);
		ret = (ret << 8) | ((int) in[offset + 0] & 0xff);
		return (ret);
	}

	public static long constructLong(byte[] in, int offset) {
		long ret = ((long) in[offset + 7] & 0xff);
		ret |= (ret << 8) | ((long) in[offset + 6] & 0xff);
		ret |= (ret << 8) | ((long) in[offset + 5] & 0xff);
		ret |= (ret << 8) | ((long) in[offset + 4] & 0xff);
		ret |= (ret << 8) | ((long) in[offset + 3] & 0xff);
		ret |= (ret << 8) | ((long) in[offset + 2] & 0xff);
		ret |= (ret << 8) | ((long) in[offset + 1] & 0xff);
		ret |= (ret << 8) | ((long) in[offset + 0] & 0xff);
		return (ret);
	}

	public static double constructDouble(byte[] in, int offset) {
		long ret = constructLong(in, offset);
		return (Double.longBitsToDouble(ret));
	}

	public static short constructShort(byte[] in, int offset) {
		short ret = (short) ((short) in[offset + 1] & 0xff);
		ret = (short) ((ret << 8) | (short) ((short) in[offset + 0] & 0xff));
		return (ret);
	}

	static class BitmapHeader {
		public int iSize, ibiSize, iWidth, iHeight, iPlanes, iBitcount,
				iCompression, iSizeimage, iXpm, iYpm, iClrused, iClrimp;

		// 读取bmp文件头信息
		public void read(FileInputStream fs) throws IOException {
			final int bflen = 14;
			byte bf[] = new byte[bflen];
			fs.read(bf, 0, bflen);
			final int bilen = 40;
			byte bi[] = new byte[bilen];
			fs.read(bi, 0, bilen);
			iSize = constructInt(bf, 2);
			ibiSize = constructInt(bi, 2);
			iWidth = constructInt(bi, 4);
			iHeight = constructInt(bi, 8);
			iPlanes = constructShort(bi, 12);
			iBitcount = constructShort(bi, 14);
			iCompression = constructInt(bi, 16);
			iSizeimage = constructInt(bi, 20);
			iXpm = constructInt(bi, 24);
			iYpm = constructInt(bi, 28);
			iClrused = constructInt(bi, 32);
			iClrimp = constructInt(bi, 36);
		}
	}

	public static Image read(FileInputStream fs) {
		try {
			BitmapHeader bh = new BitmapHeader();
			bh.read(fs);
			if (bh.iBitcount == 24) {
				return (readImage24(fs, bh));
			}
			if (bh.iBitcount == 32) {
				return (readImage32(fs, bh));
			}
			fs.close();
		} catch (IOException e) {
			System.out.println(e);
		}
		return (null);
	}

	// 24位
	protected static Image readImage24(FileInputStream fs, BitmapHeader bh)
			throws IOException {
		Image image;
		if (bh.iSizeimage == 0) {
			bh.iSizeimage = ((((bh.iWidth * bh.iBitcount) + 31) & ~31) >> 3);
			bh.iSizeimage *= bh.iHeight;
		}
		int npad = (bh.iSizeimage / bh.iHeight) - bh.iWidth * 3;
		int ndata[] = new int[bh.iHeight * bh.iWidth];
		byte brgb[] = new byte[(bh.iWidth + npad) * 3 * bh.iHeight];
		fs.read(brgb, 0, (bh.iWidth + npad) * 3 * bh.iHeight);
		int nindex = 0;
		for (int j = 0; j < bh.iHeight; j++) {
			for (int i = 0; i < bh.iWidth; i++) {
				ndata[bh.iWidth * (bh.iHeight - j - 1) + i] = constructInt3(
						brgb, nindex);
				nindex += 3;
			}
			nindex += npad;
		}
		image = Toolkit.getDefaultToolkit().createImage(
				new MemoryImageSource(bh.iWidth, bh.iHeight, ndata, 0,
						bh.iWidth));
		fs.close();
		return (image);
	}

	// 32位
	protected static Image readImage32(FileInputStream fs, BitmapHeader bh)
			throws IOException {
		Image image;
		int xwidth = bh.iSizeimage / bh.iHeight;
		int ndata[] = new int[bh.iHeight * bh.iWidth];
		byte brgb[] = new byte[bh.iWidth * 4 * bh.iHeight];
		fs.read(brgb, 0, bh.iWidth * 4 * bh.iHeight);
		int nindex = 0;
		for (int j = 0; j < bh.iHeight; j++) {
			for (int i = 0; i < bh.iWidth; i++) {
				ndata[bh.iWidth * (bh.iHeight - j - 1) + i] = constructInt3(
						brgb, nindex);
				nindex += 4;
			}
		}
		image = Toolkit.getDefaultToolkit().createImage(
				new MemoryImageSource(bh.iWidth, bh.iHeight, ndata, 0,
						bh.iWidth));
		fs.close();
		return (image);
	}

	public static Image load(String sdir, String sfile) {
		return (load(sdir + sfile));
	}

	public static Image load(String sdir) {
		try {
			FileInputStream fs = new FileInputStream(sdir);
			return (read(fs));
		} catch (IOException ex) {
			return (null);
		}
	}

	public static ImageDialog getBmpImage(String filePath) throws IOException {
		if (filePath.equals("")) {
			System.out.println("输入bmp文件名");
			return null;
		} else {
			FileInputStream in = new FileInputStream(filePath);
			Image TheImage = read(in);
			ImageDialog container = new ImageDialog(new ImageIcon(TheImage));
			return container;
		}
	}

	public static ImageIcon getBmpImageIcon(String filePath) throws IOException {
		if (filePath.equals("")) {
			System.err.println("输入bmp文件名");
			return null;
		} else {
			FileInputStream in = new FileInputStream(filePath);
			Image TheImage = read(in);
			return new ImageIcon(TheImage);
		}
	}

	public static void main(String[] args) {
		try {
			ImageDialog dlg = getBmpImage("C:\\diskf\\data\\images\\x2.bmp");
			dlg.setVisible(true);
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
	
	
	
	
	
	
	
}
