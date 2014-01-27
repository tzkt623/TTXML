/***********************************************************************
*  TTXML_Policy.h
*  作者 : 扈晨龙(特兹卡特)
*  功能 : XML 查找策略
*  时间 : 14-1-13
*  版权 : Copyright (C) by 扈晨龙 2014年
***********************************************************************/
#ifndef __TT_XML_Policy_H__
#define __TT_XML_Policy_H__

#include <list>
#include <string>

class TTXML_Policy
{
public:
	TTXML_Policy(const std::string &key);
	
	/**
	 *	@brief 加入一个下级搜索键
	 */
	void pushKey(const std::string &key);

private:
	~TTXML_Policy();

	/**
	 *	@brief 当前搜索键
	 */
	std::string frontKey();

	/**
	 *	@brief 弹出一个搜索键
	 */
	void popKey();

	/**
	 *	@brief 搜索键个数
	 */
	unsigned int keyCount() const { return keys.size(); }

private:
	friend class TTXML;
	std::list<std::string> keys;
};

#endif//__TT_XML_Policy_H__