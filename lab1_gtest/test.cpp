#include "pch.h"
using namespace LabTritSetSpace;
using namespace std;
using TritSetReference = TritSet::TritReference;

/* Self rules:
0) Getters, setters and constructors don't require any tests.
1) If result of test is incorrect:
1.1) Check that you try to test right function
1.2) Avoid using functions that you haven't tested before (except getters, setters and constructors)

2) Name of a case always is a class-name

3) Name of a test in the case always is:
3.1.1) a function name, if it's not an operator
3.1.2) If there're any overloads: 
		with typename-prefix of returning value

3.2.1) %opName% + "Operator" + "s" (optionally), if it's not a logic operator
3.2.2) "logicOperators" and all logic operators must be tested in one test
*/
TEST(TritSet, braceOperators) 
{
	const size_t len = 16;
	TritSet set(len);
	EXPECT_EQ(typeid(set[len - 1]), typeid(TritSetReference));

	set[len - 1] = Trit::True;
	set[len - 2] = Trit::False;
	EXPECT_EQ(set[len - 1], Trit::True);
	EXPECT_EQ(set[len - 2], Trit::False);

	set[len - 2] = Trit::True;
	set[len - 1] = Trit::False;
	EXPECT_EQ(set[len - 1], Trit::False);
	EXPECT_EQ(set[len - 2], Trit::True);

	set[len - 1] = Trit::Unknown;
	set[len - 2] = Trit::Unknown;
	EXPECT_EQ(set[len - 1], Trit::Unknown);
	EXPECT_EQ(set[len - 2], Trit::Unknown);
	
	const TritSet cset(len);
	EXPECT_EQ(typeid(cset[len - 1]), typeid(Trit));
}

TEST(TritSet, getLenght)
{
	const size_t len = 16;
	TritSet set(len);
	EXPECT_EQ(set.getLength(), len);

	set[len] = Trit::Unknown;
	EXPECT_EQ(set.getLength(), len);

	set[len] = Trit::True;
	EXPECT_EQ(set.getLength(), len + 1);
}

TEST(TritSet, capacity)
{
	const size_t len = tritPerUint;
	TritSet set(len);
	EXPECT_EQ(set.capacity(), uintSize);

	set[tritPerUint * tritPerUint] = Trit::Unknown;
	EXPECT_EQ(set.capacity(), uintSize);

	set[tritPerUint * 4 - 1] = Trit::True;
	EXPECT_EQ(set.capacity(), uintSize * 4);
}

TEST(TritSet, getSize)
{
	const size_t len = tritPerUint;
	TritSet set(len);
	EXPECT_EQ(set.getSize(), 1);

	set[tritPerUint * tritPerUint] = Trit::Unknown;
	EXPECT_EQ(set.getSize(), 1);

	set[tritPerUint * 4 - 1] = Trit::True;
	EXPECT_EQ(set.getSize(), 4);
}

TEST(TritSet, getLastSetTrit)
{
	const size_t len = 16;
	TritSet set(len);
	EXPECT_EQ(set.getLastSetTrit(), -1);

	set[len] = Trit::Unknown;
	EXPECT_EQ(set.getLastSetTrit(), -1);

	set[len - 1] = Trit::Unknown;
	EXPECT_EQ(set.getLastSetTrit(), -1);

	set[len - 2] = Trit::True;
	EXPECT_EQ(set.getLastSetTrit(), len - 2);

	set[len - 1] = Trit::True;
	EXPECT_EQ(set.getLastSetTrit(), len - 1);
}

TEST(TritSet, size_tCardinality)
{
	const size_t len = 16;
	TritSet set(len);
	EXPECT_EQ(set.cardinality(Trit::Unknown), len);
	EXPECT_EQ(set.cardinality(Trit::False), 0);
	EXPECT_EQ(set.cardinality(Trit::True), 0);

	set[len - 1] = Trit::True;
	EXPECT_EQ(set.cardinality(Trit::Unknown), len - 1);
	EXPECT_EQ(set.cardinality(Trit::False), 0);
	EXPECT_EQ(set.cardinality(Trit::True), 1);

	set[len - 1] = Trit::False;
	EXPECT_EQ(set.cardinality(Trit::Unknown), len - 1);
	EXPECT_EQ(set.cardinality(Trit::False), 1);
	EXPECT_EQ(set.cardinality(Trit::True), 0);

	set[len - 2] = Trit::True;
	EXPECT_EQ(set.cardinality(Trit::Unknown), len - 2);
	EXPECT_EQ(set.cardinality(Trit::False), 1);
	EXPECT_EQ(set.cardinality(Trit::True), 1);

	set[len * 2 - 1] = Trit::True;
	EXPECT_EQ(set.cardinality(Trit::Unknown), 2 * len - 3);
	EXPECT_EQ(set.cardinality(Trit::False), 1);
	EXPECT_EQ(set.cardinality(Trit::True), 2);
}

TEST(TritSet, unordered_mapCardinality)
{
	const size_t len = 16;
	TritSet set(len);
	EXPECT_EQ(set.cardinality()[Trit::Unknown], len);
	EXPECT_EQ(set.cardinality()[Trit::False], 0);
	EXPECT_EQ(set.cardinality()[Trit::True], 0);

	set[len - 1] = Trit::True;
	EXPECT_EQ(set.cardinality()[Trit::Unknown], len - 1);
	EXPECT_EQ(set.cardinality()[Trit::False], 0);
	EXPECT_EQ(set.cardinality()[Trit::True], 1);

	set[len - 1] = Trit::False;
	EXPECT_EQ(set.cardinality()[Trit::Unknown], len - 1);
	EXPECT_EQ(set.cardinality()[Trit::False], 1);
	EXPECT_EQ(set.cardinality()[Trit::True], 0);

	set[len - 2] = Trit::True;
	EXPECT_EQ(set.cardinality()[Trit::Unknown], len - 2);
	EXPECT_EQ(set.cardinality()[Trit::False], 1);
	EXPECT_EQ(set.cardinality()[Trit::True], 1);

	set[len * 2 - 1] = Trit::True;
	EXPECT_EQ(set.cardinality()[Trit::Unknown], 2 * len - 3);
	EXPECT_EQ(set.cardinality()[Trit::False], 1);
	EXPECT_EQ(set.cardinality()[Trit::True], 2);
}

TEST(TritSet, logicOperators)
{
	const size_t len = 16;
	TritSet set1(len);
	TritSet set2(len/2);
	for (size_t i = 0; i < len/2; ++i)
	{
		set1[i] = Trit::True;
		set2[i] = Trit::False;
	}
	for (size_t i = len / 2; i < len; ++i)
		set1[i] = Trit::True;

	TritSet set3 = set1 & set2;
	EXPECT_EQ(set3.getLength(), set1.getLength());
	EXPECT_EQ(set3.capacity(), set1.capacity());

	for (size_t i = 0; i < len; ++i)
	{
		if (i < len / 2)
			EXPECT_EQ(set3[i], Trit::False);
		else
			EXPECT_EQ(set3[i], Trit::Unknown);
	}
	
	TritSet set4 = set1 | set2;
	EXPECT_EQ(set4.getLength(), set1.getLength());
	EXPECT_EQ(set4.capacity(), set1.capacity());
	for (size_t i = 0; i < len; ++i)
	{
		if (i < len / 2)
			EXPECT_EQ(set4[i], Trit::True);
		else
			EXPECT_EQ(set4[i], Trit::True);
	}

	TritSet set5 = !set2;
	EXPECT_EQ(set5.getLength(), set2.getLength());
	EXPECT_EQ(set5.capacity(), set2.capacity());
	for (size_t i = 0; i < len; ++i)
	{
		if (i < len / 2)
			EXPECT_EQ(set5[i], Trit::True);
		else
			EXPECT_EQ(set5[i], Trit::Unknown);
	}
}

TEST(TritSet, shrink)
{
	const size_t len = 16;
	TritSet set(len);

	size_t capacity = set.capacity();
	set.shrink();
	EXPECT_EQ(set.getLength(), len);
	EXPECT_EQ(set.capacity(), capacity);
	
	set[len * 2 - 1] = Trit::True;
	EXPECT_EQ(set.getLength(), len * 2);

	set.shrink();
	EXPECT_EQ(set.getLength(), len * 2);

	set[len * 2 - 2] = Trit::True;
	set.shrink();
	EXPECT_EQ(set.getLength(), len * 2);
}

TEST(TritSet, trim)
{
	const size_t len = 16;
	TritSet set(len);


	set[0] = Trit::True;
	set.trim(0);
	EXPECT_EQ(set[0], Trit::Unknown);
	
	set[3] = Trit::True;
	set[6] = Trit::True;
	set.trim(6);
	EXPECT_EQ(set[6], Trit::Unknown);
	set.trim(3);
	EXPECT_EQ(set[3], Trit::Unknown);
}

TEST(TritSet, range_based_for)
{
	// using http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0184r0.html
	// and https://en.cppreference.com/w/cpp/iterator/iterator

	TritSet set(10);
	set[9] = Trit::True;

	set[10] = Trit::False;

	int i = 0;
	for (auto iter : set)
	{
		if(i == 9)
			EXPECT_EQ(iter, Trit::True);
		else if(i == 10)
			EXPECT_EQ(iter, Trit::False);
		else
			EXPECT_EQ(iter, Trit::Unknown);
		i++;
	}
	EXPECT_EQ(i, 11);

	for (auto iter : set)
		iter = Trit::True;
	for (i = 0; i < 11; ++i)
		EXPECT_EQ(set[i], Trit::True);

	EXPECT_EQ(i, 11);
}

TritSet createSetWithMoving(LabTritSetSpace::uint **pointer)
{
	TritSet set(10);
	*pointer = const_cast<uint*>(set.getArr());
	return set; // moving constructor expected
}

TEST(TritSet, constructors)
{
	TritSet source(10); 
	source[9] = Trit::False;
	
	TritSet dest = source; // copying expected
	source[9] = Trit::True;
	EXPECT_EQ(dest[9], Trit::False);
	
	dest = TritSet(10); // moving expected
	uint * pointer = nullptr;

	TritSet set(createSetWithMoving(&pointer)); // moving constructor expected
	EXPECT_EQ(set.getArr(), pointer);
}
