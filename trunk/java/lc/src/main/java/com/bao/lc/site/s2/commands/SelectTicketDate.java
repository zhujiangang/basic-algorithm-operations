package com.bao.lc.site.s2.commands;

import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpUriRequest;
import org.htmlparser.Node;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.Tag;
import org.htmlparser.filters.AndFilter;
import org.htmlparser.filters.HasAttributeFilter;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.filters.OrFilter;
import org.htmlparser.tags.FormTag;
import org.htmlparser.tags.InputTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.RequestBuilder;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s2.ZyConstants;
import com.bao.lc.util.MiscUtils;

public class SelectTicketDate extends BasicHttpCommand
{
	private static Log log = LogFactory.getLog(SelectTicketDate.class);

	protected IDValuePair postExecute(Context context) throws Exception
	{
		return parse(context);
	}

	private IDValuePair parse(Context context) throws ParserException, IOException
	{
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		String encoding = MapUtils.getString(context, ZyConstants.PARAM_RSP_ENCODING, "UTF-8");
		String content = HttpClientUtils.saveToString(rsp.getEntity(), encoding);

		Parser parser = MiscUtils.createParser(content, encoding, log);

		// Set filters
		List<NodeFilter> predicates = new ArrayList<NodeFilter>(2);
		NodeFilter[] a = new NodeFilter[0];

		// 1. Submit filters
		predicates.add(new HasAttributeFilter("id", "submitForm"));
		predicates.add(new NodeClassFilter(FormTag.class));
		NodeFilter submitFormFilter = new AndFilter(predicates.toArray(a));
		predicates.clear();

		// 2. time select
		predicates.add(new HasAttributeFilter("name", "ddlselect"));
		predicates.add(new NodeClassFilter(InputTag.class));
		NodeFilter selectFilter = new AndFilter(predicates.toArray(a));
		predicates.clear();

		// 3.
		predicates.add(new HasAttributeFilter("name", "ddlRegisterTime"));
		predicates.add(new HasAttributeFilter("date"));
		NodeFilter regTimeFilter = new AndFilter(predicates.toArray(a));
		predicates.clear();

		// 4. final filter
		predicates.add(submitFormFilter);
		predicates.add(selectFilter);
		predicates.add(regTimeFilter);
		NodeFilter finalFilter = new OrFilter(predicates.toArray(a));
		predicates.clear();

		// Fire!
		NodeList nodeList = parser.parse(finalFilter);

		FormTag submitForm = null;
		List<InputTag> timeList = new ArrayList<InputTag>(2);
		List<Tag> regTimeList = new ArrayList<Tag>(0);

		for(int i = 0, size = nodeList.size(); i < size; i++)
		{
			Node node = nodeList.elementAt(i);
			if(!(node instanceof Tag))
			{
				log.debug("Non-Tag node. " + node);
				continue;
			}

			Tag tagNode = (Tag) node;

			String name = tagNode.getAttribute("name");
			if(StringUtils.equals(name, "ddlRegisterTime"))
			{
				regTimeList.add(tagNode);
				continue;
			}
			else if(StringUtils.equals(name, "ddlselect"))
			{
				timeList.add((InputTag) tagNode);
				continue;
			}

			String id = tagNode.getAttribute("id");
			if(StringUtils.equals(id, "submitForm"))
			{
				submitForm = (FormTag) tagNode;
			}
		}

		if(log.isInfoEnabled())
		{
			StringBuilder sb = new StringBuilder("[SelectTicket]: count=");
			sb.append(timeList.size());
			for(int i = 0, size = timeList.size(); i < size; i++)
			{
				sb.append(" ").append(timeList.get(i).getAttribute("value"));
			}
			log.info(sb.toString());
		}

		if(timeList.isEmpty())
		{
			log.warn("[IMPORTANT]: time list is empty!");
			return ResultCode.RC_DOCTOR_REG_LIST_FULL;
		}		
		String diagDate = getFinalDiagDate(context, timeList);
		log.info("Target reg date: " + diagDate);
		if(diagDate == null)
		{
			log.warn("[IMPORTANT]: Can't find the target diag date.");
			return ResultCode.RC_DOCTOR_REG_LIST_FULL;
		}

		String registertime = "";
		for(int i = 0, size = regTimeList.size(); i < size; i++)
		{
			Tag tag = regTimeList.get(i);
			String date = tag.getAttribute("date");
			if(StringUtils.equals(date, diagDate))
			{
				registertime = tag.getAttribute("value");
				break;
			}
		}

		Map<String, String> paramMap = new HashMap<String, String>();
		NodeList submitInputs = submitForm.getFormInputs();
		for(int i = 0, size = submitInputs.size(); i < size; i++)
		{
			InputTag input = (InputTag) submitInputs.elementAt(i);

			String name = input.getAttribute("name");
			String value = input.getAttribute("value");
			if(name.equals("diagdate"))
			{
				value = diagDate;
			}
			else if(name.equals("registertime"))
			{
				value = registertime;
			}
			paramMap.put(name, value);
		}

		if(log.isDebugEnabled())
		{
			log.debug("ParamMap: " + MiscUtils.toString(paramMap));
		}

		String method = submitForm.getFormMethod();
		String location = submitForm.getFormLocation();

		// Build the next URI
		URI requestURI = HttpCommandParams.getTargetRequestURI(context);

		// Next Request
		RequestBuilder rb = new RequestBuilder();
		rb.method(method).reference(location).baseURI(requestURI);
		rb.parameters(paramMap).encoding(encoding);
		HttpUriRequest nextRequest = rb.create();

		context.put(HttpCommandPNames.TARGET_REQUEST, nextRequest);
		context.put(HttpCommandPNames.TARGET_REFERER, requestURI.toString());
		
		return ResultCode.RC_OK;
	}
	
	private Calendar toCalendar(String str)
	{		
		String regex = "(\\d+?)-(\\d+?)-(\\d+?)";
		List<String> valueList = new ArrayList<String>();

		int matchCount = MiscUtils.getRegexValue(str, regex, valueList, true, 0);
		if(matchCount != 1)
		{
			log.error("Failed to format str to date: " + str);
			return null;
		}
		Calendar cal = Calendar.getInstance();
		cal.set(Calendar.MILLISECOND, 0);

		int index = 1;
		cal.set(Calendar.YEAR, MiscUtils.toInt(valueList.get(index++)));
		cal.set(Calendar.MONTH, MiscUtils.toInt(valueList.get(index++)) - 1);
		cal.set(Calendar.DAY_OF_MONTH, MiscUtils.toInt(valueList.get(index++)));
		
		return cal;
	}
	
	private String getFinalDiagDate(Context context, List<InputTag> timeList)
	{
		String diagDate = null;
		
		do
		{
			Calendar targetDay = (Calendar)context.get(ZyConstants.PARAM_TARGET_DAY);
			if(targetDay == null)
			{
				break;
			}
			log.info("Target day: " + MiscUtils.toString(targetDay));
			for(int i = 0, size = timeList.size(); i < size; i++)
			{
				String value = timeList.get(i).getAttribute("value");
				Calendar cal = toCalendar(value);
				if(MiscUtils.isSameDay(targetDay, cal))
				{
					diagDate = value;
					break;
				}
			}
		}
		while(false);
		
		if(diagDate != null)
		{
			return diagDate;
		}
		boolean fixedDay = MapUtils.getBooleanValue(context, ZyConstants.PARAM_FIXED_DAY, false);
		if(fixedDay)
		{
			log.info("Can't find the target day, stop to try.");
			return null;
		}
		
		log.info("Can't find the target day, try to find an alternative day.");
		for(int i = 0, size = timeList.size(); i < size; i++)
		{
			String value = timeList.get(i).getAttribute("value");
			if(diagDate == null)
			{
				diagDate = value;
			}
			else
			{
				// pick up the big one
				if(diagDate.compareTo(value) < 0)
				{
					diagDate = value;
				}
			}
		}
		return diagDate;
	}
}
