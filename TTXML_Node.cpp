#include "TTXML_Node.h"


TTXML_Node *TTXML_Node::nullNode = new TTXML_Node("NullNode", "Null");
std::string TTXML_Node::errorValue = "_tt_xml_error";

TTXML_Node::TTXML_Node():
	parent(nullptr)
{
	key.clear();
	value.clear();
	children.clear();
	attributes.clear();
}

TTXML_Node::TTXML_Node(const std::string &k, const std::string &v):
	key(k),
	value(v),
	parent(nullptr)
{
	children.clear();
	attributes.clear();
}

TTXML_Node::~TTXML_Node()
{
	//清理子节点
	clear();
}

void TTXML_Node::linkChild( TTXML_Node *child )
{
//	printf("node %s add a child %s\n", this->key.c_str(), child->key.c_str());
#if TT_Debug
	printf("node %s add a child %s\n", this->key.c_str(), child->key.c_str());
#endif
	child->parent = this;
	this->children.push_back(child);
}

void TTXML_Node::clear()
{
#if TT_Debug
	printf("node %s clear!\n", this->key.c_str());
#endif

	if (this->hasChildren())
	{
		auto it = this->children.begin();
		while (it != this->children.end())
		{
			//删除节点
			delete (*it);
			//删除索引
			this->children.erase(it++);
		}
		this->children.clear();
	}
}

TTXML_Node * TTXML_Node::find( const std::string &key )
{
#if TT_Debug
	printf("node %s finding %s\n", this->key.c_str(), key.c_str());
#endif
	auto it = children.begin();
	while(it != children.end())
	{
		if ( (*it)->key == key )
		{
			return (*it);
		}
		++it;
	}

	return nullNode;
}

void TTXML_Node::removeChild( const std::string &key )
{
#if TT_Debug
	printf("node %s remove child %s\n", this->key.c_str(), key.c_str());
#endif

	auto temp = this->find(key);
	if (!temp->isNull())
	{
		//删除链表里temp节点的引用
		this->removeRef(temp);
		delete temp;
	}
}

void TTXML_Node::removeChild(TTXML_Node *node)
{
#if TT_Debug
	printf("node %s remove child %s\n", this->key.c_str(), node->key.c_str());
#endif
	if (!node->isNull())
	{
		//删除链表里temp节点的引用
		this->removeRef(node);
		delete node;
	}
}

void TTXML_Node::foreachChild( const std::function<void(TTXML_Node *)> &function )
{
	auto it = this->children.begin();
	while (it != this->children.end())
	{
		function( (*it) );
		if ( (*it)->hasChildren() )
		{
			(*it)->foreachChild(function);
		}
		++it;
	}
}

bool TTXML_Node::removeRef( TTXML_Node *node )
{
	auto it = this->children.begin();
	while (it != this->children.end())
	{
		if ( (*it) == node )
		{
			this->children.erase(it);
			return true;
		}
		++it;
	}

	return false;
}

int TTXML_Node::toInt(const std::string &k)
{
	return std::stoi(k);
}

float TTXML_Node::toFloat(const std::string &k)
{
	return std::stof(k);
}

double TTXML_Node::toDouble(const std::string &k)
{
	return std::stod(k);
}

bool TTXML_Node::toBool(const std::string &k)
{
	if (k == "false" || k == "FALSE")
	{
		return false;
	}
	else if (k == "true" || k == "TRUE")
	{
		return true;
	}

	auto temp = k.at(0);
	if (temp > '9' || temp <= '0')
	{
		return false;
	}
	else if (temp > '0')
	{
		return true;
	}
}

std::string TTXML_Node::toString(const std::string &k)
{
	return k;
}

bool TTXML_Node::isEqual( const TTXML_Node *node )
{
	if (this->key == node->key && this->value == node->value)
	{
		return true;
	}
	return false;
}

bool TTXML_Node::isNull() const
{
	if (this->key == nullNode->key && this->value == nullNode->value)
	{
		return true;
	}
	return false;
}

TTXML_Node *TTXML_Node::simpleCreate( const std::string &k, const std::string &v /*= TTXML_NODE*/ )
{
	auto newNode = new TTXML_Node(k, v);
	this->linkChild(newNode);
	return newNode;
}

void TTXML_Node::simpleCopy( TTXML_Node *node )
{
	this->value = node->value;
}

void TTXML_Node::insert( TTXML_Node *node )
{
	auto temp_node = this->find(node->key);

	//如果插入的是一组数据
	if (node->isNode())
	{
		if (!temp_node->isNull())
		{
			this->removeChild(temp_node);
			this->linkChild(node);
		}
		else
		{
			this->linkChild(node);
		}
	}
	//如果是单个数据
	else
	{
		if (temp_node->isNull())
		{
			this->linkChild(node);
		}
		else
		{
			temp_node->simpleCopy(node);
			delete node;
		}
	}
}

void TTXML_Node::setValue( const std::string &key, const std::string &value )
{
	auto temp = this->find(key);
	if (!temp->isNull() && !temp->isNode())
	{
		temp->value = value;
	}
	else
	{
		auto value_node = new TTXML_Node(key, value);
		this->linkChild(value_node);
	}
}

unsigned int TTXML_Node::classSize()
{
	auto size = sizeof(TTXML_Node);
	if (this->hasChildren())
	{
		this->foreachChild([&](TTXML_Node *node)
		{
			size += node->classSize();
		});
	}
	return size;
}

bool TTXML_Node::cutLinkFromParent()
{
	if (parent)
	{
		if(parent->removeRef(this))
		{
			parent = nullptr;
		}
		return true;
	}
	return false;
}

void TTXML_Node::addAttribute( const std::string &key, const std::string &value )
{
	this->attributes[key] = value;
}

void TTXML_Node::foreachAttribute( const std::function<void(std::string, std::string)> &function )
{
	if (this->attributes.empty())
	{
		return;
	}

	auto it = this->attributes.begin();
	while (it != this->attributes.end())
	{
		function(it->first, it->second);
		++it;
	}
}

std::string TTXML_Node::getAttribute( const std::string &key )
{
	if (this->attributes.empty())
	{
		return errorValue;
	}

	auto it = this->attributes.find(key);
	if (it != this->attributes.end())
	{
		return it->second;
	}

	return errorValue;
}

void TTXML_Node::removeAttribute( const std::string &key )
{
	auto it = this->attributes.find(key);
	if ( it != this->attributes.end())
	{
		this->attributes.erase(it);
	}
}





