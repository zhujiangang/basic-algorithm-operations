package com.bao.lc.site.s2.commands;

import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
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
import org.htmlparser.filters.AndFilter;
import org.htmlparser.filters.HasAttributeFilter;
import org.htmlparser.filters.HasChildFilter;
import org.htmlparser.filters.HasSiblingFilter;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.filters.OrFilter;
import org.htmlparser.tags.FormTag;
import org.htmlparser.tags.InputTag;
import org.htmlparser.tags.OptionTag;
import org.htmlparser.tags.SelectTag;
import org.htmlparser.tags.TableColumn;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.RequestBuilder;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s2.ZyContants;
import com.bao.lc.util.AppUtils;
import com.bao.lc.util.MiscUtils;

public class GetTicketDetail extends BasicHttpCommand
{
	private static Log log = LogFactory.getLog(GetTicketDetail.class);

	private String targetConusmer = null;
	public GetTicketDetail(String consumerName)
	{
		targetConusmer = consumerName;
		log.info("targetConusmer=" + targetConusmer);
	}
	
	public GetTicketDetail()
	{
		this(null);
	}
	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{
		parse(context);
		return ResultCode.RC_OK;
	}

	private void parse(Context context) throws ParserException, IOException
	{
		HttpResponse rsp = HttpCommandParams.getResponse(context);
		String encoding = MapUtils.getString(context, ZyContants.PARAM_RSP_ENCODING, "UTF-8");
		String queryResult = HttpClientUtils.saveToString(rsp.getEntity(), encoding);

		Parser parser = MiscUtils.createParser(queryResult, encoding, log);

		// Set filters
		List<NodeFilter> predicates = new ArrayList<NodeFilter>(4);
		NodeFilter[] a = new NodeFilter[0];

		// 1. Submit filters
		predicates.add(new HasAttributeFilter("id", "submitForm"));
		predicates.add(new HasAttributeFilter("name", "submitForm"));
		predicates.add(new NodeClassFilter(FormTag.class));
		NodeFilter submitFormFilter = new AndFilter(predicates.toArray(a));

		// 2. Diag time filters
		predicates.clear();
		predicates.add(new HasAttributeFilter("id", "rdDiagtime"));
		predicates.add(new HasAttributeFilter("name", "rdDiagtime"));
		predicates.add(new HasAttributeFilter("type", "radio"));
		predicates.add(new NodeClassFilter(InputTag.class));
		NodeFilter inputsFilter = new AndFilter(predicates.toArray(a));

		NodeFilter uncleFilter = new AndFilter(new HasSiblingFilter(
			new HasChildFilter(inputsFilter)), new NodeClassFilter(TableColumn.class));
		NodeFilter diagTimeFilter = new OrFilter(inputsFilter, uncleFilter);

		// 3. Consumer List filters
		predicates.clear();
		predicates.add(new HasAttributeFilter("id", "ddlConsumerList"));
		predicates.add(new HasAttributeFilter("name", "ddlConsumerList"));
		predicates.add(new NodeClassFilter(SelectTag.class));
		NodeFilter consumerListFilter = new AndFilter(predicates.toArray(a));

		// Total filter
		predicates.clear();
		predicates.add(submitFormFilter);
		predicates.add(diagTimeFilter);
		predicates.add(consumerListFilter);
		NodeFilter finalFilter = new OrFilter(predicates.toArray(a));

		// Fire!
		NodeList nodeList = parser.parse(finalFilter);

		FormTag submitForm = null;
		SelectTag consumerList = null;
		Map<String, String> diagTimeMap = new HashMap<String, String>();

		InputTag diagTimeInput = null;
		TableColumn diagTimeColumn = null;
		for(int i = 0, size = nodeList.size(); i < size; i++)
		{
			Node node = nodeList.elementAt(i);

			if(node instanceof FormTag)
			{
				submitForm = (FormTag) node;
				continue;
			}
			else if(node instanceof SelectTag)
			{
				consumerList = (SelectTag) node;
				continue;
			}
			else if(node instanceof InputTag)
			{
				diagTimeInput = (InputTag) node;
			}
			else if(node instanceof TableColumn)
			{
				if(i == 0)
				{
					throw new ParserException("Unexpected TableColumn at index(0).");
				}
				Node prevNode = nodeList.elementAt(i - 1);
				if(prevNode == diagTimeInput)
				{
					diagTimeColumn = (TableColumn) node;

					String timeText = StringUtils.strip(diagTimeColumn.toPlainTextString(),
						" \t\r\n");
					String timeValue = diagTimeInput.getAttribute("value");

					timeText = MiscUtils.getTableColumnText(diagTimeColumn);
					timeValue = StringUtils.trim(timeValue);
					if(StringUtils.isEmpty(timeText))
					{
						throw new ParserException("Unexpected empty diag time.");
					}

					diagTimeMap.put(timeText, timeValue);
				}
			}
		}

		if(submitForm == null || consumerList == null || diagTimeMap.isEmpty())
		{
			throw new ParserException("The required filed does't exist.");
		}

		// 1. Diag Time
		String dayOfWeek = MapUtils.getString(context, ZyContants.PARAM_DAY_OF_WEEK);

		String expectedDiagTime = AppUtils.getDayOfWeekTextCN1(dayOfWeek);
		String diagTimeValue = diagTimeMap.get(expectedDiagTime);
		log.debug("expectedDiagTime=" + expectedDiagTime + ", diagTimeValue=" + diagTimeValue);

		Map<String, ConsumerInfo> consumerMap = new HashMap<String, ConsumerInfo>();
		OptionTag[] options = consumerList.getOptionTags();
		for(int i = 0; i < options.length; i++)
		{
			String name = options[i].getAttribute("name");
			if(name == null || name.isEmpty())
			{
				continue;
			}

			ConsumerInfo consumer = new ConsumerInfo();

			consumer.name = name;
			consumer.id = options[i].getAttribute("id");
			consumer.value = options[i].getAttribute("value");
			consumer.phone = options[i].getAttribute("phone");
			consumer.medical = options[i].getAttribute("medical");

			consumerMap.put(name, consumer);
		}
		String consumerName = this.targetConusmer;
		if(consumerName == null || targetConusmer.isEmpty())
		{
			consumerName = MapUtils.getString(context, ZyContants.PARAM_CONSUMER_NAME);
		}
		log.info("consumerName=" + consumerName);
		ConsumerInfo consumer = consumerMap.get(consumerName);
		if(consumer == null)
		{
			throw new IllegalArgumentException("The user doesn't exist. " + consumerName);
		}

		Map<String, String> paramMap = new HashMap<String, String>();
		NodeList submitInputs = submitForm.getFormInputs();
		for(int i = 0, size = submitInputs.size(); i < size; i++)
		{
			InputTag input = (InputTag) submitInputs.elementAt(i);

			String name = input.getAttribute("name");
			if(name == null || name.isEmpty())
			{
				log.debug("Empty name input: " + input);
				continue;
			}
			String value = null;
			// Time
			if(name.equals("diagtimeid"))
			{
				value = diagTimeValue;
			}
			// Consumer
			else if(name.equals("txtConsumerName"))
			{
				value = consumer.name;
			}
			else if(name.equals("txtConsumerIdNo"))
			{
				value = consumer.value;
			}
			else if(name.equals("txtConsumerId"))
			{
				value = consumer.id;
			}
			else if(name.equals("txtConsumerTel"))
			{
				value = consumer.phone;
			}
			// other fields
			else
			{
				value = input.getAttribute("value");
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
	}

	private static class ConsumerInfo
	{
		public String name;

		public String id;
		public String value;
		public String phone;
		public String medical;

		public String toString()
		{
			StringBuilder sb = new StringBuilder();
			sb.append("name=").append(name);
			sb.append(",id=").append(id);
			sb.append(",value=").append(value);
			sb.append(",phone=").append(phone);
			sb.append(",medical=").append(medical);
			return sb.toString();
		}
	}
}
