package com.bao.lc.site.s2.commands;

import java.io.IOException;
import java.net.URI;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.chain.Context;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.utils.URIUtils;
import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.Tag;
import org.htmlparser.Text;
import org.htmlparser.filters.AndFilter;
import org.htmlparser.filters.HasAttributeFilter;
import org.htmlparser.filters.NodeClassFilter;
import org.htmlparser.tags.LinkTag;
import org.htmlparser.tags.TableColumn;
import org.htmlparser.tags.TableRow;
import org.htmlparser.tags.TableTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;
import org.htmlparser.visitors.NodeVisitor;

import com.bao.lc.AppConfig;
import com.bao.lc.bean.IDValuePair;
import com.bao.lc.bean.ResultCode;
import com.bao.lc.client.utils.HttpClientUtils;
import com.bao.lc.httpcommand.BasicHttpCommand;
import com.bao.lc.httpcommand.params.HttpCommandPNames;
import com.bao.lc.httpcommand.params.HttpCommandParams;
import com.bao.lc.site.s2.ZyConstants;
import com.bao.lc.util.MiscUtils;

public class QueryTicketNumResult extends BasicHttpCommand
{
	private static Log log = LogFactory.getLog(QueryTicketNumResult.class);

	@Override
	protected IDValuePair preExecute(Context context) throws Exception
	{
		// Remove the response if it exists
		context.remove(HttpCommandPNames.TARGET_RESPONSE);
		context.remove(HttpCommandPNames.HTTP_COMMAND_RESULT_CODE);
		
		//Get target URI
		String encoding = MapUtils.getString(context, ZyConstants.PARAM_RSP_ENCODING, "UTF-8");
		String doctor = MapUtils.getString(context, ZyConstants.PARAM_DOCTOR);
		
		String escapedName = MiscUtils.encode(doctor, encoding);
		String url = AppConfig.getInstance().getPropInternal("zy.query.expert.url");
		url = MessageFormat.format(url, escapedName);
		
		String userPageURI = MapUtils.getString(context, ZyConstants.USER_PAGE_URI);
		URI baseURI = URI.create(userPageURI);
		URI queryURI = URIUtils.resolve(baseURI, url);
		
		context.put(HttpCommandPNames.TARGET_REQUEST, new HttpGet(queryURI));
		context.put(HttpCommandPNames.TARGET_REFERER, userPageURI);

		return ResultCode.RC_OK;
	}
	
	@Override
	protected IDValuePair postExecute(Context context) throws Exception
	{
		// 1. Response
		HttpResponse rsp = HttpCommandParams.getResponse(context);

		// 2. parse
		TicketNumResult regInfo = parse(context, rsp);
		if(regInfo == null)
		{
			return ResultCode.RC_REG_LIST_EMPTY;
		}

		// 3. Set next hop request
		URI requestURI = HttpCommandParams.getTargetRequestURI(context);
		URI nextURI = URIUtils.resolve(requestURI, regInfo.getLink());

		context.put(HttpCommandPNames.TARGET_REQUEST, new HttpGet(nextURI));
		context.put(HttpCommandPNames.TARGET_REFERER, requestURI.toString());

		return ResultCode.RC_OK;
	}

	private TicketNumResult parse(Context context, HttpResponse rsp) throws ParserException,
		IOException
	{
		String encoding = MapUtils.getString(context, ZyConstants.PARAM_RSP_ENCODING, "UTF-8");
		String queryResult = HttpClientUtils.saveToString(rsp.getEntity(), encoding);

		Parser parser = MiscUtils.createParser(queryResult, encoding, log);

		// Set filters
		NodeFilter[] predicates = { new HasAttributeFilter("class", "datatable"),
			new NodeClassFilter(TableTag.class) };
		NodeFilter filter = new AndFilter(predicates);

		NodeList dataTableList = parser.parse(filter);
		if(dataTableList.size() == 0)
		{
			log.info("No available reg list.");
			return null;
		}

		TableTag dataTable = (TableTag) dataTableList.elementAt(0);
		TableRow[] rows = dataTable.getRows();
		if(rows.length <= 0)
		{
			log.info("No available reg list.");
			return null;
		}

		List<TicketNumResult> regInfoList = new ArrayList<TicketNumResult>(3);

		TicketNumResult regInfo = new TicketNumResult();
		RegInfoVisitor visitor = new RegInfoVisitor();
		for(int i = 0; i < rows.length; i++)
		{
			TableColumn[] cols = rows[i].getColumns();
			for(int j = 0; j < cols.length; j++)
			{
				NodeList children = cols[j].getChildren();

				visitor.setData(regInfo, i, false);
				children.visitAllNodesWith(visitor);

				if(visitor.isMeetHR())
				{
					regInfoList.add(regInfo);
					regInfo = new TicketNumResult();
				}
			}
			regInfo.prevText = null;
		}
		log.debug("done: " + regInfoList.size() + ", " + regInfoList.toString());

		String doctor = MapUtils.getString(context, ZyConstants.PARAM_DOCTOR);
		String hospital = MapUtils.getString(context, ZyConstants.PARAM_HOSPITAL);

		for(int i = 0, size = regInfoList.size(); i < size; i++)
		{
			regInfo = regInfoList.get(i);

			if(StringUtils.equals(regInfo.doctor, doctor)
				&& StringUtils.equals(regInfo.hospital, hospital) && regInfo.isAvailable())
			{
				log.info("Found target: " + regInfo);
				return regInfo;
			}
		}

		log.info("Can't find the target registration.");
		return null;
	}

	private static class RegInfoVisitor extends NodeVisitor
	{
		private boolean isMeetHR = false;
		private int rowIndex = -1;
		private TicketNumResult regInfo = null;

		public RegInfoVisitor()
		{
		}

		public void setData(TicketNumResult regInfo, int rowIndex, boolean separator)
		{
			this.regInfo = regInfo;
			this.rowIndex = rowIndex;
			this.isMeetHR = separator;
		}

		public void visitTag(Tag tag)
		{
			if(tag.getTagName().equals("HR"))
			{
				isMeetHR = true;
			}
			else if(tag instanceof LinkTag)
			{
				if(StringUtils.equals(regInfo.prevText,
					AppConfig.getInstance().getPropInternal("zy.available.doctor.name")))
				{
					regInfo.doctorAnchor = (LinkTag) tag;
				}
				else
				{
					regInfo.regAnchor = (LinkTag) tag;
				}
			}
		}

		public void visitStringNode(Text string)
		{
			String text = StringUtils.strip(string.getText(), " \t\r\n");
			if(StringUtils.isEmpty(text))
			{
				return;
			}

			if((rowIndex % 6) == 3)
			{
				regInfo.available = text;
				return;
			}

			if(StringUtils.equals(regInfo.prevText,
				AppConfig.getInstance().getPropInternal("zy.available.doctor.name")))
			{
				regInfo.doctor = text;
			}
			else if(StringUtils.equals(regInfo.prevText,
				AppConfig.getInstance().getPropInternal("zy.available.hospital.name")))
			{
				regInfo.hospital = text;
			}
			regInfo.prevText = text;
		}

		public boolean isMeetHR()
		{
			return isMeetHR;
		}
	}

	private static class TicketNumResult
	{
		public String doctor;
		public LinkTag doctorAnchor;
		public String hospital;
		public String available;
		public LinkTag regAnchor;

		public String prevText = null;

		public boolean isAvailable()
		{
			if(available == null || regAnchor == null)
			{
				return false;
			}
			if(!available
				.contains(AppConfig.getInstance().getPropInternal("zy.available.reg.mark")))
			{
				return false;
			}

			String aText = StringUtils.strip(regAnchor.getStringText(), " \t\r\n");
			String href = AppConfig.getInstance().getPropInternal("zy.available.link.mark");
			if(!StringUtils.equals(aText, href))
			{
				return false;
			}
			return true;
		}

		public String getLink()
		{
			if(isAvailable())
			{
				return regAnchor.getLink();
			}
			return null;
		}

		public String toString()
		{
			StringBuilder sb = new StringBuilder();
			sb.append("doctor=").append(doctor);
			sb.append(",hospital=").append(hospital);
			sb.append(",available=").append(available);
			sb.append("(").append(isAvailable()).append(")");
			sb.append(",regAnchor=").append(regAnchor.getLink());
			sb.append(",doctorAnchor=").append(doctorAnchor.getLink());
			return sb.toString();
		}
	}
}
