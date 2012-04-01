package com.bao.lc.site.s3.ai;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;

import org.apache.commons.chain.Context;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.bao.lc.site.s3.bean.TrainTicketInfo;
import com.bao.lc.util.SortFilter;

public class TrainTicketSelector
{
	private static Log log = LogFactory.getLog(TrainTicketSelector.class);

	private List<TrainTicketInfo> tickets = null;
	private int passengerCount = 1;

	private List<SortFilter<TrainTicketInfo>> filter = null;

	public TrainTicketSelector(List<TrainTicketInfo> ticketList, Context context)
	{
		this.tickets = new ArrayList<TrainTicketInfo>(ticketList.size());
		this.tickets.addAll(ticketList);

		// @TODO: get from context
		this.passengerCount = 2;

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

		return tickets.get(0);
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
