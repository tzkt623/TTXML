/****************************************************************************************************
*  TTXML_Node.h
*  ���� : �賿��(���ȿ���)
*  ���� : XML���ݽڵ�
*  ʱ�� : 14-1-13
*  ��Ȩ : Copyright (C) by �賿�� 2014��.
*  ˵�� : 1.������֯�ṹ,���Ե�����XML�ļ�
*           2.ÿ���ڵ���ڴ����丸�ڵ����,�����ֶ������ڴ�
*           3.���нڵ����ȫ��ͨ��TTXML,TTXML_Node��create����,����ʹ��new���߶���
****************************************************************************************************/
#ifndef __TT_XML_Node_H__
#define __TT_XML_Node_H__

#include "TTXML_Global.h"
#include <functional>
#include <string>
#include <list>
#include <map>

enum class TTXML_NodeSwitchType
{
	ST_KV,
	ST_AT
};

class TTXML_Node
{
public:
	/**
	 *	@brief �������ڵ���ӽڵ�
	 *	@param k ��
	 *	@param vֵ ����Ϊ������ڵ�
	 *	@return ���ش����õĽڵ�
	 *	@����һ�����ڵ���ӽڵ�,�����ɽڵ�
	 */
	TTXML_Node *simpleCreate(const std::string &k, const std::string &v = TTXML_NODE);

	/**
	 *	@brief �ҽ�һ���ӽڵ�
	 *	@param child �ӽڵ� ֵ�ڵ����ڵ�
	 */
	void linkChild(TTXML_Node *child);

	/**
	 *	@brief �Ƿ����ӽڵ�
	 *	@�ж��Ƿ����ӽڵ�
	 */
	bool hasChildren() { return !children.empty(); }

	/**
	 *	@brief �Ƿ�������
	 *	@
	 */
	bool hasAtrribute() { return !this->attributes.empty(); }

	/**
	 *	@brief �Ƿ�����ڵ�
	 *	@�ж��Ƿ�����ڵ�
	 */
	bool isNode() { return value == TTXML_NODE; }

	/**
	 *	@brief ����
	 *	@���������ӽڵ�,���ͷ����ǵ��ڴ�
	 */
	void clear();

	/**
	 *	@brief �����ӽڵ�
	 *	@param key �ӽڵ��
	 */
	TTXML_Node *find(const std::string &key);

	/**
	 *	@brief �Ƴ��ӽڵ�
	 *	@param key Ҫ�Ƴ��Ľڵ�ļ�
	 *	@������ֻ֪��key��������Ƴ��ڵ�,���û�д˽ڵ�,���������κ�����
	 */
	void removeChild(const std::string &key);

	/**
	 *	@brief �Ƴ��ӽڵ�
	 *	@param node Ҫ�Ƴ��Ľڵ�
	 *	@������֪���ڵ�ָ���������Ƴ��ڵ�,���û�д˽ڵ�,���������κ�����
	 */
	void removeChild(TTXML_Node *node);

	/**
	 *	@brief �ж����
	 *	@param node ��һ���ڵ�
	 *	@�򵥵��ж�,ֻ���жϼ�ֵ���Ƿ����,�����ж��ӽڵ�
	 */
	bool isEqual(const TTXML_Node *node);

	/**
	 *	@brief �սڵ�
	 *	@�ж��Ƿ�Ϊ�սڵ�
	 */
	bool isNull() const;

	/**
	 *	@brief �����ӽڵ�
	 *	@param function ������ַ
	 *	@һ�������ӽڵ�ķ���
	 */
	void foreachChild(const std::function<void(TTXML_Node *)> &function);

	/**
	 *	@brief ����һ����ڵ����ֵ�ڵ�
	 *	@param node Ҫ����Ľڵ�
	 *	@����ֵ�ڵ�:�������,������ԭ����ֵ,���������,���
	 *	@������ڵ�:�������,��ɾ����ǰ��,�����,���������,���
	 */
	void insert(TTXML_Node *node);

	/**
	 *	@brief �ı�һ��ֵ�ڵ��ֵ
	 *	@param key ��
	 *	@param value ֵ
	 *	@�ı䱾�ڵ���һ����ֵ�ڵ��ֵ,���û��,���ڱ��ڵ������һ��,���Ϊ��ڵ�,�����κ�����
	 */
	void setValue(const std::string &key, const std::string &value);

	/**
	 *	@brief �����
	 *	@param Ҫ�����Ľڵ�
	 *	@�򵥵Ŀ���,ֻ������ǰ�ڵ��ֵ��,�����ӽڵ�
	 */
	void simpleCopy(TTXML_Node *node);

	/**
	 *	@brief �ṹ��Сͳ��
	 */
	unsigned int classSize();

	/**
	 *	@brief �ж��븸�ڵ������
	 *	@return �Ƿ��жϳɹ�
	 *	@�ж��븸�ڵ������,���û�и��ڵ�,�򷵻�false
	 */
	bool cutLinkFromParent();

	/**
	 *	@brief �������
	 *	@param key���Լ�
	 *	@param value ����ֵ
	 */
	void addAttribute(const std::string &key, const std::string &value);

	/**
	 *	@brief �Ƴ�����
	 *	@param key ���Լ�
	 */
	void removeAttribute(const std::string &key);

	/**
	 *	@brief ��������
	 */
	void foreachAttribute(const std::function<void(std::string, std::string)> &function);

	/**
	 *	@brief �������
	 */
	std::string getAttribute(const std::string &key);


public:
	static int toInt(const std::string &value);
	static float toFloat(const std::string &value);
	static double toDouble(const std::string &value);
	static bool toBool(const std::string &value);
	static std::string toString(const std::string &value);

public:
	std::string key;
	std::string value;
	std::list<TTXML_Node *> children;
	std::map<std::string, std::string> attributes;

public:
	static TTXML_Node *nullNode;
	static std::string errorValue;

private:
	friend class TTXML;

	TTXML_Node();
	TTXML_Node(const std::string &k, const std::string &v = TTXML_NODE);
	~TTXML_Node();

	/**
	 *	@brief ɾ���ӽڵ������
	 *	@param node �ӽڵ� ֵ�ڵ����ڵ�
	 */
	bool removeRef(TTXML_Node *node);
private:
	
	TTXML_Node *parent;
};

#endif//__TT_XML_Node_H__