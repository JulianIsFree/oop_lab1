#include "pch.h"

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
	set.setTrit(this->index, trit);
	return *this;
}

TritSet::TritReference & TritSet::TritReference::operator=(const TritReference & tr)
{
	return this->operator=(Trit(tr));;
}

const Trit TritSet::TritReference::operator&(const Trit & trit) const
{
	return Trit(*this) & trit;
}

const Trit TritSet::TritReference::operator&(const TritReference & tr) const
{
	return this->operator&(tr());
}

const Trit TritSet::TritReference::operator|(const Trit & trit) const
{
	return Trit(*this) | trit;
}

const Trit TritSet::TritReference::operator|(const TritReference & tr) const
{
	return this->operator|(tr());
}

const Trit TritSet::TritReference::operator!() const
{
	return !Trit(*this);
}

Trit TritSet::TritReference::operator()() const
{
	return static_cast<const TritSet>(set)[index]; 
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

void TritSet::expand(const size_t newLenght)
{
	size_t prevSize = this->size;
	size_t prevLen = this->lenght;
	uint * prevArr = this->arr;
	
	this->lenght = newLenght;
	this->size = (newLenght + tritPerUint - 1) / tritPerUint;
	this->arr = new uint[size];
	size_t copySize = size > prevSize ? prevSize : size;
	

	memcpy(this->arr, prevArr,
		copySize * uintSize);

	for (size_t i = prevLen; i < newLenght; ++i)
		setTrit(i, Trit::Unknown);

	delete prevArr;
}

inline void TritSet::setTrit(const size_t index, const Trit trit)
{
	if ((index > lastSetTrit || lastSetTrit == -1) && trit == Trit::Unknown)
		return;

	if (index >= lenght)
		expand(index + 1);
	
	if (index > lastSetTrit || lastSetTrit == -1) 
	{
		size_t start = lastSetTrit + 1;
		lastSetTrit = index;

		for (size_t i = start; i < index; ++i)
			setTrit(i, Trit::Unknown);
	}
		
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
	lastSetTrit = -1;
}

TritSet::TritSet(const size_t N)
{
	lenght = N;
	initLenght = N;
	size = (N + tritPerUint - 1) / tritPerUint; // вычисление кол-ва uint'ов, необходимого для N trit'ов
	arr = new uint[size];
	lastSetTrit = -1;
}

LabTritSetSpace::TritSet::TritSet(TritSet & set)
{
	this->lenght = set.getLenght();
	this->initLenght = set.getInitLenght();
	this->size = set.getSize();
	this->lastSetTrit = set.getLastSetTrit();

	this->arr = new uint[this->lenght];
	memcpy(arr, set.getArr(), size * uintSize);
}

void LabTritSetSpace::TritSet::shrink()
{
	if (lastSetTrit == -1)
		expand(initLenght);
	else
		expand(lastSetTrit + 1);
}

void LabTritSetSpace::TritSet::trim(const size_t lastSetIndex)
{
	lastSetTrit = lastSetIndex - 1;
}

Trit TritSet::operator[](const size_t index) const
{
	if (index > lastSetTrit || lastSetTrit == -1)
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

size_t LabTritSetSpace::TritSet::getInitLenght() const
{
	return initLenght;
}

const uint * LabTritSetSpace::TritSet::getArr() const
{
	return arr;
}

size_t LabTritSetSpace::TritSet::cardinality(const Trit value) const
{
	size_t counter = 0;

	if(lastSetTrit != -1)
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

	if(lastSetTrit != -1)
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

TritSet LabTritSetSpace::TritSet::operator&(const TritSet & set) const
{
	const size_t len = set.getLenght() > this->lenght ? set.getLenght() : this->lenght;
	TritSet result(len);

	for (size_t i = 0; i < len; ++i)
		result[i] = this->operator[](i) & set[i];

	return TritSet(result);
}

TritSet LabTritSetSpace::TritSet::operator|(const TritSet & set) const
{
	const size_t len = set.getLenght() > this->lenght ? set.getLenght() : this->lenght;
	TritSet result(len);

	for (size_t i = 0; i < len; ++i)
		result[i] = this->operator[](i) | set[i];

	return TritSet(result);
}

TritSet LabTritSetSpace::TritSet::operator!() const
{
	TritSet result(this->lenght);

	for (size_t i = 0; i < this->lenght; ++i)
		result[i] = !this->operator[](i);

	return TritSet(result);
}

Trit LabTritSetSpace::operator&(const Trit & tr1, const Trit & tr2)
{
	if (tr1 == Trit::False || tr2 == Trit::False)
		return Trit::False;

	if (tr1 == Trit::Unknown || tr2 == Trit::Unknown)
		return Trit::Unknown;

	return Trit::True;
}

Trit LabTritSetSpace::operator|(const Trit & tr1, const Trit & tr2)
{
	if (tr1 == Trit::True || tr2 == Trit::True)
		return Trit::True;

	if (tr1 == Trit::Unknown || tr2 == Trit::Unknown)
		return Trit::Unknown;

	return Trit::False;
}

Trit LabTritSetSpace::operator!(const Trit & tr1)
{
	if (tr1 == Trit::True)
		return Trit::False;

	if (tr1 == Trit::False)
		return Trit::True;
	
	return Trit::Unknown;
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
