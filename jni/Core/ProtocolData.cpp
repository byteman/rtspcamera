#include "ProtocolData.h"


namespace CORE {


CElement::CElement(TiXmlElement* element,TiXmlDocument* doc):
	m_DOMElement(element),
	 m_doc(doc)
{
}

CElement::CElement()
{
	m_DOMElement = 0;
	m_doc = 0;
	m_index = 0;
}

CElement::~CElement()
{
}

int CElement::RemoveSubElement(const char* name,int index)
{
	TiXmlElement* element = m_DOMElement->FirstChildElement(name);
	while (index > 0)
	{
		element = element->NextSiblingElement(name);
		index--;
	}
	if (element!=0)
	{
		bool ret =  m_DOMElement->RemoveChild(element);
		return (ret==0)?-1:0;
	}
	return -1;
}
CElement::CElement(const CElement& element)
{
	this->m_DOMElement = element.m_DOMElement;
	this->m_doc		= element.m_doc;
}

int CElement::SetAttribute(const char* name,const char* value)
{
	m_DOMElement->SetAttribute(name,value);
	return 0;
}

int CElement::SetAttribute(const char* name,int value)
{
	CData temp(value);
	return SetAttribute(name,temp.c_str());
}

int CElement::SetElementText(const char* text)
{
	if (!m_DOMElement->FirstChild())
	{
		TiXmlText tiText(text);
		TiXmlText textCopy( tiText );
		m_DOMElement->InsertEndChild(textCopy);
	}	
	else
	{	TiXmlNode* node = m_DOMElement->FirstChild();	

		node->SetValue(text);	
	}
	
	return 0;
}

int CElement::SetElementText(int nText)
{
	CData temp(nText);
	return SetElementText(temp.c_str());
}

bool CElement::isEmpty()
{
	return m_DOMElement==0;
}
bool CElement::operator ==(TiXmlElement* element)
{
	return (m_DOMElement==element);
}

bool CElement::operator !=(TiXmlElement* element)
{
	return (m_DOMElement!=element);
}

CData CElement::GetAttribute(const char* name)
{
	return CData(m_DOMElement->Attribute(name));
}

CData CElement::GetElementText()
{
	return CData(m_DOMElement->GetText());

}

CElement CElement::GetSubElement(const char* name,int index)
{
	TiXmlElement* xmlNode = m_DOMElement->FirstChildElement(name);
	while ((index > 0 )&&( xmlNode != NULL))
	{
		xmlNode = xmlNode->NextSiblingElement(name);
		index--;
	}
	return CElement(xmlNode,m_doc);
}

CElement CElement::AddSubElement(const char* name)
{
	TiXmlElement childElement(name);// = m_doc->createElement(X(name));
	m_DOMElement->InsertEndChild(childElement); // ->appendChild(childElement);
	return CElement(m_DOMElement->LastChild(name)->ToElement(),m_doc); 
}

//////////////////////////////////////////////////////////////////////
// class CProtocolData Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLHEAD::CXMLHEAD()
{
	m_pdata = 0;
}

CXMLHEAD::~CXMLHEAD()
{
}

void CXMLHEAD::SetImp(CProtocolData* pData)
{
	m_pdata = pData;
}

CData CXMLHEAD::MessageType()
{
	return m_pdata->GetValueStep("Message/Header","Message_Type");
}

int CXMLHEAD::MessageType(char *value)
{
	return m_pdata->SetAttribute("Message/Header","Message_Type",value);
}

int CXMLHEAD::GetSeq()
{
	CData data = m_pdata->GetValueStep("Message/Header","Seq");
	return data.convertInt();
}

int CXMLHEAD::SetSeq(int seqNum)
{
	CData data(seqNum);
	return m_pdata->SetAttribute("Message/Header","Seq",data.c_str());
}


unsigned int CXMLHEAD::GetSessionId()
{
	CData data = m_pdata->GetValueStep("Message/Header","SessionId");
	return (unsigned int)(atof(data.c_str()));
}

int CXMLHEAD::SetSessionId(unsigned int sessionId)
{
	char tmp[32];
	sprintf(tmp,"%u",sessionId);
	return m_pdata->SetAttribute("Message/Header","SessionId",tmp);
}

CData CXMLHEAD::GetEleType()
{
	return m_pdata->GetValueStep("Message/Header","Type");
}

int CXMLHEAD::SetEleType(char *value)
{
	return m_pdata->SetAttribute("Message/Header","Type",value);
}
	

CData CXMLHEAD::GetFrom()
{
	return m_pdata->GetValueStep("Message/Header","From");
}

int CXMLHEAD::SetFrom(char *value)
{
	return m_pdata->SetAttribute("Message/Header","From",value);
}

CData CXMLHEAD::GetTo()
{
	return m_pdata->GetValueStep("Message/Header","To");
}

int CXMLHEAD::SetTo(char *value)
{
	return m_pdata->SetAttribute("Message/Header","To",value);
}
CProtocolData::CProtocolData()
{
	m_bParseSucc = false;
//	m_list	= 0;
	m_index = 0;
	m_tixmlDom = new TiXmlDocument();

	m_xmlHead.SetImp(this);
}

CProtocolData::~CProtocolData()
{
	if (m_tixmlDom!=0)
	{
		delete m_tixmlDom;
	}
}

CData CProtocolData::GetMessageType()
{
	return m_xmlHead.MessageType();
}

int CProtocolData::SetMessageType(const char *strType)
{
	return m_xmlHead.MessageType((char*)strType);
}

int CProtocolData::Parse(const char* fileName)
{
	m_tixmlDom->Clear();
	m_tixmlDom->LoadFile(fileName);
	if (m_tixmlDom->Error())
	{
		return -1;
	}

	m_bParseSucc = 1;
	return 0;	
}

int CProtocolData::Parse(const char* proData,int dataLen)
{
	if (dataLen<=0)
	{
		return -1;
	}
	m_tixmlDom->Clear();
	m_tixmlDom->Parse(proData);
	if(m_tixmlDom->Error())
	{
		return -1;
	}
	m_bParseSucc = 1;

	return 0;
}



CData CProtocolData::GetValue(const char* elementName,
							  const char* attributeName)
{
	if (!m_bParseSucc)
	{
		assert(0);
		return "";
	}

	TiXmlElement* element = GetDOMElement(elementName);
	return (element->Attribute(attributeName));
}

CData CProtocolData::GetValue(const char* elementName)
{
	if (!m_bParseSucc)
	{
		assert(0);
		return "";
	}
	TiXmlElement* element = GetDOMElement(elementName);
	return CData(element->GetText());
}

CData CProtocolData::GetValueStep(const char* elementNames,
								  const char* attributeName)
{
	if (!m_bParseSucc)
	{
		assert(0);
		return "";
	}
	CElement element = GetElement(elementNames);

	return (element.GetAttribute(attributeName));


}

CData CProtocolData::GetValueStep(const char* elementNames)
{
	if (!m_bParseSucc)
	{
		assert(0);
		return "";
	}
	CElement element = GetElement(elementNames);

	return element.GetElementText();
}

int CProtocolData::Query(const char* elementNames)
{
	Reset();
	int ret  = GetDOMElementList(elementNames);
	return ret;//(m_list==0)?-1:m_list->getLength();
}

void CProtocolData::Reset()
{
	m_index = 0;
//	m_list  = 0;
}

CElement CProtocolData::Next()
{
	int i = 0;
	std::list<TiXmlElement*>::iterator iter= m_elementList.begin();
	
	for ( ; (iter != m_elementList.end())&&(i <= m_index);iter++)
	{
	
		i++;
	}
	if (iter == m_elementList.end())
	{
		return 0;
	}
	TiXmlElement* node = (TiXmlElement*)*iter;//->item(m_index);
	m_index ++ ;
	if (node == NULL)
	{
		return 0;
	}
	// 获取元素值，即后续第一个Text节点的值
	if (TiXmlElement::ELEMENT == node->Type())
	{
		return CElement(node);
	}
	return 0;
}

CElement CProtocolData::AddElementStep(const char* elementNames)
{
	CData names(elementNames);

	bool bMatchFail = false;
	CData parent = names.parse(NODE_DIVISION,&bMatchFail);
	// now parent="Message" names="Test/IE"
	if (bMatchFail) // 没有找到"/"，不能添加根元素
	{
		return 0;
	}
	TiXmlElement* paremtnElement = GetDOMElementStep(parent.c_str());
	if (paremtnElement==0) // 指定的根元素不正确
	{
		return 0;
	}
	// 查找下一个元素
	CData next = names.parse(NODE_DIVISION,&bMatchFail);
	// now next="Test" names="IE"
	while(!bMatchFail)
	{
		if (paremtnElement==0)
		{
			return 0;
		}
		parent = parent + NODE_DIVISION + next;
		// 判断该级元素是否存在，不存在添加一个新元素
		if (GetDOMElementStep(parent.c_str())==0)
		{
			TiXmlElement child(next.c_str());
			paremtnElement->InsertEndChild(child);
		}
		paremtnElement = GetDOMElementStep(parent.c_str());
		next = names.parse(NODE_DIVISION,&bMatchFail);
	}
	// bMatchFail==true,names中剩下最后一级元素，添加最后一级元素
	// now names="IE"
	TiXmlElement last(names.c_str());
	paremtnElement->InsertEndChild(last);
	return CElement(paremtnElement->LastChild(names.c_str())->ToElement());;
}

CElement CProtocolData::AddElement(const char* elementNames)
{
	CData names(elementNames);
	int pos = names.find_last_of(NODE_DIVISION,names.length());
	if (pos==CDATA_NPOS) // 如果没找到"/",不能添加根元素
	{
		return 0;
	}
	CElement element = GetElement(names.substring(0,pos).c_str());
			
	return element.AddSubElement(names.substring(pos+1,names.length()).c_str());
}

CElement CProtocolData::GetElement(const char* elementNames)
{
	TiXmlElement* element = GetDOMElementStep(elementNames);
	if (element == NULL)
	{
		return 0;
	}
	return CElement(element);
}

int CProtocolData::SetAttribute(const char* elementNames, 
								const char* name, 
								int value)
{
	CData temp(value);
	return SetAttribute(elementNames,name,temp.c_str());
}

int CProtocolData::SetAttribute(const char* elementNames, 
								const char* name, 
								const char* value)
{
	CElement element = GetElement(elementNames);	
	if (element!=0)
	{
		element.SetAttribute(name,value);
		return 0;
	}
	return -1;
}

int CProtocolData::SetText(const char* elementNames,
						   int nText)
{
	CData temp(nText);
	return SetText(elementNames,temp.c_str());
}

int CProtocolData::SetText(const char* elementNames,
						   const char* text)
{
	CElement element = GetElement(elementNames);
	if (element!=0)
	{
		element.SetElementText(text);
		return 0;
	}
	return -1;
}



int CProtocolData::WritetoFile(const char* pFileName)
{
	try
	{
		m_tixmlDom->SaveFile(pFileName);

	}
	catch(...)
	{
		return -1;
	}

	return 0;
}

int CProtocolData::GetMemoryBufferData(CData &data)
{
	if (!m_bParseSucc)
	{
		return -1;
	}
	TiXmlPrinter printer;
	m_tixmlDom->Accept( &printer );
	data = CData(printer.CStr());

	return data.length();
}


bool CProtocolData::GetProtocolStr(char *out, int *outLen)
{
	CData data;
	int xmlLen;
	xmlLen=GetMemoryBufferData(data);
	if(xmlLen<=0)
	{
		return false;
	}

	int id=0x11223344;
	memcpy(out, &id, 4);
	memcpy(out+4, &xmlLen, 4);
	memcpy(out+4+4, data.c_str(), xmlLen);
	*outLen=xmlLen+8;
	return true;
}


CXMLHEAD *CProtocolData::GetXmlHead()
{
	return &m_xmlHead;
}


TiXmlElement* CProtocolData::GetDOMElementStep(const char* multiElements)
{
	CData elementNames(multiElements);
	bool bMatchFail = false;
	TiXmlElement* element = m_tixmlDom->FirstChildElement();
	CData elementName = elementNames.parse(NODE_DIVISION,&bMatchFail);

	if (bMatchFail)
	{
		if (strcmp(elementNames.c_str(),element->Value())==0)
		{
			return element;
		}
		return 0;
	}
	else
	{
		if (strcmp(elementName.c_str(),element->Value())!=0)
		{
			return 0;
		}
	}

	for (elementName = elementNames.parse(NODE_DIVISION,&bMatchFail);
		 !bMatchFail;
		 elementName = elementNames.parse(NODE_DIVISION,&bMatchFail)
		)
	{
		element = GetSubDOMElement(element,elementName.c_str());
		if (element==0)
		{
			return 0;
		}
	}
	elementName = elementNames;
	return GetSubDOMElement(element,elementName.c_str());
}


TiXmlElement* CProtocolData::GetSubDOMElement(const TiXmlElement* parent,
										 const char* elementName)
{
	TiXmlElement*  result = 0;
	result = (TiXmlElement*)parent->FirstChildElement(elementName);
	

	if (result!=0 && TiXmlNode::ELEMENT == result->Type())
	{
		return result;
	}
	return 0;
}

TiXmlElement* CProtocolData::GetDOMElement(const char* elementName)
{
	TiXmlElement* element =0;

	element = GetDOMElementStep(elementName);
	if (element != 0)
	{
		return element;
	}

	element = m_tixmlDom->FirstChildElement();
	TiXmlElement* nextElement = 0;
	TiXmlElement* parentElement = 0;

	while (element)
	{
		if (element->FirstChildElement() == 0)
		{
			if (strcmp(element->Value(),elementName) == 0)
			{
				return element;
			}
			else 
			{
				nextElement = element->NextSiblingElement();
				if (nextElement == 0)
				{
					parentElement = element->Parent()->ToElement();
					element = parentElement;
				}
				else
				{
					element = nextElement;
				}
				
			}
		}
		else if (element == parentElement)
		{
			element = element->NextSiblingElement();
		}
		else
		{
			element = element->FirstChildElement();
		}
	}
				
	if(TiXmlNode::ELEMENT == element->Type() )
	{
		return element  ;
	}
	return 0;
}



int CProtocolData::GetDOMElementList(const char* elementNames)
{	
	CData names(elementNames);
	TiXmlElement* element = 0;//m_tixmlDom->FirstChildElement(names.c_str());
	
	CData name = names.parse(NODE_DIVISION);
	CData dd = m_tixmlDom->FirstChild(name.c_str())->Value();
	if (name=="") // 一级
	{
		return GetElementsByName(m_tixmlDom->ToElement(), elementNames);
	}	
	else if (m_tixmlDom->FirstChild(name.c_str()) == NULL)
	{
		return 0;
	}

	element = GetDOMElement(name.c_str());

	bool bMatchFail = false;
	for (name = names.parse(NODE_DIVISION,&bMatchFail);
	!bMatchFail;
	name = names.parse(NODE_DIVISION,&bMatchFail)
		)
	{
		element = GetSubDOMElement(element,name.c_str());
		if (element==0)
		{
			return 0;
		}
	}
	return GetElementsByName(element, names.c_str());
}

int CProtocolData::GetElementsByName(TiXmlElement* elementconst, const char* elementName)
{
	TiXmlElement* element =0;
	element = elementconst->FirstChildElement(elementName);

	while (element)
	{
		m_elementList.push_back(element);
		element = element->NextSiblingElement(elementName);
	}
	return 0;
}


}


