#include "Collections.h"

template<class T> int Hashing::hashCode(T value)
{
	throw "Not hashable";
}

template<> int Hashing::hashCode<int>(int value)
{
	return value;
}

template<> int Hashing::hashCode<Hashing::Hashable*>(Hashing::Hashable *h)
{
	return h->hashCode();
}

template<> int Hashing::hashCode<string>(string s)
{
	int code = 0;
	for (int i = 0; i < s.length(); i++) {
		code = 31 * code + s[i];
	}
	return code;
}