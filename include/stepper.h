#ifndef STEPPER_H
#define STEPPER_H

#include "main.h"
#include <stdbool.h>

//StepperState//

typedef enum
{
    STEP_IDLE = 0,
    STEP_RUN,
    STEP_DONE
}StepperState_t;

//StepperDir//

typedef enum
{
    DIR_CW = 0,
    DIR_CCW
}StepperDir_t;

//stepper Items//

typedef struct
{
    TIM_HandleTypeDef *htim;
    uint32_t channel;

    GPIO_TypeDef *DIR_Port;
    uint16_t DIR_Pin;

    GPIO_TypeDef *EN_Port;
    uint16_t EN_Pin;

    volatile int32_t position;

    volatile uint32_t stepCount;
    uint32_t targetStep;

    StepperDir_t direction;

    volatile StepperState_t state;

}Stepper_t;

//Headers//

void Stepper_Init(Stepper_t *m, TIM_HandleTypeDef *htim, uint32_t channel, GPIO_TypeDef *DIR_Port, uint16_t DIR_Pin, GPIO_TypeDef *EN_Port, uint16_t EN_Pin);

void Stepper_Enable(Stepper_t *m);

void Stepper_Disable(Stepper_t *m);

void Stepper_SetDirection(Stepper_t *m,StepperDir_t dir);

void Stepper_MoveSteps(Stepper_t *m,uint32_t steps,uint32_t freq);

void Stepper_MoveContinuous(Stepper_t *m,uint32_t freq);

void Stepper_Stop(Stepper_t *m);

bool Stepper_Busy(Stepper_t *m);

void Stepper_IRQHandler(Stepper_t *m);

#endif
