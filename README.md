# game_of_life
In this project i try to implement the "game of life" in C and assembly. Just for fun (=

<b>Instruction:</b><br>
<b>Keyboard:</b><br>
<pre>
c - clear the game field
space - pause
s - save game to "saved_game.life"
l - load game from "saved_game.life"
1-9 buttons - change game speed (warning: fps drops and input lags on high speed)
0 button - make game do a single step and pause (works even when paused)
MOVEMENT:
    up arrow - move up across the field
    down arrow - move down across the field
    up arrow - move right across the field
    left arrow - move left across the field
    shift (while prressed) - boosts movement (4 cells at one step)
    alt (while pressed) - move step-by-step
</pre>
<b>Mouse:</b><br>
<pre>
lmb - add a cell
rmb - remove a cell
mwheel up - zoom in
mwheel down - zoom out
</pre>

<b>Most important bugs now:</b><br>
<pre>
1. Cannot resize window
2. Cannot resize game field
3. Cells may be not square if, say, window is 4:3 and game field is 16:9
4. Speed changing is too simple
5. There's no intuitive GUI
6. No ability to rollback last steps of game
7. Life runner calculates new step too slow
8. There's no VSync
</pre>
