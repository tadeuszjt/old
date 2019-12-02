/* set function pointer to controller and function will be run periodically
   inside object thread
*/ 

#ifndef _PERIODICFUNCTION_H
#define _PERIODICFUNCTION_H

#include "mbed.h"

class Periodic_function {
public:
    Periodic_function(void (*_funcp)()) :
    ticker(),
    funcp(_funcp) {
        interval = 10.0;
        
        /* start thread */
        t.start(callback(this, &Periodic_function::loop));
    }
    
    void start() {
        ticker.attach(callback(this, &Periodic_function::call), interval);
    }
    
    void set_interval(float T) {
        interval = T;
        ticker.attach(callback(this, &Periodic_function::call), interval);
    }
    
    void stop() {
        ticker.detach();
    }
    
    
private:
    float interval;
    Thread t;
    Ticker ticker;
    void (*funcp)();
    
    void call() {
        t.signal_set(1);
    }
    
    void loop() { 
        while (1) {
            Thread::signal_wait(1);
            funcp();
        }
    }
};

#endif