#include "bencode.h"
#include "bencode_value.h"
#include "bdecode.h"

#include <fstream>
#include <iterator>
#include <iostream>
#include <string>

#define ASSERT_TRUE_MSG(cond, msg) test_assert(cond, msg, __FILE__, __func__, __LINE__)
#define ASSERT_EQUAL_MSG(x, y, msg) ASSERT_TRUE_MSG(x == y, msg)

#define ASSERT_TRUE(cond) ASSERT_TRUE_MSG(cond, "")
#define ASSERT_EQUAL(x, y) ASSERT_EQUAL_MSG(x, y, "")

#define ASSERT_THROWS_HELPER(expr, msg, file, func, line) \
	do {\
		bool threw = false;\
		try {\
			expr;\
		} catch (...) { threw = true; }\
		test_assert(threw, msg, file, func, line);\
	} while (0)
#define ASSERT_THROWS_MSG(expr, msg) ASSERT_THROWS_HELPER(expr, msg, __FILE__, __func__, __LINE__)
#define ASSERT_THROWS(expr) ASSERT_THROWS_MSG(expr, "")

inline void test_assert(bool cond, const std::string& msg, const std::string& file, const std::string& func, int line)
{
	if (!cond) {
		if (msg.empty()) {
			throw std::runtime_error("Failed test in "
									+ file + " in " + func 
									+ " on line " + std::to_string(line));
		} else {
			throw std::runtime_error("Failed test: " + msg + ". " 
									+ file + " in " + func 
									+ " on line " + std::to_string(line));
		}
	}
}

inline void throw_fail(const std::string& msg, const std::string& file, const std::string& func, int line)
{
	test_assert(false, msg, file, func, line);
}

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
	
	ASSERT_THROWS(bdecode("iabce"));
	ASSERT_THROWS(bdecode("ie"));
	ASSERT_THROWS(bdecode("i-0e"));
	ASSERT_THROWS(bdecode("i-ae"));
}

void test_string()
{
	ASSERT_EQUAL(bdecode("5:hello"), bencode_value("hello"));
	ASSERT_EQUAL(bdecode("11:hello world"), bencode_value("hello world"));
	ASSERT_EQUAL(bdecode("9:!!!333@@@"), bencode_value("!!!333@@@"));
	
	ASSERT_EQUAL(bencode("hello"), "5:hello");
	ASSERT_EQUAL(bencode("hello world"), "11:hello world");
	ASSERT_EQUAL(bencode("13"), "2:13");
	ASSERT_EQUAL(bencode("-13"), "3:-13");
	
	ASSERT_THROWS(bdecode("50:!!!333@@@"));
	ASSERT_THROWS(bdecode("1:!!!333@@@"));
	ASSERT_THROWS(bdecode("-9:!!!333@@@"));
}

void test_list()
{
	return;
	const std::string encoded = "l4:spam4:eggse";
	auto list = std::vector<bencode_value>{bencode_value("spam"), bencode_value("eggs")};
	bencode_value expected = bencode_value(list);
	ASSERT_EQUAL(bdecode(encoded), expected);
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
