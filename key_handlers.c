#include "include/key_handlers.h"


control_keys pressed_keys = (control_keys){0, 0, 0, 0, 0, 0, 0};


void movement_by_keys(direction* movement, bool* move)
{
    bool move_horizontal = pressed_keys.left ^ pressed_keys.right;
    bool move_vertical = pressed_keys.up ^ pressed_keys.down;
    if (!move_horizontal && !move_vertical)
    {
        *move = false;
        return;
    }
    *move = true;

    if (move_horizontal && move_vertical)
    {
        if (pressed_keys.up && pressed_keys.left)
        {
            *movement = DOWN_RIGHT;
        }
        else if (pressed_keys.down && pressed_keys.left)
        {
            *movement = UP_RIGHT;
        }
        else if (pressed_keys.up && pressed_keys.right)
        {
            *movement = DOWN_LEFT;
        }
        else if (pressed_keys.down && pressed_keys.right)
        {
            *movement = UP_LEFT;
        }
        return;
    }

    if (move_vertical)
    {
        if (pressed_keys.up)
        {
            *movement = DOWN;
        }
        else if (pressed_keys.down)
        {
            *movement = UP;
        }
    }
    else if (move_horizontal)
    {
        if (pressed_keys.left)
        {
            *movement = RIGHT;
        }
        else if (pressed_keys.right)
        {
            *movement = LEFT;
        }
    }
}


void keydown_handler(SDL_Keycode key, bool* pause, direction* movement, bool* move)
{
    switch (key)
    {
        case SDLK_SPACE:
        {
            *pause = !*pause;
            break;
        }
        case SDLK_UP:
        {
            pressed_keys.up = true;
            break;
        }
        case SDLK_DOWN:
        {
            pressed_keys.down = true;
            break;
        }
        case SDLK_LEFT:
        {
            pressed_keys.left = true;
            break;
        }
        case SDLK_RIGHT:
        {
            pressed_keys.right = true;
            break;
        }
        case SDLK_RSHIFT:
        {
            pressed_keys.shift = true;
            break;
        }
        case SDLK_LSHIFT:
        {
            pressed_keys.shift = true;
            break;
        }
        case SDLK_LCTRL:
        {
            pressed_keys.control = true;
            break;
        }
        case SDLK_RCTRL:
        {
            pressed_keys.control = true;
            break;
        }
        case SDLK_LALT:
        {
            pressed_keys.alt = true;
            break;
        }
        case SDLK_RALT:
        {
            pressed_keys.alt = true;
            break;
        }
        
        default:
        {
            break;
        }
    }
    movement_by_keys(movement, move);
}


void keyup_handler(SDL_Keycode key, bool* pause, direction* movement, bool* move)
{
    switch (key)
    {
        case SDLK_UP:
        {
            pressed_keys.up = false;
            break;
        }
        case SDLK_DOWN:
        {
            pressed_keys.down = false;
            break;
        }
        case SDLK_LEFT:
        {
            pressed_keys.left = false;
            break;
        }
        case SDLK_RIGHT:
        {
            pressed_keys.right = false;
            break;
        }
        case SDLK_RSHIFT:
        {
            pressed_keys.shift = false;
            break;
        }
        case SDLK_LSHIFT:
        {
            pressed_keys.shift = false;
            break;
        }
        case SDLK_LCTRL:
        {
            pressed_keys.control = false;
            break;
        }
        case SDLK_RCTRL:
        {
            pressed_keys.control = false;
            break;
        }
        case SDLK_LALT:
        {
            pressed_keys.alt = false;
            break;
        }
        case SDLK_RALT:
        {
            pressed_keys.alt = false;
            break;
        }
        
        default:
        {
            break;
        }
    }
    movement_by_keys(movement, move);
}
