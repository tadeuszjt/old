#ifndef GROUP_HPP
#define GROUP_HPP

#include <cstring>

template <typename T>
class Grouper;


template <typename T>
class Group {
public:
	int cap;
	int count;
	T  *items;
	Grouper<T> **gpers;
	
	Group();
	void resizeMem  (int capacity);
	T&   operator[] (int i);
	~Group();
};


template <typename T>
class Grouper {
public:
	void *owner = nullptr;
	Group<T> *group = nullptr;
	int index;
	T itemcpy;

	Grouper(T item);
	void setItem(T item);
	void setGroup(Group<T> &_group);
	void setSameGroup(Grouper<T> &_grouper);
	void unGroup();
	~Grouper();
};




template <typename T>
Group<T>::Group() {
	cap = 16;
	count = 0;
	items = new T [16];
	gpers = new Grouper<T>* [16];
}

template <typename T>
void
Group<T>::resizeMem(int capacity) {
	T           *itemscpy = new T           [capacity];
	Grouper<T> **gperscpy = new Grouper<T>* [capacity];

	int amt = capacity > cap ? cap : capacity; 
	memcpy(itemscpy, items, amt*sizeof(T));
	memcpy(gperscpy, gpers, amt*sizeof(Grouper<T>*));
	
	delete [] items;
	delete [] gpers;
		
	items = itemscpy;
	gpers = gperscpy;
	cap   = capacity;
}

template <typename T>
T&
Group<T>::operator [] (int i) {
	return items[i];
}

template <typename T>
Group<T>::~Group() {
	for (int i=0; i<count; i++)
		gpers[i]->group = nullptr;
		
	delete [] items;
	delete [] gpers;
}






template <typename T>
Grouper<T>::Grouper(T item) {
	setItem(item);
}

template <typename T>
void
Grouper<T>::setItem(T item) {
	itemcpy = item;
	if (group) group->items[index] = item;
}

template <typename T>
void
Grouper<T>::setGroup(Group<T> &_group) {
	unGroup();
	
	group = &_group;
	index = group->count;
	
	if (index >= group->cap) // resize
		group->resizeMem(2*group->cap);
	
	group->items[index] = itemcpy;
	group->gpers[index] = this;
	group->count++;
}

template <typename T>
void
Grouper<T>::setSameGroup(Grouper<T> &_grouper) {
	unGroup();
	if (_grouper.group)
		setGroup(*_grouper.group);
}

template <typename T>
void
Grouper<T>::unGroup() {
	if (group) {
		group->count--;
		
		if (group->count > index) { // switcheroo
			group->items[index] = group->items[group->count];
			group->gpers[index] = group->gpers[group->count];
			group->gpers[index]->index = index; 
		}
		
		group = nullptr;
	}
}

template <typename T>
Grouper<T>::~Grouper() {
	unGroup();
}


#endif
