package com.e2u.re;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class TestRe
{
	private static String bigStr = "Error item\n   error code : 1012309\r\nsdfsdasdfsda";
	private static String bigStr3 = "\\r\\r\\n*******r\\r\\n***********************";
	private static void test0()
	{
		// String regex =
		// "UNDEFINED|10{1,3}_MBIT_PER_SEC(,10{1,3}_MBIT_PER_SEC)*";
		// String regex = ".*mplsvpn [0-9]* not found.*";
//		String regex = "\\d+";
		String regex = "|/e1=1";
		regex = "[^\\.]*";
		//This represents the string that doesn't contain "ab";
		regex = "(?:(?!ab).)*";
		String[] testVals = { "", "ab12", "/e1=1", "/e1=1aaa", "0", "ab34", "5", "12", "1000",
			"C:\\mu.java", "-012", "xyzabcdabe", "aaa.", bigStr3};

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

	private static void test1()
	{
		Pattern pattern = Pattern.compile("error code : (.+)$", Pattern.MULTILINE);
//		Matcher matcher = pattern.matcher("   error code : 1012309");
		Matcher matcher = pattern.matcher(bigStr);
		
		while(matcher.find())
		{
			System.out.println(matcher.group(0));
			System.out.println(matcher.group(1));		
		}
	}

	public static String replace(final String sourceString, Object[] object)
	{
		String temp = sourceString;
		for(int i = 0; i < object.length; i++)
		{
			String[] result = (String[]) object[i];
			Pattern pattern = Pattern.compile(result[0]);
			Matcher matcher = pattern.matcher(temp);
			temp = matcher.replaceAll(result[1]);
		}
		return temp;
	}

	private static void test2()
	{
		String str = "Java目前的发展史是由{0}年-{1}年";
		String[][] object = { new String[] { "\\{0\\}", "1995" },
			new String[] { "\\{1\\}", "2007" } };
		System.out.println(replace(str, object));

	}
	
	private static void test3()
	{
		Pattern pattern = Pattern.compile("href=\"(.+)\"");
		Matcher matcher = pattern.matcher("<a href=\"index.html\">主页</a>");
		if(matcher.find())
		{
			System.out.println(matcher.group(1));
		}
	}
	
	private static void test4()
	{
		Pattern pattern = Pattern.compile("(http://|https://){1}[\\w\\.\\-/:]+");
		Matcher matcher = pattern.matcher("dsdsds<http://dsds//gfgffdfd>fdf");
		StringBuffer buffer = new StringBuffer();
		while(matcher.find()){              
		    buffer.append(matcher.group());        
		    buffer.append("\r\n");              
		System.out.println(buffer.toString());
		}
	}
    public static final char SPLITER_EQUAL = '=';
    static public boolean MatchIdValues(List<String> idNameValues, String pattern)
    {
        if ( pattern == null || pattern.isEmpty() )
            return true;
        if ( idNameValues == null )
            return false;

        //split the pattern
        int spl = pattern.charAt(0);
        int len = pattern.length();
        int i = 1;
        int count = 0;
        while ( i < len ){
            String pn = "";
            String pv = "";
            int j;
            for ( j = i; j < len && pattern.charAt(j) != spl; j++);
            
            if ( i != j ){
                pn = pattern.substring(i, j);
                int k = pn.indexOf(SPLITER_EQUAL);
                pv = pn.substring(k + 1);
                if ( k > 0 )
                    pn = pn.substring(0, k);
                else
                    pn = "";
            }
            i = j + 1;
            
            //select id value
            String value = "";
            if ( pn.isEmpty() ){
                if ( count >= idNameValues.size() )
                    return false;
                value = idNameValues.get(count);
                int l = value.indexOf(SPLITER_EQUAL);
                if ( l >= 0 )
                    value = value.substring(l + 1);
            }
            else{
                for ( j = idNameValues.size() - 1; j >= 0; j-- ){
                    value = idNameValues.get(j);
                    if ( value.length() > pn.length() ){
                        if ( value.charAt(pn.length()) == SPLITER_EQUAL ){
                            if ( value.startsWith(pn) ){
                                value = value.substring(pn.length() + 1);
                                break;
                            }
                        }
                    }
                }
            }
            
            //match
            if ( pv.isEmpty() ){
                if ( !value.isEmpty() )
                    return false;
            }
            else{
            	if ( !value.matches(pv) ){
                    return false;
                }
            }
            count++;
        };

        return true;
    }
    
    private static void test5()
    {
    	String pattern = "@TpMappingMode=(TM_NEITHER_TERMINATED_NOR_AVAILABLE_FOR_MAPPING|TM_TERMINATED_AND_AVAILABLE_FOR_MAPPING)@CTP=(|/e1=1)@LayerRate=(|LR_ETHERNET_VLAN|LR_IP|LR_MPLS)";
    	List<String> idNameValues = new ArrayList<String>();
    	idNameValues.add("TpMappingMode=TM_TERMINATED_AND_AVAILABLE_FOR_MAPPING");
    	idNameValues.add("CTP=/e1=1");
    	idNameValues.add("LayerRate=");
    	
    	if(MatchIdValues(idNameValues, pattern))
    	{
    		System.out.println("MATCH? [YES]");
    	}
    	else
    	{
    		System.out.println("MATCH? [NO]");
    	}
    	
    	idNameValues.set(1, "CTP=");
    	if(MatchIdValues(idNameValues, pattern))
    	{
    		System.out.println("MATCH? [YES]");
    	}
    	else
    	{
    		System.out.println("MATCH? [NO]");
    	}
    }
	/**
	 * @param args
	 */
	public static void main(String[] args)
	{
		test0();
//		test1();
//		test2();
//		test3();
//		test4();
//		test5();
	}

}
