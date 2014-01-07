#include "bdecode.h"

#include <cstddef>
#include <string>
#include <stdexcept>
#include <iostream>

static bencode_value parse_string(const std::string& str)
{
	typedef std::string::const_iterator iter;
	iter it = str.begin();
	const iter end = str.end();
	for (; it != end && *it != ':'; ++it) { /* empty */ }
	if (it == end) {
		throw std::runtime_error("");
	}
	
	std::size_t idx = 0;
	int len = std::stoi(std::string(str.begin(), it), &idx);
	
	if (static_cast<int>(idx) != it - str.begin()) {
		throw std::runtime_error("");
	}
	
	++it;
	
	if (end - it != len) {
		throw std::runtime_error("");
	} else {
		return bencode_value(std::string(it, end));
	}
}

static bencode_value parse_integer(const std::string& str)
{
	if (str.front() != 'i' || str.back() != 'e') {
		throw std::runtime_error("");
	}
	std::int64_t val = std::stoll(std::string(str.begin() + 1, str.end() - 1));
	return bencode_value(val);
}

static bencode_value parse_list(const std::string& str)
{
	return bencode_value(str);
}

static bencode_value parse_dict(const std::string& str)
{
	return bencode_value(str);
}

bencode_value bdecode(const std::string& str)
{
	switch(str[0]) {
		case 'i':
			return parse_integer(str);
		case 'l':
			return parse_list(str);
		case 'd':
			return parse_dict(str);
		default:
			return parse_string(str);
	}
}
