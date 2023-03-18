/*
 * File:   main.c
 * Author: Q
 *
 * Created on June 1, 2022, 1:28 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include<p30fxxxx.h>
#include "adc.h"

_FOSC(CSW_FSCM_OFF & XT_PLL4);//instruction takt je isti kao i kristal
_FWDT(WDT_OFF);

unsigned int sirovi0,sirovi1,sirovi2;
unsigned int broj,broj1,broj2,tempRX;
 
void initUART1(void)
{
U1BRG=0x0040;//baud rate 9600
U1MODEbits.ALTIO = 1;
IEC0bits.U1RXIE = 1;
U1STA&=0xfffc;
U1MODEbits.UARTEN=1;
U1STAbits.UTXEN=1;
}
void __attribute__((__interrupt__)) _U1RXInterrupt(void) 
{
    IFS0bits.U1RXIF = 0;
  //   tempRX=U1RXREG;

}

void WriteUART1(unsigned int data)
{


	while (U1STAbits.TRMT==0);
    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

void WriteUART1dec2string(unsigned int data)
{
	unsigned char temp;

	temp=data/1000;
	WriteUART1(temp+'0');
	data=data-temp*1000;
	temp=data/100;
	WriteUART1(temp+'0');
	data=data-temp*100;
	temp=data/10;
	WriteUART1(temp+'0');
	data=data-temp*10;
	WriteUART1(data+'0');
}

void __attribute__((__interrupt__)) _ADCInterrupt(void) 
{
							

										sirovi0=ADCBUF0;
										sirovi1=ADCBUF1;
                                        sirovi2=ADCBUF2;
									
										

    IFS0bits.ADIF = 0;

}


int main(int argc, char** argv) {
    
        for(broj1=0;broj1<10000;broj1++);
        //A/D pinovi
        ADPCFGbits.PCFG0 = 0;
        ADPCFGbits.PCFG1 = 0;
        ADPCFGbits.PCFG2 = 0;
        
        
        //joystick ulaz
		TRISBbits.TRISB0=1;
		TRISBbits.TRISB1=1;
        TRISBbits.TRISB2=1;
        
        //diode izlazi
        TRISFbits.TRISF0=0;
        TRISFbits.TRISF1=0;
        TRISFbits.TRISF2=0;
        TRISFbits.TRISF4=0;
        TRISFbits.TRISF5=0;
        
		for(broj=0;broj<60000;broj++);

 		ADCinit();//inicijalizacija AD konvertora
        initUART1();//inicijalizacija UART-a
		ADCON1bits.ADON=1;//pocetak Ad konverzije 
        while(1)
        {
            LATFbits.LATF0 = 0;
            LATFbits.LATF1 = 0;
            LATFbits.LATF2 = 0;
            LATFbits.LATF4 = 0;
            LATFbits.LATF5 = 0;
            
            if(sirovi0 < 1500)
            {
                LATFbits.LATF0 = 1;
                LATFbits.LATF1 = 0;
                LATFbits.LATF4 = 0;
                LATFbits.LATF5 = 0;
            }
            
            if(sirovi0 > 2500)
            {
                LATFbits.LATF0 = 0;
                LATFbits.LATF1 = 1;
                LATFbits.LATF4 = 0;
                LATFbits.LATF5 = 0;
            }
            
            if(sirovi2 < 1500)
            {
                LATFbits.LATF0 = 0;
                LATFbits.LATF1 = 0;
                LATFbits.LATF4 = 0;
                LATFbits.LATF5 = 1;
            }
            
            if(sirovi2 > 2500)
            {
                LATFbits.LATF0 = 0;
                LATFbits.LATF1 = 0;
                LATFbits.LATF4 = 1;
                LATFbits.LATF5 = 0;
            }
            
          
            
            
           
            
                  
        }
    return 0;
}
