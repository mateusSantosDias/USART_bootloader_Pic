/*
 * File:   main.c
 * Author: mateusszubuntu
 *
 * Created on 1 de Agosto de 2025, 13:40
 */

// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define INITIAL_PAGEH 0x01
#define INITIAL_PAGEL 0x01


#include <xc.h>
#include "usart_driver.h"

void boot_loader();
void write_page(uint16_t data, uint8_t addressH, uint8_t addressL);

void  __section("bootloader")boot_loader()
{
    write_page(0x0000, INITIAL_PAGEH, INITIAL_PAGEL);

}

void __section("bootloader") main(void)
{   
    boot_loader();
    while(1);
}


void  __section("bootloader")write_page(uint16_t data, uint8_t addressH, uint8_t addressL)
{
    EEADRH = addressH;
    EEADR  = addressL;
    
    EEDATH = (data>>8);
    EEDATA = (data&0xFF);
    
    EECON1bits.EEPGD  = 0x01;
    EECON1bits.WREN   = 0x01;
    INTCONbits.GIE    = 0x00;
    
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR     = 0x01;
    
    __asm__("NOP");
    __asm__("NOP");
    
    EECON1bits.WREN   = 0x00;
    
} 