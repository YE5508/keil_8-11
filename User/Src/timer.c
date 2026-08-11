#include "timer.h"


void timer_start(timer* timer)
{
    timer->start_time = HAL_GetTick();
    timer->running = 1;

}
bool timer_is_expired (timer* timer,uint8_t duration)
{
    timer->duration = duration;
    if(timer->running == 1 && (HAL_GetTick()-timer->start_time)>=timer->duration)
    {
        timer->running = 0;
        return true;
    }
    return false;
}
