// PropListMgr.cpp: implementation of the CPropListMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FFProfile.h"
#include "PropListMgr.h"
#include "tinyxml.h"
#include "cflbase/tstring.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPropListMgr::CPropListMgr(CBCGPPropList* pPropList) : m_pPropList(pPropList)
{

}

CPropListMgr::~CPropListMgr()
{

}

#define XML_NM_PROFILE			"profile"
#define XML_NM_PARAMS			"params"
#define XML_NM_FILE			"file"

BOOL CPropListMgr::Init(LPCTSTR lpXMLFile)
{
	/*
	CBCGPProp* pGroup1 = new CBCGPProp (_T("Appearance"));
	
	pGroup1->AddSubItem (new CBCGPProp (_T("3D Look"), (_variant_t) false,
		_T("Specifies the dialog's font will be nonbold and controls will have a 3D border")));
	
	CBCGPProp* pProp = new CBCGPProp (_T("Border"), _T("Dialog Frame1"),
		_T("One of: None, Thin, Resizable, or Dialog Frame"));
	pProp->AddOption (_T("None"));
	pProp->AddOption (_T("Thin"));
	pProp->AddOption (_T("Resizable"));
	pProp->AddOption (_T("Dialog Frame"));
	pProp->AllowEdit (FALSE);
	
	pGroup1->AddSubItem (pProp);
	pGroup1->AddSubItem (new CBCGPProp (_T("Caption"), (_variant_t) _T("About BCGProTest"),
		_T("Specifies the text that will be displayed in the dialog's title bar")));
	
	m_pPropList->AddProperty (pGroup1);
	
	CBCGPProp* pSize = new CBCGPProp (_T("Window Size"), 0, TRUE);
	
	pProp = new CBCGPProp (_T("Height"), (_variant_t) 250l,
		_T("Specifies the dialog's height"));
	pProp->EnableSpinControl (TRUE, 0, 1000);
	pSize->AddSubItem (pProp);
	
	pProp = new CBCGPProp (	_T("Width"), (_variant_t) 150l,
		_T("Specifies the dialog's width"));
	pProp->EnableSpinControl ();
	pSize->AddSubItem (pProp);
	
	m_pPropList->AddProperty (pSize);
	*/


	CString szXmlFile = lpXMLFile;
	
	TiXmlDocument doc( CFL_T2A((LPCTSTR)szXmlFile) );
	bool loadOkay = doc.LoadFile();	
	if ( !loadOkay )
	{
		AfxTrace(_T("Failed to load file %s. Error=%s.\n"), lpXMLFile, doc.ErrorDesc());
		return FALSE;
	}
	
	TiXmlNode *pNode = NULL;
	TiXmlElement *pElement = NULL;
	
	pNode = doc.FirstChild( XML_NM_PROFILE );
	ASSERT( pNode );
	pElement = pNode->ToElement();
	ASSERT(pElement);

	pNode = pElement->FirstChild(XML_NM_PARAMS);
	if(pNode == NULL)
	{
		return FALSE;
	}
	pElement = pNode->ToElement();
	ASSERT(pElement);

	
	for(pNode = pElement->FirstChild(); pNode != NULL; pNode = pNode->NextSibling())
	{
		pElement = pNode->ToElement();
		ASSERT(pElement);
		
		CBCGPProp* pGroup = new CBCGPProp (pElement->Value());

		if(!InitProp(pGroup, pElement))
		{
			return FALSE;
		}
		m_pPropList->AddProperty(pGroup);
	}

	return TRUE;
}

BOOL CPropListMgr::InitProp(CBCGPProp* pParentProp, TiXmlElement *pParentXml)
{
	TiXmlNode *pNode, *pChildNode = NULL;
	TiXmlElement *pElement, *pChildElement = NULL;

	const char* szName = NULL;
	const char* szDesc = NULL;
	const char* szValue = NULL;
	const char* szDefault = NULL;
	for(pNode = pParentXml->FirstChild(); pNode != NULL; pNode = pNode->NextSibling())
	{
		pElement = pNode->ToElement();
		ASSERT(pElement);

		szName = pElement->Attribute("name");
		if(szName == NULL)
		{
			szName = "DummyName";
		}
		szDesc = pElement->Attribute("desc");
		if(szDesc == NULL)
		{
			szDesc = szName;
		}

		CBCGPProp* pProp = new CBCGPProp (szName, _T(""), (LPCTSTR)szDesc);

		for(pChildNode = pElement->FirstChild(); pChildNode != NULL; pChildNode = pChildNode->NextSibling())
		{
			pChildElement = pChildNode->ToElement();
			ASSERT(pChildElement);
		
			szValue = pChildElement->Attribute("value");
			if(szValue)
			{
				pProp->AddOption(CFL_A2T(szValue));
			}
			szDefault = pChildElement->Attribute("default");
			if(szDefault != NULL && stricmp(szDefault, "true") == 0)
			{
				pProp->SetOriginalValue(szValue);
				pProp->SetValue(szValue);
			}
		}
		pProp->AllowEdit (FALSE);
		
		pParentProp->AddSubItem (pProp);
	}

	return TRUE;
}