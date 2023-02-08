/*
 ================================================================================================
 Name        : Mini_Project4.c
 Author      : Mohamed Ali
 Description : Measuring the distance using Ultrasonic sensor system
 ================================================================================================
 */

#include "lcd.h"
#include "ultrasonic.h"
#include <avr/io.h> /* To use the SREG register */

int main()
{
	uint16 distance = 0;

	/* Initialize the LCD & Ultrasonic driver */
	LCD_init();
	Ultrasonic_init();

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	/* Display this string "Distance=     cm" only once on LCD at the first row */
	LCD_displayString("Distance=     cm");


	while(1)
	{
		/* Read the distance from Ultrasonic sensor */
		distance = Ultrasonic_readDistance();

		/* Display the distance value every time at same position */
		LCD_moveCursor(0, 10);
		if(distance >= 100)
		{
			LCD_intgerToString(distance);
		}
		else
		{
			LCD_intgerToString(distance);
			/* In case the value is two or one digits print space in the next digit place */
			LCD_displayCharacter(' ');
		}
	}
}
