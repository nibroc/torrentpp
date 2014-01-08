#include "bdecode.h"

#include <cctype>
#include <cstddef>
#include <string>
#include <stdexcept>
#include <iostream>

template <typename Iter>
static bencode_value parse_string(Iter beg, const Iter& end)
{
	Iter it = beg;
	for (; it != end && *it != ':'; ++it) { /* empty */ }
	if (it == end) {
		throw std::runtime_error("");
	}
	
	std::size_t idx = 0;
	int len = std::stoi(std::string(beg, it), &idx);
	
	if (static_cast<int>(idx) != it - beg) {
		throw std::runtime_error("");
	}
	
	++it;
	
	if (end - it != len) {
		throw std::runtime_error("");
	} else {
		return bencode_value(std::string(it, end));
	}
}

template <typename Iter>
static bencode_value parse_integer(Iter beg, const Iter& end)
{
	for (auto it = beg; it != end; ++it) {
		const auto& ch = *it;
		if (ch == '-' && it == beg) { continue; }
		if (std::isdigit(*it)) { continue; }
		throw std::runtime_error("Failed to parse integer");
	}
	return bencode_value(std::stoll(std::string(beg, end)));
}

template <typename Iter>
static bencode_value parse_list(Iter beg, const Iter& end)
{
	if (beg == end) {
		throw std::runtime_error("Empty string cannot be bdecoded");
	}
	return bencode_value("");
}

template <typename Iter>
static bencode_value parse_dict(Iter beg, const Iter& end)
{
	if (beg == end) {
		throw std::runtime_error("Empty string cannot be bdecoded");
	}
	return bencode_value("");
}

template<typename Iter>
bencode_value bdecode(Iter begin, Iter end)
{
	if (begin == end) {
		throw std::runtime_error("Empty string cannot be bdecoded");
	}
	switch (*begin) {
		case 'i':
			return parse_integer(++begin, --end);
		case 'l':
			return parse_list(++begin, --end);
		case 'd':
			return parse_dict(++begin, --end);
		default:
			return parse_string(begin, end);
	}
}

bencode_value bdecode(const std::string& str)
{
	return bdecode(str.begin(), str.end());
}
