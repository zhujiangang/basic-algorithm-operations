package com.e2u.gc.weakref;

import java.lang.ref.Reference;
import java.lang.ref.WeakReference;

public class WeakListener implements MessageListener
{

	private Reference reference = null;

	public WeakListener(MessageListener l)
	{
		reference = new WeakReference(l);
	}

	public void recevied(String message)
	{
		if(reference == null)
		{
			return;
		}
		MessageListener l = (MessageListener) reference.get();
		if(l != null)
		{
			l.recevied(message);
		}
		else
		{
			MessagePublisher.removeMessageListener(this);
			reference = null;
		}
	}
}
