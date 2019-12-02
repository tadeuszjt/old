#ifndef __CBUFFER_H
#define __CBUFFER_H

template<class T> class CBuffer {    
public:
    const int num_elems;
    int len;
    int pos;
    T *array;

    CBuffer(int _num_elems) : num_elems(_num_elems) {
        array = new T [_num_elems];
        len = 0;
        pos = 0;
    };
    
    ~CBuffer() {
        delete [] array;
    }
    
    void put(T item) {
        array[pos++] = item;
        if (pos>=num_elems) pos = 0;
        if (len< num_elems) len++;
    }
    
    T getFront(int i) {
        T item;
        if (i<0 || i>=len) return item;
        i = pos-1 - i;
        if (i<0) i += num_elems;
        return array[i];    
    }
    
    int clearBack(int N) {
        if (N<=0) return 0;
        if (N>len) N = len;
        len -= N;
        return N;
    }
    
private:
};

#endif