#include "mbed.h"
#include "LCDWriter.h"
#include "LCDLineScroller.h"
#include "BMP280.h"
#include "Env_sensor.h"
#include "CBuffer.h"
#include "Sample_buffer.h"
#include "Periodic_function.h"
#include "Time_controller.h"

#define LCDLINE_MAX 80
#define BUFFER_SIZE 120


/* ctime controller */
Mutex stdio_mutex;
Mutex time_mutex;
Time_controller timeController(time_mutex); // get time from here


/* LCD */
Mutex lcd_mutex;
LCDWriter lcd(D9, D8, D7, D6, D4, D2);
LCDLineScroller LCDline0(lcd, lcd_mutex, LCDLINE_MAX, 0); // print to LCD here
LCDLineScroller LCDline1(lcd, lcd_mutex, LCDLINE_MAX, 1); // and here


/* Environment Sensor */
Mutex sensor_mutex;
BMP280 bmp(D14, D15);
AnalogIn lgt(PA_0);
Env_sensor sensor(bmp, lgt, sensor_mutex, timeController); // Read sensors here


/* Sample Buffer */
Mutex buffer_mutex;
CBuffer<sample> circleBuffer(BUFFER_SIZE);
Sample_buffer Buffer(circleBuffer, timeController, buffer_mutex, stdio_mutex); // Read buffer here



/* periodic sampling controller */
void recordSample() {
    /* add new sample to buffer */
    sample spl = sensor.getSample();
    Buffer.addSample(spl);
    
    /* print sample on LCD */
    if (!LCDline0.stillScrolling()) {
        LCDline0.printf(true, "T:%3.2f%cC P:%4.1fmBar L:%1.4f",
            spl.temp,
            233,
            spl.press,
            spl.light
        );
    }
}
Periodic_function pSample(&recordSample); // set sampling interval here




/* commands */
void command(char *str) {
    const int CMD_NUM = 7;
    const char *cmdlist[] = {
        "READ",
        "DELETE",
        "SETDATE",
        "SETTIME",
        "SETT",
        "STATE",
        "LOGGING",
    };

    /* get words */
    char word[4][LCDLINE_MAX];
    sscanf(str,"%s %s %s %s",word[0],word[1],word[2],word[3]);

    /* find command value */
    int cmd = -1;
    for (int i=0; i<CMD_NUM; i++) {
        if (strcmp(word[0], cmdlist[i]) == 0) {
            cmd = i;
            break;
        }
    }

    /* execute command */
    switch (cmd) {
        case -1: {
            LCDline1.puts(false, "INVALID COMMAND");
            break;
        }

        /* READ */
        case 0: {
            int n = -1;
            if (strcmp("ALL", word[1])==0 || sscanf(word[1],"%d",&n)==1) {
                n = Buffer.print(n);
                LCDline1.printf(true, "READ %d RECORDS", n);
            } else LCDline1.puts(false, "INVALID ARGUMENT");
            break;
        }

        /* DELETE */
        case 1: {
            int n = -1;
            if (strcmp("ALL",word[1])==0 || sscanf(word[1],"%d",&n)==1) {
                n = Buffer.deleteOldest(n);
                LCDline1.printf(true, "DELETED %d RECORDS", n);
            } else LCDline1.puts(false, "INVALID ARGUMENT");
            break;
        }

        /* SETDATE */
        case 2: {
            int d, m, y;
            if (sscanf(str, "SETDATE %d %d %d", &d, &m, &y) == 3) {
                
                if (timeController.setDate(d, m, y) == 0) {
                    char buf[] = "        ";
                    timeController.sprintDate(buf, timeController.currentTime());
                    LCDline1.printf(true, "DATE UPDATED TO %s", buf);
                }
                else LCDline1.puts(false, "INVALID DATE!");
                
            } else LCDline1.puts(false, "INVALID ARGUMENT");
            break;
        }


        /* SETTIME */
        case 3: {
            int h, m, s;
            if (sscanf(str, "SETTIME %d %d %d", &h, &m, &s) == 3) {
                    
                if (timeController.setTime(h, m, s) == 0) {
                    char buf[] = "        ";
                    timeController.sprintTime(buf, timeController.currentTime());
                    LCDline1.printf(true, "TIME UPDATED TO %s", buf);
                }
                else LCDline1.puts(false, "INVALID TIME!");
            }
            else LCDline1.puts(false, "INVALID ARGUMENT");
            break;
        }


        /* SETT */
        case 4: {
            float T;
            if (sscanf(str, "SETT %f", &T) == 1) {
                /* out of range */
                if (T<0.1f || T>60.0f) {
                    LCDline1.printf(true, "T=%.3f OUT OF RANGE (0.1 - 60.0)", T);
                    break;
                }
                
                pSample.set_interval(T);
                
                LCDline1.printf(true,"T UPDATED TO %.3f", T);
            } else LCDline1.puts(false, "INVALID ARGUMENT");
            break;
        }

        /* STATE */
        case 5: {
            if (strcmp(word[1], "ON")==0) {
                pSample.start();
                LCDline1.puts(false, "SAMPLING ON");
            }

            else if (strcmp(word[1], "OFF")==0) {
                pSample.stop();
                LCDline1.puts(false, "SAMPLING OFF");
            } else LCDline1.puts(false, "INVALID ARGUMENT");
            break;
        }
    }
}


int main() {
    pSample.set_interval(15.0);
    pSample.start();
    
    LCDline0.printf(true, "Welcome to Environment Sensor");
    
    /* loop keyboard input */
    while (1) {
        char str[LCDLINE_MAX+1];
        /* get command */
        for (int i = 0;;) {
            int c = getchar();
            
            /* check end */
            if (c == 13 || c == EOF) {
                str[i] = '\0';
                LCDline1.puts(false, "");
                break;
            }
            
            else {
                /* check backspace */
                if (c==8 || c==127) {
                    if (i>0) str[--i] = '\0';
                } 
                
                /* check max line */
                else if (i<LCDLINE_MAX) {
                    str[i] = c;
                    str[++i] = '\0';
                }
                    
                /* display */
                if (i>16) LCDline1.puts(false, &str[i-16]);
                else      LCDline1.puts(false, str); 
            }
        }
        command(str);
    }
}
