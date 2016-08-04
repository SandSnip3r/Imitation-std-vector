#include <iostream>
#include <iterator>
#include <utility>
#include "myVector.hpp"

using namespace std;
using namespace sandsnip3r;

class Mine {
public:
	Mine();
	Mine(const Mine &other);
	Mine(Mine &&other);
	Mine& operator=(const Mine &other);
	Mine& operator=(Mine &&other);
	~Mine();
};

Mine::Mine() {
	cout << "\"Mine\" default constructor" << endl;
}

Mine::Mine(const Mine &other) {
	cout << "\"Mine\" copy constructor" << endl;
}

Mine::Mine(Mine &&other) {
	cout << "\"Mine\" move constructor" << endl;
}

Mine& Mine::operator=(const Mine &other) {
	cout << "\"Mine\" copy assignment" << endl;
	return *this;
}

Mine& Mine::operator=(Mine &&other) {
	cout << "\"Mine\" move assignment" << endl;
	return *this;
}

Mine::~Mine() {
	cout << "Destroying \"Mine\"" << endl;
}

ostream& operator<<(ostream &stream, const Mine &x) {
	stream << "Mine";
	return stream;
}

bool operator==(const Mine &left, const Mine &right) {
	return true;
}

bool operator!=(const Mine &left, const Mine &right) {
	return !(left == right);
}

int main() {
	// using VectorType = MyVector<Mine>;
	using VectorType = std::vector<Mine>;
	cout << "Declaring vector" << endl;
	VectorType v;
	int reserveAmount = 11;
	cout << endl << "Reserving " << reserveAmount << endl;
	v.reserve(reserveAmount);
	cout << "Size=" << v.size() << ", capacity=" << v.capacity() << endl;
	cout << endl << "pushing back #1" << endl;
	Mine temp;
	v.push_back(temp);
	cout << endl << "pushing back #2" << endl;
	v.push_back(Mine());
	cout << endl << "pushing back #3" << endl;
	v.push_back(Mine());
	cout << endl << "pushing back #4" << endl;
	v.push_back(Mine());
	cout << endl << "pushing back #5" << endl;
	v.push_back(Mine());
	cout << endl << "pushing back #6" << endl;
	v.push_back(Mine());
	cout << endl << "emplacing back #6" << endl;
	v.emplace_back();
	cout << endl << "done pushing" << endl;
	const Mine &a = static_cast<const VectorType&>(v).at(1);
	const Mine &b = static_cast<const VectorType&>(v)[2];
	cout << "[v.front()] " << v.front() << endl << endl;
	cout << "[a == static_cast<const VectorType&>(v).at(1)] " << a << endl << endl;
	cout << "[b == static_cast<const VectorType&>(v)[2]] " << b << endl << endl;
	cout << "[v.at(3)] " << v.at(3) << endl << endl;
	cout << "[v.data()[4]] " << v.data()[4] << endl << endl;
	cout << "[v[5]] " << v[5] << endl << endl;
	cout << "[v.back()] " << v.back() << endl << endl;
	cout << "pop_back" << endl;
	v.pop_back();
	int newResizeValue = 8;
	cout << endl << "Going to resize from " << v.size() << " to " << newResizeValue << ", (capacity=" << v.capacity() << ")" << endl;
	v.resize(newResizeValue);
	newResizeValue = 10;
	cout << endl << "Going to resize from " << v.size() << " to " << newResizeValue << ", (capacity=" << v.capacity() << ") with specified value" << endl;
	v.resize(newResizeValue, Mine());
	newResizeValue = 7;
	cout << endl << "Going to resize from " << v.size() << " to " << newResizeValue << ", (capacity=" << v.capacity() << ")" << endl;
	v.resize(newResizeValue);
	cout << endl << "Size now " << v.size() << " and capacity=" << v.capacity() << endl;
	{
		VectorType v2;
		cout << endl << "Assigning v to v2" << endl;
		v2 = v;
		cout << endl << "v2 assigned, size=" << v2.size() << ", capacity=" << v2.capacity()<< endl;
		{
			VectorType v3;
			cout << endl << "Moving v2 to v3" << endl;
			v3 = std::move(v2);
			cout << endl << "v2 moved into v3, v3.size=" << v3.size() << ", v3.capacity=" << v3.capacity()<< ". Destroying v3" << endl;
		}
		cout << endl << "v3 destroyed. Destroying v2" << endl;
	}
	cout << endl << "v2 destroyed" << endl;
	cout << "v size=" << v.size() << " and capacity=" << v.capacity() << endl;
	{
		cout << endl << "Copy constructing v to v2" << endl;
		VectorType v2(v);
		cout << endl << "v2 copy constructed, v2 size=" << v2.size() << " and capacity=" << v2.capacity()<< endl;
		{
			cout << endl << "Move constructing v2 to v3" << endl;
			VectorType v3(std::move(v2));
			cout << endl << "v3 move constructed, v3 size=" << v3.size() << " and capacity=" << v3.capacity()<< endl;
			{
				cout << endl << "Constructing with initializer_list" << endl;
				VectorType v4{ Mine(), Mine(), Mine() };
				cout << endl << "v4 constructed with init list, v4 size=" << v4.size() << " and capacity=" << v4.capacity()<< endl;
				cout << endl << "Destroying v4" << endl;
			}
			cout << endl << "Destroying v3" << endl;
		}
		cout << endl << "v3 destroyed. Destroying v2" << endl;
	}
	cout << endl << "v2 destroyed. Clearing v" << endl;
	v.clear();
	cout << endl << "cleared. size=" << v.size() << ", capacity=" << v.capacity() << ". Now assigning using initializer_list" << endl;
	v = {{},{}};
	cout << endl << "Assigned. size=" << v.size() << ", capacity=" << v.capacity() << endl;

	{
		cout << endl << "Creating temp vector for comparison" << endl;
		VectorType tempVector = v;
		if (v == tempVector) {
			cout << "[Equality compare 1] Equal" << endl;
		} else {
			cout << "[Equality compare 1] Not equal" << endl;
		}
		if (v != tempVector) {
			cout << "[Equality compare 2] Not equal" << endl;
		} else {
			cout << "[Equality compare 2] Equal" << endl;
		}
		cout << endl << "Destroying temp" << endl;
	}
	cout << endl << "temp destroyed" << endl;
	return 0;
}