package com.bao.lc.site.s3.ai;

import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.ResMgr;
import com.bao.lc.site.s3.TdUtils;
import com.bao.lc.site.s3.bean.ComparableFilter;
import com.bao.lc.site.s3.bean.PassengerInfo;
import com.bao.lc.site.s3.bean.TrainTicketInfo;
import com.bao.lc.site.s3.params.TdPNames;
import com.bao.lc.site.s3.params.TdParams;
import com.bao.lc.util.SortFilter;

public class TrainTicketSelector
{
	private static Log log = LogFactory.getLog(TrainTicketSelector.class);

	private List<TrainTicketInfo> tickets = null;
	private Context context = null;
	private int passengerCount = 1;

	private List<SortFilter<TrainTicketInfo>> filter = null;

	public TrainTicketSelector(List<TrainTicketInfo> ticketList, Context context)
	{
		this.tickets = new ArrayList<TrainTicketInfo>(ticketList.size());
		this.tickets.addAll(ticketList);

		// @TODO: get from context
		this.context = context;
		this.passengerCount = MapUtils.getIntValue(context, TdPNames.PARAM_PASSENGER_COUNT, 1);

		TrainTicketSortFilterBuilder fb = new TrainTicketSortFilterBuilder(context);
		this.filter = fb.build();
	}

	public TrainTicketInfo select()
	{
		for(Iterator<TrainTicketInfo> iter = tickets.iterator(); iter.hasNext();)
		{
			TrainTicketInfo ticket = iter.next();
			for(SortFilter<TrainTicketInfo> cond : filter)
			{
				if(!cond.accept(ticket))
				{
					iter.remove();

					log.info("The ticket has been filtered by filter: " + cond.getCategory()
						+ ", ticket=" + ticket);
					
					String message = MessageFormat.format(
						ResMgr.getString("td.msg.ticket.filtered"),
						TdUtils.getFilterCatName(cond.getCategory()), ticket.toString());
					TdParams.getUI(context).info(message);
					
					break;
				}
			}
		}

		if(tickets.isEmpty())
		{
			log.info("All the tickets has been filtered.");
			return null;
		}

		Collections.sort(tickets, new TrainTicketComparator(filter, passengerCount));

		TrainTicketInfo selectedTicket = tickets.get(0);
		
		updatePassengerSeatClass(context, selectedTicket);
		
		return selectedTicket;
	}
	
	private void updatePassengerSeatClass(Context context, TrainTicketInfo ticket)
	{		
		SortFilter<TrainTicketInfo> seatClassCond = null;
		for(SortFilter<TrainTicketInfo> cond : filter)
		{
			if(cond.getCategory() == TrainTicketInfo.CAT_SEAT_CLASS)
			{
				seatClassCond = cond;
				break;
			}
		}
		if(seatClassCond == null)
		{
			log.warn("Can't find the seat class filter");
			return;
		}

		List<Integer> seatClassList = (List<Integer>) seatClassCond.getValues();

		ComparableFilter columnFilter = ticket.getColumn(seatClassCond.getCategory(),
			seatClassList, this.passengerCount);

		List<Integer> seatCountList = (List<Integer>) columnFilter.getValue();
		
		String passengerSeatClass = TdUtils.getSeatClassValue(TrainTicketInfo.HARD_SEAT);
		
		for(int i = 0, size = seatCountList.size(); i < size; i++)
		{
			int seatCount = seatCountList.get(i);
			if(seatCount >= this.passengerCount)
			{
				int seatClass = seatClassList.get(i);
				passengerSeatClass = TdUtils.getSeatClassValue(seatClass);
				log.info("Select seatClass: index=" + seatClass + ", value=" + passengerSeatClass);
			}
		}
		
		List<PassengerInfo> passengers = TdParams.getPassengerList(context);
		for(PassengerInfo passenger: passengers)
		{
			passenger.seatClass = passengerSeatClass;
		}
	}

	private static class TrainTicketComparator implements Comparator<TrainTicketInfo>
	{
		private List<SortFilter<TrainTicketInfo>> conds = null;
		private int passengerCount = 1;

		public TrainTicketComparator(List<SortFilter<TrainTicketInfo>> conds, int passengerCount)
		{
			this.conds = conds;
			this.passengerCount = passengerCount;
		}

		@Override
		public int compare(TrainTicketInfo o1, TrainTicketInfo o2)
		{
			for(SortFilter<TrainTicketInfo> cond : conds)
			{
				Comparable value1 = o1.getColumn(cond.getCategory(), cond.getValues(),
					passengerCount);
				Comparable value2 = o2.getColumn(cond.getCategory(), cond.getValues(),
					passengerCount);

				if(value1 != null && value2 != null)
				{
					int diff = value1.compareTo(value2);
					if(diff != 0)
					{
						return diff;
					}
				}
			}
			return 0;
		}

	}
}
