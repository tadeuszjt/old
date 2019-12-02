/* A thread-safe sampler of the sensor arrray */

#ifndef __ENVSENSOR_H
#define __ENVSENSOR_H

#include "mbed.h"
#include "BMP280.h"
#include "Time_controller.h"

typedef struct {
    time_t time;
    float temp;
    float press;
    float light;
} sample;


class Env_sensor {
public:
    Env_sensor(BMP280 &_bmp, AnalogIn &_lgt, Mutex &_sensor_mutex, Time_controller &_tim) :
    bmp(_bmp),
    lgt(_lgt),
    sensor_mutex(_sensor_mutex),
    tim(_tim) {
    }
    
    sample getSample() {
        sample spl;
        spl.time = tim.currentTime();
        
        /* get sample from sensors */
        sensor_mutex.lock();
        spl.temp  = bmp.getTemperature();
        spl.press = bmp.getPressure();
        spl.light = lgt.read();
        sensor_mutex.unlock();
        
        return spl;
    }
    
private:
    BMP280 &bmp;
    AnalogIn &lgt;
    Mutex &sensor_mutex;
    Time_controller &tim;
};

#endif