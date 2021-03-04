#include "InterfacesConf.hh"

using Drivers::BusStatus;

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *){
	Drivers::i2c1.handleFinish(nullptr);
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *){
	Drivers::i2c1.handleFinish(nullptr);
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *){
	Drivers::spi2.handleFinish(nullptr);
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *){
	Drivers::spi2.handleError(nullptr);
}

void HAL_SPI_AbortCpltCallback(SPI_HandleTypeDef *){
	Drivers::spi2.handleAbort(nullptr);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == RADIO_IRQ_Pin){
		Drivers::gpio.setChangedPin(InterruptInputList::RadioIRQ);
		Drivers::gpio.handleFinish(nullptr);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim6){
		Drivers::doCircle = true;
		Drivers::time++;
		Drivers::gpio.handleTimeEvent(nullptr);
		Drivers::spi2.handleTimeEvent(nullptr);
		Drivers::i2c1.handleTimeEvent(nullptr);
	}
}
