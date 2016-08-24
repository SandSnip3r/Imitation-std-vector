#ifndef VECTOR_HPP
#define VECTOR_HPP 1

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>

namespace sandsnip3r {

	template<class Type, class Allocator = std::allocator<Type>>
	class vector {
	public:
		using allocator_type 	= Allocator;
		using value_type 			= typename Allocator::value_type;
		using size_type 			= typename Allocator::size_type;
		using difference_type = typename Allocator::difference_type;
		using reference 			= typename Allocator::reference;
		using const_reference = typename Allocator::const_reference;
		using pointer 				= typename Allocator::pointer;
		using const_pointer 	= typename Allocator::const_pointer;

		class iterator {
		public:
			using value_type 				= typename Allocator::value_type;
			using size_type 				= typename Allocator::size_type;
			using difference_type 	= typename Allocator::difference_type;
			using reference 				= typename Allocator::reference;
			using const_reference 	= typename Allocator::const_reference;
			using pointer 					= typename Allocator::pointer;
			using const_pointer 		= typename Allocator::const_pointer;
			using iterator_category = std::random_access_iterator_tag;
		private:
		};
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
				throw std::length_error("vector::reallocate() newCapacity (which is "+std::to_string(newCapacity)+") > max_size (which is "+std::to_string(this->max_size())+")");
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
				throw std::out_of_range("vector::"+methodName+"() pos (which is "+std::to_string(pos)+") >= size (which is "+std::to_string(this->size())+")");
			}
		}

		void throwIfEmpty(const std::string &methodName) const {
			if (this->empty()) {
				throw std::out_of_range("vector::"+methodName+"() is empty");
			}
		}

		void resizeDown(size_type count) {
			for (size_type i=count; i<this->size(); ++i) {
				allocatorTraits::destroy(vectorAllocator, &vectorData[i]);
			}
			vectorSize = count;
		}

	public:
		vector() : vector(Allocator()) {}

		explicit vector(const Allocator& alloc) : vectorAllocator(alloc) {}

		vector(size_type count, const Allocator &alloc = Allocator()) : vectorAllocator(alloc) {
			reallocate(count);
			while (vectorSize < count) {
				//Fill with (count) default constructed elements
				allocatorTraits::construct(vectorAllocator, &vectorData[vectorSize]);
				++vectorSize;
			}
		}

		vector(size_type count, const Type &value, const Allocator &alloc = Allocator()) : vectorAllocator(alloc) {
			reallocate(count);
			while (vectorSize < count) {
				//Fill with (count) 'value' elements
				allocatorTraits::construct(vectorAllocator, &vectorData[vectorSize], value);
				++vectorSize;
			}
		}

		vector(std::initializer_list<Type> ilist, const Allocator &alloc = Allocator()) : vectorAllocator(alloc) {
			const size_type length = std::distance(ilist.begin(), ilist.end());
			reallocate(length);
			for (auto it=ilist.begin(), end=ilist.end(); it!=end; ++it) {
				emplace_back(*it);
			}
		}

		vector(const vector &other) {
			this->vectorAllocator = allocatorTraits::select_on_container_copy_construction(other.get_allocator());
			auto otherVectorSize = other.size();
			//Allocate for higher capacity
			reallocate(otherVectorSize);
			//Copy construct all elements into this list
			for (size_type i=0; i<otherVectorSize; ++i) {
				allocatorTraits::construct(vectorAllocator, &vectorData[i], other[i]);
				++vectorSize;
			}
		}

		vector(const vector &other, const Allocator &alloc) : vectorAllocator(alloc) {
			auto otherVectorSize = other.size();
			//Allocate for higher capacity
			reallocate(otherVectorSize);
			//Copy construct all elements into this list
			for (size_type i=0; i<otherVectorSize; ++i) {
				allocatorTraits::construct(vectorAllocator, &vectorData[i], other[i]);
				++vectorSize;
			}
		}

		vector(vector &&other) {
			//Take ownership of everything from the other vector
			this->vectorAllocator = std::move(other.vectorAllocator);
			this->vectorSize = std::move(other.vectorSize);
			other.vectorSize = 0;
			this->vectorCapacity = std::move(other.vectorCapacity);
			this->vectorData = std::move(other.vectorData);
		}

		vector(vector &&other, const Allocator &alloc) : vectorAllocator(alloc) {
			//Take ownership of everything from the other vector
			this->vectorSize = std::move(other.vectorSize);
			other.vectorSize = 0;
			this->vectorCapacity = std::move(other.vectorCapacity);
			this->vectorData = std::move(other.vectorData);
		}

		virtual ~vector() {
			//destroy all elements
			this->resizeDown(0);
			//std::unique_ptr will handle deallocation
		}

		vector& operator=(const vector &other) {
			if (&other != this) {
				//Destroy everything in this container
				this->resizeDown(0);

				if (typename allocatorTraits::propagate_on_container_copy_assignment()) {
					this->vectorAllocator = other.vectorAllocator;
				} else if (this->vectorAllocator != other.vectorAllocator) {
					//Allocators dont propigate and are different
					//use current to deallocate
					this->vectorData.reset(nullptr);
					this->vectorCapacity = 0;
					this->vectorAllocator = other.vectorAllocator;
				} else {
					//Allocators are the same, no need to copy
				}

				//Allocate for higher capacity if neccessary
				//	if 'other' has a smaller capacity, we dont reduce ours
				this->reallocateToNewSizeIfNecessary(other.size());
				//Copy construct all elements into this list
				for (size_type i=0; i<other.size(); ++i) {
					allocatorTraits::construct(vectorAllocator, &vectorData[i], other[i]);
					++vectorSize;
				}
			}
			return *this;
		}

		vector& operator=(vector &&other) {
			if (&other != this) {
				//Destroy everything in this container
				this->resizeDown(0);

				if (typename allocatorTraits::propagate_on_container_move_assignment()) {
					//Allocator is propigated
					this->vectorAllocator = other.vectorAllocator;
					//Take ownership of everything from the other vector
					this->vectorSize = std::move(other.vectorSize);
					other.vectorSize = 0;
					this->vectorCapacity = std::move(other.vectorCapacity);
					this->vectorData = std::move(other.vectorData);
				} else if (vectorAllocator != other.vectorAllocator) {
					//Allocators are different
					//keep current and move-construct all elements in-place
					auto otherSize = other.size();
					this->reallocateToNewSizeIfNecessary(otherSize);
					for (size_type i=0; i<otherSize; ++i) {
						allocatorTraits::construct(vectorAllocator, &vectorData[i], std::move(other.elementAt(i)));
					}
					vectorSize = otherSize;
				} else {
					//Allocators are equal
					//Take ownership of everything from the other vector
					this->vectorSize = std::move(other.vectorSize);
					other.vectorSize = 0;
					this->vectorCapacity = std::move(other.vectorCapacity);
					this->vectorData = std::move(other.vectorData);
				}
			}
			return *this;
		}

		vector& operator=(std::initializer_list<value_type> ilist) {
			//Destroy everything in this container
			this->resizeDown(0);
			//Reallocate if this is larger than our current container
			const size_type length = std::distance(ilist.begin(), ilist.end());
			reallocateToNewSizeIfNecessary(length);
			for (auto it=ilist.begin(), end=ilist.end(); it!=end; ++it) {
				emplace_back(*it);
			}
			return *this;
		}

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


		//std::swap
	};

	template<class T, class Alloc>
	bool operator==(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		if (left.size() != right.size()) {
			return false;
		}
		size_t sizes = left.size();
		for (size_t i=0; i<sizes; ++i) {
			if (left[i] != right[i]) {
				return false;
			}
		}
		return true;
	}

	template<class T, class Alloc>
	bool operator!=(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		return !(left == right);
	}

	template<class T, class Alloc>
	bool operator<(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		if (left.size() != right.size()) {
			return false;
		}
		auto leftSize = left.size();
		auto rightSize = right.size();
		auto smallerSize = std::min(leftSize, rightSize);
		for (size_t i=0; i<smallerSize; ++i) {
			if (left[i] < right[i]) {
				//This element is less
				return true;
			} else if (left[i] != right[i]) {
				//This element is greater
				return false;
			}
		}
		//All elements [0, smallerSize) are equal
		if (leftSize < rightSize) {
			//left is a prefix of right
			return true;
		} else {
			//both are either completely equal or left is longer
			return false;
		}
	}

	template<class T, class Alloc>
	bool operator<=(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		return ((left == right) || (left < right));
	}

	template<class T, class Alloc>
	bool operator>(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		return (!(left == right) && !(left < right));
	}

	template<class T, class Alloc>
	bool operator>=(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		return !(left < right);
	}
	//operators:
	//	<
	//	<=
	//	>
	//	>=
}

#endif //VECTOR_HPP