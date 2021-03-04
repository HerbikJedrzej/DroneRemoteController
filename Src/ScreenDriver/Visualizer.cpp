#include "Visualizer.hh"

Visualizer::Visualizer(LCD_ifc* _lcd, TreeCell<ScreenCellIfc>* rootCell, void (*_delay)(uint32_t)):
	ok(false),
	cancel(false),
	up(false),
	down(false),
	newSceanLoaded(true),
	filter{0, 0, 0, 0},
	lcd(_lcd),
	base(rootCell),
	delay(_delay){}

void Visualizer::printBar(const StateOfOperation& state, const bool& transsmit, const bool& recive, const uint8_t& battery, const uint8_t& deviceBatery){
	(*lcd)(0, 0);
	lcd->writeData(transsmit? 'T' : 'F');
	lcd->writeData(recive? 'R' : 'F');

	(*lcd)(0, 3);
	if(state == StateOfOperation::none)
		lcd->writeData(' ');
	else if(state == StateOfOperation::waiting)
		lcd->writeData('W');
	else if(state == StateOfOperation::succes)
		lcd->writeData('S');
	else if(state == StateOfOperation::failure)
		lcd->writeData('F');
	
	(*lcd)(0, 6);
	if(battery == 254)
		lcd->write((uint8_t*)"Err", 3);
	else if(battery == 255)
		lcd->write((uint8_t*)"NEx", 3);
	else
		lcd->writeU8(battery, 3);
	(*lcd)(0, 12);
	if(deviceBatery == 254)
		lcd->write((uint8_t*)"Err", 3);
	else if(deviceBatery == 255)
		lcd->write((uint8_t*)"NEx", 3);
	else
		lcd->writeU8(deviceBatery, 3);	
}

void Visualizer::run(const StateOfOperation& state, const bool& transsmit, const bool& recive, const uint8_t& battery, const uint8_t& deviceBatery){
	for(uint8_t i = 0; i < 4; i++)
		if(filter[i] > 0)
			filter[i]--;
	transmitFilter <<= 1;
	reciveFilter <<= 1;
	if(transsmit)
		transmitFilter |= 1;
	if(recive)
		reciveFilter |= 1;
	printBar(state, transmitFilter != 0, reciveFilter != 0, battery, deviceBatery);
	unsigned int size = base.getCell()->getSize();
	if(!newSceanLoaded){
		if(size == 0)
			base.getCell()->getVal()->refresh();
		return;
	}
	newSceanLoaded = false;
	if(size == 0){
		base.getCell()->getVal()->print();
		return;
	}
	const uint8_t* toPrint = nameToPrint(base.getCell()->getPointed()->getVal());
	(*lcd)(2, 0); lcd->writeData('>'); lcd->write(toPrint, 15);
	if(size == 1){
		(*lcd)(1, 0); lcd->write((uint8_t*)"                ", 16);
		(*lcd)(3, 0); lcd->write((uint8_t*)"                ", 16);
		return;
	}
	toPrint = nameToPrint(base.getCell()->getNextOtp()->getVal());
	(*lcd)(3, 0); lcd->writeData(' '); lcd->write(toPrint, 15);
	if(size == 2){
		(*lcd)(1, 0); lcd->write((uint8_t*)"                ", 16);
		return;
	}
	toPrint = nameToPrint(base.getCell()->getPrevOtp()->getVal());
	(*lcd)(1, 0); lcd->writeData(' '); lcd->write(toPrint, 15);
}

void Visualizer::init(){
	(*lcd)(0, 0);
	lcd->write((uint8_t*)"TR F Rxxx% Dxxx%", 16);
}

const uint8_t* Visualizer::nameToPrint(ScreenCellIfc const* cell){
	static uint8_t toPrint[15];
	for(uint8_t i = 0; i < cell->getNameSize(); i++)
		toPrint[i] = cell->getName()[i];
	for(uint8_t i = cell->getNameSize(); i < 15; i++)
		toPrint[i] = ' ';
	return toPrint;
}

void Visualizer::Up(){
	if(filter[filterID::UP_DOWN] > 0)
		return;
	filter[filterID::UP_DOWN] = 1;
	newSceanLoaded = base.incrementOtp();
	if(!newSceanLoaded)
		base.getCell()->getVal()->increment();
}

void Visualizer::Down(){
	if(filter[filterID::UP_DOWN] > 0)
		return;
	filter[filterID::UP_DOWN] = 1;
	newSceanLoaded = base.decrementOtp();
	if(!newSceanLoaded)
		base.getCell()->getVal()->decrement();
}

void Visualizer::Ok(){
	if(filter[filterID::OK] > 0)
		return;
	filter[filterID::OK] = 50;
	newSceanLoaded = base.incrementDir();
	if(!newSceanLoaded)
		base.getCell()->getVal()->execute();
}

void Visualizer::Cancel(){
	if(filter[filterID::CANCEL] > 0)
		return;
	filter[filterID::CANCEL] = 50;
	newSceanLoaded = base.decrementDir();
}

void Visualizer::Change(){
	if(filter[filterID::CHANGE] > 0)
		return;
	filter[filterID::CHANGE] = 50;
	if(base.getCell()->getVal())
		base.getCell()->getVal()->change();
}
