#include "global.h"

extern TaskFunction_t initTaskMain;
void USER_THREADS( void );

int main()
{
	USER_THREADS();
	vTaskStartScheduler();
	while (1) ;
	return 0;
}
