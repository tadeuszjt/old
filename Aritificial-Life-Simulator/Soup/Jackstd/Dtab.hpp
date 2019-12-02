#ifndef DTAB_HPP
#define DTAB_HPP

#include <cstring>

template <typename T>
class Dtab {
	void resize(int newcap);	
public:
	int cap;
	int count;
	T  *array;

	Dtab();
	~Dtab();

	void add   (T item);
	void extend(int num); 
	void remove(int i);
	bool remove(T item);
	void clear ();
	
	T& operator [] (int i) const;
};



template <typename T>
void
Dtab<T>::resize(int newcap) {
	T *arrcpy = new T [newcap];
	int cpy   = newcap > cap ? cap : newcap;
	memcpy(arrcpy, array, cpy*sizeof(T));
	
	delete [] array;
	array  = arrcpy;
	cap    = newcap;
}

template <typename T>
Dtab<T>::Dtab() {
	cap = 32; // starting capacity
	count = 0;
	array = new T [cap];
}

template <typename T>
Dtab<T>::~Dtab() {
	delete [] array;
}

template <typename T>
void
Dtab<T>::add(T item) {
	if (count >= cap) resize(2*cap); 
	array[count++] = item;
}

template <typename T>
void
Dtab<T>::extend(int num) {
	count += num;
	if (count > cap) resize(2*count);
}

template <typename T> 
void
Dtab<T>::remove(int i) {
	array[i] = array[--count];
}

template <typename T>
bool
Dtab<T>::remove(T item) {
	for (int i=0; i < count; i++) {
		if (array[i] == item) {
			array[i] = array[--count];
			return true;
		}
	}
	return false;
}

template <typename T>
void
Dtab<T>::clear() {
	count = 0;
}


template <typename T>
T&
Dtab<T>::operator [] (int i) const {
	return array[i];
}

#endif
