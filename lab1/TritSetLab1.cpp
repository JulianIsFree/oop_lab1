#include "TritSetLab1.h"
using namespace LabTritSetSpace;
/*
trit == unknown ?:
yes:
	index < set.lenght ?:
	yes:	setTrit(index, trit)
	no:		nothing
no:
	index < set.lenght ?:
	yes:	setTrit(index, trit)
	no:		realloc and setTrit
*/
TritSet::TritReference& TritSet::TritReference::operator=(const Trit & trit)
{
	if (trit == Trit::Unknown)
	{
		if (index < set.getLenght())
			set.setTrit(index, trit);
	}
	else
	{
		if (index >= set.getLenght())
			set.realloc(index + 1);
		set.setTrit(index, trit);
	}

	return *this;
}

TritSet::TritReference & TritSet::TritReference::operator=(const TritReference & tr)
{
	return this->operator=(tr);;
}

const Trit TritSet::TritReference::operator&(const Trit & trit) const
{
	Trit curr = this->operator()();

	if (trit == Trit::False || curr == Trit::False)
		return Trit::False;

	if (trit == Trit::Unknown || curr == Trit::Unknown)
		return Trit::Unknown;

	return Trit::True;
}

const Trit TritSet::TritReference::operator&(const TritReference & tr) const
{
	return this->operator&(tr());
}

TritSet::TritReference& TritSet::TritReference::operator&=(const Trit & trit)
{
	*this = *this & trit;
	return *this;
}

TritSet::TritReference& TritSet::TritReference::operator&=(const TritReference & tr)
{
	*this = *this & tr;
	return *this;
}

const Trit TritSet::TritReference::operator|(const Trit & trit) const
{
	Trit curr = this->operator()();
	
	if (trit == Trit::True || curr == Trit::True)
		return Trit::True;

	if (trit == Trit::Unknown || curr == Trit::Unknown)
		return Trit::Unknown;

	return Trit::False;
}

const Trit TritSet::TritReference::operator|(const TritReference & tr) const
{
	return this->operator|(tr());
}

TritSet::TritReference& TritSet::TritReference::operator|=(const Trit & trit)
{
	*this = *this | trit;
	return *this;
}

TritSet::TritReference& TritSet::TritReference::operator|=(const TritReference & tr)
{
	*this = *this | tr;
	return *this;
}

const Trit TritSet::TritReference::operator!() const
{
	Trit trit = this->operator()();
	switch (trit)
	{
	case Trit::False:
		return Trit::True;
	case Trit::True:
		return Trit::False;
	default:
		return Trit::Unknown;
	}
}

Trit TritSet::TritReference::operator()() const
{
	return static_cast<const TritSet>(set)[index]; // ? 
}

TritSet::TritReference::operator Trit() const
{
	return this->operator()();
}

TritSet::TritReference::operator std::string() const
{
	Trit curr = this->operator()();
	switch (curr)
	{
	case Trit::True:
		return "True";
	case Trit::False:
		return "False";
	default:
		return "Unknown";
	}
}

void TritSet::realloc(const size_t newLenght)
{
	size_t prevSize = this->size;
	size_t prevLen = this->lenght;
	uint * prevArr = this->arr;
	
	this->lenght = newLenght;
	this->size = (newLenght + tritPerUint - 1) / tritPerUint;
	this->arr = new uint[size];
	this->clear();
	
	std::cout << *this << std::endl;

	size_t copySize = size > prevSize ? prevSize : size;
	

	memcpy(this->arr, prevArr,
		copySize * uintSize);

	for (size_t i = prevLen; i < newLenght; ++i)
		setTrit(i, Trit::Unknown);
	
	std::cout << *this << std::endl;

	delete prevArr;
}

inline void TritSet::setTrit(const size_t index, const Trit trit)
{
	// function assumes that index is below than lenght of set
	if (index >= lenght)
		throw "In private function \"TritSet::setTrit(const size_t, const Trit)\": index out of range.";

	if (changed == false && trit != Trit::Unknown)
	{
		lastSetTrit = index;
		changed = true;
	}
	
	if (index > lastSetTrit && trit != Trit::Unknown)
		lastSetTrit = index;

	const size_t arrIndex = index / tritPerUint;
	const size_t cellOffset = (index % tritPerUint) * tritBitsRequired;
	arr[arrIndex] ^= arr[arrIndex] & (tritMask << cellOffset);
	arr[arrIndex] ^= static_cast<uint>(trit) << cellOffset;
}

TritSet::TritSet()
{
	lenght = 0;
	initLenght = 0;
	size = 0;
	arr = NULL;
	lastSetTrit = 0;
	changed = false;
	clear();
}

TritSet::TritSet(const size_t N)
{
	lenght = N;
	initLenght = N;
	size = (N + tritPerUint - 1) / tritPerUint; // вычисление кол-ва uint'ов, необходимого для N trit'ов
	arr = new uint[size];
	lastSetTrit = 0;
	changed = false;
	clear();
}

TritSet::TritSet(uint * arr, const size_t lenght, const size_t size, bool doArrClear = true)
{
	this->arr = arr;
	this->lenght = lenght;
	this->size = size;
	this->lastSetTrit = 0;
	this->initLenght = lenght;

	if (doArrClear)
	{
		this->changed = false;
		clear();
	}
	else
		this->changed = true;
		
}

void TritSet::clear()
{
	for (size_t i = 0; i < lenght; ++i)
		setTrit(i, Trit::Unknown);
}

void LabTritSetSpace::TritSet::shrink()
{
	if (changed)
		this->realloc(lastSetTrit + 1); 
	else
		this->realloc(initLenght);
}

void LabTritSetSpace::TritSet::trim(const size_t lastSetIndex)
{
	if (lastSetIndex >= lastSetTrit)
		return;

	for (size_t i = lastSetIndex; i <= this->lastSetTrit; ++i)
		this->operator[](i) = Trit::Unknown;

	this->lastSetTrit = lastSetIndex;
}

Trit TritSet::operator[](const size_t index) const
{
	if (index >= lenght)
		return Trit::Unknown;

	size_t arrIndex = index / tritPerUint;
	size_t cellIndex = index % tritPerUint;
	return Trit( (arr[arrIndex] >> (cellIndex * tritBitsRequired)) & tritMask );
}

TritSet::TritReference TritSet::operator[](const size_t index)
{
	return TritSet::TritReference(*this, index);
}

size_t TritSet::getLenght() const
{
	return lenght;
}

size_t TritSet::capacity() const
{
	return size * uintSize;
}

size_t LabTritSetSpace::TritSet::getSize() const
{
	return size;
}

size_t LabTritSetSpace::TritSet::getLastSetTrit() const
{
	return lastSetTrit;
}

size_t LabTritSetSpace::TritSet::cardinality(const Trit value) const
{
	
	size_t counter = 0;

	for (size_t i = 0; i <= lastSetTrit; ++i)
		if (this->operator[](i) == value)
			counter++;

	if (value == Trit::Unknown)
		counter += lenght - lastSetTrit - 1;

	return counter;
}

std::unordered_map<Trit, size_t, std::hash<Trit>> LabTritSetSpace::TritSet::cardinality() const
{
	
	std::unordered_map<Trit, size_t, std::hash<Trit>> map;
	map[Trit::Unknown] = lenght - lastSetTrit - 1;

	for (size_t i = 0; i <= lastSetTrit; ++i)
	{
		const Trit curr = this->operator[](i);
		if (curr == Trit::False)
			map[Trit::False]++;
		else if (curr == Trit::True)
			map[Trit::True]++;
		else
			map[Trit::Unknown]++;
	}

	return std::unordered_map<Trit, size_t, std::hash<Trit>>(map);
}

TritSet TritSet::operator&(const TritSet & set)
{
	size_t rSize = set.getSize() > size ? set.getSize() : size;
	size_t rLenght = set.getLenght() > lenght ? set.getLenght() : lenght;
	uint * rArr = new uint[rSize];
	TritSet subSet(rArr, rLenght, rSize);
	
	for (size_t i = 0; i < rLenght; ++i)
		subSet[i] = this->operator[](i) & set[i];

	return TritSet(rArr, rLenght, rSize, false);
}

TritSet TritSet::operator|(const TritSet & set)
{
	size_t rSize = set.getSize() > size ? set.getSize() : size;
	size_t rLenght = set.getLenght() > lenght ? set.getLenght() : lenght;
	uint * rArr = new uint[rSize];
	TritSet subSet(rArr, rLenght, rSize);

	for (size_t i = 0; i < rLenght; ++i)
		subSet[i] = this->operator[](i) | set[i];

	return TritSet(rArr, rLenght, rSize, false);
}

TritSet TritSet::operator!()
{
	uint * rArr = new uint[size];
	TritSet subSet(rArr, lenght, size);

	for (size_t i = 0; i < lenght; ++i)
		subSet[i] = !this->operator[](i);

	return TritSet(rArr, lenght, size, false);
}

std::ostream & LabTritSetSpace::operator<<(std::ostream & out, const Trit & tr)
{
	out << tritState(tr);
	return out;
}

std::string LabTritSetSpace::tritState(const Trit & tr)
{
	if (tr == Trit::Unknown)
		return "U";
	if (tr == Trit::False)
		return "F";
	return "T";
}

std::ostream & LabTritSetSpace::operator<<(std::ostream & out, const TritSet::TritReference & tr)
{
	out << std::string(tr);
	return out;
}

std::ostream & LabTritSetSpace::operator<<(std::ostream & out, const TritSet & ts)
{
	size_t len = ts.getLenght();
	out << "[";
	for (size_t i = 0; i < len; ++i)
		out << ts[i] << (i == len - 1 ? "" : ", ");
	out << "]";
	return out;
}
