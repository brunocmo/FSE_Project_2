
/*
*
* Converted to be fitted in C++ files by Bruno Nunes
*
*
* by Lewis Loflin www.bristolwatch.com lewis@bvu.net
* http://www.bristolwatch.com/rpi/i2clcd.htm
* Using wiringPi by Gordon Henderson
*
*
* Port over lcd_i2c.py to C and added improvements.
* Supports 16x2 and 20x4 screens.
* This was to learn now the I2C lcd displays operate.
* There is no warrenty of any kind use at your own risk.
* 
*/

#ifndef SHOWINFOLCD_HPP
#define SHOWINFOLCD_HPP

#include "wiringPiI2C.h"
#include "wiringPi.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

class ShowInfoLCD {

    private:
        // Define some device parameters
        const unsigned char I2C_ADDR = 0x27; // I2C device address
        // Define some device constants
        // Mode - Sending data
        const unsigned char LCD_CHR = 1;
        // Mode - Sending command
        const unsigned char LCD_CMD =  0;
        // 1st line
        const unsigned char LINE1 = 0x80;
        // 2nd line
        const unsigned char LINE2 = 0xC0; 
        // Backlight On -> 0x08 | Off -> 0x00
        const unsigned char LCD_BACKLIGHT =  0x08;  
        // Enable bit
        const unsigned char ENABLE = 0x04;


        std::string mensagemAcima16;
        std::string mensagemAbaixo16;

    public:

        ShowInfoLCD();
        ~ShowInfoLCD();

        void lcd_init(void);
        void lcd_byte(int bits, int mode);
        void lcd_toggle_enable(int bits);

        // added by Lewis
        void typeInt(int i);
        void typeFloat(float myFloat);
        void lcdLoc(int line); //move cursor
        void ClrLcd(void); // clr LCD return home
        void typeln(const char *s);
        void typeChar(char val);
        int fd;  // seen by all subroutines

        void mostrarMensagem();

        std::string get_mensagemAcima16();
        void set_mensagemAcima16( std::string mensagem );

        std::string get_mensagemAbaixo16();
        void set_mensagemAbaixo16( std::string mensagem );


};
 

#endif