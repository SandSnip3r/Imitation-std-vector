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
		typedef Type value_type;
		typedef std::allocator<value_type> allocator_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef Type& reference;
		typedef const Type& const_reference;
		typedef Type* pointer;
		typedef const Type* const_pointer;
		//iterator
		//const_iterator
		//reverse_iterator
		//const_reverse_iterator
	private:
		std::allocator<value_type> vectorAllocator;
		std::unique_ptr<value_type[], std::function<void(value_type*)>> vectorData;
		size_t vectorSize{0};
		size_t vectorCapacity{0};

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
	public:
		//constructor

		~MyVector() {
			this->clear();
		}

		//operator=
		//assign

		allocator_type get_allocator() const {
			return vectorAllocator;
		}

		reference at(size_type pos) {
			return const_cast<reference>(static_cast<const MyVector<value_type>*>(this)->at(pos));
		}

		const_reference at(size_type pos) const {
			if (!(pos < this->size())) {
				throw std::out_of_range("MyVector::at() pos (which is "+std::to_string(pos)+") >= size (which is "+std::to_string(this->size())+")");
			}
			return vectorData[pos];
		}

		reference operator[](size_type pos) {
			return vectorData[pos];
		}

		const_reference operator[](size_type pos) const {
			return vectorData[pos];
		}

		reference front() {
			return const_cast<reference>(static_cast<const MyVector<value_type>*>(this)->front());
		}

		const_reference front() const {
			if (this->empty()) {
				throw std::out_of_range("MyVector::front(), but it's empty");
			}
			return vectorData[0];
		}

		reference back() {
			return const_cast<reference>(static_cast<const MyVector<value_type>*>(this)->back());
		}

		const_reference back() const {
			if (this->empty()) {
				throw std::out_of_range("MyVector::back(), but it's empty");
			}
			return vectorData[this->size()-1];
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

		//max_size
		size_type max_size() const {
			return std::numeric_limits<decltype(vectorSize)>::max();
		}

		void reserve(size_type newCapacity) {
			if (this->capacity() < newCapacity) {
				reallocate(newCapacity);
			}
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
			if (this->size() == this->capacity()) {
				//Need to reallocate to make space
				auto newCapacity = (vectorCapacity == 0 ? 1 : vectorCapacity * 2);
				reserve(newCapacity);
			}
			new(vectorData.get()+vectorSize) value_type{obj};
			++vectorSize;
		}

		void push_back(value_type &&obj) {
			if (this->size() == this->capacity()) {
				//Need to reallocate to make space
				auto newCapacity = (vectorCapacity == 0 ? 1 : vectorCapacity * 2);
				reserve(newCapacity);
			}
			new(vectorData.get()+vectorSize) value_type{std::move(obj)};
			++vectorSize;
		}

		template<class... Args>
		void emplace_back(Args&&... args) {
			if (this->size() == this->capacity()) {
				//Need to reallocate to make space
				auto newCapacity = (vectorCapacity == 0 ? 1 : vectorCapacity * 2);
				reserve(newCapacity);
			}
			new(vectorData.get()+vectorSize) value_type{std::forward<Args>(args)...};
			++vectorSize;
		}
		
		void pop_back() {
			if (!this->empty()) {
				vectorData[this->size()-1].~value_type();
				--vectorSize;
			}
		}

		void resize(size_type count) {
			if (this->size() < count) {
				if (this->capacity() < count) {
					//need to allocate more
					reallocate(count);
				}
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
				if (this->capacity() < count) {
					//need to allocate more
					reallocate(count);
				}
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