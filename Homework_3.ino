/**
 * RBE 2001 Homework 3
 * 
 * This program was written to run an interrupt-based timer 
 * on an Arduino uno. It uses the LiquidCrystal library to 
 * drive a small LCD display and the TimerOne library to 
 * control the clock based on timer interrupts. It also has
 * the ability to use a button to reset the clock to 00:00:00.
 * 
 * Digital Pin 2 is used for the button interrupt.
 * 
 * @author   Arthur Lockman
 * @created  9/16/2014
 * @modified 9/19/2014 
 */

#include "TimerOne.h" //Including the TimerOne library.
#include "LiquidCrystal.h" //Including LiquidCrystal Library
#include "Arduino.h" //Including arduino core

#define INT0 2
#define INT1 3

//@TODO: Fix this line
LiquidCrystal lcd(12, 11, 5, 4, 3, 6); //The LCD object that drives the display.
volatile int time = 0; //The current time.

static const int timerPeriod = 1000000; //The period for the timer update.
static const int lcdRows = 2; //Number of rows in LCD display.
static const int lcdCols = 16; //Number of columns in LCD display.

void setup()
{
	lcd.begin(lcdCols, lcdRows); //Initialize LCD display size.
	Timer1.initialize(timerPeriod); //Set timer to update every .5 seconds
	Timer1.attachInterrupt(updateTimer); //Attach timer update routine
	attachInterrupt(INT0, resetTimer, LOW); //Interrupt when the button drops low.
}

void loop()
{
	updateDisplay();	
}

void resetTimer()
{
	time = 0;
}

void updateTimer()
{
	time++;
}

/**
 * @brief [brief description]
 * @details [long description]
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
	timerCount -= hours * 3600;
	int minutes = timerCount / 60;
	timerCount -= minutes * 60;
	int seconds = timerCount;
	if (digits(hours) == 1) result += "0";
	result += hours;
	result += ":";
	if (digits(minutes) == 1) result += "0";
	result += minutes;
	result += ":";
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
	int i = 1;
	int counter = 1;
	bool flag = false;
	while (!flag)
	{
		if (number >= i) 
		{
			return counter;
			flag = true;
		}
		else
		{
			i = i * 10;
			counter++;
		}
	}
	return counter;
}
