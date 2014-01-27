/****************************************************************************************************
*  TTXML_Node.h
*  作者 : 扈晨龙(特兹卡特)
*  功能 : XML数据节点
*  时间 : 14-1-13
*  版权 : Copyright (C) by 扈晨龙 2014年.
*  说明 : 1.数据组织结构,可以导出成XML文件
*           2.每个节点的内存由其父节点管理,不用手动管理内存
*           3.所有节点必须全部通过TTXML,TTXML_Node类create出来,不能使用new或者对象
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
	 *	@brief 创建本节点的子节点
	 *	@param k 键
	 *	@param v值 不填为创建组节点
	 *	@return 返回创建好的节点
	 *	@创建一个本节点的子节点,非自由节点
	 */
	TTXML_Node *simpleCreate(const std::string &k, const std::string &v = TTXML_NODE);

	/**
	 *	@brief 挂接一个子节点
	 *	@param child 子节点 值节点或组节点
	 */
	void linkChild(TTXML_Node *child);

	/**
	 *	@brief 是否有子节点
	 *	@判断是否有子节点
	 */
	bool hasChildren() { return !children.empty(); }

	/**
	 *	@brief 是否有属性
	 *	@
	 */
	bool hasAtrribute() { return !this->attributes.empty(); }

	/**
	 *	@brief 是否是组节点
	 *	@判断是否是组节点
	 */
	bool isNode() { return value == TTXML_NODE; }

	/**
	 *	@brief 清理
	 *	@清理所有子节点,会释放他们的内存
	 */
	void clear();

	/**
	 *	@brief 查找子节点
	 *	@param key 子节点键
	 */
	TTXML_Node *find(const std::string &key);

	/**
	 *	@brief 移除子节点
	 *	@param key 要移除的节点的键
	 *	@用于在只知道key的情况下移除节点,如果没有此节点,将不会做任何事情
	 */
	void removeChild(const std::string &key);

	/**
	 *	@brief 移除子节点
	 *	@param node 要移除的节点
	 *	@用于在知道节点指针的情况下移除节点,如果没有此节点,将不会做任何事情
	 */
	void removeChild(TTXML_Node *node);

	/**
	 *	@brief 判断相等
	 *	@param node 另一个节点
	 *	@简单的判断,只会判断键值对是否相等,不会判断子节点
	 */
	bool isEqual(const TTXML_Node *node);

	/**
	 *	@brief 空节点
	 *	@判断是否为空节点
	 */
	bool isNull() const;

	/**
	 *	@brief 遍历子节点
	 *	@param function 函数地址
	 *	@一个遍历子节点的方法
	 */
	void foreachChild(const std::function<void(TTXML_Node *)> &function);

	/**
	 *	@brief 插入一个组节点或者值节点
	 *	@param node 要插入的节点
	 *	@插入值节点:如果存在,将覆盖原来的值,如果不存在,添加
	 *	@插入组节点:如果存在,将删除以前的,再添加,如果不存在,添加
	 */
	void insert(TTXML_Node *node);

	/**
	 *	@brief 改变一个值节点的值
	 *	@param key 键
	 *	@param value 值
	 *	@改变本节点上一个子值节点的值,如果没有,则在本节点上添加一个,如果为组节点,则不做任何事情
	 */
	void setValue(const std::string &key, const std::string &value);

	/**
	 *	@brief 简单深拷贝
	 *	@param 要拷贝的节点
	 *	@简单的拷贝,只拷贝当前节点键值对,不管子节点
	 */
	void simpleCopy(TTXML_Node *node);

	/**
	 *	@brief 结构大小统计
	 */
	unsigned int classSize();

	/**
	 *	@brief 切断与父节点的链接
	 *	@return 是否切断成功
	 *	@切断与父节点的链接,如果没有父节点,则返回false
	 */
	bool cutLinkFromParent();

	/**
	 *	@brief 添加属性
	 *	@param key属性键
	 *	@param value 属性值
	 */
	void addAttribute(const std::string &key, const std::string &value);

	/**
	 *	@brief 移除属性
	 *	@param key 属性键
	 */
	void removeAttribute(const std::string &key);

	/**
	 *	@brief 遍历属性
	 */
	void foreachAttribute(const std::function<void(std::string, std::string)> &function);

	/**
	 *	@brief 获得属性
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
	 *	@brief 删除子节点的引用
	 *	@param node 子节点 值节点或组节点
	 */
	bool removeRef(TTXML_Node *node);
private:
	
	TTXML_Node *parent;
};

#endif//__TT_XML_Node_H__