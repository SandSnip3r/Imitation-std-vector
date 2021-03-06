#ifndef VECTOR_HPP
#define VECTOR_HPP 1

#include <algorithm>
#include <cmath>
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
			iterator() = default;
			iterator(const iterator &it) : iteratorPointer(it.iteratorPointer) {}
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

			reference operator[](size_type n) const {
				return iteratorPointer[n];
			}
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
			const_iterator() = default;
			const_iterator(const const_iterator &it) : iteratorPointer(it.iteratorPointer) {}
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

			reference operator[](size_type n) const {
				return iteratorPointer[n];
			}
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
				size_type newCapacity = (cap == 0 ? 1 : std::llround(cap * GROWTH_FACTOR));
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
			pointer newDataBegin = allocate(newCapacity);
			pointer newDataEnd = newDataBegin;
			auto it = dataBegin;
			//Move construct new elements into place
			//	also destroy previous
			while (it != dataEnd) {
				allocatorTraits::construct(vectorAllocator, newDataEnd, std::move(*it));
				allocatorTraits::destroy(vectorAllocator, it);
				++it;
				++newDataEnd;
			}
			//Deallocate previous memory
			deallocate(dataBegin, containerEnd - dataBegin);
			//Update data pointers
			dataBegin = newDataBegin;
			dataEnd = newDataEnd;
			containerEnd = dataBegin + newCapacity;
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

		explicit vector(size_type count, const Allocator &alloc = Allocator()) : vectorAllocator(alloc) {
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

		template<class InputIt, typename = std::enable_if_t<
														std::is_base_of<
															std::input_iterator_tag,
															typename std::iterator_traits<InputIt>::iterator_category
														>::value,
														InputIt
													>>
		vector(InputIt first, InputIt last, const Allocator &alloc = Allocator()) : vectorAllocator(alloc) {
			reallocate(last-first);
			while (first != last) {
				//Fill with elements in range
				emplace_back(*first);
				++first;
			}
		}

		vector(std::initializer_list<Type> ilist, const Allocator &alloc = Allocator()) : vectorAllocator(alloc) {
			const size_type length = std::distance(ilist.begin(), ilist.end());
			reallocate(length);
			for (auto it=ilist.begin(), end=ilist.end(); it!=end; ++it) {
				emplace_back(*it);
			}
		}

		vector(const vector &other) : vectorAllocator(allocatorTraits::select_on_container_copy_construction(other.get_allocator())) {
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

		reverse_iterator rbegin() {
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(end());
		}

		const_reverse_iterator crbegin() const {
			return const_reverse_iterator(end());
		}

		reverse_iterator rend() {
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const {
			return const_reverse_iterator(begin());
		}

		const_reverse_iterator crend() const {
			return const_reverse_iterator(begin());
		}

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

		// iterator insert(const_iterator pos, const value_type& value)
		// iterator insert(const_iterator pos, T&& value)
		// iterator insert(const_iterator pos, size_type count, const T& value)
		// template< class InputIt >
		// iterator insert(const_iterator pos, InputIt first, InputIt last)
		// iterator insert(const_iterator pos, std::initializer_list<T> ilist)

		// template< class... Args > 
		// iterator emplace(const_iterator pos, Args&&... args)

		// iterator erase(const_iterator pos);
		// iterator erase(const_iterator first, const_iterator last);

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
			return *(dataEnd++);
		}
		
		void pop_back() {
			--dataEnd;
			allocatorTraits::destroy(vectorAllocator, dataEnd);
		}

		void resize(size_type count) {
			auto dataSize = size();
			if (dataSize < count) {
				reallocateToNewSizeIfNecessary(count);
				for (auto i=dataSize; i<count; ++i) {
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
				for (auto i=dataSize; i<count; ++i) {
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

	template<class Iterator1, class Iterator2>
	bool myComparisonWithoutEqual(Iterator1 leftIt, Iterator1 leftEnd, Iterator2 rightIt, Iterator2 rightEnd) {
		while ((leftIt != leftEnd) && (rightIt != rightEnd)) {
			if (*leftIt < *rightIt) {
				return true;
			}
			if (*rightIt < *leftIt) {
				return false;
			}
			++leftIt;
			++rightIt;
		}
		//Return if left is a subset of right
		return (leftIt == leftEnd) && (rightIt != rightEnd);
	}

	template<class Iterator1, class Iterator2>
	bool myComparisonWithEqual(Iterator1 leftIt, Iterator1 leftEnd, Iterator2 rightIt, Iterator2 rightEnd) {
		while ((leftIt != leftEnd) && (rightIt != rightEnd)) {
			if (*leftIt < *rightIt) {
				return true;
			}
			if (*rightIt < *leftIt) {
				return false;
			}
			++leftIt;
			++rightIt;
		}
		//If leftIt!=leftEnd, that means that it still has remaining elements but right doesnt
		//	this means right is a subset of left
		return leftIt == leftEnd;
	}
	
	template<class T, class Alloc>
	bool operator<(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		return myComparisonWithoutEqual(left.begin(), left.end(), right.begin(), right.end());
	}

	template<class T, class Alloc>
	bool operator<=(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		return myComparisonWithEqual(left.begin(), left.end(), right.begin(), right.end());
	}

	template<class T, class Alloc>
	bool operator>(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		return myComparisonWithoutEqual(right.begin(), right.end(), left.begin(), left.end());
	}

	template<class T, class Alloc>
	bool operator>=(const vector<T, Alloc> &left, const vector<T, Alloc> &right) {
		return myComparisonWithEqual(right.begin(), right.end(), left.begin(), left.end());
	}
}

#endif //VECTOR_HPP