# game_of_life
In this project i try to implement the "game of life" in C and assembly. Just for fun (=

<b>Instruction:</b><br>
<b>Keyboard:</b><br>
<pre>
c - clear the game field
space - pause
s - save game to "saved_game.life"
l - load game from "saved_game.life"
q (or just close the window) - quit
1-9 buttons - change game speed (fps drops and input lags on high speed)
0 button - make game do a single step and pause (works even when paused)
MOVEMENT:
    up arrow - move up across the field
    down arrow - move down across the field
    up arrow - move right across the field
    left arrow - move left across the field
    shift (while prressed) - boosts movement (4 cells at one step)
    alt (while pressed) - move step-by-step
    mmb - move the field to a certain position
</pre>
<b>Mouse:</b><br>
<pre>
left mouse button - add a cell
right mouse button - remove a cell
middle mouse button (hold) - move the field
mouse wheel up - zoom in
mouse wheel down - zoom out
</pre>

<b>Most important issues now:</b><br>
<pre>
1. Cannot resize the game field
2. Sometimes an attempt to resize the window fails with SIGSEGV
3. Speed changing is too primitive
4. There's no intuitive GUI, just a key bindings
5. No ability to rollback last N steps of game
6. A new step is calculated too slowly since the app doesn't use Hashlife (and the logic works on a single thread)
</pre>
