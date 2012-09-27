package com.bao.examples.security;

import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.security.KeyStore;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.Arrays;

import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESKeySpec;

//http://www.cnblogs.com/ytfei/archive/2005/06/01/166084.html
public class EncryptSign
{
	static byte[] desKeyData = { (byte) 0x01, (byte) 0x02, (byte) 0x03, (byte) 0x04, (byte) 0x05,
		(byte) 0x06, (byte) 0x07, (byte) 0x08 };

	/**
	 * Crypt the data into outFileName
	 * 
	 * @param cipherText
	 * @param outFileName
	 */
	public static void crypt(byte[] cipherText, boolean encrypt, String outFileName)
	{
		try
		{
			DESKeySpec desKeySpec = new DESKeySpec(desKeyData);
			SecretKeyFactory keyFactory = SecretKeyFactory.getInstance("DES");
			SecretKey secretKey = keyFactory.generateSecret(desKeySpec);
			Cipher cdes = Cipher.getInstance("DES");
			if(encrypt)
			{
				cdes.init(Cipher.ENCRYPT_MODE, secretKey);
			}
			else
			{
				cdes.init(Cipher.DECRYPT_MODE, secretKey);
			}
			byte[] ct = cdes.doFinal(cipherText);
			try
			{
				FileOutputStream out = new FileOutputStream(outFileName);
				out.write(ct);
				out.close();
			}
			catch(IOException e)
			{
				e.printStackTrace();
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}

	public static void sign(byte[] sigText, String outFileName, String keystoreName, String alias,
		String keypass)
	{
		try
		{
			// Create keystore instance
			KeyStore ks = KeyStore.getInstance("JKS");

			// load data from keystore file
			BufferedInputStream bis = new BufferedInputStream(new FileInputStream(keystoreName));
			char[] password = keypass.toCharArray();
			ks.load(bis, password);
			bis.close();

			// get the private key
			PrivateKey priv = (PrivateKey) ks.getKey(alias, password);
			System.out.println("Key - alg=" + priv.getAlgorithm() + ", format=" + priv.getFormat());

			// Sign the data with the private key
			Signature rsa = Signature.getInstance(priv.getAlgorithm());
			rsa.initSign(priv);
			rsa.update(sigText);
			byte[] sig = rsa.sign();
			System.out.println("sign is done");
			try
			{
				FileOutputStream out = new FileOutputStream(outFileName);
				out.write(sig);
				out.close();
			}
			catch(IOException e)
			{
				e.printStackTrace();
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}

	public static boolean verify(byte[] updateData, byte[] sigedText, String certName)
	{
		try
		{
			// Create X509 certificate factory
			CertificateFactory certificatefactory = CertificateFactory.getInstance("X.509");

			// Load X509 certificate from file
			FileInputStream fin = new FileInputStream(certName);
			X509Certificate certificate = (X509Certificate) certificatefactory
				.generateCertificate(fin);
			fin.close();

			PublicKey pub = certificate.getPublicKey();
			System.out.println("Sign Alg Name: " + certificate.getSigAlgName());
			Signature rsa = Signature.getInstance(certificate.getSigAlgName());
			rsa.initVerify(pub);
			rsa.update(updateData);
			boolean verifies = rsa.verify(sigedText);

			System.out.println("Verify " + (verifies ? "OK" : "Failed"));
			
			return verifies;
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
		return false;
	}
	
	private static byte[] readFileContent(String file)
	{
		try
		{
			FileInputStream fis = new FileInputStream(file);
			int length = fis.available();
			byte[] buffer = new byte[length];
			
			int read = fis.read(buffer, 0, length);
			
			if(read == length)
			{
				return buffer;
			}
			System.err.println("read != length");
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		return null;
	}

	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		byte[] rawData = "Hello, World!".getBytes();
		String encyptFile = "encrypt.dat";
		
		//A encrypt the data into file
		crypt(rawData, true, encyptFile);
		byte[] encryptData = readFileContent(encyptFile);
		if(encryptData == null)
		{
			System.exit(1);
		}
		
		String keystore = "alice.certs";
		String alias = "alice";
		String keypass = "password";
		
		String signFile = "sign.data";
		//sign the encrypted data
		sign(encryptData, signFile, keystore, alias, keypass);
		byte[] sig = readFileContent(signFile);
		if(sig == null)
		{
			System.exit(1);
		}
		
		String certFile = "alice.cer";
//		certFile = "alice_signedby_root.cer";
		if(!verify(encryptData, sig, certFile))
		{
			System.exit(1);
		}
		
		String decryptFile = "decrypt.dat";
		crypt(encryptData, false, decryptFile);
		byte[] decryptedData = readFileContent(decryptFile);
		if(decryptedData == null)
		{
			System.exit(1);
		}
		
		boolean ok = Arrays.equals(rawData, decryptedData);
		System.out.println("Data decrypted: " + (ok ? "OK" : "Failed"));
	}

}
