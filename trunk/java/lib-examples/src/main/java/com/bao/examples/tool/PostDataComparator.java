package com.bao.examples.tool;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.TreeMap;

import org.apache.commons.io.IOUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.http.NameValuePair;
import org.apache.http.client.utils.URLEncodedUtils;
import org.apache.http.message.BasicNameValuePair;
import org.apache.log4j.Logger;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.filters.AndFilter;
import org.htmlparser.filters.HasAttributeFilter;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.tags.FormTag;
import org.htmlparser.tags.InputTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

import com.bao.examples.util.MapOperation;
import com.bao.examples.util.MapRelation;
import com.bao.examples.util.MapXorResult;

public class PostDataComparator
{
	private static final Logger log = Logger.getLogger(PostDataComparator.class);

	public PostDataComparator()
	{

	}

	public static List<NameValuePair> toPairs(String postData, String encoding)
	{
		List<NameValuePair> parameters = new ArrayList<NameValuePair>();

		URLEncodedUtils.parse(parameters, new Scanner(postData), encoding);

		return parameters;
	}

	public static Map<String, String> toMap(List<NameValuePair> parameters)
	{
		Map<String, String> map = new TreeMap<String, String>();
		for(NameValuePair param : parameters)
		{
			if(map.containsKey(param.getName()))
			{
				StringBuilder sb = new StringBuilder();
				sb.append("The key [").append(param.getName()).append("]");
				sb.append(" already exist. Update the value from [");
				sb.append(map.get(param.getName())).append("] to [");
				sb.append(param.getValue()).append("]");
				log.warn(sb.toString());
			}
			map.put(param.getName(), param.getValue());
		}
		return map;
	}

	public static List<NameValuePair> getPost(String file, String encoding) throws IOException
	{
		InputStream is = PostDataComparator.class.getResourceAsStream(file);
		String fileContent = IOUtils.toString(is, encoding);
		
		return toPairs(fileContent, encoding);
	}
	public static List<NameValuePair> getInputs(String file, String encoding, String formId,
		String formName) throws IOException, ParserException
	{
		InputStream is = PostDataComparator.class.getResourceAsStream(file);
		String fileContent = IOUtils.toString(is, encoding);

		Parser myParser = Parser.createParser(fileContent, encoding);

		NodeFilter[] a = new NodeFilter[0];

		List<NodeFilter> filters = new ArrayList<NodeFilter>();
		filters.add(new NodeClassFilter(FormTag.class));
		filters.add(new HasAttributeFilter("id", formId));
		if(formName != null)
		{
			filters.add(new HasAttributeFilter("name", formName));
		}

		NodeFilter filter = new AndFilter(filters.toArray(a));
		NodeList nodeList = myParser.parse(filter);

		List<NameValuePair> parameters = new ArrayList<NameValuePair>();
		if(nodeList.size() <= 0)
		{
			log.error("Can't find any Form!");
			return parameters;
		}

		FormTag form = (FormTag) nodeList.elementAt(0);
		NodeList inputs = form.getFormInputs();
		for(int i = 0, size = inputs.size(); i < size; i++)
		{
			InputTag input = (InputTag) inputs.elementAt(i);
			String name = input.getAttribute("name");
			if(name == null)
			{
				log.warn("null name, raw: " + input.toHtml());
				continue;
			}
			String type = input.getAttribute("type");
			if("radio".equals(type))
			{
				String checked = input.getAttribute("checked");
				if(!"checked".equals(checked))
				{
					continue;
				}
			}
			String value = input.getAttribute("value");
			parameters.add(new BasicNameValuePair(name, value));
		}
		return parameters;
	}
	
	public static String toString(Map<?, ?> map)
	{
		if(map == null)
		{
			return null;
		}
		StringBuilder sb = new StringBuilder();
		sb.append("size=[").append(map.size()).append("] ");
		for(Map.Entry<?, ?> entry : map.entrySet())
		{
			sb.append(entry.getKey()).append("=[").append(entry.getValue()).append("] ");
		}
		return sb.toString();
	}
	
	private static void showParamList(List<NameValuePair> paramList)
	{
		for(int i = 0, size = paramList.size(); i < size; i++)
		{
			NameValuePair input = paramList.get(i);
			
//			if(StringUtils.containsIgnoreCase(input.getName(), "passenger"))
//			{
//				continue;
//			}
//			if(StringUtils.containsIgnoreCase(input.getName(), "checkbox"))
//			{
//				continue;
//			}

			System.out.printf("[%02d]:%s=%s\n", i, input.getName(), input.getValue());
		}
	}

	public static void main(String[] args) throws Exception
	{
		String formFile = "/content.html";
		String formEncoding = "UTF-8";
		String formId = "confirmPassenger";
		String formName = "save_passenger_single";

		String postFile = "/post.txt";
		String postEncoding = "UTF-8";

		log.info("GetInputs");
		List<NameValuePair> formInputs = getInputs(formFile, formEncoding, formId, formName);
		showParamList(formInputs);
		
		Map<String, String> formMap = toMap(formInputs);

		log.info("getPost");
		List<NameValuePair> postParams = getPost(postFile, postEncoding);
		showParamList(postParams);
		Map<String, String> postMap = toMap(postParams);

		MapOperation<String, String> mapOper = new MapOperation<String, String>(formMap, postMap);
		
		MapXorResult xorResult = mapOper.xor(MapOperation.XOR_KEY_VALUE);
		
		MapRelation relation = xorResult.getRelation();
		log.info("MapXorResult = " + relation);
		
		if(relation != MapRelation.EQUALS)
		{
			StringBuilder sb = new StringBuilder();
			sb.append("Only in form: ").append(toString(xorResult.getOnly1()));
			sb.append("\nOnly in post: ").append(toString(xorResult.getOnly2()));
			log.info(sb.toString());
		}
	}
}
