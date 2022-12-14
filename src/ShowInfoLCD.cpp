#include "ShowInfoLCD.hpp"

ShowInfoLCD::ShowInfoLCD() {
    mensagemAcima16.resize(16, ' ');
    mensagemAbaixo16.resize(16, ' ');

    fd = wiringPiI2CSetup(I2C_ADDR);

    if (wiringPiSetup() == -1){ 
		std::cout << "Erro abertura LCD" << '\n';
		exit (1);
	}

    lcd_init(); // setup LCD
}

ShowInfoLCD::~ShowInfoLCD() {}

void ShowInfoLCD::mostrarMensagem() {
    //printf("fd = %d ", fd);
    lcdLoc(LINE1);
    typeln(get_mensagemAcima16().c_str());
    lcdLoc(LINE2);
    typeln(get_mensagemAbaixo16().c_str());
}

// float to string
void ShowInfoLCD::typeFloat(float myFloat)   {
  this->mensagemAbaixo16 = myFloat;

}

// int to string
void ShowInfoLCD::typeInt(int i)   {
  this->mensagemAbaixo16 = i;
}

// clr lcd go home loc 0x80
void ShowInfoLCD::ClrLcd(void)   {
  lcd_byte(0x01, LCD_CMD);
  lcd_byte(0x02, LCD_CMD);
}

// go to location on LCD
void ShowInfoLCD::lcdLoc(int line)   {
  lcd_byte(line, LCD_CMD);
}

// out char to LCD at current position
void ShowInfoLCD::typeChar(char val)   {

  lcd_byte(val, LCD_CHR);
}


// this allows use of any size string
void ShowInfoLCD::typeln(const char *s)   {

  while ( *s ) lcd_byte(*(s++), LCD_CHR);

}

void ShowInfoLCD::lcd_byte(int bits, int mode)   {

  //Send byte to data pins
  // bits = the data
  // mode = 1 for data, 0 for command
  int bits_high;
  int bits_low;
  // uses the two half byte writes to LCD
  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

  // High bits
  wiringPiI2CReadReg8(fd, bits_high);
  lcd_toggle_enable(bits_high);

  // Low bits
  wiringPiI2CReadReg8(fd, bits_low);
  lcd_toggle_enable(bits_low);
}

void ShowInfoLCD::lcd_toggle_enable(int bits)   {
  // Toggle enable pin on LCD display
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits | ENABLE));
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
  delayMicroseconds(500);
}


void ShowInfoLCD::lcd_init()   {
  // Initialise display
  lcd_byte(0x33, LCD_CMD); // Initialise
  lcd_byte(0x32, LCD_CMD); // Initialise
  lcd_byte(0x06, LCD_CMD); // Cursor move direction
  lcd_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
  lcd_byte(0x28, LCD_CMD); // Data length, number of lines, font size
  lcd_byte(0x01, LCD_CMD); // Clear display
  delayMicroseconds(500);
}

std::string ShowInfoLCD::get_mensagemAcima16() {
    return this->mensagemAcima16;
}

void ShowInfoLCD::set_mensagemAcima16( std::string mensagem ){
    this->mensagemAcima16 = mensagem;
}

std::string ShowInfoLCD::get_mensagemAbaixo16() {
    return this->mensagemAbaixo16;
}

void ShowInfoLCD::set_mensagemAbaixo16( std::string mensagem ){
    this->mensagemAbaixo16 = mensagem;
}
