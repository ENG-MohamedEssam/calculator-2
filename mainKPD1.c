#include "STD_TYPES.h"

#include "LCD_interface.h"

#include "DIO_interface.h"

#include "KPD_interface.h"

#include <avr/delay.h>

u16 calc(u16 num1,u8 char1,u16 num2,u8 char2,u16 num3, u8 char3, u16 num4);
u16 concatenate(u8 digit1, u8 digit2) ;
void numCondition(u8 Local_u8num);
void charCondition(u8 Local_u8Char);
u16 subCalc(u8 Local_u8Char, u16 n1, u16 n2);

u8 Xindex = 0, Yindex = 0;

int main (void)
{
	DIO_u8Init();
	LCD_voidInit();
	u8 Local_u8Key;
	u8 num1 = 0, num2 = 0, num3 = 0, num4 = 0, char1 = 0, char2 = 0, char3 = 0; // vars to save the value of the pressed key
	u16 num1_digit1 = 0, num2_digit1 = 0, num3_digit1 = 0, num4_digit1 = 0;
	u16 num1_digit2 = 0, num2_digit2 = 0, num3_digit2 = 0, num4_digit2 = 0;
	while(1)
	{
		KPD_u8GetKeyValue(&Local_u8Key);
		if(Local_u8Key != KPD_u8_KEY_NOT_PRESSED)
		{
			LCD_u8GoToXY(Xindex,Yindex); // start from 0,0
			LCD_voidWriteChar(Local_u8Key);
			switch(Yindex)
			{
			case 0: num1_digit1  = Local_u8Key - '0';
				numCondition(num1_digit1);break; // char to num
			case 1: num1_digit2  = Local_u8Key - '0';
				numCondition(num1_digit2);break;
			case 2: char1 		 = Local_u8Key;
			charCondition(char1);break;
			case 3: num2_digit1  = Local_u8Key - '0';
				numCondition(num2_digit1);break; // char to num
			case 4: num2_digit2  = Local_u8Key - '0';
				numCondition(num2_digit2);break;
			case 5: char2 		 = Local_u8Key;
				charCondition(char2);break;
			case 6: num3_digit1  = Local_u8Key - '0';
				numCondition(num3_digit1);break;		 // char to num
			case 7: num3_digit2  = Local_u8Key - '0';
				numCondition(num3_digit2);break;
			case 8: char3 		 = Local_u8Key;
				charCondition(char3);break;
			case 9: num4_digit1  = Local_u8Key - '0';
				numCondition(num4_digit1);break;		 // char to num
			case 10: num4_digit2  = Local_u8Key - '0';
				numCondition(num4_digit2);
				num1 = concatenate(num1_digit1,num1_digit2);
				num2 = concatenate(num2_digit1,num2_digit2);
				num3 = concatenate(num3_digit1,num3_digit2);
				num4 = concatenate(num4_digit1,num4_digit2);
				LCD_u8GoToXY(1,0);
				LCD_voidWriteChar('=');
				LCD_voidWriteNumber(calc(num1,char1,num2,char2,num3,char3,num4));		// ans is a num
				Yindex +=2 ; // move curser away
				break;
			}
			Yindex++;
		}
	}

	return 0;
}


// check for priority then do switch and make the function work .
u16 calc(u16 num1,u8 char1,u16 num2,u8 char2,u16 num3, u8 char3, u16 num4) // function for calculations
{
	u16 ans = 0;
	u16 ans2 = 0;
	if (char1 == '*' || char1 == '/') // special condition 1
	{
		ans = subCalc(char1,num1,num2);
		if(char2 == '*' || char2 == '/')
		{
			ans = subCalc(char2,ans,num3);
			ans = subCalc(char3,ans,num4);
		}
		else if (char3 == '*' || char3 == '/')
		{
			ans2 = subCalc(char3,num3,num4);
			ans = subCalc(char2,ans,ans2);
			ans2 = 0;
		}
		else
		{
			ans = subCalc(char2,ans,num3);
			ans = subCalc(char3,ans,num4);
		}
	}
	else if (char2 == '*' || char2 == '/') // special condition 2
	{
		ans = subCalc(char2,num2,num3);
		if(char3 == '*' || char3 == '/')
		{
			ans = subCalc(char3,ans,num4);
			ans = subCalc(char1,num1,ans);
		}
		else
		{
			ans = subCalc(char1,num1,ans);
			ans = subCalc(char3,ans,num4);
		}
	}
	else if (char3 == '*' || char3 == '/') // special condition 3
	{
		ans = subCalc(char3,num3,num4);
		ans2 = subCalc(char1,num1,num2);
		ans = subCalc(char2,ans2,ans);

	}
	else // normal operation condition
	{
		ans = subCalc(char1,num1,num2);
		ans = subCalc(char2,ans,num3);
		ans = subCalc(char3,ans,num4);
	}
	return ans;
}

u16 concatenate(u8 digit1, u8 digit2)
{
    u8 Local_u8pow = 10;
    /*while(digit2 >= Local_u8pow)
        Local_u8pow *= 10;*/
    return digit1 * Local_u8pow + digit2;
}


u16 subCalc(u8 Local_u8Char, u16 n1, u16 n2)
{
	switch(Local_u8Char) // check char 2 value
			{
			case 42 : n1*= n2;break;
			case 43 : n1+= n2;break;
			case 45 : n1-= n2;break;
			case 47 : n1/= n2;break;
			}
	return n1;
}

void numCondition(u8 Local_u8num)
{
	if (Local_u8num > 9 || Local_u8num < 0)
	{
	Yindex = 0;
	LCD_u8GoToXY(0,Yindex);
	LCD_voidWriteString("Wrong Input");
	}
}

void charCondition(u8 Local_u8Char)
{
	if (Local_u8Char > 47 || Local_u8Char < 42)
	{
	Yindex = 0;
	LCD_u8GoToXY(0,Yindex);
	LCD_voidWriteString("Wrong Input");
	}
}
