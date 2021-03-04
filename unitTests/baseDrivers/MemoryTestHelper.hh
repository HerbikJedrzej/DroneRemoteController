#include <queue>
#include <vector>
#include <nRF24MemoryMap.hh>
#include "TreeCell.hh"
#include <AT24C.hh>

// #define REZISTOR1 47000
// #define REZISTOR2 10000
// #define MAX_BATTERY 0x0763
// #define MIN_BATTERY 0x0425
// #define ADC_MASK 12

// class MemoryHelper : public Drivers::Memory{
// public:
// 	MemoryHelper();
// 	~MemoryHelper();
// 	bool write(uint16_t addr, uint8_t* data, uint16_t dataSize) override;
// 	bool read(uint16_t addr, uint8_t* data, uint16_t dataSize) override;
// 	void writeDMA(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish) override;
// 	void writeDMAwithoutDataAlocate(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish) override;
// 	bool readDMA(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish) override;
// 	bool init() override;
// 	void lockMemory() override;
// 	void unlockMemory() override;
// 	void addReadTest(uint16_t addr, std::vector<uint8_t> data);
// 	void addWriteTest(uint16_t addr, std::vector<uint8_t> data);
// 	void empty(bool check = true);
// 	void initFailure();
// 	void autoModeOn();
// 	const bool getLock();
// private:
// 	struct dataCell{
// 		uint16_t addr;
// 		std::vector<uint8_t> data;
// 	};
// 	std::queue<dataCell> sended;
// 	std::queue<dataCell> recived;
// protected:
// 	bool mainCheck(const bool& empty, const dataCell& cell, const uint16_t& addr, const uint16_t& dataSize);
// 	bool lock;
// 	bool autoMode;
// 	bool initialised;
// };