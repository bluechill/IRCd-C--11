//
//  Utility.h
//  IRCd
//
//  Created by Alex Turner on 4/3/14.
//  Copyright (c) 2014 EvilZone. All rights reserved.
//

#ifndef IRCd_Utility_h
#define IRCd_Utility_h

#include <vector>
#include <string>
#include <algorithm>

namespace utility
{
	std::vector<std::string> split_string_spaces(const std::string& to_split)
	{
		std::stringstream ss(to_split);
		std::istream_iterator<std::string> begin(ss);
		std::istream_iterator<std::string> end;
		std::vector<std::string> vec(begin, end);

		return vec;
	}

	std::vector<std::string> split_string(const std::string& to_split, const char split_character)
	{
		if (split_character == ' ')
			return split_string_spaces(to_split);

		std::vector<std::string> vec;
		std::string container;

		for (auto const c : to_split)
		{
			if (c != split_character)
				container += c;
			else
			{
				if (!container.empty())
					vec.push_back(container);

				container.clear();
			}
		}

		if (!container.empty())
			vec.push_back(container);
		
		return vec;
	}	
};

#endif
