#pragma once
#include <iostream>
#include <unordered_map>
#include <string>

namespace LabTritSetSpace 
{
	using uint = unsigned long long;
	
	// Impl. defined constants
	const size_t uintSize = sizeof(uint);
	const uint tritMask = 0x3;
	const size_t tritBitsRequired = 2;
	const size_t tritPerUint = (uintSize * 8) / tritBitsRequired;
	
	enum class Trit : uint {False, Unknown, True};
	std::ostream& operator<< (std::ostream& out, const Trit& tr);
	std::string tritState(const Trit& tr);

	class TritSet 
	{
		friend class TritReference;
	public:
		class TritReference // nested friend class required for direct accessing trit
		{
			friend class TritSet;
		private:
			TritSet& set;
			const size_t index;
			TritReference(TritSet& set, const size_t index) : set(set), index(index) {};
			Trit operator()() const; //returns referenced trit value
		public:
			TritReference& operator= (const Trit& trit); // may realloc set, if destination index is above or equal set's lenght
			TritReference& operator= (const TritReference& tr);

			// Logic operations
			const Trit operator& (const Trit& trit) const;
			const Trit operator& (const TritReference& tr) const;
			TritReference& operator&= (const Trit& trit);
			TritReference& operator&= (const TritReference& tr);

			const Trit operator| (const Trit& trit) const;
			const Trit operator| (const TritReference& tr) const;
			TritReference& operator|= (const Trit& trit);
			TritReference& operator|= (const TritReference& tr);
			
			const Trit operator! () const;
			
			// Casts and output representation
			operator Trit() const;	// cast operator
			operator std::string() const;
			friend std::ostream& operator<< (std::ostream& out, const TritSet::TritReference& tr);
		};
	private:
		uint * arr; // container
		size_t size; // arr size
		size_t lenght; // elements in arr
		size_t initLenght; // initial number of elements
		size_t lastSetTrit; // last set trit in set, initially is 0
		bool changed; // has been anything changed since the 'arr' allocation?

		void realloc(const size_t N);
		inline void setTrit(const size_t index, const Trit trit); // not a part of TritSet's interface
	public:
		TritSet();
		TritSet(const size_t N);
		TritSet(uint * arr, const size_t lenght, const size_t size, bool doArrClear); // if doArrClear == true then initialize values of arr to Unknown

		void clear();
		void shrink();
		void trim(const size_t lastSetIndex);
		Trit operator[](const size_t index) const;
		TritReference operator[](const size_t index);

		size_t getLenght() const; //number of elements in arr
		size_t capacity() const; //efficient size of arr 
		size_t getSize() const;
		size_t getLastSetTrit() const;
		size_t cardinality(const Trit value) const;
		std::unordered_map<Trit, size_t, std::hash<Trit>> cardinality() const;

		TritSet operator&(const TritSet &set);
		TritSet operator|(const TritSet &set);
		TritSet operator!();
		friend std::ostream& operator<< (std::ostream& out, const TritSet & ts);
	};
}
