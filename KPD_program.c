/********************************************************/
/*						Name : Mohamed Essam			*/
/*						Date : 25/9/2021				*/
/*						SWC  : Keypad					*/
/*					 version : 1.0						*/
/********************************************************/

/* Lib Layer */
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#define F_CPU 8000000UL
#include <avr/delay.h>

/* MCAL Layer */
#include "DIO_interface.h"

/* HAL */
#include "KPD_interface.h"
#include "KPD_private.h"
#include "KPD_config.h"

/* Keypad Values */
static u8 KPD_Au8Keys[4][4] = KPD_KEYS;

/* Array of row pins */
static u8 KPD_Au8RowsPins[4] = {KPD_u8_R1_PIN,KPD_u8_R2_PIN,KPD_u8_R3_PIN,KPD_u8_R4_PIN};
static u8 KPD_Au8RowsPorts[4] = {KPD_u8_R1_PORT,KPD_u8_R2_PORT,KPD_u8_R3_PORT,KPD_u8_R4_PORT};

static u8 KPD_Au8ColsPins[4] = {KPD_u8_C1_PIN,KPD_u8_C2_PIN,KPD_u8_C3_PIN,KPD_u8_C4_PIN};
static u8 KPD_Au8ColsPorts[4] = {KPD_u8_C1_PORT,KPD_u8_C2_PORT,KPD_u8_C3_PORT,KPD_u8_C4_PORT};



u8 KPD_u8GetKeyValue(u8 *Copy_pu8ReturnedValue)
{
	u8 Local_u8ErrorState = STD_TYPES_OK;
	u8 Local_u8RowsCounter = 0, Local_u8ColsCounter, Local_u8PinValue, Local_u8Flag = 0;
	if(Copy_pu8ReturnedValue != NULL)
	{
		/* Initial Value */
		*Copy_pu8ReturnedValue = KPD_u8_KEY_NOT_PRESSED;
		/* Step 1 : Activate Rows */
		/* Create Loop to activate each row */
		for(Local_u8RowsCounter = 0; Local_u8RowsCounter < 4; Local_u8RowsCounter++)
		{
			DIO_u8SetPinValue(KPD_Au8RowsPorts[Local_u8RowsCounter],KPD_Au8RowsPins[Local_u8RowsCounter],DIO_u8_LOW);
			/* Step 2 : Check each column to get the pressed switch */
			for(Local_u8ColsCounter = 0;Local_u8ColsCounter < 4;Local_u8ColsCounter++)
			{
				/* Get Pin Value */
				DIO_u8GetPinValue(KPD_Au8ColsPorts[Local_u8ColsCounter],KPD_Au8ColsPins[Local_u8ColsCounter],&Local_u8PinValue);
				/* Check pin value */
				if(Local_u8PinValue == DIO_u8_LOW) /* Switch is pressed */
				{
					/* Debouncing */
					_delay_ms(20);
					/* wait for pin to go high */
					do
					{
						DIO_u8GetPinValue(KPD_Au8ColsPorts[Local_u8ColsCounter],KPD_Au8ColsPins[Local_u8ColsCounter],&Local_u8PinValue);
					}
					while(Local_u8PinValue == DIO_u8_LOW);
					/* return the result */
					*Copy_pu8ReturnedValue = KPD_Au8Keys[Local_u8RowsCounter][Local_u8ColsCounter];
					/* Update flag to terminate outer loop */
					Local_u8Flag = 1;
					/* Terminate Loop */
					break;
				}
			}
		/* Deactivate Row*/
		DIO_u8SetPinValue(KPD_Au8RowsPorts[Local_u8RowsCounter],KPD_Au8RowsPins[Local_u8RowsCounter],DIO_u8_HIGH);
		if (Local_u8Flag == 1)// if it was written Local_u8Flag != 0 it'll be faster
		{
			break;
		}
		}
	}
	else
	{
		Local_u8ErrorState = STD_TYPES_NOK;
	}

	return Local_u8ErrorState;
}
