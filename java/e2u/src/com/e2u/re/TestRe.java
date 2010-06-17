package com.e2u.re;

public class TestRe {

	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		String regex = "UNDEFINED|10{1,3}_MBIT_PER_SEC(,10{1,3}_MBIT_PER_SEC)*";
		String[] testVals = {"UNDEFINED", "123", "cd", "0", "", "5", "12", "1000"};
		
		for(int i = 0; i < testVals.length; i++)
		{
			System.out.print("REGEX=");
			System.out.print(regex);
			System.out.print(", val=");
			System.out.print(testVals[i]);
			System.out.print(", MATCH? [");
			if(testVals[i].matches(regex))
			{
				System.out.print("YES");
			}
			else
			{
				System.out.print("NO");
			}
			System.out.print("]");
			System.out.println();
		}
	}

}
