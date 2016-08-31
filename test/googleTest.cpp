#include <iostream>
#include "gtest/gtest.h"
#include "vector.hpp"

//Easily switch between my vector and the STL vector
//	to ensure the test results are equivalent
template <class Type>
// using Vector = std::vector<Type>;
using Vector = sandsnip3r::vector<Type>;

class TestObj {
public:
	TestObj();
	TestObj(int num);
	TestObj(const TestObj &other) noexcept;
	TestObj(TestObj &&other) noexcept;
	~TestObj();
	TestObj& operator=(const TestObj &other) noexcept;
	TestObj& operator=(TestObj &&other) noexcept;
	static int defaultConstruction;
	static int valueConstruction;
	static int copyConstruction;
	static int moveConstruction;
	static int copyAssignment;
	static int moveAssignment;
	static int destruction;
	static void resetCounts();
};

//Declare TestObj's static members
//	these are used for counting constructions and destructions
int TestObj::defaultConstruction;
int TestObj::valueConstruction;
int TestObj::copyConstruction;
int TestObj::moveConstruction;
int TestObj::copyAssignment;
int TestObj::moveAssignment;
int TestObj::destruction;

TestObj::TestObj() {
	++defaultConstruction;
};

TestObj::TestObj(int num) {
	++valueConstruction;
}

TestObj::TestObj(const TestObj &other) noexcept {
	++copyConstruction;
}

TestObj::TestObj(TestObj &&other) noexcept {
	++moveConstruction;
}

TestObj::~TestObj() {
	++destruction;
};

TestObj& TestObj::operator=(const TestObj &other) noexcept {
	++copyAssignment;
	return *this;
}

TestObj& TestObj::operator=(TestObj &&other) noexcept {
	++moveAssignment;
	return *this;
}

void TestObj::resetCounts() {
	defaultConstruction = 0;
	valueConstruction = 0;
	copyConstruction = 0;
	moveConstruction = 0;
	copyAssignment = 0;
	moveAssignment = 0;
	destruction = 0;
}

TEST(Construction, defaultConstruction) {
	Vector<int> v;
	ASSERT_TRUE(v.empty());
	ASSERT_EQ(v.size(), 0);
	ASSERT_EQ(v.capacity(), 0);
}

TEST(Construction, defaultConstructionWithCounts) {
	TestObj::resetCounts();
	{
		Vector<TestObj> v;
	}
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
	ASSERT_EQ(TestObj::destruction, 0);
}

TEST(Construction, countWithoutValueConstruction) {
	const size_t CREATE_COUNT = 10;

	Vector<int> v(CREATE_COUNT);

	ASSERT_EQ(v.empty(), false);
	ASSERT_EQ(v.size(), CREATE_COUNT);
	ASSERT_GE(v.capacity(), CREATE_COUNT);
}

TEST(Construction, countWithoutValueConstructionWithCounts) {
	const size_t CREATE_COUNT = 10;

	TestObj::resetCounts();
	{
		Vector<TestObj> v(CREATE_COUNT);
	}
	//Default constructs elements in place
	ASSERT_EQ(TestObj::defaultConstruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
}

TEST(Construction, countWithValueConstruction) {
	const size_t CREATE_COUNT = 10;
	const size_t DEFAULT_VALUE = 987654321;

	Vector<int> v(CREATE_COUNT, DEFAULT_VALUE);

	ASSERT_EQ(v.empty(), false);
	ASSERT_EQ(v.size(), CREATE_COUNT);
	ASSERT_GE(v.capacity(), CREATE_COUNT);
	for (size_t i=0; i<CREATE_COUNT; ++i) {
		ASSERT_EQ(v.at(i), DEFAULT_VALUE);
	}
}

TEST(Construction, countWithValueConstructionWithCounts) {
	const size_t CREATE_COUNT = 10;
	const auto DEFAULT_VALUE = TestObj();

	TestObj::resetCounts();
	{
		Vector<TestObj> v(CREATE_COUNT, DEFAULT_VALUE);
	}
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	//Copy constructs elements in place copying constructing using the given value
	ASSERT_EQ(TestObj::copyConstruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
}

TEST(Construction, initializerListConstruction) {
	Vector<int> v{0, 1, 2, 3, 4};

	ASSERT_EQ(v.empty(), false);
	ASSERT_EQ(v.size(), 5);
	ASSERT_GE(v.capacity(), 5);
	for (size_t i=0; i<5; ++i) {
		ASSERT_EQ(v.at(i), i);
	}
}

TEST(Construction, initializerListConstructionWithCounts) {
	TestObj::resetCounts();
	{
		Vector<TestObj> v{0, 1, 2, 3, 4};
	}
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	//Constructs elements by value into the initializer list
	ASSERT_EQ(TestObj::valueConstruction, 5);
	//Copy constructs elements in place from the initializer list
	ASSERT_EQ(TestObj::copyConstruction, 5);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
	//Elements in initializer list and vector are destroyed
	ASSERT_EQ(TestObj::destruction, 10);
}

TEST(Construction, copyConstruction) {
	const size_t CREATE_COUNT = 10;
	const size_t DEFAULT_VALUE = 987654321;

	Vector<int> v(CREATE_COUNT, DEFAULT_VALUE);
	Vector<int> newV(v);

	ASSERT_EQ(newV.empty(), false);
	ASSERT_EQ(newV.size(), CREATE_COUNT);
	ASSERT_GE(newV.capacity(), CREATE_COUNT);
	for (size_t i=0; i<CREATE_COUNT; ++i) {
		ASSERT_EQ(newV.at(i), DEFAULT_VALUE);
	}
}

TEST(Construction, copyConstructionWithCount) {
	const size_t CREATE_COUNT = 10;
	const auto DEFAULT_VALUE = TestObj();

	Vector<TestObj> v(CREATE_COUNT, DEFAULT_VALUE);

	TestObj::resetCounts();
	{
		Vector<TestObj> newV(v);
	}
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	//Copy constructs elements in place copying constructing using each element in the source
	ASSERT_EQ(TestObj::copyConstruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
}

TEST(Construction, moveConstruction) {
	const size_t CREATE_COUNT = 10;
	const size_t DEFAULT_VALUE = 987654321;

	Vector<int> v(CREATE_COUNT, DEFAULT_VALUE);
	Vector<int> newV(std::move(v));

	ASSERT_EQ(newV.empty(), false);
	ASSERT_EQ(newV.size(), CREATE_COUNT);
	ASSERT_GE(newV.capacity(), CREATE_COUNT);
	for (size_t i=0; i<CREATE_COUNT; ++i) {
		ASSERT_EQ(newV.at(i), DEFAULT_VALUE);
	}

	ASSERT_TRUE(v.empty());
	ASSERT_EQ(v.size(), 0);
}

TEST(Construction, moveConstructionWithCount) {
	const size_t CREATE_COUNT = 10;
	const auto DEFAULT_VALUE = TestObj();

	Vector<TestObj> v(CREATE_COUNT, DEFAULT_VALUE);

	TestObj::resetCounts();
	{
		Vector<TestObj> newV(std::move(v));
	}
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
}

TEST(Assignment, copyAssignment) {
	const size_t CREATE_COUNT = 10;
	const size_t DEFAULT_VALUE = 987654321;

	Vector<int> v(CREATE_COUNT, DEFAULT_VALUE);
	Vector<int> newV = v;

	ASSERT_EQ(newV.empty(), false);
	ASSERT_EQ(newV.size(), CREATE_COUNT);
	ASSERT_GE(newV.capacity(), CREATE_COUNT);
	for (size_t i=0; i<CREATE_COUNT; ++i) {
		ASSERT_EQ(newV.at(i), DEFAULT_VALUE);
	}
}

TEST(Assignment, copyAssignmentWithCount) {
	const size_t CREATE_COUNT = 10;
	const auto DEFAULT_VALUE = TestObj();

	Vector<TestObj> v(CREATE_COUNT, DEFAULT_VALUE);

	TestObj::resetCounts();
	{
		Vector<TestObj> newV;
		newV = v;
	}
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	//Copy constructs elements in place copying constructing using each element in the source
	ASSERT_EQ(TestObj::copyConstruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
}

TEST(Assignment, moveAssignment) {
	const size_t CREATE_COUNT = 10;
	const size_t DEFAULT_VALUE = 987654321;

	Vector<int> v(CREATE_COUNT, DEFAULT_VALUE);
	Vector<int> newV = std::move(v);

	ASSERT_FALSE(newV.empty());
	ASSERT_EQ(newV.size(), CREATE_COUNT);
	ASSERT_GE(newV.capacity(), CREATE_COUNT);
	for (size_t i=0; i<CREATE_COUNT; ++i) {
		ASSERT_EQ(newV.at(i), DEFAULT_VALUE);
	}

	ASSERT_TRUE(v.empty());
	ASSERT_EQ(v.size(), 0);
}

TEST(Assignment, moveAssignmentWithCount) {
	const size_t CREATE_COUNT = 10;
	const auto DEFAULT_VALUE = TestObj();

	Vector<TestObj> v(CREATE_COUNT, DEFAULT_VALUE);

	TestObj::resetCounts();
	{
		Vector<TestObj> newV;
		newV = std::move(v);
	}
	//Nothing should've been created while moving the vector
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
}

TEST(ElementAccess, atBeyondBounds) {
	Vector<int> v;

	bool exceptionThrown = false;
	try {
		v.at(0);
	} catch (std::out_of_range &ex) {
		exceptionThrown = true;
	}
	
	ASSERT_TRUE(exceptionThrown);
}

TEST(ElementAccess, frontBeyondBounds) {
	Vector<int> v;

	//Standard states calling 'front' on an empty vector is undefined
	bool exceptionThrown = false;
	try {
		v.front();
	} catch (std::out_of_range &ex) {
		exceptionThrown = true;
	}
	
	//Expect that no exception was thrown
	ASSERT_FALSE(exceptionThrown);
}

TEST(ElementAccess, backBeyondBounds) {
	Vector<int> v;

	//Standard states calling 'back' on an empty vector is undefined
	bool exceptionThrown = false;
	try {
		v.back();
	} catch (std::out_of_range &ex) {
		exceptionThrown = true;
	}
	
	//Expect that no exception was thrown
	ASSERT_FALSE(exceptionThrown);
}

TEST(Capacity, reserveUp) {
	const int RESERVE_AMOUNT = 100;

	Vector<int> v;
	v.reserve(RESERVE_AMOUNT);
	
	ASSERT_GE(v.capacity(), RESERVE_AMOUNT);
}

TEST(Capacity, reserveUpWithCount) {
	const int RESERVE_AMOUNT = 100;

	TestObj::resetCounts();
	{
		Vector<TestObj> v;
		v.reserve(RESERVE_AMOUNT);
	}
	//Nothing should've been created or destroyed
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
	ASSERT_EQ(TestObj::destruction, 0);
}

TEST(Capacity, reserveDown) {
	const int RESERVE_AMOUNT = 100;

	Vector<int> v;
	v.reserve(RESERVE_AMOUNT);
	v.reserve(0);
	
	//The size of the container is not expected to be reduced
	ASSERT_GE(v.capacity(), RESERVE_AMOUNT);
}

TEST(Capacity, reserveDownWithCount) {
	const int RESERVE_AMOUNT = 100;
	
	TestObj::resetCounts();
	{
		Vector<TestObj> v;
		v.reserve(RESERVE_AMOUNT);
		v.reserve(0);
	}
	//Nothing should've been created or destroyed
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
	ASSERT_EQ(TestObj::destruction, 0);
}

TEST(Capacity, shrinkToFit) {
	const size_t CREATE_COUNT = 10;
	const int RESERVE_AMOUNT = 100;

	Vector<int> v(CREATE_COUNT);
	v.reserve(RESERVE_AMOUNT);
	v.shrink_to_fit();
	
	ASSERT_EQ(v.capacity(), CREATE_COUNT);
}

TEST(Capacity, DISABLED_shrinkToFitWithCount) {
	const size_t CREATE_COUNT = 10;
	const int RESERVE_AMOUNT = 100;
	
	TestObj::resetCounts();
	{
		Vector<TestObj> v(CREATE_COUNT);
		v.reserve(RESERVE_AMOUNT);
		v.shrink_to_fit();
	}
	//Default constructs elements in place on vector construction
	ASSERT_EQ(TestObj::defaultConstruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	//Move assign once when we reserve more space
	//Move assign once when we shrink into a smaller space
	ASSERT_EQ(TestObj::moveAssignment, CREATE_COUNT*2);
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
}

TEST(Deletion, popBack) {
	const size_t CREATE_COUNT = 10;

	Vector<int> v(CREATE_COUNT);
	v.pop_back();
	
	ASSERT_EQ(v.size(), CREATE_COUNT-1);
	ASSERT_GE(v.capacity(), CREATE_COUNT);
}

TEST(Deletion, popBackWithCount) {
	const size_t CREATE_COUNT = 10;

	TestObj::resetCounts();
	{
		Vector<TestObj> v(CREATE_COUNT);
		v.pop_back();
		//pop_back should have only destroyed one object
		ASSERT_EQ(TestObj::destruction, 1);
	}
	//Default constructs elements in place on vector construction
	ASSERT_EQ(TestObj::defaultConstruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
}

TEST(Deletion, clear) {
	const size_t CREATE_COUNT = 10;

	Vector<int> v(CREATE_COUNT);
	v.clear();
	
	ASSERT_EQ(v.capacity(), CREATE_COUNT);
	ASSERT_EQ(v.size(), 0);
	ASSERT_TRUE(v.empty());
}

TEST(Deletion, clearWithCount) {
	const size_t CREATE_COUNT = 10;
	
	TestObj::resetCounts();
	{
		Vector<TestObj> v(CREATE_COUNT);
		v.clear();
		//Default constructs elements in place on vector construction
		ASSERT_EQ(TestObj::defaultConstruction, CREATE_COUNT);
		ASSERT_EQ(TestObj::valueConstruction, 0);
		ASSERT_EQ(TestObj::copyConstruction, 0);
		ASSERT_EQ(TestObj::moveConstruction, 0);
		ASSERT_EQ(TestObj::copyAssignment, 0);
		ASSERT_EQ(TestObj::moveAssignment, 0);
		ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
	}
	//Destruction of the vector shouldnt have changed anything
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
}

TEST(Swap, memberSwapBothNonEmpty) {
	const size_t CREATE_COUNT_1 = 10;
	const size_t CREATE_COUNT_2 = 500;

	ASSERT_NE(CREATE_COUNT_1, CREATE_COUNT_2);

	Vector<int> v1(CREATE_COUNT_1);
	Vector<int> v2(CREATE_COUNT_2);
	
	v1.swap(v2);

	ASSERT_EQ(v2.size(), CREATE_COUNT_1);
	ASSERT_EQ(v2.capacity(), CREATE_COUNT_1);
	ASSERT_EQ(v1.size(), CREATE_COUNT_2);
	ASSERT_EQ(v1.capacity(), CREATE_COUNT_2);
}

TEST(Swap, memberSwapBothNonEmptyWithCount) {
	const size_t CREATE_COUNT_1 = 10;
	const size_t CREATE_COUNT_2 = 500;

	ASSERT_NE(CREATE_COUNT_1, CREATE_COUNT_2);

	Vector<int> v1(CREATE_COUNT_1);
	Vector<int> v2(CREATE_COUNT_2);
	
	TestObj::resetCounts();
	v1.swap(v2);

	//Nothing should've been created or destroyed
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
	ASSERT_EQ(TestObj::destruction, 0);
}

TEST(Swap, memberSwapOneEmpty) {
	const size_t CREATE_COUNT = 10;

	Vector<int> v1(CREATE_COUNT);
	Vector<int> v2;
	
	v1.swap(v2);

	ASSERT_EQ(v2.size(), CREATE_COUNT);
	ASSERT_EQ(v2.capacity(), CREATE_COUNT);
	ASSERT_TRUE(v1.empty());
	ASSERT_EQ(v1.size(), 0);
	ASSERT_EQ(v1.capacity(), 0);
}

TEST(Swap, memberSwapOneEmptyWithCount) {
	const size_t CREATE_COUNT = 10;

	Vector<int> v1(CREATE_COUNT);
	Vector<int> v2;
	
	TestObj::resetCounts();
	v1.swap(v2);

	//Nothing should've been created or destroyed
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
	ASSERT_EQ(TestObj::destruction, 0);
}

TEST(Swap, memberSwapBothEmpty) {
	Vector<int> v1, v2;
	
	v1.swap(v2);

	ASSERT_TRUE(v2.empty());
	ASSERT_EQ(v2.size(), 0);
	ASSERT_EQ(v2.capacity(), 0);
	ASSERT_TRUE(v1.empty());
	ASSERT_EQ(v1.size(), 0);
	ASSERT_EQ(v1.capacity(), 0);
}

TEST(Swap, memberSwapBothEmptyWithCount) {
	Vector<int> v1, v2;
	
	TestObj::resetCounts();
	v1.swap(v2);

	//Nothing should've been created or destroyed
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
	ASSERT_EQ(TestObj::destruction, 0);
}

TEST(Comparison, bothEmpty) {
	Vector<int> v1, v2;
	
	ASSERT_TRUE(v1==v2);
	ASSERT_FALSE(v1<v2);
	ASSERT_TRUE(v1<=v2);
	ASSERT_FALSE(v1>v2);
	ASSERT_TRUE(v1>=v2);
}

TEST(Comparison, oneEmpty) {
	const size_t CREATE_COUNT = 10;
	Vector<int> v1;
	Vector<int> v2(CREATE_COUNT, 0);
	
	ASSERT_FALSE(v1==v2);
	ASSERT_TRUE(v1<v2);
	ASSERT_TRUE(v1<=v2);
	ASSERT_FALSE(v1>v2);
	ASSERT_FALSE(v1>=v2);
}

TEST(Comparison, differentSizes) {
	const size_t CREATE_COUNT_1 = 5;
	const size_t CREATE_COUNT_2 = 10;
	Vector<int> v1(CREATE_COUNT_1, 0);
	Vector<int> v2(CREATE_COUNT_2, 0);
	
	ASSERT_FALSE(v1==v2);
	ASSERT_TRUE(v1<v2);
	ASSERT_TRUE(v1<=v2);
	ASSERT_FALSE(v1>v2);
	ASSERT_FALSE(v1>=v2);
}

TEST(Comparison, sameSize) {
	Vector<int> v1{{1,1,1,1,1}};
	Vector<int> v2{{1,1,1,1,1}};
	
	ASSERT_TRUE(v1==v2);
	ASSERT_FALSE(v1<v2);
	ASSERT_TRUE(v1<=v2);
	ASSERT_FALSE(v1>v2);
	ASSERT_TRUE(v1>=v2);
}

TEST(Comparison, sameSizeDifferentElements) {
	Vector<int> v1{{1,2,3}};
	Vector<int> v2{{2,2,3}};
	
	ASSERT_FALSE(v1==v2);
	ASSERT_TRUE(v1<v2);
	ASSERT_TRUE(v1<=v2);
	ASSERT_FALSE(v1>v2);
	ASSERT_FALSE(v1>=v2);
}

TEST(Comparison, sameSizeDifferentCapacity) {
	const size_t CREATE_COUNT = 10;
	const size_t RESERVE_COUNT = 100;
	Vector<int> v1(CREATE_COUNT, 0);
	Vector<int> v2(CREATE_COUNT, 0);
	v2.reserve(RESERVE_COUNT);
	
	ASSERT_TRUE(v1==v2);
	ASSERT_FALSE(v1<v2);
	ASSERT_TRUE(v1<=v2);
	ASSERT_FALSE(v1>v2);
	ASSERT_TRUE(v1>=v2);
}