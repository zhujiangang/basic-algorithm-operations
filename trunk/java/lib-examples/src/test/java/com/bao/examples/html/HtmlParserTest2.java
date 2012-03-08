package com.bao.examples.html;

import java.io.InputStream;

import org.apache.commons.io.IOUtils;
import org.apache.log4j.Logger;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.filters.OrFilter;
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
}
