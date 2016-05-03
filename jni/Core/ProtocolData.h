#ifndef CProtocolData_H
#define CProtocolData_H


#include "Core.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <list>
#include "Data.h"


namespace CORE {


class CProtocolData;
class CORE_API CXMLHEAD
{
public:
	CXMLHEAD();
	~CXMLHEAD();
	CData MessageType();
	int MessageType(char *value);
	int GetSeq();
	int SetSeq(int seqNum);
	unsigned int GetSessionId();
	int SetSessionId(unsigned int sessionId);
	CData GetFrom();
	int SetFrom(char *value);
	CData GetEleType();
	int SetEleType(char *value);
	CData GetTo();
	int SetTo(char *value);

	void SetImp(CProtocolData* pData);
private:
	CProtocolData *m_pdata;
};

// 多级元素分隔符
const char NODE_DIVISION[3] = "/";
/**
 * @class CElement
 * @brief XML文档中一个元素的简单封装
 * 
 * @n 该类对外只提供了获取和设置元素属性、元素值的方法以及简单的和空指
 * @n 针比较的方法。该类是CProtocolData类GetElement()方法和AddElement()
 * @n 方法的返回值，可以通过和0比较判断是否存在该元素，例如：
 * @n CProtocolData data;
 * @n data.Parse("test.xml");
 * @n CElement element = data.GetElement("Message/Header");
 * @n if (element!=0)
 * @n {
 * @n	element.SetAttribute("name","test");
 * @n	element.GetAttribute("name");
 * @n	...
 * @n }
 * @n CElement IE_result = data.AddElement("Message/IE_RESULT");
 * @n if (IE_result!=0)
 * @n {
 * @n	IE_result.SetAttribute("ErrorCode","10020");
 * @n	...
 * @n }
 * @n 同时，CProtocolData是该类的友元类，CProtocolData的方法中可以直接访
 * @n 问其私有成员m_DOMElement，以方便操作
 * @n 注意：这个类是访问XML数据的辅助类，只有在CProtocolData类还没有析构
 * @n 时访问有效，如果CProtocolData已析构，访问会非法操作
 *
 */
class CORE_API CElement
{
	friend class CProtocolData;
public:
	// 默认构造函数m_DOMElement = 0,m_doc = 0;
	CElement();
	// 构造函数
	CElement(TiXmlElement* element,TiXmlDocument* doc = 0);
	// 析构函数
	~CElement();
	// 拷贝构造函数，拷贝m_DOMElement指针值
	CElement(const CElement& element);
	// 设置元素属性，如果属性已存在，替换属性值，否则添加一个新的属性
	int   SetAttribute(const char* name,const char* value);
	int   SetAttribute(const char* name,int value);
	// 设置元素值，0:success -1:error
	int   SetElementText(const char* text);
	int   SetElementText(int nText);
	// 获取元素属性值，不存在或失败时返回""
	CData GetAttribute(const char* name);
	// 获取属性值，元素为空或获取失败时返回值为""
	CData GetElementText();
	// 判断该元素是否为空和判断是否==0相同
	bool isEmpty();
	// 判断是否相等，主要用于和0比较判断是否为空
	bool operator == (TiXmlElement* element);
	// 判断是否不相等，主要用于和0比较判断是否不为空
	bool operator != (TiXmlElement* element);
	CElement GetSubElement(const char* name,int index=0);
	CElement AddSubElement(const char* name);

	// 删除子元素
	int RemoveSubElement(const char* name,int index = 0);
	
private:

	TiXmlElement*  m_DOMElement;
	TiXmlDocument* m_doc;
	int		  m_index;
};


/**
 * @class CProtocolData
 * @brief XML协议数据解析访问类
 *
 * @n 该文件定义了访问系统协议数据区XML格式数据的接口方法
 * @n 对XML数据采用DOM解析器解析。Parse()方法用于输入XML数据，
 * @n 并解析。然说使用多种GetValue方法获取所需要的数据。
 * @li GetValue()用于直接获取XML数据中的某个元素，如果出现了多个
 * @n 这样的元素,只能获取第一个元素的属性和值
 * @li GetValueStep()用于获取指定元素的属性和值，参数由多个元素
 * @n 名称和"/"组成，例如：Message/IE_Result
 * @n 如果需要访问多个相同的元素，使用Query()先查询，然后调用
 * @n Next()逐个访问元素，例如：
 * @n <?xml version="1.0" encoding="GB2312"?>
 * @n <Message>
 * @n	 <Test name="t1"/>
 * @n	 <Test name="t2"/>
 * @n </Message>
 * @n test.xml文件中的数据如上所示
 * @n CProtocolData xmlData;
 * @n xmlData.Parse("test.xml"); 
 * @n CElement element = xmlData.Next();
 * @n while(element!=0)
 * @n {
 * @n	element.GetAttribute("name");
 * @n }
 */
class CORE_API CProtocolData  
{
	//私有拷贝构造函数，不支持拷贝构造
	CProtocolData(CProtocolData& proData);
public:
	// 构造函数
	CProtocolData();
	// 析构函数
	virtual ~CProtocolData();

	//获取协议号 char 型
	CData GetMessageType();
	int SetMessageType(const char *strType);  //设置协议号
	/**
	 * @brief 输入并解析XML数据
	 * 
	 * @param[in] proData xml数据
	 * @param[in] dataLen 数据长度
	 * @return 0  for success
	 * @return -1 for error,参数错误或者解析失败,检查日志信息察看错误
	 */
	int Parse(const char* proData,int dataLen);
	/**
	 * @brief 解析XML文件
	 *
	 * @param[in] fileName 文件名
	 * @return 0  for success
	 * @return -1 for error,参数错误或者解析失败,检查日志信息察看错误
	 */
	int Parse(const char* fileName);

	/**
	 * @brief 直接获取元素属性
	 *
	 * 如果系统中有多个相同的元素，该函数只获取第一个，例如：
	 * @n <?xml version="1.0" encoding="GB2312" ?>
	 * @n <Message>
	 * @n   <IE><Test name="t1"/></IE>
	 * @n   <Test name="t2"/>
	 * @n </Message>
	 * @n 如果解析并调用GetValue("Test","name")=="t1"
	 * 
	 * @param[in] elementName 元素名称
	 * @param[in] attributeName 属性名称
	 * @return CData 属性值，如果没有获取到，返回""
	 */
	CData GetValue(const char* elementName,
				   const char* attributeName);
	/**
	 * @brief 直接获取元素值
	 * 
	 * 和获取元素属性函数一样，只能获取第一个元素的值，
	 * 由于协议数据没有定义DTD,获取的元素值会包含空格
	 * 需要在生成协议时保证不能有空格，否则容易出错。例如：
	 * @n <?xml version="1.0" encoding="GB2312" ?>
	 * @n <Message>
	 * @n  <IE>fdsfs</IE>
	 * @n  <IE>fdsfs
	 * @n  </IE>
	 * @n </Message>
	 * 在获取第一个IE元素值时为"fdsfs"，获取第二个IE元素值
	 * 时，包括fdsfs后的所有空格字符(包括' ',回车换行,Tab)
	 * 
	 * @param[in] elementName 元素名称
	 * @return CData 元素值，如果没有获取到，返回""
	 */
	CData GetValue(const char* elementName);

	/**
	 * @brief 获取元素属性，逐级获取
	 *
	 * 获取指定元素属性，同GetValue函数一样，如果有多个，
	 * 只能获取第一个元素，如果XML数据中没有相同的元素，可
	 * 使用GetValue()直接获取较快，例如：
	 * @n <?xml version="1.0" encoding="GB2312" ?>
	 * @n  <Message>
	 * @n	<IE name="test">fdsfs</IE>
	 * @n	<IE>fdsfs d</IE>
	 * @n  </Message>
	 * @n  GetValueStep("Message/IE","name")=="test"
	 * 
	 * @param[in] elementNames 多级元素名称，name1/name2
	 * @param[in] attributeName 属性名称
	 * @return CData 属性值，如果没有获取到，返回""
	 */
	CData GetValueStep(const char* elementNames,
					   const char* attributeName);
	/**
	 * @brief 获取元素属值，逐级获取
	 *
	 * 获取指定元素属值，同GetValue函数一样，如果有多个，
	 * 只能获取第一个元素，如果XML数据中没有相同的元素，可
	 * 使用GetValue()直接获取较快，例如：
	 * @n  <?xml version="1.0" encoding="GB2312" ?>
	 * @n  <Message>
	 * @n	<IE name="test">fdsfs</IE>
	 * @n	<IE>fdsfs d</IE>
	 * @n  </Message>
	 * @n  GetValueStep("Message/IE")=="fdsfs"
	 * 
	 * @param[in] elementNames 多级元素名称，name1/name2
	 * @return CData 元素值，如果没有获取到，返回""
	 */
	CData GetValueStep(const char* elementNames);

	/**
	 * @brief 查询多个元素
	 *
	 * 使用Query可以查询多个元素，如果elementNames不是多级方式
	 * 在整个文档中查找，如果指定到某个元素(即多级名称方式)，只
	 * 查找指定的多个元素。查询后使用Next()访问查询的元素，在执行
	 * 新查询前，最好先调用Reset()方法。例如：
	 * CProtocolData proData;
	 * proData.parse("XML数据",数据长度);
	 * proData.Reset();
	 * proData.Query("IE");
	 * CElement element = proData.Next();
	 * while (element!=0)
	 * {
	 *	CData value = element.GetAttribute("attributeName");
	 *	// do something
	 * }
	 * 
	 * @param[in] elementNames 元素名称
	 * @return >0 查询到的元素数量
	 * @return -1 for error,
	 */
	int   Query(const char* elementNames);
	/**
	 * @brief 下一个元素
	 *
	 * Query()后使用Next()逐个访问查询到的元素
	 * 
	 * @return !=0 下一个元素
	 * @return ==0  访问结束
	 */
	CElement Next();
	/**
	 * @brief 重置查询
	 *
	 * 在执行一个查询后，再执行新的查询前需要调用该方法，重置查询索引
	 * 和查询结果
	 * 
	 * @return void
	 */
	void  Reset();

	// 增加一个新的元素，如果增加失败，返回0
	CElement AddElement(const char* elementNames);
	CElement AddElementStep(const char* elementNames);
	/**
	 * @brief 设置元素属性
	 * 
	 * @param[in] elementNames 为多级元素方式，例如：Message/IE
	 * @param[in] name 属性名称
	 * @param[in] value 属性值
	 * @return 0  for success
	 * @return -1 for error,
	 */
	int SetAttribute(const char* elementNames,
					 const char* name,
					 const char* value);
	int SetAttribute(const char* elementNames, 
					 const char* name, 
					 int  value);
	// 设置员素值，和SetAttribute相同elementNames为多级元素方式
	int SetText(const char* elementNames,const char* text);
	int SetText(const char* elementNames,int nText);
	/**
	 * @brief 获取一个元素
	 *
	 * 如果有多个相同的元素，只获取第一个
	 * 
	 * @param[in] elementNames为多级元素方式
	 * @return !=0  获取成功
	 * @return ==0  未获取到
	 */
	CElement GetElement(const char* elementNames);
	/**
	 * @brief 获取XML文档的二进制形式的数据
	 *
	 * @param[out] data，获取到的数据 
	 * @return 0  for success
	 * @return -1 for error,
	 */
	int GetMemoryBufferData(CData &data);

	bool GetProtocolStr(char *out, int *outLen);
	
	/**
	 * @brief 将XML文档写入文件
	 *
	 * @param[int] pFileName，文件名 
	 * @return 0  for success
	 * @return -1 for error,
	 */
	int WritetoFile(const char* pFileName);
	
	///获得heard操作
	CXMLHEAD *GetXmlHead();

private:

	TiXmlElement*  GetSubDOMElement(const TiXmlElement* parent,const char* elementName);
	
	TiXmlElement*  GetDOMElement(const char* elementName);
	
	TiXmlElement*  GetDOMElementStep(const char* multiElements);
	
	int GetDOMElementList(const char* elementNames);
	
	int GetElementsByName(TiXmlElement* element, const char* elementName);
	
private:

	TiXmlDocument* m_tixmlDom;
	int		  m_index;
	bool m_bParseSucc;
	CXMLHEAD m_xmlHead;
	std::list<TiXmlElement*> m_elementList;

};


}


#endif 


