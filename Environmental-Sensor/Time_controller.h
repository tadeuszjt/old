#ifndef _TIMECONTROLLER_H
#define _TIMECONTROLLER_H

#include "mbed.h"

class Time_controller {
public:
    Time_controller(Mutex &_time_mutex) :
    time_mutex(_time_mutex) {
    }
    
    int setTime(int h, int m, int s) {
        /* invalid time */
        if (
            h<0 || h>23 ||
            m<0 || m>59 ||
            s<0 || s>59
        ) return -1;
        
        time_mutex.lock();
        
        time_t rawtime;
        time(&rawtime);
        struct tm *tmptr;
        tmptr = localtime(&rawtime);
        tmptr->tm_hour = h;
        tmptr->tm_min  = m;
        tmptr->tm_sec  = s;
        rawtime = mktime(tmptr);
        
        set_time(rawtime);
        
        time_mutex.unlock();
        return 0;
    }
    
    int setDate(int d, int m, int y) {
        time_mutex.lock(); 
                
        time_t rawtime;
        time(&rawtime);
        struct tm *tmptr;
        tmptr = localtime(&rawtime);
        tmptr->tm_mday = d;
        tmptr->tm_mon  = m - 1;
        tmptr->tm_year = y - 1900;
        rawtime = mktime(tmptr);
        
        /* invalid date */
        if (rawtime == 0) return -1;
        
        set_time(rawtime);
        
        time_mutex.unlock();
        return 0;
    }
    
    time_t currentTime() {
        time_t t;
        time_mutex.lock();
        time(&t);
        time_mutex.unlock();
        return t;
    }
    
    void sprintTime(char *str, time_t tim) {
        time_mutex.lock();
        strftime(str, 8, "%T", localtime(&tim));
        time_mutex.unlock();
    }
    
    void sprintDate(char *str, time_t tim) {
        time_mutex.lock();
        strftime(str, 8, "%d/%m/%y", localtime(&tim));
        time_mutex.unlock();
    }
    
private:
    Mutex &time_mutex;
};

#endif