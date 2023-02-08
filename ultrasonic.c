 /******************************************************************************
 *
 * Module: Ultrasonic
 *
 * File Name: ultrasonic.c
 *
 * Description: Source file for the ultrasonic driver
 *
 * Author: Mohamed Ali
 *
 *******************************************************************************/

#include <util/delay.h> /* For the delay functions */
#include "gpio.h"
#include "icu.h"
#include "ultrasonic.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * 1. Initialize the ICU driver as required.
 * 2. Setup the ICU call back function.
 * 3. Setup the direction for the trigger pin as output pin through the GPIO driver.
 */
void Ultrasonic_init(void)
{
	/* Create configuration structure for ICU driver */
	Icu_ConfigType Icu_Config = {F_CPU_8,RISING};

	/* Initialize the ICU driver */
	Icu_init(&Icu_Config);

	/* Set the Call back function pointer in the ICU driver */
	Icu_setCallBack(Ultrasonic_edgeProcessing);

	/* Setup the direction for the trigger pin as output pin */
	GPIO_setupPinDirection(Ultrasonic_TRIGGER_PORT_ID, Ultrasonic_TRIGGER_PIN_ID, PIN_OUTPUT);

	/* Initialize the output of the trigger pin to LOGIC_LOW */
	GPIO_writePin(Ultrasonic_TRIGGER_PORT_ID, Ultrasonic_TRIGGER_PIN_ID, LOGIC_LOW);
}

/*
 * Description :
 * Send the Trigger pulse to the ultrasonic.
 */
void Ultrasonic_Trigger(void)
{
	/* Transmit trigger pulse of at least 10 us to the Trigger Pin */
	GPIO_writePin(Ultrasonic_TRIGGER_PORT_ID, Ultrasonic_TRIGGER_PIN_ID, LOGIC_HIGH);
	_delay_us(12);
	GPIO_writePin(Ultrasonic_TRIGGER_PORT_ID, Ultrasonic_TRIGGER_PIN_ID, LOGIC_LOW);
}

/*
 * Description :
 * 1. Send the trigger pulse by using Ultrasonic_Trigger function.
 * 2. Start the measurements by the ICU from this moment.
 */
uint16 Ultrasonic_readDistance(void)
{
	uint16 distance = 0;

	/* Send the trigger pulse */
	Ultrasonic_Trigger();

	/* Wait until the high time is calculated */
	while(g_edgeCount < 2){}

	/* Clear the edge counter to start measurements again */
	g_edgeCount = 0;

	/* Calculate the distance in cm */
	distance = g_timeHigh / (58);

	return distance;
}

/*
 * Description :
 * 1. This is the call back function called by the ICU driver.
 * 2. This is used to calculate the high time (pulse time) generated by the ultrasonic sensor.
 */
void Ultrasonic_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();
		/* Clear the timer counter register to start measurements again */
		Icu_clearTimerValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}
}
