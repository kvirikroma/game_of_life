#include "include/key_handler.h"


void movement_by_keys(key_handler* self)
{
    bool move_horizontal = self->pressed_keys.left ^ self->pressed_keys.right;
    bool move_vertical = self->pressed_keys.up ^ self->pressed_keys.down;
    if (!move_horizontal && !move_vertical)
    {
        *self->move = false;
        return;
    }
    *self->move = true;

    if (move_horizontal && move_vertical)
    {
        if (self->pressed_keys.up && self->pressed_keys.left)
        {
            *self->movement = DOWN_RIGHT;
        }
        else if (self->pressed_keys.down && self->pressed_keys.left)
        {
            *self->movement = UP_RIGHT;
        }
        else if (self->pressed_keys.up && self->pressed_keys.right)
        {
            *self->movement = DOWN_LEFT;
        }
        else if (self->pressed_keys.down && self->pressed_keys.right)
        {
            *self->movement = UP_LEFT;
        }
        return;
    }

    if (move_vertical)
    {
        if (self->pressed_keys.up)
        {
            *self->movement = DOWN;
        }
        else if (self->pressed_keys.down)
        {
            *self->movement = UP;
        }
    }
    else if (move_horizontal)
    {
        if (self->pressed_keys.left)
        {
            *self->movement = RIGHT;
        }
        else if (self->pressed_keys.right)
        {
            *self->movement = LEFT;
        }
    }
}


void key_handler_init(key_handler* self, bool* pause, direction* movement, bool* move, uint8_t* speed)
{
    self->pressed_keys = (control_keys){0, 0, 0, 0, 0, 0, 0};
    self->pause = pause;
    self->movement = movement;
    self->move = move;
    self->speed = speed;
}


void key_handler_down(key_handler* self, SDL_Keycode key)
{
    switch (key)
    {
        case SDLK_SPACE:
            *self->pause = !*self->pause;
            if (*self->speed == 0)
                *self->speed = 1;
            break;
        case SDLK_UP:
            self->pressed_keys.up = true;
            break;
        case SDLK_DOWN:
            self->pressed_keys.down = true;
            break;
        case SDLK_LEFT:
            self->pressed_keys.left = true;
            break;
        case SDLK_RIGHT:
            self->pressed_keys.right = true;
            break;
        case SDLK_RSHIFT:
            self->pressed_keys.shift = true;
            break;
        case SDLK_LSHIFT:
            self->pressed_keys.shift = true;
            break;
        case SDLK_LCTRL:
            self->pressed_keys.control = true;
            break;
        case SDLK_RCTRL:
            self->pressed_keys.control = true;
            break;
        case SDLK_LALT:
            self->pressed_keys.alt = true;
            break;
        case SDLK_RALT:
            self->pressed_keys.alt = true;
            break;
        case SDLK_1:
            *self->speed = 1;
            break;
        case SDLK_2:
            *self->speed = 2;
            break;
        case SDLK_3:
            *self->speed = 3;
            break;
        case SDLK_4:
            *self->speed = 4;
            break;
        case SDLK_5:
            *self->speed = 5;
            break;
        case SDLK_6:
            *self->speed = 6;
            break;
        case SDLK_7:
            *self->speed = 7;
            break;
        case SDLK_8:
            *self->speed = 8;
            break;
        case SDLK_9:
            *self->speed = 9;
            break;
        
        default:
            break;
    }
    movement_by_keys(self);
}


void key_handler_up(key_handler* self, SDL_Keycode key)
{
    switch (key)
    {
        case SDLK_UP:
            self->pressed_keys.up = false;
            break;

        case SDLK_DOWN:
            self->pressed_keys.down = false;
            break;

        case SDLK_LEFT:
            self->pressed_keys.left = false;
            break;

        case SDLK_RIGHT:
            self->pressed_keys.right = false;
            break;

        case SDLK_RSHIFT:
            self->pressed_keys.shift = false;
            break;

        case SDLK_LSHIFT:
            self->pressed_keys.shift = false;
            break;

        case SDLK_LCTRL:
            self->pressed_keys.control = false;
            break;

        case SDLK_RCTRL:
            self->pressed_keys.control = false;
            break;

        case SDLK_LALT:
            self->pressed_keys.alt = false;
            break;

        case SDLK_RALT:
            self->pressed_keys.alt = false;
            break;
        
        default:
            break;
    }
    movement_by_keys(self);
}
