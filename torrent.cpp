#include "bencode.h"
#include "bdecode.h"

#include <iostream>

int main()
{
	
	std::vector<bencode_value> simple_list{bencode_value("string"), bencode_value(35)};
	
	std::map<bencode_value, bencode_value> map;
	map[bencode_value("foo")] = bencode_value("bar");
	map[bencode_value("size")] = bencode_value(500000);
	map[bencode_value("string")] = bencode_value(simple_list);

	std::vector<bencode_value> list{bencode_value(55), bencode_value("string"), bencode_value(map)};
	
	std::cout << bencode(map) << std::endl;
	std::cout << bencode(simple_list) << std::endl;
	std::cout << bencode(list) << std::endl;
	
	std::cout << bencode("This is a test string!") << std::endl;
	std::cout << bencode(355) << std::endl;
	
	std::cout << bdecode("22:This is a test string!").string_value() << std::endl;
	std::cout << bdecode("i355324e").int_value() << std::endl;
}
