#include "TTXML.h"


TTXML::TTXML():
	m_RootNode(nullptr),
	m_iLayer(0)
{

}

TTXML::~TTXML()
{
	if (m_RootNode)
	{
		delete m_RootNode;
	}
}

void TTXML::init(const std::string &root_key, const std::string &root_value)
{
	m_iLayer = 0;
	if (m_RootNode)
	{
		delete m_RootNode;
	}
	m_RootNode = new TTXML_Node();
	m_RootNode->key = root_key;
	m_RootNode->value = root_value;
}

TTXML_Node * TTXML::createNode( const std::string &key, const std::string &value /*= TTXML_NODE*/ )
{
	return new TTXML_Node(key, value);
}

void TTXML::createXML( const std::string &file, const std::string &root_key, const std::string &root_value )
{
	this->init(root_key, root_value);
	m_FilePath = file;
}

bool TTXML::loadFile( const std::string &file, const std::string &root_key, const std::string &root_value )
{
	this->init(root_key, root_value);
	tinyxml2::XMLDocument doc;
	auto errorcode = doc.LoadFile( file.c_str() );

	switch (errorcode)
	{
	case tinyxml2::XML_NO_ERROR:
		m_FilePath = file;
		this->analyze(doc.FirstChildElement(), m_RootNode);
		return true;
		break;
	case tinyxml2::XML_NO_ATTRIBUTE:
		break;
	case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
		break;
	case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
		return false;
		break;
	case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
		break;
	case tinyxml2::XML_ERROR_FILE_READ_ERROR:
		break;
	case tinyxml2::XML_ERROR_ELEMENT_MISMATCH:
		break;
	case tinyxml2::XML_ERROR_PARSING_ELEMENT:
		break;
	case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE:
		break;
	case tinyxml2::XML_ERROR_IDENTIFYING_TAG:
		break;
	case tinyxml2::XML_ERROR_PARSING_TEXT:
		break;
	case tinyxml2::XML_ERROR_PARSING_CDATA:
		break;
	case tinyxml2::XML_ERROR_PARSING_COMMENT:
		break;
	case tinyxml2::XML_ERROR_PARSING_DECLARATION:
		break;
	case tinyxml2::XML_ERROR_PARSING_UNKNOWN:
		break;
	case tinyxml2::XML_ERROR_EMPTY_DOCUMENT:
		break;
	case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT:
		break;
	case tinyxml2::XML_ERROR_PARSING:
		break;
	case tinyxml2::XML_CAN_NOT_CONVERT_TEXT:
		break;
	case tinyxml2::XML_NO_TEXT_NODE:
		break;
	default:
		break;
	}

	return false;
}

bool TTXML::loadFileForData( const std::string &content, const std::string &root_key, const std::string &root_value )
{
	if (content.empty())
	{
		return false;
	}
	this->init(root_key, root_value);
	tinyxml2::XMLDocument doc;
	if (tinyxml2::XMLError::XML_ERROR_PARSING_TEXT != doc.Parse(content.c_str(), content.size()))
	{
		this->analyze(doc.FirstChildElement(), m_RootNode);
		return true;
	}
	return false;
}


void TTXML::analyze( tinyxml2::XMLElement *root, TTXML_Node *node )
{
	if (root == nullptr)
	{
		return;
	}

	//创建一个节点
	auto rootNode = new TTXML_Node();
	rootNode->key = root->Name();
	if (root->GetText()) rootNode->value = root->GetText();
	else rootNode->value = TTXML_NODE;

	//遍历解析节点属性,并加入节点
	const tinyxml2::XMLAttribute  *rootAttribute = root->FirstAttribute();
	while (rootAttribute)
	{
		const char *attributeName = rootAttribute->Name();
		const char *attributeValue = rootAttribute->Value();
		rootNode->addAttribute(attributeName, attributeValue);
		rootAttribute = rootAttribute->Next();
	}

	node->linkChild(rootNode);

	//递归遍历子节点
	auto child = root->FirstChildElement();
	if (child != nullptr)
	{
		analyze(child, rootNode);
	}

	//递归遍历同级节点
	auto nextSibling = root->NextSiblingElement();
	if (nextSibling != nullptr)
	{
		analyze(nextSibling, node);
	}
}

TTXML_Node * TTXML::foreach( TTXML_Node *node, TTXML_Policy *policy )
{
#if TT_Debug
	printf("foreach node %s, policy key %d\n", node->key.c_str(), policy->keyCount());
#endif
	
	if (policy->keyCount() == 0)
	{
#if TT_Debug
		printf("null policy key\n");
#endif
		return node;
	}

	node = node->find(policy->frontKey());
	
	if (!node->isNull())
	{
		policy->popKey();
		node = this->foreach(node, policy);
	}

	return node;
}

void TTXML::loadToXML( const std::list<TTXML_Node *> &node, tinyxml2::XMLDocument &doc ,tinyxml2::XMLNode *like_node )
{
	auto it = node.begin();
	while (it != node.end())
	{
		auto temp = (*it);
		auto rootNode = doc.NewElement(temp->key.c_str());
		like_node->LinkEndChild(rootNode);
		temp->foreachAttribute([&](std::string key, std::string value)
		{
#if TT_Debug
			printf("%s,%s\n",key.c_str(), value.c_str());
#endif
			rootNode->SetAttribute(key.c_str(), value.c_str());
		});

		if (temp->value == TTXML_NODE)
		{
			this->loadToXML(temp->children, doc, rootNode);
		}
		else
		{
			rootNode->LinkEndChild(doc.NewText(temp->value.c_str()));
		}
		++it;
	}
}

void TTXML::insert( const std::string &key, const std::string &value, TTXML_Policy *policy )
{
	//如果有搜索策略
	if (policy)
	{
		auto temp = this->foreach(m_RootNode, policy);
#if TT_Debug
		printf("find %s\n", temp->key.c_str());
#endif
		if (!temp->isNull() && temp->isNode())
		{
			//测试键是否存在
			auto child = temp->find(key);
			if (child->isNull())//如果不存在,添加一个新的node
			{
				auto newNode = new TTXML_Node(key, value);
				temp->linkChild(newNode);
			}
			else//如果存在 直接改变值
			{
				child->value = value;
			}
		}

		delete policy;
	}
	else//没有搜索策略,直接加入跟节点
	{
		auto newNode = new TTXML_Node(key, value);
		m_RootNode->linkChild(newNode);
	}
}

void TTXML::insert( TTXML_Node *node, TTXML_Policy *policy /*= nullptr*/ )
{
	if (policy)
	{
		auto temp = this->foreach(m_RootNode, policy);
		if (!temp->isNull())
		{
			temp->insert(node);
		}
	}
	else
	{
		m_RootNode->insert(node);
	}

}

void TTXML::setValue( const std::string &key, const std::string &value, TTXML_Policy *policy /*= nullptr*/ )
{
	if (policy)
	{
		auto temp = this->foreach(m_RootNode, policy);
		//找到的不是空节点,并且是组节点
		if (!temp->isNull() && temp->isNode())
		{
			temp->setValue(key, value);
		}
	}
	else
	{
		m_RootNode->setValue(key, value);
	}
}

void TTXML::remove( const std::string &key, TTXML_Policy *policy /*= nullptr*/ )
{
	if (policy)
	{
		auto temp = this->foreach(m_RootNode, policy);
		temp->removeChild(key);
		delete policy;
	}
	else
	{
		m_RootNode->removeChild(key);
	}
}

void TTXML::save(const std::string &flie)
{
	tinyxml2::XMLDocument doc;
	doc.LinkEndChild(doc.NewDeclaration());
	this->loadToXML(m_RootNode->children, doc, &doc);
	doc.SaveFile(flie.c_str());

//	m_RootNode->clear();
#if TT_Debug
	printf("save done! root node children count %d\n", m_RootNode->children.size());
#endif
}

void TTXML::save()
{
	this->save(m_FilePath);
}

void TTXML::clear()
{
	m_RootNode->clear();

#if TT_Debug
	printf("clear done! root node children count %d\n", m_RootNode->children.size());
#endif
}

TTXML_Node * TTXML::getNode( TTXML_Policy *policy /*= nullptr*/ )
{
	auto temp = TTXML_Node::nullNode;
	if (policy)
	{
		temp = this->foreach(m_RootNode, policy);
		delete policy;
	}
	else
	{
		temp = m_RootNode;
	}

	return temp;
}

void TTXML::printXML()
{
	printf("===========XML==================BEGIN==================\n");
	this->formatPrint(m_RootNode);
	printf("===========XML==================END====================\n");
	m_iLayer = 0;
}

void TTXML::formatPrint( TTXML_Node *node )
{
	auto it = node->children.begin();
	while (it != node->children.end())
	{
		if ( (*it)->isNode() )
		{
			for (int i = 0; i < m_iLayer; i++)
			{
				printf("\t");
			}

			if ((*it)->hasAtrribute())
			{
				printf("<%s ", (*it)->key.c_str());
				(*it)->foreachAttribute([&](std::string key, std::string value)
				{
					printf("%s=\"%s\" ", key.c_str(), value.c_str());
				});
				printf(">\n");
			}
			else
			{
				printf("<%s>\n", (*it)->key.c_str());
			}
		}
		else
		{
			for (int i = 0; i < m_iLayer; i++)
			{
				printf("\t");
			}
			printf("<%s>%s</%s>\n", (*it)->key.c_str(), (*it)->value.c_str(), (*it)->key.c_str());
		}

		if ( (*it)->hasChildren() )
		{
			++m_iLayer;
			formatPrint((*it));
		}

		if ( (*it)->isNode() )
		{
			for (int i = 0; i < m_iLayer; i++)
			{
				printf("\t");
			}
			printf("</%s>\n", (*it)->key.c_str());
		}
		++it;
	}
	--m_iLayer;
}

std::string TTXML::treeSize()
{
	/*
	std::string temp;
	auto size = m_RootNode->classSize();
	//Byte
	temp = std::to_string(size);
	temp += "B";
	//KB
	if (size > 1024)
	{
		size /= 1024;
		temp = std::to_string(size);
		temp += "KB";
	}
	//MB
	if (size > 1024)
	{
		size /= 1024;
		temp = std::to_string(size);
		temp += "MB";
	}
	//GB
	if (size > 1024)
	{
		size /= 1024;
		temp = std::to_string(size);
		temp += "GB";
	}
	return temp;
	*/
	return "KB";
}






