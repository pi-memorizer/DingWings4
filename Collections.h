#ifndef COLLECTIONS_H
#define COLLECTIONS_H
#include <string>
using namespace std;

//This file holds implementations for generic Stacks, Lists, and Hashmaps
//Pretty self explanatory

//If a non-string or integer type is used for the key in a Hashmap,
//it needs to inherit from the Hashable class

template<class T> class Stack {
	int top;
	T *stack = nullptr;
	int size;

public:
	Stack();
	~Stack();
	Stack(Stack &other);
	Stack& operator=(Stack &other);
	Stack(Stack &&other);
	Stack& operator=(Stack &&other);
	T pop();
	void push(T val);
	T peek();
	unsigned int length();
};

template<class T> Stack<T>::Stack()
{
	size = 10;
	stack = new T[size];
	top = 0;
}

template<class T> Stack<T>::~Stack()
{
	delete[] stack;
}

template<class T> Stack<T>::Stack(Stack &other)
{
	top = other.top;
	size = other.size;
	stack = new T[size];
	for (int i = 0; i < size; i++)
	{
		stack[i] = other.stack[i];
	}
}

template<class T> Stack<T>& Stack<T>::operator=(Stack &other)
{
	delete[] stack;
	top = other.top;
	size = other.size;
	stack = new T[size];
	for (int i = 0; i < size; i++)
	{
		stack[i] = other.stack[i];
	}
	return *this;
}

template<class T> Stack<T>::Stack(Stack &&other)
{
	top = other.top;
	size = other.size;
	stack = other.stack;
	other.stack = nullptr;
}

template<class T> Stack<T>& Stack<T>::operator=(Stack &&other)
{
	delete[] stack;
	top = other.top;
	size = other.size;
	stack = other.stack;
	other.stack = nullptr;
	return *this;
}

template<class T> T Stack<T>::pop()
{
	if (top == 0) return 0;
	top--;
	T val = stack[top];
	return val;
}

template<class T> void Stack<T>::push(T val)
{
	if (size == top)
	{
		T *buffer = new T[size * 2];
		for (int i = 0; i < size; i++)
		{
			buffer[i] = stack[i];
		}
		delete[] stack;
		size *= 2;
		stack = buffer;
	}
	stack[top] = val;
	top++;
}

template<class T> T Stack<T>::peek()
{
	if (top == 0) return 0;
	return stack[top - 1];
}

template<class T> unsigned int Stack<T>::length()
{
	return top;
}

template<class T> class List
{
	int top;
	T *back = nullptr;
	int size;

public:
	List();
	List(int length);
	~List();
	List(List &&other);
	List(List &other);
	List& operator=(List &other);
	List& operator=(List &&other);
	unsigned int length();
	T& operator[](int index);
	const T& operator[](int index) const;
	void add(T val);
	T remove(T value);
	T removeAt(int index);
};

template<class T> List<T>::List()
{
	top = 0;
	back = new T[10];
	size = 10;
}

template<class T> List<T>::List(int length) : size(length), back(new T[length]), top(0) {}

template<class T> List<T>::~List()
{
	delete[] back;
}

template<class T> List<T>::List(List<T> &&other)
{
	size = other.size;
	back = other.back;
	other.back = nullptr;
	top = other.top;
}

template<class T> List<T>::List(List<T> &other)
{
	size = other.size;
	back = new T[size];
	top = other.top;
	for (int i = 0; i < size; i++)
	{
		back[i] = other.back[i];
	}
}

template<class T> List<T>& List<T>::operator=(List<T> &other)
{
	delete[] back;
	size = other.size;
	back = new T[size];
	top = other.top;
	for (int i = 0; i < size; i++)
	{
		back[i] = other.back[i];
	}
	return *this;
}

template<class T> List<T>& List<T>::operator=(List<T> &&other)
{
	delete[] back;
	size = other.size;
	back = other.back;
	other.back = nullptr;
	top = other.top;
	return *this;
}

template<class T> unsigned int List<T>::length()
{
	return top;
}

template<class T> T& List<T>::operator[](int index)
{
	return back[index];
}

template<class T> const T& List<T>::operator[](int index) const
{
	return back[index];
}

template<class T> void List<T>::add(T val)
{
	if (top == size)
	{
		T *buffer = new T[size * 2];
		for (int i = 0; i < size; i++)
		{
			buffer[i] = back[i];
		}
		delete[] back;
		size *= 2;
		back = buffer;
	}
	back[top] = val;
	top++;
}

template<class T> T List<T>::remove(T value)
{
	int index = -1;

	for (int i = 0; i < top; i++)
	{
		if (value == back[i])
		{
			index = i;
			break;
		}
	}
	if (index == -1)
		return 0;
	else
		return removeAt(index);
}

template<class T> T List<T>::removeAt(int index)
{
	T val = back[index];
	for (int i = index; i < top - 1; i++)
	{
		back[i] = back[i + 1];
	}
	top--;
	return val;
}

namespace Hashing
{
	class Hashable
	{
	public:
		virtual int hashCode() = 0;
	};

	template<class T> int hashCode(T value);

	template<> int hashCode<int>(int value);

	template<> int hashCode<Hashable*>(Hashable *h);

	template<> int hashCode<string>(string s);
};

template<class T, class V> class Hashmap
{
	class Tuple
	{
	public:
		T key;
		V value;

		Tuple(T k, V v)
		{
			key = k;
			value = v;
		}
	};
	Tuple** back;
	int size;
	int count;
	const double FILL_FACTOR = .75;
	Tuple *empty = new Tuple(T(), V());

	int hashCode(T value)
	{
		return Hashing::hashCode(value);
	}

	inline int abs(int a)
	{
		return (a < 0) ? -a : a;
	}
public:
	Hashmap();
	~Hashmap();
	Hashmap(Hashmap &other);
	Hashmap(Hashmap &&other);
	Hashmap& operator=(Hashmap &other);
	Hashmap& operator=(Hashmap &&other);
	unsigned int length();
	void add(T key, V value);
	V& operator[](T index);
	const V& operator[](T index) const;
	List<V> values();
	List<T> keys();
	void remove(T key);
	bool contains(T key);
};

template<class T, class V> Hashmap<T,V>::Hashmap()
{
	size = 31;
	count = 0;
	back = new Tuple*[size];
	for (int i = 0; i < size; i++)
	{
		back[i] = nullptr;
	}
}

template<class T, class V> Hashmap<T,V>::~Hashmap()
{
	for (int i = 0; i < size; i++)
	{
		delete back[i];
	}
	delete[] back;
	delete empty;
}

template<class T, class V> Hashmap<T,V>::Hashmap(Hashmap<T,V> &other)
{
	size = other.size;
	count = other.count;
	back = new Tuple*[size];
	for (int i = 0; i < size; i++)
	{
		back[i] = new Tuple();
		back[i]->key = other.back[i]->key;
		back[i]->value = other.back[i]->value;
	}
}

template<class T, class V> Hashmap<T,V>::Hashmap(Hashmap<T,V> &&other)
{
	size = other.size;
	count = other.count;
	back = other.back;
	other.back = nullptr;
}

template<class T, class V> Hashmap<T,V>& Hashmap<T,V>::operator=(Hashmap<T,V> &other)
{
	for (int i = 0; i < size; i++)
	{
		delete back[i];
	}
	delete[] back;
	size = other.size;
	count = other.count;
	back = new Tuple*[size];
	for (int i = 0; i < size; i++)
	{
		back[i] = new Tuple();
		back[i]->key = other.back[i]->key;
		back[i]->value = other.back[i]->value;
	}
	return *this;
}

template<class T, class V> Hashmap<T,V>& Hashmap<T,V>::operator=(Hashmap<T,V> &&other)
{
	for (int i = 0; i < size; i++)
	{
		delete back[i];
	}
	delete[] back;
	size = other.size;
	count = other.count;
	back = other.back;
	other.back = nullptr;
	return *this;
}

template<class T, class V> unsigned int Hashmap<T,V>::length()
{
	return count;
}

template<class T, class V> void Hashmap<T,V>::add(T key, V value)
{
	if (count >= FILL_FACTOR*size)
	{
		Tuple **buffer = new Tuple*[size * 2+1];
		for (int i = 0; i < size * 2+1; i++)
		{
			buffer[i] = nullptr;
		}
		for (int i = 0; i < size; i++)
		{
			Tuple *t = back[i];
			if (t != nullptr&&t != empty)
			{
				int index = abs(hashCode(t->key)) % (2 * size+1);
				for (int i = index; true; i = (i + 1) % (2 * size+1))
				{
					if (buffer[i] == empty || buffer[i] == nullptr)
					{
						buffer[i] = t;
						break;
					}
				}
			}
		}
		delete[] back;
		size = size*2+1;
		back = buffer;
	}
	int index = abs(hashCode(key)) % size;
	for (int i = index; true; i = (i + 1) % size)
	{
		if (back[i] == empty || back[i] == nullptr)
		{
			back[i] = new Tuple(key, value);
			break;
		}
	}
	count++;
}

template<class T, class V> V& Hashmap<T,V>::operator[](T index)
{
	int i = abs(hashCode(index)) % size;
	Tuple *t = back[i];
	while (t != nullptr)
	{
		if (index == t->key)
		{
			return t->value;
		}
		i = (i + 1) % size;
		t = back[i];
	}
	return empty->value;
}

template<class T, class V> const V& Hashmap<T,V>::operator[](T index) const
{
	return this[index];
}

template<class T, class V> List<V> Hashmap<T,V>::values()
{
	List<V> list;
	for (int i = 0; i < size; i++)
	{
		Tuple *t = back[i];
		if (t != nullptr&&t != empty)
		{
			list.add(t->value);
		}
	}
	return list;
}

template<class T, class V> List<T> Hashmap<T,V>::keys()
{
	List<T> list;
	for (int i = 0; i < size; i++)
	{
		Tuple *t = back[i];
		if (t != nullptr&&t != empty)
		{
			list.add(t->key);
		}
	}
	return list;
}

template<class T, class V> void Hashmap<T,V>::remove(T key)
{
	int i = abs(hashCode(key)) % size;
	Tuple *t = back[i];
	while (t != nullptr&&t->key != key)
	{
		i = (i + 1) % size;
		t = back[i];
	}
	if (t == nullptr) return;
	if (t->key == key)
	{
		delete t;
		back[i] = empty;
	}
}

template<class T, class V> bool Hashmap<T, V>::contains(T key)
{
	int i = abs(hashCode(key)) % size;
	Tuple *t = back[i];
	while (t != nullptr&&t->key != key)
	{
		i = (i + 1) % size;
		t = back[i];
	}
	if (t == nullptr) return false;
	if (t->key == key)
	{
		return true;
	}
	return false;
}
#endif