/*
 * File:   main.c
 * Author: Q
 *
 * Created on June 6, 2022, 3:29 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p30fxxxx.h>
#include "tajmeri.h"



_FOSC(CSW_FSCM_OFF & XT_PLL4);//instruction takt je isti kao i kristal
_FWDT(WDT_OFF);

//---------------------------------
unsigned int tempRX = 0, stoperica, flag=0;
unsigned char unos = 0;




void initUART1(void)
{
U1BRG=0x0207;//ovim odredjujemo baudrate 1200
				
U1MODEbits.ALTIO=0;//biramo koje pinove koristimo za komunikaciju osnovne ili alternativne

IEC0bits.U1RXIE=1;//omogucavamo rx1 interupt

U1STA&=0xfffc;

U1MODEbits.UARTEN=1;//ukljucujemo ovaj modul

U1STAbits.UTXEN=1;//ukljucujemo predaju
}

void __attribute__((__interrupt__)) _U1RXInterrupt(void) 
{
    IFS0bits.U1RXIF = 0;
    tempRX=U1RXREG;
    
    if(flag)
    {
        unos = tempRX;
        flag = 0;
    } 

} 
 
void WriteUART1(unsigned int data)
{
	  while(!U1STAbits.TRMT);

    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

void putString(register const char *str)
{
    while(*str != 0) //ceka se nekakav upis
    {
        WriteUART1(*str); 
        if(*str == 13) WriteUART1(10);
        if(*str == 10) WriteUART1(13);
        str++;
    }
}

void delay_ms(int vreme)
{
    stoperica = 0;
    while(stoperica < vreme);
}

void __attribute__ ((__interrupt__)) _T2Interrupt(void) // svakih 1ms
{

	TMR2 =0; 
    stoperica++;
	IFS0bits.T2IF = 0; 
       
}


int main(void) {
    
    ADPCFG = 0xffff;//dig
    TRISB = 0x0000;//izlaz
    TRISD = 0x0000;//izlaz

    LATDbits.LATD0 = 1;
    LATDbits.LATD1 = 1;
    
    initUART1();
    Init_T2();

    flag = 1;
    
    while(1)
    {
        WriteUART1(unos);

        
        if(unos == 'u')
        {
            

            
            LATBbits.LATB0 = 1; //in1
            LATBbits.LATB10 = 0; //in2
            LATBbits.LATB11 = 0; //in3
            LATBbits.LATB12 = 0; //in4
            
            
            
            delay_ms(200);
            
            unos = NULL;
            flag = 1;
            
        }
        else if(unos == 'd')
        {
 
            
            LATBbits.LATB0 = 0; //in1
            LATBbits.LATB10 = 1; //in2
            LATBbits.LATB11 = 0; //in3
            LATBbits.LATB12 = 0; //in4
            
            
            delay_ms(200);
            
            unos = NULL;
            flag = 1;
           
        }
        else if(unos == 'l')
        {
            
            
            LATBbits.LATB0 = 1; //in1
            LATBbits.LATB10 = 0; //in2
            LATBbits.LATB11 = 1; //in3
            LATBbits.LATB12 = 0; //in4
            
            delay_ms(200);
            
            unos = NULL;
            flag = 1;
            
        }
        else if(unos == 'r')
        {
            
            
            LATBbits.LATB0 = 1; //in1
            LATBbits.LATB10 = 0; //in2
            LATBbits.LATB11 = 0; //in3
            LATBbits.LATB12 = 1; //in4
            
            delay_ms(200);
            
            unos = NULL;
            flag = 1;
            
        }
        else
        {
            LATBbits.LATB0 = 0; //in1
            LATBbits.LATB10 = 0; //in2
            LATBbits.LATB11 = 0; //in3
            LATBbits.LATB12 = 0; //in4
            
            
        }


    }
        
   
    return 0;
}
