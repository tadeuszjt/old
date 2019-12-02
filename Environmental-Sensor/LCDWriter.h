#ifndef __LCDWRITER_H
#define __LCDWRITER_H

#include "mbed.h"

class LCDWriter {
public:
    LCDWriter(PinName RS,PinName E,PinName D4,PinName D5,PinName D6,PinName D7):
    rs(RS), en(E), db(D4, D5, D6, D7) {
        /* wait for initialise */
        wait_ms(3);
        
        /* assure 8-bit mode */
        command4bit(0x3);
        command4bit(0x3);
        command4bit(0x3);
        
        command4bit(0x2);   // 4-bit mode
        command(0x28);      // 16x2 lines and 5x8 font
        command(0x06);      // writedir right and no shift
        command(0x0C);      // display on no cursor 
        command(0x01);      // clear 
        command(0x02);      // home
    }
    
    void puts(char *str) {
        rs.write(1);
        while (*str != '\0') writeByte(*str++);
    }
    
    void putc(char c) {
        rs.write(1);
        writeByte(c);
    }
    
    void setPos(int row, int column) {
        if (row == 0) command(0x80 + column);
        else if (row == 1) command(0x80 + 64 + column);
    }
    
    void clear() {
        command(0x1);
    }
    
private:
    DigitalOut rs;
    DigitalOut en;
    BusOut db;
    Mutex mutex;

    void command(char data) {
        rs.write(0);
        writeByte(data);
        if (data < 0x4) wait_us(1600); // Clear & Home take longer
    }
    
    void command4bit(char data) {
        data &= 0xF;
        rs.write(0);
        db.write(data);
        en.write(1);
        wait_us(1);
        en.write(0);
        
        wait_us(40);
        if (data < 0x4) wait_us(1600); // Clear & Home take longer
    }

    void writeByte(char data) {
        /* first chunk */
        db.write(data >> 4);
        en.write(1);
        wait_us(1);
        en.write(0);
        wait_us(1);
        
        /* second chunk */
        db.write(data & 0x0F);
        en.write(1);
        wait_us(1);
        en.write(0);
        
        /* wait for lcd */
        wait_us(40);
    }
};

#endif