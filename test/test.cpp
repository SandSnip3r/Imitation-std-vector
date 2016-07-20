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
	using VectorType = MyVector<Mine>;
	// using VectorType = std::vector<Mine>;
	VectorType v;
	v.reserve(10);
	cout << v.size() << " " << v.capacity() << endl;
	cout << endl << "pushing back #1" << endl;
	v.push_back(Mine());
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
	cout << endl << "Going to resize from " << v.size() << " to " << newResizeValue << endl;
	v.resize(newResizeValue);
	newResizeValue = 7;
	cout << endl << "Going to resize from " << v.size() << " to " << newResizeValue << endl;
	v.resize(newResizeValue);
	cout << endl << "Clearing" << endl;
	v.clear();
	cout << endl << "All done" << endl;
	return 0;
}