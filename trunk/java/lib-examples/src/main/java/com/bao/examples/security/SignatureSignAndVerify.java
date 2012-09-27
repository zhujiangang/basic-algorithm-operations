package com.bao.examples.security;

import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.SecureRandom;
import java.security.Signature;

//http://www.java2s.com/Code/Java/Security/SignatureSignAndVerify.htm
public class SignatureSignAndVerify
{
	public static void main(String args[]) throws Exception
	{
		KeyPair keyPair = generateKeyPair(999);

		byte[] data = { 65, 66, 67, 68, 69, 70, 71, 72, 73, 74 };
		byte[] digitalSignature = signData(data, keyPair.getPrivate());

		boolean verified;

		verified = verifySig(data, keyPair.getPublic(), digitalSignature);
		System.out.println(verified);

		keyPair = generateKeyPair(888);
		verified = verifySig(data, keyPair.getPublic(), digitalSignature);
		System.out.println(verified);

	}

	public static byte[] signData(byte[] data, PrivateKey key) throws Exception
	{
		Signature signer = Signature.getInstance("SHA1withDSA");
		signer.initSign(key);
		signer.update(data);
		return (signer.sign());
	}

	public static boolean verifySig(byte[] data, PublicKey key, byte[] sig) throws Exception
	{
		Signature signer = Signature.getInstance("SHA1withDSA");
		signer.initVerify(key);
		signer.update(data);
		return (signer.verify(sig));

	}

	public static KeyPair generateKeyPair(long seed) throws Exception
	{
		KeyPairGenerator keyGenerator = KeyPairGenerator.getInstance("DSA");
		SecureRandom rng = SecureRandom.getInstance("SHA1PRNG", "SUN");
		rng.setSeed(seed);
		keyGenerator.initialize(1024, rng);

		return (keyGenerator.generateKeyPair());
	}
}
