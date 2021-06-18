#include "PrintToBuf.h"
#include "USART_UART_DMA.h"

extern UART_DMA_Handle_Td TUART2;

volatile uint32_t shortblocke=0;

void _putchar(char character)
{
  TUART_DMA_Trasmit(&TUART2,(uint8_t*)&character);
	for(shortblocke=1000; shortblocke; shortblocke--){}; //anyway veryfast soultion ;) but also fatal :D 
}


