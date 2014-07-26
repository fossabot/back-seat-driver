/*
 * BackSeatDriver.h
 *
 *  Created on: Jul 16, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#ifndef BACKSEATDRIVER_H
#define BACKSEATDRIVER_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include "BackSeatDriver_IMotorAdapter.h"

#define LOG_BUFFER_LEN 50
#define MIN_DEBUG_LOG_FREQ 50

typedef void(*maneuverCallback)(void);

typedef struct maneuverStruct {
	unsigned long durationMs;
	unsigned long startMs;
	maneuverCallback callback;
	bool running;
} maneuver;


class BackSeatDriver {
public:
	BackSeatDriver(BackSeatDriver_IMotorAdapter *adapter);

	void attach();
	void detach();
 	void goForward(uint8_t speedPercent);
 	void goForward(uint8_t speedPercent,
 			unsigned int durationMs,
 			maneuverCallback callback);
	void goBackward(uint8_t speedPercent);
	void goBackward(uint8_t speedPercent,
			unsigned int durationMs,
			maneuverCallback callback);
	void turn(int angle,
			maneuverCallback callback);
	void stop();
	bool isMoving();
	bool isManeuvering();
	void debug(bool debugEnabled);

	// each adapter provides it's own nominal speed of movement and turning,
	// however individual robots may need adjustment
	void adjustMovement(
		// 0 < .. < 10,000, where 100 = no change, 50 = half speed, 200 is double, etc.
		unsigned short movingSpeedPercentCoefficient,
		// 0 < .. < 10,000, where 100 = no change, 50 = half speed, 200 is double, etc.
		unsigned short turningSpeedPercentCoefficient);

private:
	unsigned short _movingSpeedPercent, _turningSpeedPercent;
	BackSeatDriver_IMotorAdapter *_adapter;
	// positive = forward, negative = backward
	signed short _currentSpeedPercent;
	maneuver _maneuver;
	char _logBuffer[LOG_BUFFER_LEN];
	unsigned long _initMs, _lastDebugMs;

	bool _debug;

	void moveAtSpeed(signed short leftPercent, signed short rightPercent);
	void moveAtCurrentSpeed();
	void checkManeuveringState();
	void startManeuverTimer(unsigned int durationMs,
			maneuverCallback callback);
	void stopManeuverTimer();
	void log(void);
};

#endif /* BACKSEATDRIVER_H */