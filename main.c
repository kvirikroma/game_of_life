#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "include/utils.h"
#include "include/main.h"


uint32_t step_delay;  // recommended minimum is 20
io_threader threader;


int main()
{
    step_delay = 50;

    io_threader_init(&threader, 1600, 900, 512, 288);
    sleep_ms(10);

    while (threader.eventlistener.run)
    {
        int64_t start_ms = get_current_millisecond();
        
        while (threader.eventlistener.pause && threader.eventlistener.run)
        {
            sleep_ms(1);
        }
        if (!threader.eventlistener.lmb_pressed && !threader.eventlistener.rmb_pressed)
        {
            if (!threader.eventlistener.pause)
            {
                io_threader_lock_drawer(&threader);
                for (uint8_t step = 0; step < round(pow(1.435, threader.eventlistener.speed)); step++)
                {
                    life_runner_make_step(&threader.drawer.game);
                    if ((step % 7) == 1)
                    {
                        life_drawer_redraw(&threader.drawer);
                        event_listener_listen(&threader.eventlistener, &threader);
                        event_listener_apply_movement(&threader.eventlistener, &threader, false);
                    }
                    if (!threader.eventlistener.run)
                    {
                        break;
                    }
                }
                io_threader_unlock_drawer(&threader);
            }
        }

        if (!threader.eventlistener.move || threader.eventlistener.moved_once)
        {
            int64_t msec_total = get_current_millisecond() - start_ms;
            if (((int64_t)step_delay - msec_total) > 2)
            {
                sleep_ms(((int64_t)step_delay - msec_total) / (1 << (threader.eventlistener.speed - 1)));
            }
            else
            {
                sleep_ms(2);
            }
            
        }
    }

    io_threader_delete(&threader);
    return 0;
}
