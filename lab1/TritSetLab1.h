#pragma once
#include <iostream>
#include <unordered_map>
#include <string>

namespace LabTritSetSpace 
{
	//typedef unsigned long long uint;
	using uint = unsigned long long;

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
		class TritReference
		{
			friend class TritSet;
		private:
			TritSet& set;
			const size_t index;
			TritReference(TritSet& set, const size_t index) : set(set), index(index) {};
			Trit operator()() const; //returns referenced trit value
		public:
			TritReference& operator= (const Trit& trit); // may reallof sec, if destination index is above or equal set's lenght
			TritReference& operator= (const TritReference& tr);

			Trit operator& (const Trit& trit) const;
			Trit operator& (const TritReference& tr) const;
			TritReference& operator&= (const Trit& trit);
			TritReference& operator&= (const TritReference& tr);

			Trit operator| (const Trit& trit) const;
			Trit operator| (const TritReference& tr) const;
			TritReference& operator|= (const Trit& trit);
			TritReference& operator|= (const TritReference& tr);

			Trit operator! () const;
			operator Trit() const;	// cast operator
			operator std::string() const;
			friend std::ostream& operator<< (std::ostream& out, const TritSet::TritReference& tr);
		};
	private:
		uint * arr;
		size_t size; // arr size
		size_t lenght; // elements in arr
		size_t lastSetTrit;
		bool changed;

		void realloc(const size_t N); // TODO
		inline void setTrit(const size_t index, const Trit trit);
	public:
		TritSet();
		TritSet(const size_t N);
		TritSet(uint * arr, const size_t lenght, const size_t size, bool doArrClear);

		void clear();
		void shrink();
		Trit operator[](const size_t index) const;
		TritReference operator[](const size_t index);

		size_t getLenght() const; //number of elements in arr
		size_t capacity() const; //efficient size of arr 
		size_t getSize() const;
		size_t getLastSetTrit() const;

		TritSet operator&(const TritSet &set);
		TritSet operator|(const TritSet &set);
		TritSet operator!();
		friend std::ostream& operator<< (std::ostream& out, const TritSet & ts);
	};
}
