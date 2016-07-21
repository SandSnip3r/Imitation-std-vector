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
	cout << endl << "Size now " << v.size() << endl;
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
		cout << endl << "v3 destroyed. Destorying v2" << endl;
	}
	cout << endl << "v2 destroyed. Clearing v" << endl;
	v.clear();
	cout << endl << "cleared. size=" << v.size() << ", capacity=" << v.capacity() << ". Now assigning using initializer_list" << endl;
	v = {{},{}};
	cout << endl << "Assigned. size=" << v.size() << ", capacity=" << v.capacity() << endl;
	return 0;
}