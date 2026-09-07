#include "stepper.h"


//Functions//

              //Stepper_StartPWM//

static void Stepper_StartPWM(Stepper_t *m, uint32_t freq)
{
    uint32_t arr;
    
        if(freq == 0)
    {
        m->state = STEP_IDLE;
        return;
    }

    arr = (1000000UL / freq) - 1;

    __HAL_TIM_SET_AUTORELOAD(m->htim, arr);
    __HAL_TIM_SET_COUNTER(m->htim, 0);
    __HAL_TIM_SET_COMPARE(m->htim, m->channel, (arr + 1) / 2);
    
    HAL_TIM_PWM_Start_IT(m->htim, m->channel);
}

              //Stepper_Init//

void Stepper_Init(Stepper_t *m, TIM_HandleTypeDef *htim, uint32_t channel, GPIO_TypeDef *DIR_Port, uint16_t DIR_Pin, GPIO_TypeDef *EN_Port, uint16_t EN_Pin)
{
    m->htim = htim;
    m->channel = channel;

    m->DIR_Port = DIR_Port;
    m->DIR_Pin = DIR_Pin;

    m->EN_Port = EN_Port;
    m->EN_Pin = EN_Pin;

    m->position = 0;

    m->stepCount = 0;
    m->targetStep = 0;

    m->state = STEP_IDLE;
}

              //Stepper_Enable//


void Stepper_Enable(Stepper_t *m)
{
    HAL_GPIO_WritePin(m->EN_Port, m->EN_Pin, GPIO_PIN_RESET);
}

              //Stepper_Disable//

void Stepper_Disable(Stepper_t *m)
{
    HAL_GPIO_WritePin(m->EN_Port, m->EN_Pin, GPIO_PIN_SET);
}

              //Stepper_SetDirection//

void Stepper_SetDirection(Stepper_t *m, StepperDir_t dir)
{
    m->direction = dir;

    HAL_GPIO_WritePin(m->DIR_Port, m->DIR_Pin, (dir == DIR_CW) ? GPIO_PIN_SET : GPIO_PIN_RESET);
             
}

              //Stepper_MoveContinuous//

void Stepper_MoveContinuous(Stepper_t *m, uint32_t freq)
{
    m->targetStep = 0;
    m->stepCount = 0;
    m->state = STEP_RUN;

    Stepper_StartPWM(m, freq);
}

              //Stepper_MoveSteps//

void Stepper_MoveSteps(Stepper_t *m, uint32_t steps, uint32_t freq)
{
    m->targetStep = steps;
    m->stepCount = 0;
    m->state = STEP_RUN;

    Stepper_StartPWM(m, freq);
}

              //Stepper_Stop//

void Stepper_Stop(Stepper_t *m)
{
    HAL_TIM_PWM_Stop_IT(m->htim, m->channel);

    m->state = STEP_IDLE;
}

              //Stepper_Busy//

bool Stepper_Busy(Stepper_t *m)
{
    return (m->state == STEP_RUN);
}

              //Stepper_IRQHandler//

void Stepper_IRQHandler(Stepper_t *m)
{
    if(m->state != STEP_RUN)
        return;

    m->stepCount++;
  
    if(m->direction == DIR_CW)
    {
        m->position++;
    }
    else
    {
        m->position--;
    }

    if(m->targetStep == 0)
        return;

    uint32_t current = m->stepCount;
    uint32_t target  = m->targetStep;

if(current >= target)
{
    Stepper_Stop(m);
    m->state = STEP_DONE;
}
}
