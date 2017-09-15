Name: Pratyush Singh

Clemson Email Address: pratyus@clemson.edu

Project no: final project

Project due date: April 30th, 2017

Project description: A Playable Game

all the sprites and images i have used got from online.
sprites are downloaded from (https://www.spriters-resource.com/)
background images are downloaded from below links
http://www.clipartkid.com/cartoon-sky-background-cliparts/
https://www.pinterest.com/pgaraude/marionnette-d%C3%A9cors/
https://www.pinterest.com

The software has following leak summary.

Valgrind log

==23740== LEAK SUMMARY:
==23740==    definitely lost: 123 bytes in 9 blocks
==23740==    indirectly lost: 0 bytes in 0 blocks
==23740==      possibly lost: 0 bytes in 0 blocks
==23740==    still reachable: 58,194 bytes in 470 blocks
==23740==         suppressed: 0 bytes in 0 blocks
==23740== Rerun with --leak-check=full to see details of leaked memory
==23740== 
==23740== For counts of detected and suppressed errors, rerun with: -v
==23740== Use --track-origins=yes to see where uninitialised values come from
==23740== ERROR SUMMARY: 8 errors from 8 contexts (suppressed: 6 from 1)

All the memory leak are from SDL librareies. (no extra memory leak)

Behaviour of game:

The player is a eagel who has to reach to its destination home.
In midway on jorney it finds enimies which are big black crow.
The player can use a,s,d,w to move left, down, right, up respestivly.
the player can shoot enimies from space bar.
if the player collides with enimies then it will explode and would loose health.
when the player reaches home the game ends and player wins.
if the player looses all its health before reaching destination home the the player lose the game.

This game has following features.
1.  well controled player. use a,s,d,w to control player.
2.  it implements illusion of depth through painters algorithm.
3.  all information can be seen on HUD. press F1 to toggel HUD.
4.  projectiles are diplayed by birds flying.
5.  Collision detection that triggers explosions. player collision with enimies and bullet collision with enimies.
6.  player and enimies explosion on collision.
7.  Demonstrate object pooling by bullet pool can be seen on HUD.
8.  R button to reset that restarts the game.
9.  Music in background and on shooting.
10. g button for  a “god” mode.
11. AI when the player tries to skip away from enimies the enimies tend to chase the player. implemented through observer patern.
    Here player is a subject as well as observer. since my player and enimies both are implemented through same player class.
	




