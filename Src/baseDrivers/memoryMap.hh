#pragma once

enum memoryMap{
	// BEGIN initial test 0x0000
	// END initial test 0x0009

	// BEGIN battery 0x000A
		minOfBatteryMSB = 0x000A,
		minOfBatteryLSB,
		maxOfBatteryMSB,
		maxOfBatteryLSB,
		settings = 0x000E, // bit 7 - auto tune of min value (1 - on, 0 - off) / bit 4 - 0 - resolution of adc
	// END battery 0x000E

	// BEGIN adcRezistors 0x000F
		rezistor1h = 0x000F,
		rezistor1m,
		rezistor1l = 0x0011, // (uint) resistance = rezistorh | rezistorm | rezistorl
	// END adcRezistors 0x0011

	// BEGIN Measurements of math model 0x0012
		trybeOfMainloop = 0x0012,
		slot_1_sinusAmplitude,
		slot_1_sinusFrequency,
		slot_2_sinusAmplitude,
		slot_2_sinusFrequency,
		slot_3_sinusAmplitude,
		slot_3_sinusFrequency,
		slot_4_sinusAmplitude,
		slot_4_sinusFrequency,
		slot_5_sinusAmplitude,
		slot_5_sinusFrequency = 0x001C,
	// END Measurements of math model 0x001C

	// BEGIN PID parameters 0x001D
		commonFactorForAxisXY = 0x001D,
		pid_P_XY_00,
		pid_I_XY_00,
		pid_D_XY_00,
		pid_P_XY_01,
		pid_I_XY_01,
		pid_D_XY_01,
		pid_P_XY_10,
		pid_I_XY_10,
		pid_D_XY_10,
		pid_P_XY_11,
		pid_I_XY_11,
		pid_D_XY_11,
		pid_P_Z,
		pid_I_Z,
		pid_D_Z,
		pid_P_H,
		pid_I_H,
		pid_D_H = 0x002F,
	// END Measurements of math model 0x002F

	// BEGIN Angle offset of axises X and Y 0x0030
		angleOffsetX = 0x0030,
		angleOffsetY = 0x0031,
	// END Angle offset of axises X and Y 0x0031
};
