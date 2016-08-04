#include <iostream>
#include "gtest/gtest.h"
#include "myVector.hpp"

class TestObj;

// using IntVector = std::vector<int>;
// using CountingVector = std::vector<TestObj>;
using IntVector = sandsnip3r::MyVector<int>;
using CountingVector = sandsnip3r::MyVector<TestObj>;

class TestObj {
public:
	TestObj();
	TestObj(int num);
	TestObj(const TestObj &other);
	TestObj(TestObj &&other);
	~TestObj();
	TestObj& operator=(const TestObj &other);
	TestObj& operator=(TestObj &&other);
	static int defaultConstruction;
	static int valueConstruction;
	static int copyConstruction;
	static int moveConstruction;
	static int destruction;
	static int copyAssignment;
	static int moveAssignment;
	static void resetCounts();
};

int TestObj::defaultConstruction;
int TestObj::valueConstruction;
int TestObj::copyConstruction;
int TestObj::moveConstruction;
int TestObj::destruction;
int TestObj::copyAssignment;
int TestObj::moveAssignment;

TestObj::TestObj() {
	++defaultConstruction;
};

TestObj::TestObj(int num) {
	++valueConstruction;
}

TestObj::TestObj(const TestObj &other) {
	++copyConstruction;
}

TestObj::TestObj(TestObj &&other) {
	++moveConstruction;
}

TestObj::~TestObj() {
	++destruction;
};

TestObj& TestObj::operator=(const TestObj &other) {
	++copyAssignment;
	return *this;
}

TestObj& TestObj::operator=(TestObj &&other) {
	++moveAssignment;
	return *this;
}

void TestObj::resetCounts() {
	defaultConstruction = 0;
	valueConstruction = 0;
	copyConstruction = 0;
	moveConstruction = 0;
	destruction = 0;
	copyAssignment = 0;
	moveAssignment = 0;
}

TEST(Construction, defaultConstruction) {
	IntVector v;
	ASSERT_EQ(v.empty(), true);
	ASSERT_EQ(v.size(), 0);
	ASSERT_EQ(v.capacity(), 0);
}

TEST(Construction, defaultConstructionWithCounts) {
	TestObj::resetCounts();
	{
		CountingVector v;
	}
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::destruction, 0);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
}

TEST(Construction, countWithoutValueConstruction) {
	const size_t CREATE_COUNT = 10;

	IntVector v(CREATE_COUNT);

	ASSERT_EQ(v.empty(), false);
	ASSERT_EQ(v.size(), CREATE_COUNT);
	ASSERT_GE(v.capacity(), CREATE_COUNT);
}

TEST(Construction, countWithoutValueConstructionWithCounts) {
	const size_t CREATE_COUNT = 10;

	TestObj::resetCounts();
	{
		CountingVector v(CREATE_COUNT);
	}
	//Default constructs elements in place
	ASSERT_EQ(TestObj::defaultConstruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
}

TEST(Construction, countWithValueConstruction) {
	const size_t CREATE_COUNT = 10;
	const size_t DEFAULT_VALUE = 987654321;

	IntVector v(CREATE_COUNT, DEFAULT_VALUE);

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
		CountingVector v(CREATE_COUNT, DEFAULT_VALUE);
	}
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	//Copy constructs elements in place copying constructing using the given value
	ASSERT_EQ(TestObj::copyConstruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
}

TEST(Construction, initializerListConstruction) {
	IntVector v{0, 1, 2, 3, 4};

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
		CountingVector v{0, 1, 2, 3, 4};
	}
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	//Constructs elements by value into the initializer list
	ASSERT_EQ(TestObj::valueConstruction, 5);
	//Copy constructs elements in place from the initializer list
	ASSERT_EQ(TestObj::copyConstruction, 5);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	//Elements in initializer list and vector are destroyed
	ASSERT_EQ(TestObj::destruction, 10);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
}

TEST(Construction, copyConstruction) {
	const size_t CREATE_COUNT = 10;
	const size_t DEFAULT_VALUE = 987654321;

	IntVector v(CREATE_COUNT, DEFAULT_VALUE);
	IntVector newV(v);

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

	CountingVector v(CREATE_COUNT, DEFAULT_VALUE);

	TestObj::resetCounts();
	{
		CountingVector newV(v);
	}
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	//Copy constructs elements in place copying constructing using each element in the source
	ASSERT_EQ(TestObj::copyConstruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
}

TEST(Construction, moveConstruction) {
	const size_t CREATE_COUNT = 10;
	const size_t DEFAULT_VALUE = 987654321;

	IntVector v(CREATE_COUNT, DEFAULT_VALUE);
	IntVector newV(std::move(v));

	ASSERT_EQ(newV.empty(), false);
	ASSERT_EQ(newV.size(), CREATE_COUNT);
	ASSERT_GE(newV.capacity(), CREATE_COUNT);
	for (size_t i=0; i<CREATE_COUNT; ++i) {
		ASSERT_EQ(newV.at(i), DEFAULT_VALUE);
	}

	ASSERT_EQ(v.empty(), true);
	ASSERT_EQ(v.size(), 0);
}

TEST(Construction, moveConstructionWithCount) {
	const size_t CREATE_COUNT = 10;
	const auto DEFAULT_VALUE = TestObj();

	CountingVector v(CREATE_COUNT, DEFAULT_VALUE);

	TestObj::resetCounts();
	{
		CountingVector newV(std::move(v));
	}
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
}

TEST(Assignment, copyAssignment) {
	const size_t CREATE_COUNT = 10;
	const size_t DEFAULT_VALUE = 987654321;

	IntVector v(CREATE_COUNT, DEFAULT_VALUE);
	IntVector newV = v;

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

	CountingVector v(CREATE_COUNT, DEFAULT_VALUE);

	TestObj::resetCounts();
	{
		CountingVector newV = v;
	}
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	//Copy constructs elements in place copying constructing using each element in the source
	ASSERT_EQ(TestObj::copyConstruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
}

TEST(Assignment, moveAssignment) {
	const size_t CREATE_COUNT = 10;
	const size_t DEFAULT_VALUE = 987654321;

	IntVector v(CREATE_COUNT, DEFAULT_VALUE);
	IntVector newV = std::move(v);

	ASSERT_EQ(newV.empty(), false);
	ASSERT_EQ(newV.size(), CREATE_COUNT);
	ASSERT_GE(newV.capacity(), CREATE_COUNT);
	for (size_t i=0; i<CREATE_COUNT; ++i) {
		ASSERT_EQ(newV.at(i), DEFAULT_VALUE);
	}

	ASSERT_EQ(v.empty(), true);
	ASSERT_EQ(v.size(), 0);
}

TEST(Assignment, moveAssignmentWithCount) {
	const size_t CREATE_COUNT = 10;
	const auto DEFAULT_VALUE = TestObj();

	CountingVector v(CREATE_COUNT, DEFAULT_VALUE);

	TestObj::resetCounts();
	{
		CountingVector newV = std::move(v);
	}
	ASSERT_EQ(TestObj::defaultConstruction, 0);
	ASSERT_EQ(TestObj::valueConstruction, 0);
	ASSERT_EQ(TestObj::copyConstruction, 0);
	ASSERT_EQ(TestObj::moveConstruction, 0);
	ASSERT_EQ(TestObj::destruction, CREATE_COUNT);
	ASSERT_EQ(TestObj::copyAssignment, 0);
	ASSERT_EQ(TestObj::moveAssignment, 0);
}