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
		using allocatorTraits = std::allocator_traits<allocator_type>;
		allocator_type vectorAllocator;
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
			//destroy all elements
			this->resizeDown(0);
			//std::unique_ptr will handle deallocation
		}

		MyVector& operator=(const MyVector &other) {
			this->vectorAllocator = other.vectorAllocator;
			//Destroy everything in this container
			this->resizeDown(0);
			//Allocate for higher capacity if neccessary
			//	if 'other' has a smaller capacity, we dont reduce ours
			this->reallocateToNewSizeIfNecessary(other.capacity());
			//Copy construct all elements into this list
			for (size_type i=0; i<other.size(); ++i) {
				allocatorTraits::construct(vectorAllocator, &vectorData[i], other[i]);
				++vectorSize;
			}
			return *this;
		}

		MyVector& operator=(MyVector &&other) {
			this->vectorAllocator = std::move(other.vectorAllocator);
			//Destroy everything in this container
			this->resizeDown(0);
			//Take ownership of everything from the other vector
			this->vectorSize = std::move(other.vectorSize);
			this->vectorCapacity = std::move(other.vectorCapacity);
			this->vectorData = std::move(other.vectorData);
			return *this;
		}
		//operator= initializer list

		//assign

		allocator_type get_allocator() const {
			return vectorAllocator;
		}

		reference at(size_type pos) {
			this->throwIfOutOfBounds(pos, "at");
			return this->elementAt(pos);
		}

		const_reference at(size_type pos) const {
			this->throwIfOutOfBounds(pos, "at");
			return this->elementAt(pos);
		}

		reference operator[](size_type pos) {
			return this->elementAt(pos);
		}

		const_reference operator[](size_type pos) const {
			return this->elementAt(pos);
		}

		reference front() {
			this->throwIfEmpty("front");
			return this->elementAt(0);
		}

		const_reference front() const {
			this->throwIfEmpty("front");
			return this->elementAt(0);
		}

		reference back() {
			this->throwIfEmpty("back");
			return this->elementAt(this->size()-1);
		}

		const_reference back() const {
			this->throwIfEmpty("back");
			return this->elementAt(this->size()-1);
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
			this->reallocateToNewSizeIfNecessary(newCapacity);
		}

		size_type capacity() const {
			return vectorCapacity;
		}

		void shrink_to_fit() {
			if (this->size() < this->capacity()) {
				this->reallocate(vectorSize);
			}
		}

		void clear() {
			this->resizeDown(0);
		}

		//insert
		//emplace
		//erase

		void push_back(const value_type &obj) {
			this->reallocateIfNecessary();
			allocatorTraits::construct(vectorAllocator, &vectorData[vectorSize], obj);
			++vectorSize;
		}

		void push_back(value_type &&obj) {
			this->reallocateIfNecessary();
			allocatorTraits::construct(vectorAllocator, &vectorData[vectorSize], std::move(obj));
			++vectorSize;
		}

		template<class... Args>
		void emplace_back(Args&&... args) {
			this->reallocateIfNecessary();
			allocatorTraits::construct(vectorAllocator, &vectorData[vectorSize], std::forward<Args>(args)...);
			++vectorSize;
		}
		
		void pop_back() {
			this->throwIfEmpty("pop_back");
			this->resizeDown(this->size()-1);
		}

		void resize(size_type count) {
			if (this->size() < count) {
				this->reallocateToNewSizeIfNecessary(count);
				while (vectorSize < count) {
					//Fill with default constructed elements
					allocatorTraits::construct(vectorAllocator, &vectorData[vectorSize]);
					++vectorSize;
				}
			} else if (this->size() > count) {
				this->resizeDown(count);
			}
		}

		void resize(size_type count, const value_type &value) {
			if (this->size() < count) {
				this->reallocateToNewSizeIfNecessary(count);
				while (vectorSize < count) {
					//Fill with default constructed elements
					allocatorTraits::construct(vectorAllocator, &vectorData[vectorSize], value);
					++vectorSize;
				}
			} else if (this->size() > count) {
				this->resizeDown(count);
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