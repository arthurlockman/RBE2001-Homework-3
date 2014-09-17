/**
 * RBE 2001 Homework 3
 * 
 * This program was written to run an interrupt-based timer 
 * on an Arduino uno. It uses the LiquidCrystal library to 
 * drive a small LCD display and the TimerOne library to 
 * control the clock based on timer interrupts. It also has
 * the ability to use a button to reset the clock to 00:00:00.
 * 
 * Digital Pin 3 is used for the button interrupt.
 * 
 * @author   Arthur Lockman
 * @created  9/16/2014
 * @modified 9/19/2014 
 */

#include "TimerOne.h" //Including the TimerOne library.
#include "LiquidCrystal.h" //Including LiquidCrystal Library
#include "Arduino.h" //Including arduino core

#define INT0 2 //Digital 2
#define INT1 3 //Digital 3
#define PIN_RS 13
#define PIN_E 12
#define PIN_DB4 11
#define PIN_DB5 10
#define PIN_DB6 9
#define PIN_DB7 8

//@TODO: Fix this line
LiquidCrystal lcd(PIN_RS, PIN_E, PIN_DB4, PIN_DB5, PIN_DB6, PIN_DB7); //The LCD object that drives the display.
volatile int time = 0; //The current time.

static const long timerPeriod = 1000000; //The period for the timer update.
static const int lcdRows = 2; //Number of rows in LCD display.
static const int lcdCols = 16; //Number of columns in LCD display.

void setup()
{
	Serial.begin(115200);
	lcd.begin(lcdCols, lcdRows); //Initialize LCD display size.
	Timer1.initialize(timerPeriod); //Set timer to update on set interval
	Timer1.attachInterrupt(updateTimer); //Attach timer update routine
	pinMode(INT0, INPUT_PULLUP); //Set pin to input.
	attachInterrupt(0, resetTimer, FALLING); //Interrupt when the button drops low.
}

void loop()
{
	updateDisplay(); //Update the display
	Serial.println(convertToTime(time));
	delay(1000);
}

/**
 * @brief Reset the timer.
 * @details This method resets the timer
 * to 0.
 */
void resetTimer()
{
	time = 0;
}

/**
 * @brief Update the timer.
 * @details Update the timer. This
 * method increments the timer by one second.
 */
void updateTimer()
{
	time++;
}

/**
 * @brief Update the displayed time
 * @details Update the time displayed on the 
 * LiquidCrystal display.
 */
void updateDisplay()
{
	lcd.setCursor(0,1); //Set cursor to beginning
	lcd.print(convertToTime(time)); //Write time to display
}

/**
 * @brief Converts timer time into time.
 * @details Converts the time from the 
 * interrupt timer to the usable time.
 * 
 * @param timerCount The count from the timer.
 * 
 * @return The string of the converted time.
 */
String convertToTime(int timerCount)
{
	String result = "";
	int hours = timerCount / 3600;
	if (digits(hours) == 1) result += "0";
	result += hours;
	result += ":";
	int minutes = (timerCount / 60) % 60;
	if (digits(minutes) == 1) result += "0";
	result += minutes;
	result += ":";
	int seconds = (timerCount % 60);
	if (digits(seconds) == 1) result += "0";
	result += seconds;
	return result;
}

/**
 * @brief Get number of digits in number.
 * @details Get the number of digits in an 
 * integer. This is needed to add padding 
 * to the numbers in the clock.
 * 
 * @param number The number to size.
 * @return The number of digits in number.
 */
int digits(int number)
{
	if (number == 0) return 1;
	else
	{
		int digits = 0;
		while (number) 
		{
			number /= 10;
			digits++;
		}
		return digits;
	}
}
