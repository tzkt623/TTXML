#include "TTXML_Policy.h"
#include "TTXML_Global.h"

TTXML_Policy::TTXML_Policy(const std::string &key)
{
	keys.push_back(key);
}


TTXML_Policy::~TTXML_Policy()
{
}

void TTXML_Policy::pushKey( const std::string &key )
{
	keys.push_back(key);
}

void TTXML_Policy::popKey()
{
	keys.pop_front();
}

std::string TTXML_Policy::frontKey()
{
	if (keys.empty())
	{
		return TTXML_NULL_PKEY;
	}

	return keys.front();
}
