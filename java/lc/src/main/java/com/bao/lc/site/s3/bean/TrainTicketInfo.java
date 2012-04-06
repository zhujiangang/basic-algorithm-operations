package com.bao.lc.site.s3.bean;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.htmlparser.Parser;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.tags.InputTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

import com.bao.lc.AppConfig;
import com.bao.lc.util.MiscUtils;

public class TrainTicketInfo implements Columnable
{
	private static Log log = LogFactory.getLog(TrainTicketInfo.class);
	
	public static final int RAW_FIELD_COUNT = 17;
	
	public static final int PARSE_ERROR = -2;
	public static final int NONE_EXIST = -1;	//--
	public static final int HAVE_NOT = 0;	//无
	public static final int HAVE = Integer.MAX_VALUE;	//有
	
	private static Map<String, Integer> ticketNumMap = new HashMap<String, Integer>();
	static
	{
		ticketNumMap.put(AppConfig.getInstance().getPropInternal("td.ticket.num.none"), NONE_EXIST);
		ticketNumMap.put(AppConfig.getInstance().getPropInternal("td.ticket.num.have_not"), HAVE_NOT);
		ticketNumMap.put(AppConfig.getInstance().getPropInternal("td.ticket.num.have"), HAVE);
	}
	
	public static final int CAT_STATION_FROM_TO = 0;
	public static final int CAT_TRAIN_NO = 1;
	public static final int CAT_TRAIN_CLASS = 2;
	public static final int CAT_COST_TIME = 3;
	public static final int CAT_SEAT_CLASS = 4;
	
	public static final int BUSINESS_SEAT = 0; // 商务座
	public static final int SPECIAL_SEAT = 1; // 特等座
	public static final int FIRST_CLASS_SEAT = 2; // 一等座
	public static final int SECOND_CLASS_SEAT = 3; //二等座
	public static final int ADVANCED_SOFT_SLEEPER = 4;// 高级软卧
	public static final int SOFT_SLEEPER = 5; //软卧
	public static final int HARD_SLEEPER = 6; //硬卧
	public static final int SOFT_SEAT = 7; //软座
	public static final int HARD_SEAT = 8; // 硬座
	public static final int NONE_SEAT = 9; // 无座
	public static final int OTHER_SEAT = 10; // 其他
	
	public static final int SEAT_TYPE_COUNT = (OTHER_SEAT - BUSINESS_SEAT + 1);
	
	/*Raw informations*/
	private String index; //序号
	
	private String trainNo;  // 车次
	
	//查询区间
	private String fromStation; // 发站
	private String toStation; //到站
	private String timeCost; //历时
	
	//余票信息
	private int buss_seat; // 商务座
	private int best_seat;// 特等座
	private int one_seat;// 一等座
	private int two_seat;// 二等座
	private int vag_sleeper;// 高级软卧
	private int soft_sleeper;// 软卧
	private int hard_sleeper;// 硬卧
	private int soft_seat;// 软座
	private int hard_seat;// 硬座
	private int none_seat;// 无座
	private int other_seat;// 其他
	
	private int[] seats;
	
	//购票
	public String bookButton;
	
	/*Distilled informations*/
	private String _bookButtonParameter = null;
	private String _station_train_code = null; //车次
	private String _from_station_name = null; //发站
	private String _to_station_name = null; //到站
	private String _start_time = null; //发车时间
	private String _end_time = null; //到站时间
	

	public TrainTicketInfo(String[] args, int startIndex)
	{
		if(args.length < startIndex + RAW_FIELD_COUNT)
		{
			throw new IllegalArgumentException("No enough fields. args.length=" + args.length);
		}
		
		this.index = args[startIndex++];
		this.trainNo = args[startIndex++];
		this.fromStation = args[startIndex++];
		this.toStation = args[startIndex++];
		this.timeCost = args[startIndex++];
		
		this.seats = new int[SEAT_TYPE_COUNT];
		for(int i = BUSINESS_SEAT; i <= OTHER_SEAT; i++)
		{
			this.seats[i] = parseSeat(args[startIndex++]);
		}
		this.bookButton = args[startIndex++];
		
		//Legacy
		int seatIndex = BUSINESS_SEAT;
		this.buss_seat = this.seats[seatIndex++];
		this.best_seat = this.seats[seatIndex++];
		this.one_seat = this.seats[seatIndex++];
		this.two_seat = this.seats[seatIndex++];
		this.vag_sleeper = this.seats[seatIndex++];
		this.soft_sleeper = this.seats[seatIndex++];
		this.hard_sleeper = this.seats[seatIndex++];
		this.soft_seat = this.seats[seatIndex++];
		this.hard_seat = this.seats[seatIndex++];
		this.none_seat = this.seats[seatIndex++];
		this.other_seat = this.seats[seatIndex++];
		
		//distilled
		distill();
	}
	
	private void distill()
	{
		//
		parseBookButtonParameter();
		parseOtherFields();
	}
	
	private int parseSeat(String seatContent)
	{
		String htmlContent = StringUtils.substringBetween(seatContent, ">", "</");
		if(htmlContent != null)
		{
			seatContent = htmlContent;
		}
		seatContent = seatContent.trim();
		
		int seatNum = 0;
		
		if(ticketNumMap.containsKey(seatContent))
		{
			seatNum = ticketNumMap.get(seatContent);
		}
		else
		{
			try
			{
				seatNum = Integer.parseInt(seatContent);
			}
			catch(NumberFormatException e)
			{
				log.error("Can't convert seatContent [" + seatContent + "] to number.");
				seatNum = PARSE_ERROR;
			}
		}
//		if(log.isDebugEnabled())
//		{
//			log.debug("Parse seatContent from [" + seatContent + "] to [" + seatNum + "]");
//		}
		return seatNum;
	}
	
	@Override
	public String toString()
	{
		StringBuilder sb = new StringBuilder();
		sb.append("index=").append(index);
		
		if(this._bookButtonParameter == null)
		{
			sb.append(",trainNo=").append(trainNo);
			sb.append(",fromStation=").append(fromStation);
			sb.append(",toStation=").append(toStation);
			sb.append(",timeCost=").append(timeCost);
			
			sb.append(",buss_seat=").append(buss_seat);
			sb.append(",best_seat=").append(best_seat);
			sb.append(",one_seat=").append(one_seat);
			sb.append(",two_seat=").append(two_seat);
			sb.append(",vag_sleeper=").append(vag_sleeper);
			sb.append(",soft_sleeper=").append(soft_sleeper);
			sb.append(",hard_sleeper=").append(hard_sleeper);
			sb.append(",soft_seat=").append(soft_seat);
			sb.append(",hard_seat=").append(hard_seat);
			sb.append(",none_seat=").append(none_seat);
			sb.append(",other_seat=").append(other_seat);
			
			sb.append(",bookButton=").append(bookButton);
		}
		else
		{
			sb.append(",bookButtonParameter=").append(_bookButtonParameter);
			sb.append(",seats:{");
			for(int i = BUSINESS_SEAT; i <= OTHER_SEAT; i++)
			{
				if(this.seats[i] > 0)
				{
					sb.append(i).append("=").append(this.seats[i]).append(" ");
				}
			}
			sb.append("}");
		}

		return sb.toString();
	}
	
	public String getBookButtonParameter()
	{
		return this._bookButtonParameter;
	}
	
	private void parseBookButtonParameter()
	{
		Parser parser = MiscUtils.createParser(bookButton, "UTF-8", log);
		NodeList nodeList = null;
		try
		{
			nodeList = parser.parse(new NodeClassFilter(InputTag.class));
		}
		catch(ParserException e)
		{
			log.error("parseBookButtonParameter failed.", e);
			return;
		}
		
		if(nodeList == null || nodeList.size() <= 0)
		{
			log.error("parseBookButtonParameter result nodeList is empty.");
			return;
		}
		
		InputTag input = (InputTag)nodeList.elementAt(0);
		String onclick = input.getAttribute("onclick");
		
		if(onclick == null || onclick.isEmpty())
		{
			log.error("parseBookButtonParameter error: onclick attribute doesn't exist.");
			return;
		}
		
		String parameter = StringUtils.substringBetween(onclick, "'", "'");
		if(parameter == null || parameter.isEmpty())
		{
			log.error("parseBookButtonParameter error: onclick parameter is empty.");
			return;		
		}
		
		this._bookButtonParameter = parameter;
		log.info("index=" + index + ", bookButtonParameter=" + _bookButtonParameter);
	}
	
	private void parseOtherFields()
	{
		if(this._bookButtonParameter == null)
		{
			log.error("bookButtonParameter = null.");
			return;
		}

		
		String[] bookButtonParams = this._bookButtonParameter.split("#");

		//let SubmitOrder#getCurrParamMap to check
//		String[] trainParams = { "station_train_code", "lishi", "train_start_time", "trainno",
//			"from_station_telecode", "to_station_telecode", "arrive_time", "from_station_name",
//			"to_station_name", "ypInfoDetail" };
//		if(bookButtonParams.length != trainParams.length)
//		{
//			log.error("[parseOtherFields]: Book Button Parameter length is unexpected. bookButtonParams.length="
//				+ bookButtonParams.length);
//			return;
//		}
		
		if(bookButtonParams.length < 9)
		{
			log.error("[parseOtherFields]: Book Button Parameter length is too short. bookButtonParams.length="
				+ bookButtonParams.length);
			return;
		}
		
		_station_train_code = bookButtonParams[0];
		
		_start_time = bookButtonParams[2];
		_end_time = bookButtonParams[6];
		
		_from_station_name = bookButtonParams[7];
		_to_station_name = bookButtonParams[8];
	}
	
	public String getTrainClass()
	{
		if(this._station_train_code == null || this._station_train_code.isEmpty())
		{
			return null;
		}
		
		char c = this._station_train_code.charAt(0);
		if(Character.isLetter(c))
		{
			return String.valueOf(c);
		}
		return null;
	}
	
	public String getStationTrainCode()
	{
		return this._station_train_code;
	}
	
	public String getFromStationName()
	{
		return this._from_station_name;
	}
	
	public String getToStationName()
	{
		return this._to_station_name;
	}
	
	public String getStartTime()
	{
		return this._start_time;
	}
	public String getEndTime()
	{
		return this._end_time;
	}
	
	@Override
	public ComparableFilter<TrainTicketInfo, ?> getColumn(int column, Object ... args)
	{
		ComparableFilter<TrainTicketInfo, ?> result = null;
		
		if(column == CAT_TRAIN_CLASS)
		{
			result = new ListElementIndex(getTrainClass(), (List<?>)args[0]);
		}
		else if(column == CAT_COST_TIME)
		{
			result = new ListFilter(this.timeCost, (List<?>)args[0]);
		}
		else if(column == CAT_SEAT_CLASS)
		{
			List<Integer> seatClassIndex = (List<Integer>)args[0];
			
			List<Integer> seatClassCount = new ArrayList<Integer>(seatClassIndex.size());
			
			for(int index : seatClassIndex)
			{
				seatClassCount.add(this.seats[index]);
			}
			return new ListElementCount(seatClassCount, (Integer)args[1]);
		}

		return result;
	}
}
