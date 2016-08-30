#ifndef VECTOR_HPP
#define VECTOR_HPP 1

#include <algorithm>
#include <cstddef>
#include <iterator>
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
		friend class vector;
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
			pointer iteratorPointer{nullptr};
			iterator(pointer ptr) : iteratorPointer(ptr) {} 
		public:
			iterator() {}
			iterator(const iterator &it) : iteratorPointer(it.iteratorPointer) {}
			// ~iterator();
			iterator& operator=(const iterator &it) {
				if (this != &it) {
					this->iteratorPointer = it.iteratorPointer;
				}
				return *this;
			}

			reference operator*() const {
				return *iteratorPointer;
			}

			pointer operator->() const {
				return iteratorPointer;
			}

			friend bool operator==(const iterator &left, const iterator &right) {
				return left.iteratorPointer == right.iteratorPointer;
			}

			friend bool operator!=(const iterator &left, const iterator &right) {
				return !(left == right);
			}

			//Prefix operators
			iterator& operator++() {
				++iteratorPointer;
				return *this;
			}

			iterator& operator--() {
				--iteratorPointer;
				return *this;
			}

			//Postfix operators
			iterator operator++(int n) {
				auto temp = *this;
				++iteratorPointer;
				return temp;
			}

			iterator operator--(int n) {
				auto temp = *this;
				--iteratorPointer;
				return temp;
			}

			friend bool operator<(const iterator &left, const iterator &right) {
				return left.iteratorPointer < right.iteratorPointer;
			}

			friend bool operator>(const iterator &left, const iterator &right) {
				return left.iteratorPointer > right.iteratorPointer;
			}

			friend bool operator<=(const iterator &left, const iterator &right) {
				return left.iteratorPointer <= right.iteratorPointer;
			}

			friend bool operator>=(const iterator &left, const iterator &right) {
				return left.iteratorPointer >= right.iteratorPointer;
			}

			iterator& operator+=(size_type n) {
				iteratorPointer += n;
				return *this;
			}

			friend iterator operator+(const iterator &it, size_type n) {
				return (it.iteratorPointer+n);
			}

			friend iterator operator+(size_type n, const iterator &it) {
				return (it.iteratorPointer+n);
			}

			iterator& operator-=(size_type n) {
				iteratorPointer += n;
				return *this;
			}

			friend iterator operator-(const iterator &it, size_type n) {
				return (it.iteratorPointer-n);
			}

			friend difference_type operator-(const iterator &left, const iterator &right) {
				return (left.iteratorPointer-right.iteratorPointer);
			}

			// reference operator[](size_type) const;
		};

		class const_iterator {
		friend class vector;
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
			pointer iteratorPointer{nullptr};
			const_iterator(pointer ptr) : iteratorPointer(ptr) {} 
		public:
			const_iterator() {}
			const_iterator(const const_iterator &it) : iteratorPointer(it.iteratorPointer) {}
			// ~const_iterator();
			const_iterator& operator=(const const_iterator &it) {
				if (this != &it) {
					this->iteratorPointer = it.iteratorPointer;
				}
				return *this;
			}

			const_iterator& operator=(const iterator &it) {
				if (this != &it) {
					this->iteratorPointer = it.iteratorPointer;
				}
				return *this;
			}

			reference operator*() const {
				return *iteratorPointer;
			}

			pointer operator->() const {
				return iteratorPointer;
			}

			friend bool operator==(const const_iterator &left, const const_iterator &right) {
				return left.iteratorPointer == right.iteratorPointer;
			}

			friend bool operator!=(const const_iterator &left, const const_iterator &right) {
				return !(left == right);
			}

			//Prefix operators
			const_iterator& operator++() {
				++iteratorPointer;
				return *this;
			}

			const_iterator& operator--() {
				--iteratorPointer;
				return *this;
			}

			//Postfix operators
			const_iterator operator++(int n) {
				auto temp = *this;
				++iteratorPointer;
				return temp;
			}

			const_iterator operator--(int n) {
				auto temp = *this;
				--iteratorPointer;
				return temp;
			}

			friend bool operator<(const const_iterator &left, const const_iterator &right) {
				return left.iteratorPointer < right.iteratorPointer;
			}

			friend bool operator>(const const_iterator &left, const const_iterator &right) {
				return left.iteratorPointer > right.iteratorPointer;
			}

			friend bool operator<=(const const_iterator &left, const const_iterator &right) {
				return left.iteratorPointer <= right.iteratorPointer;
			}

			friend bool operator>=(const const_iterator &left, const const_iterator &right) {
				return left.iteratorPointer >= right.iteratorPointer;
			}

			const_iterator& operator+=(size_type n) {
				iteratorPointer += n;
				return *this;
			}

			friend const_iterator operator+(const const_iterator &it, size_type n) {
				return (it.iteratorPointer+n);
			}

			friend const_iterator operator+(size_type n, const const_iterator &it) {
				return (it.iteratorPointer+n);
			}

			const_iterator& operator-=(size_type n) {
				iteratorPointer += n;
				return *this;
			}

			friend const_iterator operator-(const const_iterator &it, size_type n) {
				return (it.iteratorPointer-n);
			}

			friend difference_type operator-(const const_iterator &left, const const_iterator &right) {
				return (left.iteratorPointer-right.iteratorPointer);
			}

			// reference operator[](size_type) const;
		};
		using reverse_iterator 				= std::reverse_iterator<iterator>;
		using const_reverse_iterator 	= std::reverse_iterator<const_iterator>;

	private:
		using allocatorTraits = std::allocator_traits<allocator_type>;
		allocator_type vectorAllocator;
		pointer dataBegin{nullptr};
		pointer dataEnd{nullptr};
		pointer containerEnd{nullptr};

		pointer allocate(size_type capacity) {
			return allocatorTraits::allocate(vectorAllocator, capacity);
		}

		void deallocate(pointer data, size_type capacity) {
			allocatorTraits::deallocate(vectorAllocator, data, capacity);	
		}

		void reallocateIfNecessary() {
			if (dataEnd == containerEnd) {
				//Need to reallocate to make space
				//Arguments state the the golden ratio is the most appropriate growth factor
				const auto GROWTH_FACTOR = 1.618;
				auto cap = capacity();
				size_type newCapacity = (cap == 0 ? 1 : cap * GROWTH_FACTOR);
				reallocate(newCapacity);
			}
		}

		void reallocateToNewSizeIfNecessary(size_type newCapacity) {
			if (capacity() < newCapacity) {
				reallocate(newCapacity);
			}
		}

		void reallocate(size_type newCapacity) {
			//Allocate memory for the new data
			if (newCapacity > max_size()) {
				throw std::length_error("vector::reallocate() newCapacity (which is "+std::to_string(newCapacity)+") > max_size (which is "+std::to_string(max_size())+")");
			}
			pointer newData = allocate(newCapacity);
			auto dataSize = dataEnd-dataBegin;
			//Move the old data into our new storage chunk
			std::move(dataBegin, std::next(dataBegin, dataSize), newData);
			//Assign the new data to be the current
			dataBegin = std::move(newData);
			//Update the end-of-data pointer
			dataEnd = dataBegin + dataSize;
			//Update the end-of-container pointer
			containerEnd = dataBegin + newCapacity;
		}

		void throwIfOutOfBounds(size_type pos, const std::string &methodName) const {
			if (!(pos < size())) {
				throw std::out_of_range("vector::"+methodName+"() pos (which is "+std::to_string(pos)+") >= size (which is "+std::to_string(size())+")");
			}
		}

		void resizeDown(size_type count) {
			int distance = size() - count;
			for (int i=0; i<distance; ++i) {
				--dataEnd;
				allocatorTraits::destroy(vectorAllocator, dataEnd);
			}
		}

	public:
		vector() : vector(Allocator()) {}

		explicit vector(const Allocator& alloc) : vectorAllocator(alloc) {}

		vector(size_type count, const Allocator &alloc = Allocator()) : vectorAllocator(alloc) {
			reallocate(count);
			for (size_type i=0; i<count; ++i) {
				//Fill with (count) default constructed elements
				allocatorTraits::construct(vectorAllocator, dataEnd);
				++dataEnd;
			}
		}

		vector(size_type count, const Type &value, const Allocator &alloc = Allocator()) : vectorAllocator(alloc) {
			reallocate(count);
			for (size_type i=0; i<count; ++i) {
				//Fill with (count) 'value' elements
				allocatorTraits::construct(vectorAllocator, dataEnd, value);
				++dataEnd;
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
			//TODO ^
			vectorAllocator = allocatorTraits::select_on_container_copy_construction(other.get_allocator());
			auto otherVectorSize = other.size();
			//Allocate for higher capacity
			reallocate(otherVectorSize);
			//Copy construct all elements into this list
			dataEnd = dataBegin;
			for (size_type i=0; i<otherVectorSize; ++i) {
				allocatorTraits::construct(vectorAllocator, dataEnd, other[i]);
				++dataEnd;
			}
		}

		vector(const vector &other, const Allocator &alloc) : vectorAllocator(alloc) {
			auto otherVectorSize = other.size();
			//Allocate for higher capacity
			this->reallocate(otherVectorSize);
			//Copy construct all elements into this list
			this->dataEnd = this->dataBegin;
			for (size_type i=0; i<otherVectorSize; ++i) {
				allocatorTraits::construct(this->vectorAllocator, this->dataEnd, other[i]);
				++this->dataEnd;
			}
		}

		vector(vector &&other) : vectorAllocator(std::move(other.vectorAllocator)) {
			std::swap(dataBegin, other.dataBegin);
			std::swap(dataEnd, other.dataEnd);
			std::swap(containerEnd, other.containerEnd);
			//Take ownership of everything from the other vector
		}

		vector(vector &&other, const Allocator &alloc) : vectorAllocator(alloc) {
			std::swap(dataBegin, other.dataBegin);
			std::swap(dataEnd, other.dataEnd);
			std::swap(containerEnd, other.containerEnd);
			//Take ownership of everything from the other vector
		}

		virtual ~vector() {
			//destroy all elements
			this->resizeDown(0);
			this->deallocate(this->dataBegin, this->capacity());
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
					this->deallocate(this->dataBegin, this->capacity());
					//Grab other allocator
					this->vectorAllocator = other.vectorAllocator;
				} else {
					//Allocators are the same, no need to copy allocators
				}

				//Allocate for higher capacity if neccessary
				//	if 'other' has a smaller capacity, we dont reduce ours
				//	we only allocate up to other's size
				this->reallocateToNewSizeIfNecessary(other.size());
				//Copy construct all elements into this list
				dataEnd = dataBegin;
				for (size_type i=0; i<other.size(); ++i) {
					allocatorTraits::construct(vectorAllocator, dataEnd, other[i]);
					++dataEnd;
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
					this->dataBegin = std::move(other.dataBegin);
					other.dataBegin = nullptr;
					this->dataEnd = std::move(other.dataEnd);
					other.dataEnd = nullptr;
					this->containerEnd = std::move(other.containerEnd);
					other.containerEnd = nullptr;
				} else if (vectorAllocator != other.vectorAllocator) {
					//Allocators are different and dont propigate
					//keep current and move-construct all elements in-place
					auto otherSize = other.size();
					this->reallocateToNewSizeIfNecessary(otherSize);
					this->dataEnd = this->dataBegin;
					for (size_type i=0; i<otherSize; ++i) {
						allocatorTraits::construct(this->vectorAllocator, this->dataEnd, std::move(other[i]));
						++this->dataEnd;
					}
				} else {
					//Allocators are equal
					//Take ownership of everything from the other vector
					this->dataBegin = std::move(other.dataBegin);
					other.dataBegin = nullptr;
					this->dataEnd = std::move(other.dataEnd);
					other.dataEnd = nullptr;
					this->containerEnd = std::move(other.containerEnd);
					other.containerEnd = nullptr;
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
			if (pos >= size()) {
				throw std::out_of_range("vector::at() pos (which is "+std::to_string(pos)+") >= size (which is "+std::to_string(size())+")");
			}
			return dataBegin[pos];
		}

		const_reference at(size_type pos) const {
			if (pos >= size()) {
				throw std::out_of_range("vector::at() pos (which is "+std::to_string(pos)+") >= size (which is "+std::to_string(size())+")");
			}
			return dataBegin[pos];
		}

		reference operator[](size_type pos) {
			return dataBegin[pos];
		}

		const_reference operator[](size_type pos) const {
			return dataBegin[pos];
		}

		reference front() {
			return *begin();
		}

		const_reference front() const {
			return *begin();
		}

		reference back() {
			return *(end() - 1);
		}

		const_reference back() const {
			return *(end() - 1);
		}

		pointer data() {
			return dataBegin;
		}

		const_pointer data() const {
			return dataBegin;
		}

		iterator begin() {
			return dataBegin;
		}

		const_iterator begin() const {
			return dataBegin;
		}

		const_iterator cbegin() const {
			return dataBegin;
		}

		iterator end() {
			return dataEnd;
		}

		const_iterator end() const {
			return dataEnd;
		}

		const_iterator cend() const {
			return dataEnd;
		}

		//rbegin, crbegin
		//rend, crend

		bool empty() const {
			return begin() == end();
		}

		size_type size() const {
			return dataEnd - dataBegin;
		}

		size_type max_size() const {
			return allocatorTraits::max_size(vectorAllocator);
		}

		void reserve(size_type newCapacity) {
			reallocateToNewSizeIfNecessary(newCapacity);
		}

		size_type capacity() const {
			return containerEnd - dataBegin;
		}

		void shrink_to_fit() {
			auto dataSize = size();
			if (dataSize < capacity()) {
				reallocate(dataSize);
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
			allocatorTraits::construct(vectorAllocator, dataEnd, obj);
			++dataEnd;
		}

		void push_back(value_type &&obj) {
			reallocateIfNecessary();
			allocatorTraits::construct(vectorAllocator, dataEnd, std::move(obj));
			++dataEnd;
		}

		template<class... Args>
		reference emplace_back(Args&&... args) {
			reallocateIfNecessary();
			allocatorTraits::construct(vectorAllocator, dataEnd, std::forward<Args>(args)...);
			++dataEnd;
			return *(end()-1);
		}
		
		void pop_back() {
			--dataEnd;
			allocatorTraits::destroy(vectorAllocator, dataEnd);
		}

		void resize(size_type count) {
			auto dataSize = size();
			if (dataSize < count) {
				reallocateToNewSizeIfNecessary(count);
				for (auto i=dataSize; i<=count; ++i) {
					//Fill with default constructed elements
					allocatorTraits::construct(vectorAllocator, dataEnd);
					++dataEnd;
				}
			} else if (dataSize > count) {
				//Resize down(destroying things at the end), dont deallocate
				resizeDown(count);
			}
		}

		void resize(size_type count, const value_type &value) {
			auto dataSize = size();
			if (dataSize < count) {
				reallocateToNewSizeIfNecessary(count);
				for (auto i=dataSize; i<=count; ++i) {
					//Fill with default constructed elements
					allocatorTraits::construct(vectorAllocator, dataEnd, value);
					++dataEnd;
				}
			} else if (dataSize > count) {
				//Resize down(destroying things at the end), dont deallocate
				resizeDown(count);
			}
		}

		void swap(vector &other) {
			if (typename allocatorTraits::propagate_on_container_swap()) {
				//Exchange allocators
				std::swap(vectorAllocator, other.vectorAllocator);
			}
			std::swap(dataBegin, other.dataBegin);
			std::swap(dataEnd, other.dataEnd);
			std::swap(containerEnd, other.containerEnd);
		}


		//std::swap
	};

	template<class T, class Alloc>
	bool operator==(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		if (left.size() != right.size()) {
			return false;
		}
		auto leftIt = left.begin();
		auto leftEnd = left.end();
		auto rightIt = right.begin();
		if (leftIt != leftEnd) {
			if (*leftIt != *rightIt) {
				return false;
			}
			++leftIt;
			++rightIt;
		}
		return true;
	}

	template<class T, class Alloc>
	bool operator!=(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		return !(left == right);
	}

	template<class T, class Alloc, class Compare>
	bool myComparison(const vector<T, Alloc> &left, const vector<T, Alloc> &right, Compare comp) {
		auto leftIt = left.begin();
		auto leftEnd = left.end();
		auto rightIt = right.begin();
		auto rightEnd = right.end();
		for (; (leftIt != leftEnd) && (rightIt != rightEnd); leftIt++, rightIt++ ) {
			if (comp(*leftIt, *rightIt)) {
				return true;
			}
			if (comp(*rightIt, *leftIt)) {
				return false;
			}
		}
		return (leftIt == leftEnd) && (rightIt != rightEnd);

	}
	
	template<class T, class Alloc>
	bool operator<(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		return myComparison(left, right, std::less<T>());
	}

	template<class T, class Alloc>
	bool operator<=(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		return myComparison(left, right, std::less_equal<T>());
	}

	template<class T, class Alloc>
	bool operator>(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		return myComparison(left, right, std::greater<T>());
	}

	template<class T, class Alloc>
	bool operator>=(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		return myComparison(left, right, std::greater_equal<T>());
	}
}

#endif //VECTOR_HPP