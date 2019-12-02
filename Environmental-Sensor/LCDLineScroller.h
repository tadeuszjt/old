#ifndef __LCDLINESCROLLER_H
#define __LCDLINESCROLLER_H

#include "mbed.h"
#include "LCDWriter.h"

class LCDLineScroller {
public:
    LCDLineScroller(
        LCDWriter& _lcd,
        Mutex& _lcd_mutex,
        const int _line_max,
        const int _row
    ) :
    lcd(_lcd), lcd_mutex(_lcd_mutex), line_max(_line_max), row(_row) {
        t.start(callback(this, &LCDLineScroller::loop));
        buf = new char [_line_max+1];
        buf[line_max] = '\0';
    }
    
    ~LCDLineScroller() {
        delete [] buf;
    }

    void puts(bool scroll_text, char *str) {
        /* set thread info */
        mutex.lock();
        strncpy(buf, str, line_max);
        scroll = scroll_text;
        reset  = true;
        mutex.unlock();
        
        t.signal_set(1);
    }
    
    void printf(bool scroll_text, char *fmt, ...) {
        char str[line_max];
        va_list args;
        va_start (args, fmt);
        vsnprintf(str, line_max, fmt, args);
        puts(scroll_text, str);
    }
    
    bool stillScrolling() {
        return scrolling;
    }

private:
    LCDWriter& lcd;
    Mutex& lcd_mutex;
    const int line_max;
    const int row;
    
    /* thread info */
    Thread t;
    Mutex mutex;
    char *buf;
    bool reset;
    bool scroll;
    bool scrolling;

    void loop() {
        /* info copy */
        char tbuf[line_max+1];
        int  len;
        bool scr;
        int  scrpos;
        
        while(1) {
            /* wait condition */
            if (scr) Thread::signal_wait(1, 280);
            else     Thread::signal_wait(1);
            
            /* copy info */
            mutex.lock();
            if (reset) {
                reset   = false;
                scr     = scroll;
                scrolling = scroll;
                scrpos  = -16; 
                memcpy(tbuf, buf, sizeof(tbuf));
                len = strlen(buf);
            }
            mutex.unlock();
            
            /* display string */
            lcd_mutex.lock();
            if (scr) {
                /* terminate scrolling */
                if (scrpos > len) {
                    scr = false;
                    scrolling = false;
                }
                
                else {
                    /* scrolling text */
                    lcd.setPos(row, 0);
                    int s = scrpos++;
                    for (int i=0; i<16; i++, s++) {
                        if (s>=0 && s<len)  lcd.putc(buf[s]);
                        else                lcd.putc(' ');
                    }
                }
            }
            
            else {
                /* static text */
                lcd.setPos(row, 0);
                int i = 0;
                while (i<16 && tbuf[i] != '\0') lcd.putc(tbuf[i++]);
                while (i++<16)                  lcd.putc(' ');
            }
            lcd_mutex.unlock();
        }
    }
};

#endif