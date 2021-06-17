#include "main.h"

//License: hmm BeerWare ;) 

// Copyright (c) 2021 Teodor Rosolowski
// trteodor@gmail.com

/* Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define LED1_SET     GPIOA->GPIO_ODR_ODR1=1;   //funkcja z argumentami ktore ustawia diode
#define LED1_RESET 	 GPIOA->GPIO_ODR_ODR1=0;
volatile uint32_t SoftTimer;


uint8_t rx_BUF[1655]="ASDFGHH";
UART_DMA_Handle_Td TUART2;  
//@@@@

#pragma   arm section code = "RAMCODE" //some tricks
void TIM1_Cnf()   //timer on 500ms
{	
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
		TIM1->PSC = 639;
		TIM1->ARR = 1000; //config on 100ms interrupt
	TIM1->DIER |= TIM_DIER_UIE;
		TIM1->CR1 |= TIM_CR1_CEN;	
		NVIC_EnableIRQ(TIM1_UP_IRQn);
}
#pragma arm section

int main(void) 
{
	//Frist config the PLL
	PLL_Config64MHZ();
	//SysTick
	SysTick_Config(64000/8);
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk; // SysTickClk/8
	//Enable the GPIOA and GPIOB port
	//	GPIO_Pin_Cfg(GPIOA,Px5,gpio_mode_output_PP_2MHz);		
	
	//Config UART Pins
	GPIO_Pin_Cfg(GPIOA,Px2, gpio_mode_alternate_PP_10MHz);		
	GPIO_Pin_Cfg(GPIOA,Px3, gpio_mode_alternate_PP_10MHz);		
	
	//Config Struct for UART ECHO with DMA
	TUART2.Instance=USART2;
	TUART2.UART_DMA_RX_CHANNEL=6;
	TUART2.UART_DMA_TX_CHANNEL=7;
	TUART2.NbofRecData=0;
	TUART2.ubNbDataToTransmit=0;
	TUART2.ubReceptionComplete=1;  //This must be 1 initialized
	TUART2.ubTransmissionComplete=1; //This must be 1 initialized
	TUART2.NbDataToReceive=1655;
	TUART2.ECHO_Mode=1;
	
		USARTx_DMA_Config(&TUART2);
		TUART_DMA_Trasmit(&TUART2, (uint8_t*)
"\n\n\n\n"
" +=========+=========+============+========+=========+========+ \n"
" |  Ten    | Program |   Dziala   |  Jako  |  Uart   |  ECHO  | \n"
" +=========+=========+============+========+=========+========+ \n"
" | Polecam | Rowniez | Generator  | Tabel  | Asii    |        | \n"
" +---------+---------+------------+--------+---------+--------+ \n"
" | Bardzo  | Latwo   | Sie        | w nim  | Tworzy  | Tabele | \n"
" +---------+---------+------------+--------+---------+--------+ \n"
"https://ozh.github.io/ascii-tables/ \n\n\n\r"
"Wprowadz Swoja Wiadomosc >"
		 );
	  TUART_DMA_Receive(&TUART2, rx_BUF, 1655);   //the receiving buffer address must be indicated and listening must be turned on
			/*This program now saves the received string in the receiving buffer, and after
			detecting the end of the transmission by the IDLE flag, returns the received message back to the sender */
			
	  TIM1_Cnf();
		
	 uint32_t zT_LED=0;
																	//napisalem to tylko dla pokazania funkcji debuggera do video... ten kod jest nie poprawny!
																				//brakuje odpowiedniej inicjalizacji pinow wyjsciowych, wgl piny 2i3 wykorzystuje uart...
	 	 GPIOA->ODR &=((uint32_t)(0xffffff00));  //zeruje dolne bity portu wyjsciowego! 		
			GPIOA->ODR=1;
			uint8_t helper=0;
		 
while(1)
 {
		if(  (SysTime-zT_LED) > 500)  //SysTick Handler and the Variable SysTime Definded in RCC_Config (its software timer now)
	{
		zT_LED=SysTime;	
		if(helper==0)
		{
			GPIOA->ODR =(GPIOA->ODR&(0xffffff00)) | ((GPIOA->ODR&(0x000000FF)) << 1);	
		}
	 if( ((GPIOA->ODR & 0x000000FF) == 0x80) || helper ) //jesli swieci ostatnia dioda
	 {
			if(helper==0)
			{
				helper=1;
			}
			else{
				helper=0;
			GPIOA->ODR &=((uint32_t)(0xffffff00)); //zeruje
				GPIOA->ODR|=(0x00000001);
			}

	 }
	}
 }
}

void TIM1_UP_IRQHandler(void){
 if (TIM1->SR & TIM_SR_UIF){
TIM1->SR = ~TIM_SR_UIF;
//tooglePIN(GPIOA,Psm5);
	 SoftTimer++;
 }
}


