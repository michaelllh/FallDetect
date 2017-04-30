#ifndef _SENSORTILE_OLEDSHOW_H_
#define _SENSORTILE_OLEDSHOW_H_

typedef enum {
	DeviceName = 1,
	SensorData,
	Loading,
	FallDetect,
}Page;

void SensorTile_oledShow(Page info);

#endif


