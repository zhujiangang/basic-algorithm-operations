package com.bao.lc.site.s3.commands;

import java.net.URI;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.NameValuePair;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.Tag;
import org.htmlparser.filters.AndFilter;
import org.htmlparser.filters.HasAttributeFilter;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.tags.FormTag;
import org.htmlparser.tags.InputTag;
import org.htmlparser.tags.OptionTag;
import org.htmlparser.tags.SelectTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

import com.bao.lc.ResMgr;
import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.ParamListBuilder;
import com.bao.lc.client.RequestBuilder;
import com.bao.lc.httpcommand.AbstractCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.RandCodeBuilder;
import com.bao.lc.site.s3.TdUtils;
import com.bao.lc.site.s3.bean.GenericBean;
import com.bao.lc.site.s3.bean.PassengerInfo;
import com.bao.lc.site.s3.bean.TrainTicketInfo;
import com.bao.lc.site.s3.gui.GUIUtils;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.site.s3.params.TdParams;

public class ParseSubmitOrderPage extends AbstractCommand
{
	private static Log log = LogFactory.getLog(ParseSubmitOrderPage.class);

	private static final int MAX_PASSENGER_COUNT = 5;

	@Override
	protected IDValuePair preExecute(Context context) throws Exception
	{
		return ResultCode.RC_OK;
	}

	@Override
	protected IDValuePair doExecute(Context context) throws Exception
	{
		return ResultCode.RC_OK;
	}

	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{
		String submitOrderPage = MapUtils.getString(context, TdPNames._CONFIRM_PASSENGER_CONTENT);
		String charset = MapUtils.getString(context, TdPNames._CONFIRM_PASSENGER_ENCODING);

		if(submitOrderPage == null || charset == null)
		{
			log.fatal("Assert failure: submitOrderPage or(and) charset is null");
			return ResultCode.RC_ASSERT_FAILURE;
		}

		return buildNextRequest(context, submitOrderPage, charset);
	}

	private IDValuePair buildNextRequest(Context context, String submitOrderPage, String charset)
		throws ParserException
	{
		FormTag form = getSubmitForm(submitOrderPage, charset);
		if(form == null)
		{
			return ResultCode.RC_ASSERT_FAILURE;
		}

		String randCode = MapUtils.getString(context, TdPNames._CONFIRM_PASSENGER_VOCDE);
		if(randCode == null)
		{
			randCode = getRandCode(context, submitOrderPage, charset);
		}
		if(randCode == null)
		{
			TdParams.getUI(context).info(ResMgr.getString("td.msg.vcode.input.null"));
			return ResultCode.RC_INPUT_NULL_VCODE;
		}
		context.put(TdPNames._CONFIRM_PASSENGER_VOCDE, randCode);

		ParamListBuilder pb = new ParamListBuilder();
		getNonPassengerParams(context, form, randCode, pb);
		getPassengerParams(submitOrderPage, charset, context, pb);
		List<NameValuePair> paramList = pb.build();

		if(log.isDebugEnabled())
		{
			log.debug("confirm parameters: " + paramList);
		}

		RequestBuilder rb = new RequestBuilder();
		URI baseURI = HttpCommandParams.getTargetRequestURI(context);
		rb.method(form.getFormMethod()).baseURI(baseURI).reference(form.getFormLocation());
		rb.paramList(paramList).encoding(charset);

		context.put(HttpCommandPNames.TARGET_REQUEST, rb.create());
		context.put(HttpCommandPNames.TARGET_REFERER, baseURI.toString());

		return ResultCode.RC_OK;
	}

	private FormTag getSubmitForm(String submitOrderPage, String charset) throws ParserException
	{
		Parser myParser = Parser.createParser(submitOrderPage, charset);

		NodeFilter[] a = new NodeFilter[0];

		List<NodeFilter> filters = new ArrayList<NodeFilter>();
		filters.add(new NodeClassFilter(FormTag.class));
		filters.add(new HasAttributeFilter("id", "confirmPassenger"));
		filters.add(new HasAttributeFilter("name", "save_passenger_single"));

		NodeFilter filter = new AndFilter(filters.toArray(a));
		NodeList nodeList = myParser.parse(filter);

		if(nodeList.size() <= 0)
		{
			log.error("Can't find the target form. id=confirmPassenger");
			return null;
		}

		FormTag form = (FormTag) nodeList.elementAt(0);
		return form;
	}

	private String getRandCode(Context context, String submitOrderPage, String charset)
		throws ParserException
	{
		Parser myParser = Parser.createParser(submitOrderPage, charset);

		NodeFilter[] a = new NodeFilter[0];

		List<NodeFilter> filters = new ArrayList<NodeFilter>();
		filters.add(new NodeClassFilter(Tag.class));
		filters.add(new HasAttributeFilter("id", "img_rrand_code"));
		filters.add(new HasAttributeFilter("src"));

		NodeFilter filter = new AndFilter(filters.toArray(a));
		NodeList nodeList = myParser.parse(filter);

		if(nodeList.size() <= 0)
		{
			log.error("Can't find the img_rrand_code node.");
			return null;
		}

		Tag randCodeTag = (Tag) nodeList.elementAt(0);
		String location = randCodeTag.getAttribute("src");

		RequestBuilder rb = new RequestBuilder();
		URI baseURI = HttpCommandParams.getTargetRequestURI(context);
		rb.baseURI(baseURI).reference(location).encoding(charset);
		
		RandCodeBuilder rcb = new RandCodeBuilder();
		rcb.context(context).request(rb.create()).referer(baseURI.toString());
		rcb.prompt(ResMgr.getString("td.rand.code.input")).parent(GUIUtils.getMainFrame());
		return rcb.build();
	}

	private void getNonPassengerParams(Context context, FormTag form, String randCode,
		ParamListBuilder pb)
	{
		TrainTicketInfo ticketInfo = TdParams.getOrderTicketInfo(context);

		NodeList inputs = form.getFormInputs();
		for(int i = 0, size = inputs.size(); i < size; i++)
		{
			InputTag input = (InputTag) inputs.elementAt(i);
			String name = input.getAttribute("name");
			if(name == null)
			{
				log.debug("null name input in form [confirmPassenger], raw: " + input.toHtml());
				continue;
			}
			if(StringUtils.containsIgnoreCase(name, "passenger"))
			{
				continue;
			}
			if(StringUtils.containsIgnoreCase(name, "checkbox"))
			{
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

			if(name.equals("orderRequest.train_date"))
			{
				value = MapUtils.getString(context, TdPNames.PARAM_TICKET_DATE);
			}
			else if(name.equals("orderRequest.start_time"))
			{
				value = ticketInfo.getStartTime();
			}
			else if(name.equals("orderRequest.end_time"))
			{
				value = ticketInfo.getEndTime();
			}
			else if(name.equals("randCode"))
			{
				value = randCode;
			}

			pb.append(name, value);
		}
	}

	private IDValuePair getPassengerParams(String submitOrderPage, String charset, Context context, ParamListBuilder pb)
	{
		List<PassengerInfo> passengers = TdParams.getPassengerList(context);
		for(int i = 0, size = passengers.size(); i < size; i++)
		{
			int index = i + 1;
			PassengerInfo passenger = passengers.get(i);

			// checkbox i
			pb.append("checkbox" + i, String.valueOf(i));

			// passengerTickets
			pb.append("passengerTickets", passenger.getPassengerTickets());

			// old passengers
			pb.append("oldPassengers", passenger.getOldPassenger());

			// passenger filed info
			String seatClassValue = passenger.seatClass;
			String similarSeatClass = getPassengerSeat(submitOrderPage, charset, seatClassValue, index);
			if(similarSeatClass != null)
			{
				seatClassValue = similarSeatClass;
			}
			pb.append("passenger_" + index + "_seat", seatClassValue);
			pb.append("passenger_" + index + "_ticket", passenger.tiketType);
			pb.append("passenger_" + index + "_name", passenger.name);
			pb.append("passenger_" + index + "_cardtype", passenger.cardType);
			pb.append("passenger_" + index + "_cardno", passenger.cardNo);
			pb.append("passenger_" + index + "_mobileno", passenger.phone);
			// save?
			pb.append("checkbox9", passenger.isSave);
		}

		for(int i = passengers.size(); i < MAX_PASSENGER_COUNT; i++)
		{
			pb.append("oldPassengers", "");
			pb.append("checkbox9", "Y");
		}

		return ResultCode.RC_OK;
	}
	
	private SelectTag getSeatSelect(String submitOrderPage, String charset, int index) throws ParserException
	{
		Parser myParser = Parser.createParser(submitOrderPage, charset);

		NodeFilter[] a = new NodeFilter[0];

		List<NodeFilter> filters = new ArrayList<NodeFilter>();
		filters.add(new NodeClassFilter(SelectTag.class));
		filters.add(new HasAttributeFilter("id", "passenger_" + index + "_seat"));

		NodeFilter filter = new AndFilter(filters.toArray(a));
		NodeList nodeList = myParser.parse(filter);

		if(nodeList.size() <= 0)
		{
			log.error("Can't find the target form. id=passenger_" + index + "_seat");
			return null;
		}

		SelectTag seatSelect = (SelectTag) nodeList.elementAt(0);
		return seatSelect;
	}
	
	private String getPassengerSeat(String submitOrderPage, String charset, String rawSeat, int index)
	{
		SelectTag seatSelect = null;
		try
		{
			seatSelect = getSeatSelect(submitOrderPage, charset, index);
		}
		catch(ParserException e)
		{
			log.error("Can't find the seat select tag.", e);
			return null;
		}
		
		if(seatSelect == null)
		{
			return null;
		}
		
		String seatName = GenericBean.getField(rawSeat, 2, 1, TdUtils.getSeatClasses());
		OptionTag[] options = seatSelect.getOptionTags();
		for(OptionTag option : options)
		{
			//Hit in our config file.
			if(seatName.equals(option.getOptionText()))
			{
				return null;
			}
		}
		
		StringBuilder sb = new StringBuilder();
		sb.append("seatName=").append(seatName);
		sb.append(",rawSeat=").append(rawSeat).append(". ");
		for(OptionTag option : options)
		{
			sb.append("option=").append(option.getOptionText()).append(",").append(option.getValue()).append(" ");
		}
		log.info("Can't find the exactly matched seat. Try to find a similar one. " + sb.toString());
		// Errrrr..., no hit, try to find the best matched
		for(OptionTag option : options)
		{
			String optionText = option.getOptionText();
			if(seatName.length() >= 2 && optionText.length() >= 2)
			{
				// Found the similar seat
				if(seatName.regionMatches(0, optionText, 0, 2))
				{
					log.info("Find the most similar requried seat. seatName=" + seatName
						+ ", optionText=" + optionText + ", optionValue=" + option.getValue());
					return option.getValue();
				}
			}
		}
		log.error("Can't find the similar matched seat. ");
		return null;
	}
}
