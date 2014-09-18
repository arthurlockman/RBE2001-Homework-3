/**
 * RBE 2001 Homework 3
 * 
 * This program was written to run an interrupt-based timer 
 * on an Arduino uno. It uses the LiquidCrystal library to 
 * drive a small LCD display and the TimerOne library to 
 * control the clock based on timer interrupts. It also has
 * the ability to use a button to reset the clock to 00:00:00.
 * 
 * D2 : Button interrupt
 * D3 : DB7
 * D4 : DB6
 * D5 : DB5
 * D6 : DB4
 * D11: RS
 * D12: E
 * 
 * @author   Arthur Lockman
 * @created  9/16/2014
 * @modified 9/18/2014 
 */

#include "TimerOne.h" //Including the TimerOne library.
#include "LiquidCrystal.h" //Including LiquidCrystal Library
#include "Arduino.h" //Including arduino core

#define INT0 2 //Digital 2
#define INT1 3 //Digital 3

//@TODO: Fix this line
LiquidCrystal lcd(12, 11, 6, 5, 4, 3); //The LCD object that drives the display.		
volatile int time = 0; //The current time.

static const long timerPeriod = 1000000; //The period for the timer update.

void setup()
{
	lcd.begin(16, 2); //Initialize LCD display size.
	lcd.clear(); //clear LCD
	Timer1.initialize(timerPeriod); //Set timer to update on set interval
	Timer1.attachInterrupt(updateTimer); //Attach timer update routine
	pinMode(INT0, INPUT_PULLUP); //Set pin to input.
	attachInterrupt(0, resetTimer, FALLING); //Interrupt when the button drops low.
}

void loop()
{
	updateDisplay(); //Update the display
}

/**
 * @brief Reset the timer.
 * @details This method resets the timer
 * to 0.
 */
void resetTimer()
{
	time = 0; //reset timer to 0
}

/**
 * @brief Update the timer.
 * @details Update the timer. This
 * method increments the timer by one second.
 */
void updateTimer()
{
	time++; //increment timer by a second.
}

/**
 * @brief Update the displayed time
 * @details Update the time displayed on the 
 * LiquidCrystal display.
 */
void updateDisplay()
{
	lcd.setCursor(0,0); //Set cursor to beginning
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
	String result = ""; //create resultant string
	int hours = timerCount / 3600; //find number of hours
	if (digits(hours) == 1) result += "0"; //pad hours with 0
	result += hours; //add hours to result
	result += ":"; //add separator
	int minutes = (timerCount / 60) % 60; //find number of minutes
	if (digits(minutes) == 1) result += "0"; //pad minutes with 0
	result += minutes; //add minutes to result
	result += ":"; //add separator
	int seconds = (timerCount % 60); //find number of seconds
	if (digits(seconds) == 1) result += "0"; //pad seconds with 0
	result += seconds; //add seconds to result
	return result; //return result
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
	if (number == 0) return 1; //0 has one digit
	else
	{
		int digits = 0;
		while (number) //divide by 10 until number is 0
		{
			number /= 10;
			digits++;
		}
		return digits; //return number of digits
	}
}
