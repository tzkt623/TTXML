/***************************************************************************************************************
*  TTXML.h
*  ���� : �賿��(���ȿ���)
*  ���� : XML����
*  ʱ�� : 14-1-10.
*  ��Ȩ : Copyright (c) 2014�� �賿��.
*  ˵�� : 1.������֯�ṹ,���Ե�����XML�ļ�
*           2.ÿ���ڵ���ڴ����丸�ڵ����,�����ֶ������ڴ�
*           3.���нڵ����ȫ��ͨ��TTXML,TTXML_Node��create����,����ʹ��new���߶���
***************************************************************************************************************/
#ifndef __TT_XML_H__
#define __TT_XML_H__

#include "tinyxml2/tinyxml2.h"

#include <string>
#include <list>
#include <functional>
#include "TTXML_Global.h"
#include "TTXML_Node.h"
#include "TTXML_Policy.h"

class TTXML
{
public:
	TTXML();
	~TTXML();

	/**
	 *	@brief ��ȡXML�ļ�
	 *	@param file �ļ�·��
	 *	@param root_key ���ڵ�� Ĭ��ֵΪRootNode
	 *	@param root_value ���ڵ�ֵ Ĭ��ֵΪnode
	 */
	bool loadFile(const std::string &file, const std::string &root_key = TTXML_ROOT_NODE, const std::string &root_value = TTXML_NODE);

	/**
	 *	@brief ��ʽ����׼XML�ַ���
	 *	@param content xmlȫ�ı�
	 *	@param root_key ���ڵ�� Ĭ��ֵΪRootNode
	 *	@param root_value ���ڵ�ֵ Ĭ��ֵΪnode
	 */
	bool loadFileForData(const std::string &content, const std::string &root_key = TTXML_ROOT_NODE, const std::string &root_value = TTXML_NODE);

	/**
	 *	@brief ����һ��XML���ݽṹ
	 *	@param file �ļ�ȫ·��
	 *	@param root_key ���ڵ�� Ĭ��ֵΪRootNode
	 *	@param root_value ���ڵ�ֵ Ĭ��ֵΪnode
	 */
	void createXML(const std::string &file, const std::string &root_key = TTXML_ROOT_NODE, const std::string &root_value = TTXML_NODE);

	/**
	 *	@brief ����һ�����ɽڵ�
	 *	@param key �ڵ��
	 *	@param value �ڵ�ֵ Ĭ��Ϊ������ڵ�
	 */
	TTXML_Node *createNode(const std::string &key, const std::string &value = TTXML_NODE);

	/**
	 *	@brief �򵥲���     һ��ֵ����ڵ�
	*	@param key ��
	*	@param value ֵ ����ֵ״̬�� Ϊ����һ����ڵ�
	*	@param policy ��Ѱ���� Ϊ�����������ڵ�
	*	@�������ͬ�ڵ�����ͬ�������,�Ḳ��ԭ����ֵ �㲻����һ����node���͵Ľڵ��в�����ڵ��ֵ�ڵ�
	 */
	void insert(const std::string &key, const std::string &value = TTXML_NODE, TTXML_Policy *policy = nullptr);

	/**
	 *	@brief �߼�����      һ���ڵ�
	*	@param node �ڵ�
	*	@param policy ��Ѱ���� Ϊ�����������ڵ�
	*	@�������ͬ�ڵ�����ͬ�������,�Ḳ��ԭ����ֵ �㲻����һ����node���͵Ľڵ��в�����ڵ��ֵ�ڵ�
	 */
	void insert(TTXML_Node *node, TTXML_Policy *policy = nullptr);

	/**
	 *	@brief ����һ��ֵ
	 *	@param key
	 *	@param value
	 *	@���û�����ֵ,�����½�һ��
	 *	@���޷����ڵ�����ֵ
	 */
	void setValue(const std::string &key, const std::string &value, TTXML_Policy *policy = nullptr);

	/**
	 *	@brief �Ƴ�һ��ֵ����ڵ�
	 *	@param key �ڵ�ļ�
	 *	@param policy ���Ҳ��� Ĭ��Ϊ���Ҹ��ڵ�
	 *	@ͨ��һ�����Ҳ������Ƴ�һ���ڵ�
	 */
	void remove(const std::string &key, TTXML_Policy *policy = nullptr);

	/**
	 *	@brief ��������XML
	 */
	void clear();

	/**
	 *	@brief ����
	 *	@�ô����õ�·�����ٱ���
	 */
	void save(const std::string &flie);
	void save();
	
	/**
	 *	@brief �����ṹ��С
	 */
	std::string treeSize();

	/**
	 *	@brief ��ӡ
	 */
	void printXML();

	/**
	 *	@brief ��������XML
	 */
	TTXML_Node *foreach(TTXML_Node *node, TTXML_Policy *policy);

	/**
	 *	@brief ��ȡ�ڵ�
	 */
	TTXML_Node *getNode(TTXML_Policy *policy = nullptr);

	/**
	 *	@brief ��ȡ���ڵ�
	 */
	TTXML_Node *getRootNode() const { return m_RootNode; }

private:
	/**
	 *	@brief ��ʼ��
	 */
	void init( const std::string &root_key, const std::string &root_value );

	/**
	 *	@brief ����
	 */
	void analyze(tinyxml2::XMLElement *root, TTXML_Node *node);

	/**
	 *	@brief ת����XML
	 */
	void loadToXML(const std::list<TTXML_Node *> &node, tinyxml2::XMLDocument &doc, tinyxml2::XMLNode *like_node = nullptr);

	/**
	 *	@brief ��ʽ����ӡ
	 */
	void formatPrint(TTXML_Node *node);


private:
	int m_iLayer;
	std::string m_FilePath;
	TTXML_Node *m_RootNode;
};


#endif//__TT_XML_H__