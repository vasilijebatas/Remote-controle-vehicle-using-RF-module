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

void __attribute__((__interrupt__)) _ADCInterrupt(void) 
{
							

	sirovi0=ADCBUF0;
    sirovi1=ADCBUF1;
									
										

    IFS0bits.ADIF = 0;

}


int main(int argc, char** argv) {
    
        //analogni pinovi
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
        //TRISFbits.TRISF2=0;
        TRISFbits.TRISF4=0;
        TRISFbits.TRISF5=0;

       

 		ADCinit();//inicijalizacija AD konvertora
        initUART1();//inicijalizacija UART-a
		ADCON1bits.ADON=1;//pocetak Ad konverzije 
        
        while(1)
        {
            
            
            if(sirovi0 < 500)
            {
                putString("uuuuuuuuu");
                LATFbits.LATF0 = 1;
                LATFbits.LATF1 = 0;
                LATFbits.LATF2 = 0;
                LATFbits.LATF4 = 0;
            }
            
            else if(sirovi0 > 3500)
            {
               putString("ddddddddd");
                
                LATFbits.LATF0 = 0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
                LATFbits.LATF1 = 1;
                LATFbits.LATF2 = 0;
                LATFbits.LATF4 = 0;
            }
            
            else if(sirovi1 < 500)
            {
                putString("rrrrrrrrr");
                
                LATFbits.LATF0 = 0;
                LATFbits.LATF1 = 0;
                LATFbits.LATF2 = 0;
                LATFbits.LATF4 = 1;
            }
            
            else if(sirovi1 > 3500)
            {
               putString("lllllllll");
                
                LATFbits.LATF0 = 0;
                LATFbits.LATF1 = 0;
                LATFbits.LATF2 = 1;
                LATFbits.LATF4 = 0;
            } 
            else 
            {
                LATFbits.LATF0 = 0;
                LATFbits.LATF1 = 0;
                LATFbits.LATF2 = 0;
                LATFbits.LATF4 = 0;
                LATFbits.LATF5 = 0;
            }
            
                  
        }
    return 0;
}
