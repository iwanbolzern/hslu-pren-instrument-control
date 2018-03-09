int calculateTicksToDrive(int distance) {

	int l = 0;

	l = (distance / (0.121));

	return l;
//return (uint8)(distance/TICK_DISTANCE);
}

void rollOut(float x, float v) {
	int speedWasSet = 0;

	while (counter < nTicks) {

		while (counter < (nTicks * x)) {

			if (!speedWasSet) {
				LED1_Neg();
				PWM1_SetRatio16(startSpeed * v);
				speedWasSet = 1;
			}

		}

		x = x + 0.1;
		v = v + 0.15;

		if ((startSpeed * v) < (MIN_SPEED * 0.9)) {
			rollOut(x, v);
		}

	}

}

void drive(direction_t direction, int ticks, int speed) {
	PWM1_Enable();
	setDirection(direction);
	vTaskDelay(pdMS_TO_TICKS(10));
	int internTicks = ticks;
	counter = 0;
	int newSpeed = 0;
	int internSpeed = (1 - ((float) speed / 100)) * MIN_SPEED;

	newSpeed = internSpeed;

	int ticksToGo = ticks;

//Try Nico
	startSpeed = internSpeed;
	nTicks = ticks;

	LED1_On();				// Debug

	rollOut(x, v);

// Fast STop initialisierung
	setDirection(REVERSE);
	PWM1_SetRatio16(0xffff * 0.3);
	vTaskDelay(pdMS_TO_TICKS(30));
	setDirection(FAST_STOP);

//

}

void rollout_slow(int speed, int ticks, direction_t direction) {

	int initialSpeed = speed;
	int ticksToGo = ticks;
	int flag = 0;
	float speedFactor = 1.0;
	float ticksToGoFactor = 0.3;
	int fullSpeedFlag = 0;
	int noMoreSpeedIncreaseFlag = 0;

	while (counter < ticksToGo) {

		while (counter < (ticksToGo * ticksToGoFactor)) {

			if (flag == 0) {
				PWM1_SetRatio16(initialSpeed * speedFactor);
				flag = 1;
			}

		}

		flag = 0;

		if ((speedFactor * initialSpeed) > MIN_SPEED * 0.8) {

			PWM1_SetRatio16(MIN_SPEED * 0.9);
			noMoreSpeedIncreaseFlag = 1;

			speedFactor = 0;

		} else {

			if (noMoreSpeedIncreaseFlag == 0) {
				speedFactor = speedFactor + 0.15;
				ticksToGoFactor = ticksToGoFactor + 0.1;
			}

		}

		vTaskDelay(pdMS_TO_TICKS(10));
	}

	setDirection(FAST_STOP);

	speedFactor = speedFactor * 100;
	ticksToGoFactor = ticksToGoFactor * 100;
	vTaskDelay(pdMS_TO_TICKS(3000));
	int k = 878;

}

void rollout_medium(int speed, int ticks, direction_t direction) {

	int initialSpeed = speed;
	int ticksToGo = ticks;
	int flag = 0;
	float speedFactor = 1.0;
	float ticksToGoFactor = 0.3;
	int fullSpeedFlag = 0;
	int noMoreSpeedIncreaseFlag = 0;

	while (counter < ticksToGo) {

		while (counter < (ticksToGo * ticksToGoFactor)) {

			if (flag == 0) {
				PWM1_SetRatio16(initialSpeed * speedFactor);
				flag = 1;
			}

		}

		flag = 0;

		if ((speedFactor * initialSpeed) > MIN_SPEED * 0.8) {

			PWM1_SetRatio16(MIN_SPEED * 0.9);
			noMoreSpeedIncreaseFlag = 1;

			speedFactor = 0;

		} else {

			if (noMoreSpeedIncreaseFlag == 0) {
				speedFactor = speedFactor + 0.15;
				ticksToGoFactor = ticksToGoFactor + 0.1;
			}

		}

		vTaskDelay(pdMS_TO_TICKS(1));
	}

	setDirection(FAST_STOP);

	speedFactor = speedFactor * 100;
	ticksToGoFactor = ticksToGoFactor * 100;
	vTaskDelay(pdMS_TO_TICKS(3000));
	int k = 878;

}

void rollout_fast(int speed, int ticks, direction_t direction) {

	int initialSpeed = speed;
	int ticksToGo = ticks;
	int flag = 0;
	float speedFactor = 1.0;
	float ticksToGoFactor = 0.3;
	int fullSpeedFlag = 0;
	int noMoreSpeedIncreaseFlag = 0;

	while (counter < ticksToGo) {

		while (counter < (ticksToGo * ticksToGoFactor)) {

			if (flag == 0) {
				PWM1_SetRatio16(initialSpeed * speedFactor);
				flag = 1;
			}

		}

		flag = 0;

		if ((speedFactor * initialSpeed) > MIN_SPEED * 0.8) {

			PWM1_SetRatio16(MIN_SPEED * 0.9);
			noMoreSpeedIncreaseFlag = 1;

			speedFactor = 0;

		} else {

			if (noMoreSpeedIncreaseFlag == 0) {
				speedFactor = speedFactor + 0.15;
				ticksToGoFactor = ticksToGoFactor + 0.1;
			}

		}

		vTaskDelay(pdMS_TO_TICKS(1));
	}

	setDirection(FAST_STOP);

	speedFactor = speedFactor * 100;
	ticksToGoFactor = ticksToGoFactor * 100;
	vTaskDelay(pdMS_TO_TICKS(3000));
	int k = 878;

}

void setDirection(direction_t d) {
