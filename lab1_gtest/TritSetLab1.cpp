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

Trit TritSet::TritReference::operator&(const Trit & trit) const
{
	return Trit(*this) & trit;
}

Trit TritSet::TritReference::operator&(const TritReference & tr) const
{
	return this->operator&(tr());
}

Trit TritSet::TritReference::operator|(const Trit & trit) const
{
	return Trit(*this) | trit;
}

Trit TritSet::TritReference::operator|(const TritReference & tr) const
{
	return this->operator|(tr());
}

Trit TritSet::TritReference::operator!() const
{
	return !Trit(*this);
}

TritSet::TritReference & LabTritSetSpace::TritSet::TritReference::operator++()
{
	index++;
	return *this;
}

TritSet::TritReference LabTritSetSpace::TritSet::TritReference::operator++(int)
{
	TritSet::TritReference temp = *this;
	++*this;
	return temp;
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
	size_t prevLen = this->length;
	uint * prevArr = this->arr;
	
	this->length = newLenght;
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

	if (index >= length)
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
	length = 0;
	initLength = 0;
	size = 0;
	arr = NULL;
	lastSetTrit = -1;
}

TritSet::TritSet(const size_t N)
{
	length = N;
	initLength = N;
	size = (N + tritPerUint - 1) / tritPerUint; // вычисление кол-ва uint'ов, необходимого для N trit'ов
	arr = new uint[size];
	lastSetTrit = -1;
}

LabTritSetSpace::TritSet::TritSet(const TritSet & set)
{
	this->length = set.getLength();
	this->initLength = set.getInitLength();
	this->size = set.getSize();
	this->lastSetTrit = set.getLastSetTrit();

	this->arr = new uint[this->length];
	memcpy(arr, set.getArr(), size * uintSize);
}

LabTritSetSpace::TritSet::TritSet(TritSet && set)
{
	// initialing with constructor TritSet(TritSet&) allocates array
	*this = static_cast<TritSet&&>(set);
}

LabTritSetSpace::TritSet::~TritSet()
{
	delete arr;
}

void LabTritSetSpace::TritSet::shrink()
{
	if (lastSetTrit == -1)
		expand(initLength);
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

size_t TritSet::getLength() const
{
	return length;
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

size_t LabTritSetSpace::TritSet::getInitLength() const
{
	return initLength;
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
		counter += length - lastSetTrit - 1;

	return counter;
}

std::unordered_map<Trit, size_t, std::hash<Trit>> LabTritSetSpace::TritSet::cardinality() const
{
	
	std::unordered_map<Trit, size_t, std::hash<Trit>> map;
	map[Trit::Unknown] = length - lastSetTrit - 1;

	if(lastSetTrit != -1)
	for (size_t i = 0; i <= lastSetTrit; ++i)
	{
		const Trit curr = this->operator[](i);
		map[curr]++;
	}

	return std::unordered_map<Trit, size_t, std::hash<Trit>>(map);
}

TritSet LabTritSetSpace::TritSet::operator&(const TritSet & set) const
{
	const size_t len = set.getLength() > this->length ? set.getLength() : this->length;
	TritSet result(len);

	for (size_t i = 0; i < len; ++i)
		result[i] = this->operator[](i) & set[i];

	return TritSet(result);
}

TritSet LabTritSetSpace::TritSet::operator|(const TritSet & set) const
{
	const size_t len = set.getLength() > this->length ? set.getLength() : this->length;
	TritSet result(len);

	for (size_t i = 0; i < len; ++i)
		result[i] = this->operator[](i) | set[i];

	return TritSet(result);
}

TritSet LabTritSetSpace::TritSet::operator!() const
{
	TritSet result(this->length);

	for (size_t i = 0; i < this->length; ++i)
		result[i] = !this->operator[](i);

	return TritSet(result);
}

TritSet & LabTritSetSpace::TritSet::operator=(TritSet && set)
{
	if (this == &set)
		return *this;

	delete arr;
	length = set.length;
	initLength = set.initLength;
	size = set.size;
	lastSetTrit = set.lastSetTrit;
	arr = set.arr;

	set.length = 0;
	set.initLength = 0;
	set.size = 0;
	set.lastSetTrit = 0;
	set.arr = nullptr;
}

TritSet::iterator LabTritSetSpace::TritSet::begin()
{
	return TritSet::iterator(TritSet::TritReference(*this, 0));
}

TritSet::iterator LabTritSetSpace::TritSet::end()
{
	return TritSet::iterator(TritSet::TritReference(*this, length));
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
	size_t len = ts.getLength();
	out << "[";
	for (size_t i = 0; i < len; ++i)
		out << ts[i] << (i == len - 1 ? "" : ", ");
	out << "]";
	return out;
}
