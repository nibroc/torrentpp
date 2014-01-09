#include "bdecode.h"

#include <cctype>
#include <cstddef>
#include <string>
#include <stdexcept>

template <typename Iter>
static bencode_value parse_dict(Iter& beg, const Iter& end);

template<typename Iter>
static bencode_value bdecode(Iter& begin, Iter end);

template <typename Iter>
static bencode_value parse_string(Iter& beg, const Iter& end)
{
	if (beg == end) {
		throw std::runtime_error("Cannot parse empty string");
	}
	
	Iter it = beg;
	for (; it != end && *it != ':'; ++it) { /* empty */ }
	
	if (it == end) {
		throw std::runtime_error("Invalid string format");
	}
	
	std::size_t idx = 0;
	int len = std::stoi(std::string(beg, it), &idx);
	
	if (static_cast<int>(idx) != it - beg) {
		throw std::runtime_error("");
	}
	
	++it;
	beg = it + len;
	return bencode_value(std::string(it, beg));
}

template <typename Iter>
static bencode_value parse_integer(Iter& beg, const Iter& end)
{
	if (*beg != 'i') {
		throw std::runtime_error("Integers must start with i");
	}
	
	auto last_valid = ++beg;
	for (; last_valid != end && *last_valid != 'e'; ++last_valid) {
		const auto& ch = *last_valid;
		if (ch == '-' && last_valid == beg) { continue; }
		if (!std::isdigit(ch)) {
			throw std::runtime_error("Integers must be decimal");
		}
	}
	
	if (last_valid == end || *last_valid != 'e') {
		throw std::runtime_error("Integers must end with e");
	}
	if (beg == last_valid) {
		throw std::runtime_error("Integers must have at least one digit");
	}
	
	bencode_value val(std::stoll(std::string(beg, last_valid)));
	beg = ++last_valid;
	return val;
}

template <typename Iter>
static bencode_value parse_list(Iter& beg, Iter end)
{
	if (beg == end) {
		throw std::runtime_error("Empty string cannot be bdecoded");
	}
	
	//Eat the prefix and suffix l
	++beg;
	
	bencode_value::list_type list;
	while (*beg != 'e' && beg != end) {
		switch (*beg) {
			case 'i':
				list.emplace_back(parse_integer(beg, end));
				break;
			case 'l':
				list.emplace_back(parse_list(beg, end));
				break;
			case 'd':
				list.emplace_back(parse_dict(beg, end));
				break;
			default:
				list.emplace_back(parse_string(beg, end));
		}
	}
	if (beg == end || *beg != 'e') {
		throw std::runtime_error("Lists must be suffixed by e");
	}
	++beg;
	return bencode_value(list);
}

template <typename Iter>
static bencode_value parse_dict(Iter& beg, const Iter& end)
{
	if (beg == end) {
		throw std::runtime_error("Empty string cannot be bdecoded");
	}
	
	//Eat the prefix d
	++beg;
	
	bencode_value::dict_type dict;
	while (beg != end && *beg != 'e') {
		dict[bdecode(beg, end)] = bdecode(beg, end);
	}
	if (beg == end || *beg != 'e') {
		throw std::runtime_error("Dicts must be suffixed by e");
	}
	
	//Eat the suffix d
	++beg;
	
	return bencode_value(dict);
}

template<typename Iter>
static bencode_value bdecode(Iter& begin, Iter end)
{
	if (begin == end) {
		throw std::runtime_error("Empty string cannot be bdecoded");
	}
	switch (*begin) {
		case 'i':
			return parse_integer(begin, end);
		case 'l':
			return parse_list(begin, end);
		case 'd':
			return parse_dict(begin, end);
		default:
			return parse_string(begin, end);
	}
}

bencode_value bdecode(const std::string& str)
{
	auto begin = str.cbegin();
	const auto& end = str.cend();
	bencode_value val(bdecode(begin, end));
	if (begin != end) {
		throw std::runtime_error("Unconsumed tokens: " + std::string(begin, end));
	}
	return val;
}
