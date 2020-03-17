/**
  PRG1 Generated Driver  File

  @Company
    Microchip Technology Inc.

  @File Name
    prg1.c

  @Summary
    This is the generated driver implementation for the PRG1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for PRG1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.79.0
        Device            :  PIC16F1779
        Driver Version    :  2.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.10 and above
        MPLAB 	          :  MPLAB X 5.30
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "prg1.h"

/**
  Section: PRG1 APIs
*/

void PRG1_Initialize(void)
{
    // RG1GO not operating; RG1EN enabled; RG1MODE rising ramp generator; RG1OS disabled; RG1REDG level_sensitive; RG1FEDG level_sensitive; 
    PRG1CON0 = 0x88;
    // RG1FPOL active_low; RG1RPOL active_high; 
    PRG1CON1 = 0x02;
    // INS DAC3_output; 
    PRG1INS = 0x05;
    // RG1ISET 1.50  V/us; 
    PRG1CON2 = 0x15;
    // RTSS PWM3_output; 
    PRG1RTSS = 0x08;
    // FTSS PWM3_output; 
    PRG1FTSS = 0x08;

}

bool PRG1_IsReady(void)
{
    return (PRG1CON1bits.RG1RDY);
}

void PRG1_StartRampGeneration(void)
{
    // Start the PRG module by set GO bit
    PRG1CON0bits.RG1GO = 1;
}

void PRG1_StopRampGeneration(void)
{
    // Stop the PRG module by clearing GO bit
    PRG1CON0bits.RG1GO = 0;
}

void PRG1_EnableOneShot(void)
{
    // Enable One-Shot mode by setting OS bit
    PRG1CON0bits.RG1OS = 1;
}

void PRG1_DisableOneShot(void)
{
    // Disable One-Shot mode by clearing OS bit
    PRG1CON0bits.RG1OS = 0;
}

void PRG1_UpdateSlope(uint8_t slopeValue )
{
    PRG1CON2bits.RG1ISET = (slopeValue & 0x1F);
}

/**
 End of File
*/
