package com.bao.examples.html;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import org.apache.commons.io.IOUtils;
import org.apache.log4j.Logger;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.filters.AndFilter;
import org.htmlparser.filters.HasAttributeFilter;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.filters.OrFilter;
import org.htmlparser.tags.FormTag;
import org.htmlparser.tags.InputTag;
import org.htmlparser.tags.TableColumn;
import org.htmlparser.tags.TableRow;
import org.htmlparser.tags.TableTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

import junit.framework.TestCase;

public class HtmlParserTest2 extends TestCase
{
	private static final Logger logger = Logger.getLogger(HtmlParserTest.class);

	private String fileContent = null;

	protected void setUp() throws Exception
	{
		InputStream is = getClass().getResourceAsStream("/content.html");
		fileContent = IOUtils.toString(is, "GB2312");
	}

	public void testTableVisitor()
	{
		Parser myParser = Parser.createParser(fileContent, "GB2312");

		NodeFilter tableFilter = new NodeClassFilter(TableTag.class);
		OrFilter lastFilter = new OrFilter();
		lastFilter.setPredicates(new NodeFilter[] { tableFilter });
		try
		{
			NodeList nodeList = myParser.parse(lastFilter);
			for(int i = 0; i <= nodeList.size(); i++)
			{
				if(nodeList.elementAt(i) instanceof TableTag)
				{
					TableTag tag = (TableTag) nodeList.elementAt(i);
					
					if("Table1".equals(tag.getAttribute("id")))
					{
						NodeList childrenList = tag.getChildren();
						NodeList childTableList = childrenList.extractAllNodesThatMatch(tableFilter, true);
						childrenList.keepAllNodesThatMatch(tableFilter, true);
						
						for(int k = 0; k < childTableList.size(); k++)
						{
							if(childTableList.elementAt(k) instanceof TableTag)
							{
								TableTag tag2 = (TableTag) childTableList.elementAt(k);
								if("DataGrid1".equals(tag2.getAttribute("id")))
								{
									logger.fatal(tag2);
									TableRow[] rows = tag2.getRows();
									for(int j = 0; j < rows.length; j++)
									{
										TableRow tr = (TableRow) rows[j];
										logger.fatal("row [" + j + "]: " + tr.toPlainTextString().trim());
										TableColumn[] td = tr.getColumns();
										for(int k2 = 0; k2 < td.length; k2++)
										{
											logger.fatal("<td>" + td[k2].toPlainTextString());
										}

									}
								}
							}
						}
					}
				}
			}

		}
		catch(ParserException e)
		{
			e.printStackTrace();
		}
	}
	
	public void testTemp() throws ParserException
	{
		Parser myParser = Parser.createParser(fileContent, "UTF-8");
		
		NodeFilter[] a = new NodeFilter[0];
		
		List<NodeFilter> filters = new ArrayList<NodeFilter>();
		filters.add(new NodeClassFilter(FormTag.class));
		filters.add(new HasAttributeFilter("id", "confirmPassenger"));
		filters.add(new HasAttributeFilter("name", "save_passenger_single"));
		
		
		NodeFilter filter = new AndFilter(filters.toArray(a));
		NodeList nodeList = myParser.parse(filter);
		
		if(nodeList.size() <= 0)
		{
			System.err.println("Can't find any Form!");
		}
		
		FormTag form = (FormTag)nodeList.elementAt(0);
		
		NodeList inputs = form.getFormInputs();
		for(int i = 0, size = inputs.size(); i < size; i++)
		{
			InputTag input = (InputTag) inputs.elementAt(i);

			
			System.out.printf(Locale.CHINESE, "%02d: name=%s,value=%s\n", i, input.getAttribute("name"),
				input.getAttribute("value"));
		}
	}
}
