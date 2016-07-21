#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP 1

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>

namespace sandsnip3r {
	template<typename Type>
	class MyVectorIterator {
	private:
	};

	template<typename Type>
	class MyVector {
	public:
		using value_type = Type;
		using allocator_type = std::allocator<value_type>;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = Type&;
		using const_reference = const Type&;
		using pointer = Type*;
		using const_pointer = const Type*;
		//iterator
		//const_iterator
		//reverse_iterator
		//const_reverse_iterator
	private:
		std::allocator<value_type> vectorAllocator;
		std::unique_ptr<value_type[], std::function<void(value_type*)>> vectorData;
		size_t vectorSize{0};
		size_t vectorCapacity{0};		

		void reallocateIfNecessary() {
			//Arguments state the the golden ratio is the most appropriate growth factor
			const auto GROWTH_FACTOR = 1.618;
			if (this->size() == this->capacity()) {
				//Need to reallocate to make space
				size_type newCapacity = (vectorCapacity == 0 ? 1 : vectorCapacity * GROWTH_FACTOR);
				reallocate(newCapacity);
			}
		}

		void reallocateToNewSizeIfNecessary(size_type newCapacity) {
			if (this->capacity() < newCapacity) {
				reallocate(newCapacity);
			}
		}

		void reallocate(size_type newCapacity) {
			//Allocate memory for the new data
			if (newCapacity > this->max_size()) {
				throw std::length_error("MyVector::reallocate() newCapacity (which is "+std::to_string(newCapacity)+") > max_size (which is "+std::to_string(this->max_size())+")");
			}
			//	store it in a unique_ptr
			std::unique_ptr<value_type[], std::function<void(value_type*)>> newData(vectorAllocator.allocate(newCapacity), [this, newCapacity](value_type *ptr){
				vectorAllocator.deallocate(ptr, newCapacity);
			});
			//Move the old data into out new storage chunk
			const auto &dataBegin = vectorData.get();
			std::move(dataBegin, std::next(dataBegin, vectorSize), newData.get());
			//Assign the new data to be the current
			vectorData = std::move(newData);
			//Update the capacity
			vectorCapacity = newCapacity;
		}

		reference elementAt(size_type pos) {
			return vectorData[pos];
		}

		const_reference elementAt(size_type pos) const {
			return vectorData[pos];
		}

		void throwIfOutOfBounds(size_type pos, const std::string &methodName) const {
			if (!(pos < this->size())) {
				throw std::out_of_range("MyVector::"+methodName+"() pos (which is "+std::to_string(pos)+") >= size (which is "+std::to_string(this->size())+")");
			}
		}

		void throwIfEmpty(const std::string &methodName) const {
			if (this->empty()) {
				throw std::out_of_range("MyVector::"+methodName+"() is empty");
			}
		}
	public:
		//constructor

		virtual ~MyVector() {
			this->clear();
		}

		//operator=
		//assign

		allocator_type get_allocator() const {
			return vectorAllocator;
		}

		reference at(size_type pos) {
			throwIfOutOfBounds(pos, "at");
			return elementAt(pos);
		}

		const_reference at(size_type pos) const {
			throwIfOutOfBounds(pos, "at");
			return elementAt(pos);
		}

		reference operator[](size_type pos) {
			return elementAt(pos);
		}

		const_reference operator[](size_type pos) const {
			return elementAt(pos);
		}

		reference front() {
			throwIfEmpty("front");
			return elementAt(0);
		}

		const_reference front() const {
			throwIfEmpty("front");
			return elementAt(0);
		}

		reference back() {
			throwIfEmpty("back");
			return elementAt(this->size()-1);
		}

		const_reference back() const {
			throwIfEmpty("back");
			return elementAt(this->size()-1);
		}

		pointer data() {
			return vectorData.get();
		}

		const_pointer data() const {
			return vectorData.get();
		}

		//begin, cbegin
		//end, cend
		//rbegin, crbegin
		//rend, crend

		bool empty() const {
			return (this->size() == 0);
		}

		size_type size() const {
			return vectorSize;
		}

		size_type max_size() const {
			//TODO: Implement this using allocator_traits
			return std::numeric_limits<decltype(vectorSize)>::max();
		}

		void reserve(size_type newCapacity) {
			reallocateToNewSizeIfNecessary(newCapacity);
		}

		size_type capacity() const {
			return vectorCapacity;
		}

		void shrink_to_fit() {
			if (this->size() < this->capacity()) {
				reallocate(vectorSize);
			}
		}

		void clear() {
			for (size_type i=0; i<this->size(); ++i) {
				vectorData[i].~value_type();
			}
			vectorSize = 0;
		}

		//insert
		//emplace
		//erase

		void push_back(const value_type &obj) {
			reallocateIfNecessary();
			new(vectorData.get()+vectorSize) value_type{obj};
			++vectorSize;
		}

		void push_back(value_type &&obj) {
			reallocateIfNecessary();
			new(vectorData.get()+vectorSize) value_type{std::move(obj)};
			++vectorSize;
		}

		template<class... Args>
		void emplace_back(Args&&... args) {
			reallocateIfNecessary();
			new(vectorData.get()+vectorSize) value_type{std::forward<Args>(args)...};
			++vectorSize;
		}
		
		void pop_back() {
			throwIfEmpty("pop_back");
			vectorData[this->size()-1].~value_type();
			--vectorSize;
		}

		void resize(size_type count) {
			if (this->size() < count) {
				reallocateToNewSizeIfNecessary(count);
				while (vectorSize < count) {
					//Fill with default constructed elements
					new(vectorData.get()+vectorSize) value_type{};
					++vectorSize;
				}
			} else if (this->size() > count) {
				for (size_type i=this->size()-1; i>=count; --i) {
					vectorData[i].~value_type();
					--vectorSize;
				}
			}
		}

		void resize(size_type count, const value_type &value) {
			if (this->size() < count) {
				reallocateToNewSizeIfNecessary(count);
				while (vectorSize < count) {
					//Fill with default constructed elements
					new(vectorData.get()+vectorSize) value_type{value};
					++vectorSize;
				}
			} else if (this->size() > count) {
				for (size_type i=this->size()-1; i>=count; --i) {
					vectorData[i].~value_type();
					--vectorSize;
				}
			}
		}

		//swap

		//operators:
		//	==
		//	!=
		//	<
		//	<=
		//	>
		//	>=

		//std::swap
	};
}

#endif //MYVECTOR_HPP