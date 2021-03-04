#pragma once

#include "Tree.hh"
#include "Screens/ScreenCellIfc.hh"

class Visualizer{
public:
	enum StateOfOperation{
		none = 0,
		waiting,
		succes,
		failure
	};
	Visualizer(LCD_ifc* _lcd, TreeCell<ScreenCellIfc>* rootCell, void (*_delay)(uint32_t));
	void run(const StateOfOperation& state, const bool& transsmit, const bool& recive, const uint8_t& battery, const uint8_t& deviceBatery);
	void printBar(const StateOfOperation& state, const bool& transsmit, const bool& recive, const uint8_t& battery, const uint8_t& deviceBatery);
	void Up();
	void Down();
	void Ok();
	void Change();
	void Cancel();
	void init();
protected:
	const uint8_t* nameToPrint(ScreenCellIfc const* cell);
private:
	bool ok;
	bool cancel;
	bool up;
	bool down;
	bool newSceanLoaded;
	uint8_t transmitFilter = {0};
	uint8_t reciveFilter = {0};
	enum filterID{
		UP_DOWN = 0,
		OK,
		CANCEL,
		CHANGE
	};
	uint8_t filter[4];
	LCD_ifc* lcd;
	Tree<ScreenCellIfc> base;
	void (*delay)(uint32_t);
};
