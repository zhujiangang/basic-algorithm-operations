package com.bao.lc.site.s3.bean;

public class TrainTicketInfo
{
	public static final int RAW_FIELD_COUNT = 17;
	
	/*Raw informations*/
	public String index; //序号
	
	public String trainNo;  // 车次
	
	//查询区间
	public String fromStation; // 发站
	public String toStation; //到站
	public String timeCost; //历时
	
	//余票信息
	public String buss_seat; // 商务座
	public String best_seat;// 特等座(余票)
	public String one_seat;// 一等座(余票)
	public String two_seat;// 二等座(余票)
	public String vag_sleeper;// 高级软卧(余票)
	public String soft_sleeper;// 软卧(余票)
	public String hard_sleeper;// 硬卧(余票)
	public String soft_seat;// 软座(余票)
	public String hard_seat;// 硬座(余票)
	public String none_seat;// 无座(余票)
	public String other_seat;// 其他
	
	//购票
	public String bookButton;
	
	/*Distilled informations*/
	
	public TrainTicketInfo()
	{
	}
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
		this.buss_seat = args[startIndex++];
		this.best_seat = args[startIndex++];
		this.one_seat = args[startIndex++];
		this.two_seat = args[startIndex++];
		this.vag_sleeper = args[startIndex++];
		this.soft_sleeper = args[startIndex++];
		this.hard_sleeper = args[startIndex++];
		this.soft_seat = args[startIndex++];
		this.hard_seat = args[startIndex++];
		this.none_seat = args[startIndex++];
		this.other_seat = args[startIndex++];
		this.bookButton = args[startIndex++];
	}
	
	public String toString()
	{
		StringBuilder sb = new StringBuilder();
		sb.append("index=").append(index);
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
		return sb.toString();
	}
}
