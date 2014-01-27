/***********************************************************************
*  TTXML_Policy.h
*  ���� : �賿��(���ȿ���)
*  ���� : XML ���Ҳ���
*  ʱ�� : 14-1-13
*  ��Ȩ : Copyright (C) by �賿�� 2014��
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
	 *	@brief ����һ���¼�������
	 */
	void pushKey(const std::string &key);

private:
	~TTXML_Policy();

	/**
	 *	@brief ��ǰ������
	 */
	std::string frontKey();

	/**
	 *	@brief ����һ��������
	 */
	void popKey();

	/**
	 *	@brief ����������
	 */
	unsigned int keyCount() const { return keys.size(); }

private:
	friend class TTXML;
	std::list<std::string> keys;
};

#endif//__TT_XML_Policy_H__