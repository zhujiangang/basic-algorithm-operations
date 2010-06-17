package com.e2u.thread;

public class SeqThread {

	private static int index = 0;
	private static int threadCount = 0;

	static class MyThread extends Thread {
		private char c;
		private static Object lock = new Object();

		public MyThread(char c) {
			this.c = c;
		}

		public void run() {
			for (int i = 0; i < 100;) {
				synchronized (lock) {
					if (index % threadCount != (int) (c - 'A')) {
						try {
							lock.wait();
						} catch (Exception e) {
							e.printStackTrace();
							return;
						}
					} else {
						System.out.print(c);
						index = (index + 1) % threadCount;
						if(index == 0)
						{
							System.out.println();
						}
						i++;
						lock.notifyAll();
					}
				}
			}
		}
	}

	public static void testThreads() {
		threadCount = 3;
		Thread[] threads = new Thread[threadCount];
		for (int i = 0; i < threadCount; i++) {
			threads[i] = new MyThread((char) ('A' + i));
		}
		for (int i = 0; i < threadCount; i++) {
			threads[i].start();
		}
	}

	public static void main(String[] args) {

		testThreads();
	}
}
