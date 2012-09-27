package com.bao.examples.security;

import java.io.*;
import java.security.*;
import java.security.cert.*;
import sun.security.x509.X509CertInfo;
import sun.security.x509.X509CertImpl;
import sun.security.x509.X500Name;
import sun.security.x509.CertificateIssuerName;

/**
 * http://silo.cs.indiana.edu:8346/horstmann/v2ch09/CertificateSigner/CertificateSigner.java
 * This program signs a certificate, using the private key of another
 * certificate in a keystore.
 * 
 * @version 1.01 2007-10-07
 * @author Cay Horstmann
 */
public class CertificateSigner
{
	private static boolean sign(String ksname, String alias, String inname, String outname,
		char[] storepass, char[] keypass, StringBuilder sb) throws Exception
	{
		if(ksname == null || alias == null || inname == null || outname == null
			|| storepass == null || keypass == null)
		{
			setErrorMsg(sb, usage());
			return false;
		}

		// load key store
		KeyStore store = KeyStore.getInstance("JKS", "SUN");
		InputStream in = new FileInputStream(ksname);
		store.load(in, storepass);
		in.close();

		// get private key of the alias
		PrivateKey issuerPrivateKey = (PrivateKey) store.getKey(alias, keypass);
		if(issuerPrivateKey == null)
		{
			setErrorMsg(sb, "No such private key");
			return false;
		}

		// load the public cert file
		CertificateFactory factory = CertificateFactory.getInstance("X.509");
		InputStream inCertFile = new FileInputStream(inname);
		X509Certificate inCert = (X509Certificate) factory.generateCertificate(inCertFile);
		inCertFile.close();

		// Gets the DER-encoded certificate information
		byte[] inCertBytes = inCert.getTBSCertificate();

		// Get the issuer cert - private key
		X509Certificate issuerCert = (X509Certificate) store.getCertificate(alias);
		Principal issuer = issuerCert.getSubjectDN();
		String issuerSigAlg = issuerCert.getSigAlgName();

		// Generate the public cert info - X509
		X509CertInfo info = new X509CertInfo(inCertBytes);
		// replace the issuer by the current signer
		info.set(X509CertInfo.ISSUER, new CertificateIssuerName((X500Name) issuer));
		X509CertImpl outCert = new X509CertImpl(info);
		// sign it
		outCert.sign(issuerPrivateKey, issuerSigAlg);

		// Out put to file
		FileOutputStream out = new FileOutputStream(outname);
		outCert.derEncode(out);
		out.close();

		return true;
	}

	public static void main(String[] args)
	{
		String ksname = null; // the keystore name
		String alias = null; // the private key alias
		String inname = null; // the input file name
		String outname = null; // the output file name
		char[] storepass = null;
		char[] keypass = null;
		for(int i = 0; i < args.length - 1; i += 2)
		{
			if(args[i].equals("-keystore"))
				ksname = args[i + 1];
			else if(args[i].equals("-alias"))
				alias = args[i + 1];
			else if(args[i].equals("-infile"))
				inname = args[i + 1];
			else if(args[i].equals("-outfile"))
				outname = args[i + 1];
			else if(args[i].equals("-storepass"))
				storepass = args[i + 1].toCharArray();
			else if(args[i].equals("-keypass"))
				keypass = args[i + 1].toCharArray();
			else
				exit(usage());
		}

		StringBuilder sb = new StringBuilder();
		try
		{
			if(!sign(ksname, alias, inname, outname, storepass, keypass, sb))
			{
				exit(sb.toString());
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}

	}
	
	private static String usage()
	{
		return "Usage: java CertificateSigner" + " -keystore keyStore -alias issuerKeyAlias"
			+ " -infile inputFile -outfile outputFile -storepass password -keypass password";
	}

	private static void setErrorMsg(StringBuilder sb, String errorMsg)
	{
		sb.setLength(0);
		sb.append(errorMsg);
	}
	
	/**
	 * Prints a usage message and exits.
	 */
	private static void exit(String str)
	{
		System.out.println(str);
		System.exit(1);
	}
}
