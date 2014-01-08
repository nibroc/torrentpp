#include "bencode.h"
#include "bencode_value.h"
#include "bdecode.h"

#include "utestpp/utestpp.h"

#include <fstream>
#include <iterator>
#include <iostream>
#include <string>

void test_int()
{
	ASSERT_EQUAL(bdecode("i3e"), bencode_value(3));
	ASSERT_EQUAL(bdecode("i0e"), bencode_value(0));
	ASSERT_EQUAL(bdecode("i1099511627776e"), bencode_value(1099511627776ll));
	ASSERT_EQUAL(bdecode("i-3e"), bencode_value(-3));
	ASSERT_EQUAL(bencode(3), "i3e");
	ASSERT_EQUAL(bencode(0), "i0e");
	ASSERT_EQUAL(bencode(1099511627776ll), "i1099511627776e");
	ASSERT_EQUAL(bencode(-17), "i-17e");
	
	ASSERT_THROWS(bdecode("ie"));
	ASSERT_THROWS(bdecode("i123ae"));
	ASSERT_THROWS(bdecode("i123 e"));
	ASSERT_THROWS(bdecode("i 123e"));
	ASSERT_THROWS(bdecode("i 123 e"));
	ASSERT_THROWS(bdecode("i e"));
	ASSERT_THROWS(bdecode("iabce"));
	
	//Technically invalid by bencode standard, but not worth rejection over
	//ASSERT_THROWS(bdecode("i-0e"));
}

void test_string()
{
	ASSERT_EQUAL(bdecode("5:hello"), bencode_value("hello"));
	ASSERT_EQUAL(bdecode("11:hello world"), bencode_value("hello world"));
	ASSERT_EQUAL(bdecode("9:!!!333@@@"), bencode_value("!!!333@@@"));
	
	ASSERT_THROWS(bdecode("9:!!!"));
	ASSERT_THROWS(bdecode("-3:!!!"));
	ASSERT_THROWS(bdecode("abc:!!!"));
	ASSERT_THROWS(bdecode(":"));
	ASSERT_THROWS(bdecode("3:"));
	ASSERT_THROWS(bdecode("3"));
	ASSERT_THROWS(bdecode("a:1"));
}

void test_list()
{
	return;
	const std::string encoded = "l4:spam4:eggse";
	auto list = std::vector<bencode_value>{bencode_value("spam"), bencode_value("eggs")};
	bencode_value expected = bencode_value(list);
	ASSERT_TRUE(bdecode(encoded) == expected);
}

void test_dict()
{ }

void run_tests()
{
	test_int();
	test_string();
	test_list();
	test_dict();
}

int main()
{
	run_tests();
	return 0;
	std::ifstream fs("example.torrent");
	std::string s = std::string(std::istream_iterator<char>(fs), std::istream_iterator<char>());
	bencode_value v(bdecode(s));
	//std::cout << v.to_string() << std::endl;
}
