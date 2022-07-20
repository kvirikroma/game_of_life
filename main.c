#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

#include "include/utils.h"
#include "include/main.h"


uint32_t step_delay;  // recommended minimum is 45
io_threader threader;


static void signal_handler(int code)
{
    threader.input.run = false;
    io_threader_delete(&threader);
    exit(code);
}


int main()
{
    step_delay = 180;
    int64_t msec_total = 0;

    io_threader_init(&threader, 1600, 900, 512, 288);
    while(!threader.threads_started)
    {
        sleep_ms(1);
    }
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    while (threader.input.run)
    {
        int64_t start_ms = get_current_millisecond();
        int64_t actual_step_delay = step_delay;
        if (threader.input.speed)
        {
            actual_step_delay /= threader.input.speed;
        }
        
        if (!threader.input.lmb_pressed && !threader.input.rmb_pressed)
        {
            if (!threader.input.pause)
            {
                while (!threader.redrawed);
                io_threader_lock_drawer(&threader);
                if (((double)actual_step_delay * 1.4) > msec_total)
                {
                    life_runner_make_step(&threader.drawer.game);
                    threader.redrawed = false;
                }
                else
                {
                    for (uint8_t step = 0; step < round(pow(1.432, threader.input.speed)); step++)
                    {
                        life_runner_make_step(&threader.drawer.game);
                        threader.redrawed = false;
                        if ((step % 6) == 1)
                        {
                            event_listener_listen(&threader.input, &threader);
                            event_listener_apply_movement(&threader.input, &threader, false);
                        }
                        if (!threader.input.run)
                        {
                            break;
                        }
                    }
                }
                io_threader_unlock_drawer(&threader);
            }
        }

        if (!threader.input.move || threader.input.moved_once)
        {
            msec_total = get_current_millisecond() - start_ms;
            if (((int64_t)actual_step_delay - msec_total) > 2)
            {
                sleep_ms(((int64_t)actual_step_delay - msec_total) / (1 << (threader.input.speed - 1)));
            }
        }
        if (threader.input.pause && !(threader.input.lmb_pressed || threader.input.rmb_pressed) && !threader.input.move)
        {
            sleep_ms(200);
        }
    }

    io_threader_delete(&threader);
    return 0;
}

// int main()
// {
//     life_runner runner;
//     life_runner_init(&runner, 512, 288);
//     while (true)
//     {
//         int start_msec = get_current_millisecond();

//         for (uint8_t i = 0; i < 64; i++)
//         {
//             life_runner_make_step(&runner);
//         }

//         printf("Total milliseconds: %d\n", (int)(get_current_millisecond() - start_msec));
//     }

//     return 0;
// }
