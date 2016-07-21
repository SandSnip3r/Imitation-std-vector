#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP 1

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>

namespace sandsnip3r {
	template<class Type>
	class MyVectorIterator {
	private:
	};

	template<class Type, class Allocator = std::allocator<Type>>
	class MyVector {
	public:
		using value_type = Type;
		using allocator_type = Allocator;
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
		allocator_type vectorAllocator;
		using allocatorTraits = std::allocator_traits<allocator_type>;
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
			std::unique_ptr<value_type[], std::function<void(value_type*)>> newData(allocatorTraits::allocate(vectorAllocator, newCapacity), [this, newCapacity](value_type *ptr){
				allocatorTraits::deallocate(vectorAllocator, ptr, newCapacity);
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

		void resizeDown(size_type count) {
			for (size_type i=count; i<this->size(); ++i) {
				allocatorTraits::destroy(vectorAllocator, &vectorData[i]);
			}
			vectorSize = count;
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
			return allocatorTraits::max_size(vectorAllocator);
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
			resizeDown(0);
		}

		//insert
		//emplace
		//erase

		void push_back(const value_type &obj) {
			reallocateIfNecessary();
			allocatorTraits::construct(vectorAllocator, &vectorData[vectorSize], obj);
			++vectorSize;
		}

		void push_back(value_type &&obj) {
			reallocateIfNecessary();
			allocatorTraits::construct(vectorAllocator, &vectorData[vectorSize], std::move(obj));
			++vectorSize;
		}

		template<class... Args>
		void emplace_back(Args&&... args) {
			reallocateIfNecessary();
			allocatorTraits::construct(vectorAllocator, &vectorData[vectorSize], std::forward<Args>(args)...);
			++vectorSize;
		}
		
		void pop_back() {
			throwIfEmpty("pop_back");
			resizeDown(this->size()-1);
		}

		void resize(size_type count) {
			if (this->size() < count) {
				reallocateToNewSizeIfNecessary(count);
				while (vectorSize < count) {
					//Fill with default constructed elements
					allocatorTraits::construct(vectorAllocator, &vectorData[vectorSize]);
					++vectorSize;
				}
			} else if (this->size() > count) {
				resizeDown(count);
			}
		}

		void resize(size_type count, const value_type &value) {
			if (this->size() < count) {
				reallocateToNewSizeIfNecessary(count);
				while (vectorSize < count) {
					//Fill with default constructed elements
					allocatorTraits::construct(vectorAllocator, &vectorData[vectorSize], value);
					++vectorSize;
				}
			} else if (this->size() > count) {
				resizeDown(count);
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