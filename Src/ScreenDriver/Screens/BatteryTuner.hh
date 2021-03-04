#pragma once

#include "ScreenCellIfc.hh"
#include "BatteryObserver.hh"
#include "TreeCell.hh"
#include <RadioParser.hh>

/*
Interface of radio data:
	Main option:
	27 = set voltage to tune resistor in remote device. uint8 value / 10 = valtage [V]
*/

TreeCell<ScreenCellIfc>* CreateBatteryScreenDirectory(TreeCell<ScreenCellIfc>* root, LCD_ifc* const lcd, BatteryObserver* const battery, Drivers::RadioParser* const radio);

class BatteryValue : public ScreenCellIfc{
	static constexpr char name[]{"Battery value"};
	BatteryObserver* const battery;
public:
    BatteryValue(LCD_ifc* const _lcd, BatteryObserver* const _battery);
    const uint8_t* getName() const final;
	uint8_t getNameSize() const final;
	void print() final;
	void refresh() final;
	void execute() final;
	void change() final;
	void increment() final{}
	void decrement() final{}
};

class ControledDeviceBatterySaveValue : public ScreenCellIfc{
	static constexpr char name[]{"Tune rem. dev."};
	Drivers::RadioParser* const radio;
	uint8_t voltage; // 10 = 1.0V
public:
    ControledDeviceBatterySaveValue(LCD_ifc* const _lcd, Drivers::RadioParser* const _radio);
    const uint8_t* getName() const final;
	uint8_t getNameSize() const final;
	void print() final;
	void refresh() final;
	void execute() final;
	void change() final;
	void increment() final;
	void decrement() final;
};

class BatterySaveValue : public ScreenCellIfc{
	static constexpr char name[]{"Tune resistor"};
	BatteryObserver* const battery;
	bool mode9V;
public:
    BatterySaveValue(LCD_ifc* const _lcd, BatteryObserver* const _battery);
    const uint8_t* getName() const final;
	uint8_t getNameSize() const final;
	void print() final;
	void refresh() final;
	void execute() final;
	void change() final;
	void increment() final{}
	void decrement() final{}
};

class BatterySaveMinMax : public ScreenCellIfc{
	static constexpr char name[]{"Save min/max"};
	BatteryObserver* const battery;
	uint8_t option;
public:
    BatterySaveMinMax(LCD_ifc* const _lcd, BatteryObserver* const _battery);
    const uint8_t* getName() const final;
	uint8_t getNameSize() const final;
	void print() final;
	void refresh() final;
	void execute() final;
	void change() final{}
	void increment() final;
	void decrement() final;
};