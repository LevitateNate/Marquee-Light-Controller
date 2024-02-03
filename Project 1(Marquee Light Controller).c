/*==============================================================================
 * Template - Project 1
 * Author: Carlos Restrepo
 * CPEG222
 * Fall 2023
 * University of Delaware
==============================================================================*/
/*---- Board system settings. PLEASE DO NOT MODIFY THIS PART FOR PROJECT 1 ---*/
#ifndef _SUPPRESS_PLIB_WARNING //suppress the plib warning during compiling
#define _SUPPRESS_PLIB_WARNING
#endif
#pragma config FPLLIDIV = DIV_2 // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20 // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1 // System PLL Output Clock Divider 
                                //(PLL Divide by 1)
#pragma config FNOSC = PRIPLL   // Oscillator Selection Bits (Primary Osc w/PLL 
                                //(XT+,HS+,EC+PLL))
#pragma config FSOSCEN = OFF    // Secondary Oscillator Enable (Disabled)
#pragma config POSCMOD = XT     // Primary Oscillator Configuration (XT osc mode)
#pragma config FPBDIV = DIV_8   // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
/*----------------------------------------------------------------------------*/
#include <xc.h>     //Microchip XC processor header which links to the 
                    //PIC32MX370512L header
#include "config.h" // Basys MX3 configuration header
#include "lcd.h"    // NOTE: utils.c and utils.h must also be in your project 
                    //to use lcd.c
/* ----------------------- Custom types ------------------------------------- */
enum mode{MODE1, MODE2, MODE3, MODE4};
/* ----------------------- Prototypes ----------------------------------------*/
void initialize_ports();
void initialize_output_states();
void handle_button_presses();
void delay_ms(int milliseconds);
void logic_mode_one();
void logic_mode_two();
void logic_mode_three();
void logic_mode_four();
void logic_button_presses(enum mode *modePtr);
/* ------------------------ Constant Definitions ---------------------------- */
#define SYS_FREQ (80000000L) // 80MHz system clock
#define _80Mhz_ (80000000L)
#define LOOPS_NEEDED_TO_DELAY_ONE_MS_AT_80MHz 1426
#define LOOPS_NEEDED_TO_DELAY_ONE_MS (LOOPS_NEEDED_TO_DELAY_ONE_MS_AT_80MHz * (SYS_FREQ / _80Mhz_))
/* The Basys reference manual shows to which pin of the processor every IO 
connects. 
 BtnC connects to Port F pin 0. PORTF reads output values from Port F pins.
 LATF would be used to read input values on Port F pins and TRISF would be used to
 set tristate values of Port F pins. We will see LAT and TRIS later. */
#define TRUE 1
#define FALSE 0
#define BUTTON_DEBOUNCE_DELAY_MS 20
#define BUTTON_RIGHT PORTBbits.RB8
#define SW6 PORTBbits.RB10
#define SW7 PORTBbits.RB9
/* -------------------- Global Variable Declarations ------------------------ */
char buttonsLocked = FALSE;
char pressedUnlockedBtnR = FALSE;
/* ----------------------------- Main --------------------------------------- */
int main(void)
{
    /*-------------------- Port and State Initialization ---------------------*/
    initialize_ports();
    initialize_output_states();
    
    enum mode current_mode = MODE1;

    while (TRUE)
    {
        /*-------------------- Main logic and actions start ------------------*/
        handle_button_presses();
        if (pressedUnlockedBtnR) // Actions when BTNR is pressed
        {
            logic_button_presses(&current_mode);
        }
        switch (current_mode){
            case MODE1:
                logic_mode_one();
                break;
            case MODE2:
                logic_mode_two();
                current_mode = MODE3;
                break;
            case MODE3:
                logic_mode_three();
                break;
            case MODE4:
                logic_mode_four();
                current_mode = MODE1;
                break;     
        }
        /*--------------------------------------------------------------------*/
    }
}
/* ---------------------- Function Definitions ------------------------------ */
void initialize_ports()
{
    // Required to use Pin RA0 (connected to LED 0) as IO
    DDPCONbits.JTAGEN = 0; 
    
    /* 
    The following line sets the tristate of Port A bits 0-7 to 0. The LEDs are 
    connected to those pins. When the tristate of a pin is set low, the pin is 
    configured as a digital output. Notice an &= is used in conjunction with
    leading 1s (the FF) in order to keep the other bits of Port A (8-15) in
    their current state. 
    */
    TRISA &= 0xFF00;
    
    // Configure BTNR
    TRISBbits.TRISB8 = 1; // RB8 (BTNR) configured as input
    ANSELBbits.ANSB8 = 0; // RB8 (BTNR) disabled analog
    
    LCD_Init(); // A library function provided by Digilent
}
void initialize_output_states()
{
    /* The following line sets the latch values of Port A bits 0-7 to 0.
     *  The LEDs are connected to those pins. When the latch of an LED output
     *  pin is set low, the LED is turned off. Notice we again used an &= in
     *  conjunction with leading 1s in order to keep the other latch values of
     *  Port A (bits 8-15) in their current state. */
    LATA &= 0xFF00;    
    
    /* Display "Group #1" at line 0 position 0, using spaces to center it and
     * clear any previously displayed letters*/
    LCD_WriteStringAtPos("    Group #1    ", 0, 0);
    
    /* Display "Mode 1" at line 1 position 0, using spaces to center it and
     * clear any previously displayed letters*/
    LCD_WriteStringAtPos("     Mode 1     ", 1, 0); // line 1, position 0
    
}
/* The below function only handles BtnR presses. Think about how it could be
 expanded to handle all button presses.*/
void handle_button_presses()
{
    pressedUnlockedBtnR = FALSE;
    if (BUTTON_RIGHT && !buttonsLocked)
    {
        delay_ms(BUTTON_DEBOUNCE_DELAY_MS); // debounce
        buttonsLocked = TRUE;
        pressedUnlockedBtnR = TRUE;
    }
    else if (!BUTTON_RIGHT && buttonsLocked)
    {
        delay_ms(BUTTON_DEBOUNCE_DELAY_MS); // debounce
        buttonsLocked = FALSE;
    }
}
void delay_ms(int milliseconds)
{
    int i;
    for (i = 0; i < milliseconds * LOOPS_NEEDED_TO_DELAY_ONE_MS; i++) 
    {}
}

void logic_mode_one(){
    
    LCD_WriteStringAtPos("    Group 28    ", 0, 0); // line 0, position 0
    LCD_WriteStringAtPos("     Mode 1     ", 1, 0); // line 1, position 0
    
    // Turn all the LEDs on
    LATA |= 0x00FF;
    
    return;
}
void logic_mode_two(){

    LCD_WriteStringAtPos("    Group 28    ", 0, 0); // line 0, position 0
    LCD_WriteStringAtPos("     Mode 2     ", 1, 0); // line 1, position 0
    
    if(SW7 == TRUE){
        LATA &= 0x7F;
    delay_ms(300);
    LATA &= 0x3F; 
    delay_ms(300);
    LATA &= 0x1F;
    delay_ms(300);
    LATA &= 0xF;
    delay_ms(300);
    LATA &= 0x7;
    delay_ms(300);
    LATA &= 0x3;
    delay_ms(300);
    LATA &= 0x1;
    delay_ms(300);
    LATA &= 0x0;
    }
    else{
        LATA &= 0xFE;
    delay_ms(300);
    LATA &= 0xFC; 
    delay_ms(300);
    LATA &= 0xF8;
    delay_ms(300);
    LATA &= 0xF0;
    delay_ms(300);
    LATA &= 0xE0;
    delay_ms(300);
    LATA &= 0xC0;
    delay_ms(300);
    LATA &= 0x80;
    delay_ms(300);
    LATA &= 0x0;
    }
    
    // Turn on the LEDs alternating 1 and 0 with speed
    if(SW6 == TRUE){
        LATA &= 0xFE;
    delay_ms(100);
    LATA &= 0xFC; 
    delay_ms(100);
    LATA &= 0xF8;
    delay_ms(100);
    LATA &= 0xF0;
    delay_ms(100);
    LATA &= 0xE0;
    delay_ms(100);
    LATA &= 0xC0;
    delay_ms(100);
    LATA &= 0x80;
    delay_ms(100);
    LATA &= 0x0;
    }
    else{
         
     LATA &= 0xFE;
    delay_ms(300);
    LATA &= 0xFC; 
    delay_ms(300);
    LATA &= 0xF8;
    delay_ms(300);
    LATA &= 0xF0;
    delay_ms(300);
    LATA &= 0xE0;
    delay_ms(300);
    LATA &= 0xC0;
    delay_ms(300);
    LATA &= 0x80;
    delay_ms(300);
    LATA &= 0x0;
    }
    //return;
    }
void logic_mode_three(){
    LCD_WriteStringAtPos("    Group 28    ", 0, 0); // line 0, position 0
    LCD_WriteStringAtPos("     Mode 3     ", 1, 0); // line 1, position 0   
    
    // Turn all the LEDs off
    LATA |= 0xFF00;
    return;
}
void logic_mode_four(){
    
    LCD_WriteStringAtPos("    Group 28    ", 0, 0); // line 0, position 0
    LCD_WriteStringAtPos("     Mode 4     ", 1, 0); // line 1, position 0
    
    // Turn on the LEDs alternating 0 and 1
    if(SW6 == TRUE){
    LATA |= 0xFF01;
    delay_ms(100);
    LATA |= 0xFF03; 
    delay_ms(100);
    LATA |= LATA << 1;
    delay_ms(100);
    LATA |= LATA << 1;
    delay_ms(100);
    LATA |= LATA << 1;
    delay_ms(100);
    LATA |= LATA << 1;
    delay_ms(100);
    LATA |= LATA << 1;
    delay_ms(100);
    LATA |= LATA << 1;
    delay_ms(100);
    LATA |= LATA << 1;    
    }
    else{
    LATA |= 0xFF01;
    delay_ms(300);
    LATA |= 0xFF03; 
    delay_ms(300);
    LATA |= LATA << 1;
    delay_ms(300);
    LATA |= LATA << 1;
    delay_ms(300);
    LATA |= LATA << 1;
    delay_ms(300);
    LATA |= LATA << 1;
    delay_ms(300);
    LATA |= LATA << 1;
    delay_ms(300);
    LATA |= LATA << 1;
    delay_ms(300);
    LATA |= LATA << 1;
    }
    return;
}
                
void logic_button_presses(enum mode *modePtr){
    if (*modePtr == MODE1){
        *modePtr = MODE2;
    }
    else if (*modePtr == MODE2){
        *modePtr = MODE3;
    }
    else if (*modePtr == MODE3){
        *modePtr = MODE4;
    }
    else if (*modePtr == MODE4){
        *modePtr = MODE1;
    }
}
void logic_button_presses(enum mode *modePtr){
    if (*modePtr == MODE1){
        *modePtr = MODE2;
    }
    else if (*modePtr == MODE2){
        *modePtr = MODE3;
    }
    else if (*modePtr == MODE3){
        *modePtr = MODE4;
    }
    else if (*modePtr == MODE4){
        *modePtr = MODE1;
    }
}