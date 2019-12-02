#ifndef __SAMPLEBUFFER_H
#define __SAMPLEBUFFER_H

#include "mbed.h"
#include "CBuffer.h"
#include "Env_sensor.h"

class Sample_buffer {
public:
    Sample_buffer(
        CBuffer<sample> &_cbuf,
        Time_controller &_tim,
        Mutex &_buffer_mutex,
        Mutex &_stdio_mutex
    ) :
    cbuf(_cbuf),
    tim(_tim),
    buffer_mutex(_buffer_mutex),
    stdio_mutex(_stdio_mutex) {
    }
    
    void addSample(sample spl) {
        buffer_mutex.lock();
        cbuf.put(spl);
        buffer_mutex.unlock();
    }
    
    int deleteOldest(int N) {
        buffer_mutex.lock();
        if (N<0 || N>cbuf.len) N = cbuf.len; 
        N = cbuf.clearBack(N);
        buffer_mutex.unlock();
        
        return N;
    }
    
    int fprint(FILE *fp, int n) {
        /* save buffer values */
        buffer_mutex.lock();
        int pos = cbuf.pos;
        int len = cbuf.len;
        int num = cbuf.num_elems;
        buffer_mutex.unlock();
        
        if (n<0 || n>len)
            n = len;
        
        int i = 0;
        for (; i<n; i++) {
            if (--pos<0) pos += num;
            
            /* get ith newest sample */
            buffer_mutex.lock();
            sample spl = cbuf.array[pos];
            int bufpos = cbuf.pos;
            buffer_mutex.unlock();

            /* print time */
            char buf[] = "        ,        ";
            tim.sprintTime(buf, spl.time);
            tim.sprintDate(&buf[9], spl.time);
        
            /* use serial function */         
            stdio_mutex.lock();
            fprintf(fp, "%s,%3.2f,%4.1f,%1.4f\n",
                buf,
                spl.temp,
                spl.press,
                spl.light
            );
            stdio_mutex.unlock();
            
            /* break if reading newest */
            if (pos == bufpos) break;
        } 
        return i;
    }
    
    int print(int n) {
       return fprint(stdout, n);
    }
    
private:
    CBuffer<sample> &cbuf;
    Time_controller &tim;
    Mutex &buffer_mutex;
    Mutex &stdio_mutex;
};

#endif