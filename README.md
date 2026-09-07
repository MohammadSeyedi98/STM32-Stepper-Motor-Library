# STM32 Stepper Motor Library

A lightweight non-blocking stepper motor driver library for STM32 using HAL timers and PWM interrupts.

## Features

- Non-blocking motor control
- PWM-based STEP generation
- Continuous rotation
- Fixed-step movement
- Position tracking
- Direction control
- Enable/Disable driver

## Folder Structure

include/
src/
eww/

## Supported Drivers

- TMC2225
- A4988
- DRV8825

## Example

Stepper_Init(...);
Stepper_Enable(...);
Stepper_SetDirection(...);
Stepper_MoveSteps(...);

