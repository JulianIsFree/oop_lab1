#pragma once
#include <iostream>
#include <unordered_map>
#include <string>

namespace LabTritSetSpace 
{
	using uint = unsigned long long;
	
	// Impl. defined constants
	constexpr size_t uintSize = sizeof(uint);
	constexpr uint tritMask = 0x3;
	constexpr size_t tritBitsRequired = 2;
	constexpr size_t tritPerUint = (uintSize * 8) / tritBitsRequired;
	
	enum class Trit : uint {False, Unknown, True};
	
	Trit operator&(const Trit& tr1, const Trit& tr2);
	Trit operator|(const Trit& tr1, const Trit& tr2);
	Trit operator!(const Trit& tr1);

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

			const Trit operator| (const Trit& trit) const;
			const Trit operator| (const TritReference& tr) const;
			
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

		void expand(const size_t N);
		inline void setTrit(const size_t index, const Trit trit); // not a part of TritSet's interface
	public:
		TritSet();
		TritSet(const size_t N);
		TritSet(TritSet &set);

		void shrink(); // tested
		void trim(const size_t index); // tested
		Trit operator[](const size_t index) const; // tested
		TritReference operator[](const size_t index); // tested

		size_t getLenght() const; // tested //number of elements in arr
		size_t capacity() const; // tested //efficient size of arr 
		size_t getSize() const; // tested // size of array in uints
		size_t getLastSetTrit() const; // tested
		size_t getInitLenght() const; // not tested, it's just a getter
		const uint * getArr() const; // not tested, it's just a getter

		size_t cardinality(const Trit value) const; // tested
		std::unordered_map<Trit, size_t, std::hash<Trit>> cardinality() const; //tested

		TritSet operator&(const TritSet &set) const; // tested
		TritSet operator|(const TritSet &set) const; // tested
		TritSet operator!() const; // tested
		friend std::ostream& operator<< (std::ostream& out, const TritSet & ts);
	};
}
