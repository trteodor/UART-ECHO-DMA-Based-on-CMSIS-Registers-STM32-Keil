# UART ECHO

A simple program that allows you to receive characters of different lengths using UART.
 The code is written in keil basically on the CMSIS library (registers). In C with structures and hopefully in good style

I Create this application to improve my skills programming in C

The receive buffer, after detecting the end of the transmission from the transmitter, is sent back to it. 
Interrupt termination detection is based on an idle interrupt generated by the UART module in the microcontroller


video with this program:
https://www.youtube.com/watch?v=pu14ABA_2io

![Visualisation](https://github.com/trteodor/UART-ECHO-DMA-Based-on-CMSIS-Registers-STM32-Keil/blob/master/images/EchoProgramBasedOnCMSISCMSIS.PNG)

ASCII Table Generator:
https://ozh.github.io/ascii-tables/

Escape sequences: 

https://bluesock.org/~willkg/dev/ansi.html

https://askubuntu.com/questions/831971/what-type-of-sequences-are-escape-sequences-starting-with-033

![Putty](https://github.com/trteodor/UART-ECHO-DMA-Based-on-CMSIS-Registers-STM32-Keil/blob/PutyyTests/images/PuttyTests.PNG)

