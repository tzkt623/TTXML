/***************************************************************************************************************
*  TTXML.h
*  作者 : 扈晨龙(特兹卡特)
*  功能 : XML解析
*  时间 : 14-1-10.
*  版权 : Copyright (c) 2014年 扈晨龙.
*  说明 : 1.数据组织结构,可以导出成XML文件
*           2.每个节点的内存由其父节点管理,不用手动管理内存
*           3.所有节点必须全部通过TTXML,TTXML_Node类create出来,不能使用new或者对象
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
	 *	@brief 读取XML文件
	 *	@param file 文件路径
	 *	@param root_key 根节点键 默认值为RootNode
	 *	@param root_value 根节点值 默认值为node
	 */
	bool loadFile(const std::string &file, const std::string &root_key = TTXML_ROOT_NODE, const std::string &root_value = TTXML_NODE);

	/**
	 *	@brief 格式化标准XML字符串
	 *	@param content xml全文本
	 *	@param root_key 根节点键 默认值为RootNode
	 *	@param root_value 根节点值 默认值为node
	 */
	bool loadFileForData(const std::string &content, const std::string &root_key = TTXML_ROOT_NODE, const std::string &root_value = TTXML_NODE);

	/**
	 *	@brief 创建一个XML数据结构
	 *	@param file 文件全路径
	 *	@param root_key 根节点键 默认值为RootNode
	 *	@param root_value 根节点值 默认值为node
	 */
	void createXML(const std::string &file, const std::string &root_key = TTXML_ROOT_NODE, const std::string &root_value = TTXML_NODE);

	/**
	 *	@brief 创建一个自由节点
	 *	@param key 节点键
	 *	@param value 节点值 默认为创建组节点
	 */
	TTXML_Node *createNode(const std::string &key, const std::string &value = TTXML_NODE);

	/**
	 *	@brief 简单插入     一个值或组节点
	*	@param key 键
	*	@param value 值 不赋值状态下 为插入一个组节点
	*	@param policy 搜寻策略 为空则搜索根节点
	*	@如果遇到同节点下有同名的情况,会覆盖原来的值 你不能向一个非node类型的节点中插入组节点或值节点
	 */
	void insert(const std::string &key, const std::string &value = TTXML_NODE, TTXML_Policy *policy = nullptr);

	/**
	 *	@brief 高级插入      一个节点
	*	@param node 节点
	*	@param policy 搜寻策略 为空则搜索根节点
	*	@如果遇到同节点下有同名的情况,会覆盖原来的值 你不能向一个非node类型的节点中插入组节点或值节点
	 */
	void insert(TTXML_Node *node, TTXML_Policy *policy = nullptr);

	/**
	 *	@brief 设置一个值
	 *	@param key
	 *	@param value
	 *	@如果没有这个值,将会新建一个
	 *	@你无法给节点设置值
	 */
	void setValue(const std::string &key, const std::string &value, TTXML_Policy *policy = nullptr);

	/**
	 *	@brief 移除一个值或组节点
	 *	@param key 节点的键
	 *	@param policy 查找策略 默认为查找根节点
	 *	@通过一个查找策略来移除一个节点
	 */
	void remove(const std::string &key, TTXML_Policy *policy = nullptr);

	/**
	 *	@brief 清理整个XML
	 */
	void clear();

	/**
	 *	@brief 保存
	 *	@用创建好的路径快速保存
	 */
	void save(const std::string &flie);
	void save();
	
	/**
	 *	@brief 整个结构大小
	 */
	std::string treeSize();

	/**
	 *	@brief 打印
	 */
	void printXML();

	/**
	 *	@brief 遍历整个XML
	 */
	TTXML_Node *foreach(TTXML_Node *node, TTXML_Policy *policy);

	/**
	 *	@brief 获取节点
	 */
	TTXML_Node *getNode(TTXML_Policy *policy = nullptr);

	/**
	 *	@brief 获取跟节点
	 */
	TTXML_Node *getRootNode() const { return m_RootNode; }

private:
	/**
	 *	@brief 初始化
	 */
	void init( const std::string &root_key, const std::string &root_value );

	/**
	 *	@brief 解析
	 */
	void analyze(tinyxml2::XMLElement *root, TTXML_Node *node);

	/**
	 *	@brief 转化到XML
	 */
	void loadToXML(const std::list<TTXML_Node *> &node, tinyxml2::XMLDocument &doc, tinyxml2::XMLNode *like_node = nullptr);

	/**
	 *	@brief 格式化打印
	 */
	void formatPrint(TTXML_Node *node);


private:
	int m_iLayer;
	std::string m_FilePath;
	TTXML_Node *m_RootNode;
};


#endif//__TT_XML_H__