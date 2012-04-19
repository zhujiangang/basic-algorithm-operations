package com.bao.lc.site.s3.ai;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.AppConfig;
import com.bao.lc.site.s3.bean.ComparableFilter;
import com.bao.lc.site.s3.bean.TicketFilterCondition;
import com.bao.lc.site.s3.bean.TrainTicketInfo;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.site.s3.params.TdParams;
import com.bao.lc.util.SortFilter;

public class TrainTicketSortFilterBuilder
{
	private static Log log = LogFactory.getLog(TrainTicketSortFilterBuilder.class);
	
	private List<SortFilter<TrainTicketInfo>> sortFilters = null;

	public TrainTicketSortFilterBuilder(Context context)
	{
		sortFilters = new ArrayList<SortFilter<TrainTicketInfo>>(6);

		init(context);
	}

	private void init(Context context)
	{
		TicketFilterCondition filterCond = TdParams.getFilterCond(context);
		if(filterCond == null)
		{
			log.error("filterCond is null.");
			return;
		}
		
		SortFilter<TrainTicketInfo> filter = null;
		
		//Seat Class
		int passengerCount = MapUtils.getIntValue(context, TdPNames.PARAM_PASSENGER_COUNT, 1);
		filter = new SeatClassCondition(filterCond.seatClassList, passengerCount);
		sortFilters.add(filter);
		
		//Train Class
		if(filterCond.trainClassList != null && !filterCond.trainClassList.isEmpty())
		{
			filter = new TrainClassCondition(filterCond.trainClassList);
			sortFilters.add(filter);
		}
		
		//Train No: TODO
	}
	//TODO: delete
	protected void init_old(Context context)
	{
		String conditions = AppConfig.getInstance().getPropConfig("td.ticket.sort.conditions");
		String[] args = conditions.split(",");

		for(String category : args)
		{
			String propName = "td.ticket.sort.cond." + category;
			String value = AppConfig.getInstance().getPropConfig(propName);

			SortFilter<TrainTicketInfo> filter = buildSortFilter(context, category, value);
			if(filter != null)
			{
				sortFilters.add(filter);
			}
		}
	}

	private SortFilter<TrainTicketInfo> buildSortFilter(Context context, String category,
		String value)
	{
		SortFilter<TrainTicketInfo> result = null;
		if("0".equalsIgnoreCase(category))
		{
			if("Y".equalsIgnoreCase(value))
			{
				String from = MapUtils.getString(context, TdPNames.PARAM_FROM_STATION, "");
				String to = MapUtils.getString(context, TdPNames.PARAM_TO_STATION, "");
				result = new StationFromToCondition(from, to);				
			}
			else
			{
				log.info("No need to exactly match the from-to station.");
			}
		}
		else if("1".equalsIgnoreCase(category))
		{
			//TODO
		}
		else if("2".equalsIgnoreCase(category))
		{
			result = new TrainClassCondition(Arrays.asList(value.split(",")));
		}
		else if("3".equalsIgnoreCase(category))
		{
			
		}
		else if("4".equalsIgnoreCase(category))
		{
			int passengerCount = MapUtils.getIntValue(context, TdPNames.PARAM_PASSENGER_COUNT, 1);
			result = new SeatClassCondition(Arrays.asList(value.split(",")), passengerCount);
		}
		else
		{
			log.error("Unkown category filter: " + category);
		}

		return result;
	}

	private static class StationFromToCondition implements SortFilter<TrainTicketInfo>
	{
		private String from = null;
		private String to = null;

		public StationFromToCondition(String from, String to)
		{
			this.from = from;
			this.to = to;
		}

		public int getCategory()
		{
			return TrainTicketInfo.CAT_STATION_FROM_TO;
		}

		public List<Boolean> getValues()
		{
			return null;
		}

		public boolean accept(TrainTicketInfo target)
		{
			if(from.equals(target.getFromStationName()) && to.equals(target.getToStationName()))
			{
				return true;
			}
			return false;
		}
	}

	private static class TrainClassCondition implements SortFilter<TrainTicketInfo>
	{
		private List<?> values = null;
		public TrainClassCondition(List<?> values)
		{
			this.values = values;
		}
		public int getCategory()
		{
			return TrainTicketInfo.CAT_TRAIN_CLASS;
		}

		public List<?> getValues()
		{
			return this.values;
		}

		public boolean accept(TrainTicketInfo target)
		{
			ComparableFilter<TrainTicketInfo, ?> acceptor = target.getColumn(getCategory(), getValues());

			return acceptor.accept(target);
		}
	}

	private static class SeatClassCondition implements SortFilter<TrainTicketInfo>
	{
		private List<Integer> values = null;
		private int passengerCount = 1;

		public SeatClassCondition(List<String> values, int passengerCount)
		{
			this.values = new ArrayList<Integer>();
			
			for(String value : values)
			{
				value = value.trim();
				try
				{
					Integer i = new Integer(value);
					this.values.add(i);
				}
				catch(NumberFormatException e)
				{
					log.error("Failed to convert string [" + value + "] to number");
				}
			}
			
			this.passengerCount = passengerCount;
		}

		public int getCategory()
		{
			return TrainTicketInfo.CAT_SEAT_CLASS;
		}

		public List<Integer> getValues()
		{
			return this.values;
		}

		public boolean accept(TrainTicketInfo target)
		{
			ComparableFilter<TrainTicketInfo, ?> acceptor = target.getColumn(getCategory(), getValues(), passengerCount);

			return acceptor.accept(target);
		}
	}

	public List<SortFilter<TrainTicketInfo>> build()
	{
		return sortFilters;
	}
}
