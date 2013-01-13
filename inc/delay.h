#ifndef DELAY_H
#define DELAY_H


extern volatile uint32_t system_time;

void init_systick(void);
void delay_ms(uint32_t ms);


#endif
