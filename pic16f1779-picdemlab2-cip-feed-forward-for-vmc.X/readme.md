<div id="readme" class="Box-body readme blob js-code-block-container">
  <article class="markdown-body entry-content p-3 p-md-6" itemprop="text"><p><a href="https://www.microchip.com" rel="nofollow"><img src="https://camo.githubusercontent.com/5fb5505f69a28ff407841612dfe2b7004f210594/68747470733a2f2f636c6475702e636f6d2f553071684c7742696a462e706e67" alt="MCHP" data-canonical-src="https://cldup.com/U0qhLwBijF.png" style="max-width:100%;"></a></p>

#### Feed-Forward Function for Voltage Mode Control with CIPs

Implementation of the Feed-forward function for SMPS VMC loops implemented with CIPs


#  Introduction
Voltage mode control (VMC) was introduced as the first method to properly close the loop and regulate the output of the switched mode power supply (SMPS). 
The disadvantage of slow response to line and load changes shifted the attention of SMPS designers to current mode control (CMC) as it presented immediate response and it eliminated delay and gain variations.
The voltage mode control can be improved by adding voltage feed-forward, which eliminates the effects of line voltage variations. 
The PIC16F176X/7x family of mixed-signal microcontrollers are equipped with analog core independent peripherals (CIPs) capable of implementing both voltage mode and current mode control, but they do not have a feed-forward dedicated function. 
This MPLAB Xpress example presents a method to connect CIPs and obtain the needed feed-forward function. The method does not need core intervention and is implemented just by setting configuration bits with the help of the MPLAB Code Configurator (MCC) plugin.


# Detailed Description

In the classic approach to voltage feedforward for VMC the slope of the sawtooth signal is influenced by the input voltage, and a change in input voltage will affect the duty cycle of the PWM on the same or next switching cycle, 
this adjustment allows the SMPS converter to respond faster to input voltage changes and compensates the SMPS gain variations with input voltage variations. 
The Programmable Ramp Generator (PRG) in PIC16F176X/7X allows changing the slope of the ramp only through peripheral registers, which makes the classic approach core and code dependable.
What can be done instead is to keep the slope constant but change the lower starting point of the ramp (making sure that the upper part of the slope does not reach limitation till the end of the “ON” time), t
his in turn will create a similar effect on the duty-cycle of the PWM and compensate the input voltage changes within 1 switching cycles – thus making the SMPS respond faster. This solution does not compensate for gain variations though.
This internal peripheral arrangement is depicted in figure 1 and 2 as there are two possible ways to implement this : 
one is to connect Vin to the negative reference of a 5-bit Digital to Analog Converter (DAC) and set the register to 0 or to connect Vin to the positive reference of a 5-bit DAC and set the register to 63 (or the maximum value if another DAC is used).
The DAC reference can be changed during run time to adjust or refine the impact of the feedforward function to the PWM duty cycle. 
Vin can be connected directly to the input of the PRG through an IO pin, but this solution is more taxing as PRG_IN shares an IO pin with an OpAmp_OUT and the OpAmp becomes unusable.  
The rest of the VMC configuration remains the same and it can be implemented with the MCC SMPS library .

 
 
Figure 1: Internal CIP connections for the feed-forward function, Vin to REF- of the DAC


 <img src="https://static.transim.com/img/62019/9f6d9f33fcde443db34fd3b2eea7a6aa-fz6rd.png" alt="Hardware Setup" width="640"/>

 

Figure 2: Internal CIP connections for the feed-forward function, Vin to REF+ of the DAC


 <img src="https://static.transim.com/img/62019/ebd1b7f106ee42e789c5f4242485a4c3-3qgzf.png" alt="Hardware Setup" width="640"/>


Figure 3 is a visual depiction of how the signals will facilitate the feedforward function.
To see the impact of the input voltage change on the PWM duty-cycle, the OA_OUT signal (output voltage feedback error) must be kept constant. 
When the input voltage value is increasing, the VMC ramp (in a constant slope rising ramp configuration) will start at a higher initial voltage and the SMPS control PWM (COG_OUT) duty-cycle will decrease, 
and the opposite is happening when the input voltage is decreasing. This function corrects input voltage variations on the same switching cycle thus implementing feed-forward.     
As a precaution to not saturate the PRG and limit the duty-cycle range the user must set the slope and the input range accordingly. 
EQ 1: Vprg  <  (MCU_VDD - Vin*k) / TimerPeriod

As an example: 
If the maximum input voltage is 20V, the input voltage divider is set to 1:10 – we can conclude that the maximum DAC output is 2V, which leaves a headroom of 3Vfor the PRG. 
Let’s say the period is 2us, in order to not reach 5V with the ramp before the end of the period we must configure the slope to be smaller than 1.5V/us. 
 
 
Figure 3: Internal signals for the feedforward function


 <img src="https://static.transim.com/img/62019/a49e65842b0b48b3b4d37510e96add0b-s7sh0.png" alt="Hardware Setup" width="640"/>



# Test Setup

The following are used to test this function:
- One PICDEM LAB II board (or a prototyping board)
- One PIC16F1779 (any PIC16F176X/7X can be used as well)
- One potentiometer to simulate the variable input voltage
- One potentiometer to simulate the voltage feedback
- wires, MPLAB® PICkit™ 4 (any other PIC programmer will work)
- One Oscilloscope to verify the signals

The software used at the time of the tests are:
- MPLAB® X IDE 5.30 or newer [(microchip.com/mplab/mplab-x-ide)](http://www.microchip.com/mplab/mplab-x-ide)
- MPLAB® XC8 2.10 or newer compiler [(microchip.com/mplab/compilers)](http://www.microchip.com/mplab/compilers)
- MPLAB® Code Configurator (MCC) 3.95.0 or newer [(microchip.com/mplab/mplab-code-configurator)](https://www.microchip.com/mplab/mplab-code-configurator)

The following figure depicts the test setup, where VDD and GND are provided by PICkit 4. To power the
board from the PICkit 4, right click on the current project, followed by Properties/PICkit 4/Option
categories: Power; and check the box ‘Power target circuit from PICkit 4’, then select OK.
The configurations are set using the MPLAB Code Configurator plug-in from MPLAB X with minor code
intervention to start the PRG. After the configuration is done, the functions work independently of any
core intervention.

 
Figure 4: IC pin connections for the feedforward function


<img src="https://static.transim.com/img/62019/9b55196fad464f24af930116e01217bb-5bgcx.png" alt="Hardware Setup" width="640"/>



MCU Configurations: 
PIC internal oscillator 32MHz (8MHz x 4PLL)
CIPs Used:
TMR2: period 2us – this is going to decide the SMPS and PRG switching frequency. 
PWM3: 95% DC, TMR2 as source, used as start ramp rising and start ramp falling for PRG. PWM3 output is connected to an IO pin for monitoring and measurement purposes.
DAC3(5-bit): Positive reference - VDD, Negative reference – DAC3REF0-, Required ref – 0. At the negative reference of the DAC is connected a potentiometer that represents the input voltage. DAC3 output is connected to an IO pin for monitoring and measurement purposes, only during the testing phase. During normal operation DAC3 OUTPUT should not be connected to an IO pin.
PRG: PRG is configured as rising ramp generator, Voltage input source: DAC3_output, slope rate: 1.5V/us, Ramp rising timing source: PWM3_output, rising event polarity: active_high, Ramp falling timing source: PWM3_output, falling event polarity: active_low;
CMP1: Positive Input: PRG1, Negative Input: CIN4-, Output polarity: not inverted. The negative input is connected to a potentiometer that represents the output feedback error signal , the output of the comparator will represent a PWM signal with the duty cycle influenced by the input voltage variation. 
OPA1: Set as Unity gain, PRG1 set as input; is used to monitor the internal PRG signal with the oscilloscope.

For more information on the CIPs see the PIC16F176X/7X product’s data sheet.
Figure 5 depicts the MCC peripherals used, the configuration of the PRG, the pinout and the lines of code added in in the main.c file.
The lines of code added 
‘while (!PRG1_IsReady()); 
PRG1_StartRampGeneration();’ 
Allow the PRG time to initialize and start when it is ready

  
Figure 5: MCC CIPs used and MCU pinout


<img src="https://static.transim.com/img/62019/c216f22ad3684eeab5389c47a2671c7d-r48b3.png" alt="Hardware Setup" width="640"/>


**RESULTS**

The following results were obtained using an oscilloscope and a 5V source, the measurement points are depicted in figure 3. 

One of the measured signals is the Comparator 1 output, as this shows the influence from changing the input voltage will have on the SMPS PWM duty-cycle, keep in mind that the rising event of the comparator will trigger a falling event in the SMPS PWM (COG).
The second measured signal is the comparator negative input, which represents the feedback error signal in the VMC control, in this case is the scaled voltage coming from a potentiometer, this signal is kept constant as to simulate the late output response of the feedback loop.
The third measured signal is the OpAmp output, which represents the PRG output and the comparator positive input. This signal helps understand the comparison between the positive and negative inputs of the comparator and shows how the starting value of the PRG changes when the input voltage is varied. 
The fourth measured signal is the DAC output, which represents the Input Voltage of the SMPS and in this case the output of a second potentiometer. The DAC output is the input voltage of the PRG, so it is easy to notice how the PRG output signal follows the DAC output signal. 
To test the effects of input voltage variation on the feedforward function the potentiometer connecting to DAC3REF- is changed from minimum to maximum desired value, thus simulating input change.  

The previous discussed signals are depicted in figure 6 in the case when the Input Voltage is at the minimum value, it can be noticed how the rising event of the comparator starts late, which means a high duty-cycle on the SMPS PWM.
 
Figure 6: Comparator Output when VIN is close to its minimum value


<img src="https://static.transim.com/img/62019/089bb7cb138246248e796ea41c2b89ca-xrs3b.png" alt="Hardware Setup" width="640"/>



The signals when the Input voltage is at a middle value are depicted in figure 7, it can be noticed how the rising event of the comparator starts earlier than before, which means a medium duty-cycle on the SMPS PWM. 
The change in duty-cycle happened without the change of the feedback signal (C1IN-) which in case means a feedforward adaptation.

Figure 7: Comparator Output when VIN is at its medium value


<img src="https://static.transim.com/img/62019/11d2caf3937b4ce48817dd199f5379b2-fljwm.png" alt="Hardware Setup" width="640"/>



The signals when the Input voltage is at the maximum value are depicted in figure 8, it can be noticed how the rising event of the comparator starts even earlier than before, which means a smaller duty-cycle on the SMPS PWM. 
The change in duty-cycle happened without the change of the feedback signal (C1IN-) which in case means a feedforward adaptation.
 
Figure 8: Comparator Output when VIN is close to its maximum value


<img src="https://static.transim.com/img/62019/3cc5f11214ef43618334680731a6a847-13zlj.png" alt="Hardware Setup" width="640"/>



To address a measurement bandwidth limitation:
The PRG does not have a direct internal connection with an IO pin, so an OpAmp is used as unity gain to see the signal, but a limitation in internal signal bandwidth can be seen in the measurement process. 
In figure 9 the actual PRG signal superimposed over the measured OPA1OUT signal is depicted, and this can also be noticed in the comparator result that has as inputs the PRG and the FB signal. 
 
Figure 9: Internal BW limiting


<img src="https://static.transim.com/img/62019/aec872383f9140cdbdc5ed879bb10c02-fwcmy.png" alt="Hardware Setup" width="640"/>



**CONCLUSIONS**

The solution presented in this Example shows how to implement the feedforward function for voltage mode control in SMPS applications using the PIC16F176X/7X. 
The Programmable Ramp Generator peripheral in the PIC16F1 family is not equipped to implement slope change dependent to an external signal, 
so the new approach to feedforward is to maintain the slope of the ramp constant but change the starting point (Input voltage) of the sawtooth. 
This method provides the same result of changing the duty-cycle of the SMPS PWM, inverse proportionally to the input voltage variation, without waiting for the feedback signal reaction.
Applying this method to VMC will result in a faster control system response from input voltage variations. 
