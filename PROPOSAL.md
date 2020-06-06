# Proposal: 8-ball cue

## Synopsis
This is a simplified version of 8-ball cue. There are two colors of ball, and a 
white ball that is the cue ball that the player needs to hit to drive balls into
holes, just like in regular pool. If the player successfully lands a shot, the 
player can go again, and the same goes for the 'AI'. Details about how the 
opponent works are below. The game ends when there are no more non-white balls 
on the board, and the player with the higher score wins, unless there is a tie.


## Functional requirements

1. Initialize and update a pool table with active balls in a predetermined 
arrangement. Because of the spatial requirements of the board, only a few 
different initial sizes of active balls will be considered. 
	
2. There are two active players, distinguished by the type of balls they can 
play, and they need to sink balls that are distinguished by color corresponding
to each player

3. The players take turns. A turn comprises an interval in which some ball is 
being played. At the beginning of a turn, all balls are stationary, and the 
player whose turn it is is identified by the program. Because the player needs 
to hit the white ball, at least one ball will always be in motion throughout the
duration of the turn. The turn ends when, according to in-game physics 
(see point 10.) the balls are stationary again. At the end of the turn, 
everything is kept the same except the player might change based on the rules 
detailed in 4). This continues until there are no more non-white balls to play.
	
4. a. The turn rules: suppose A is responsible for color 1 and B for color 2. 
If A sinks balls of color 2, A loses turns. If the number of turns 
A has drops below -1, A hands the turn over to B, who has gained turns for each 
ball of its color A sank plus two for the white ball as well. Note that I simp-
lified this requirement from the original proposal since the original idea
was too hard to implement.
b. If the white ball is sunk, it needs to be replaced on the board. Resolved
an open question: the player gets to choose where to place the white ball, the 
'ai' always palces it back at start (see Open questions 4.)

5. The Players are structs whose members give information about 1. the number of
turns available to them (nr_turns_), the color of the ball they are responsible
for (team_, determined upon initialization, implicit, P1 is the player, P2 the 
ai), and their score (score).

6. The turns are computed as follows: Set nr_turns_ = 0 for both players, set A 
to start. A plays the white ball via a mouse click; the turn lasts until all 
balls have stopped. At the end of the turn, the turns available to A are given 
by [(number of color 1 balls sunk) - 1], if no balls of color 2 were sunk, 
and -1 otherwise. A gets to keep playing until the number of turns reaches -1. 
At this point, it's B's turn. At the point where the number of turns available 
to A is -1, the number of turns for B is incremented by the rules above and B 
becomes the player. At each turn, the same rules apply to B.  

7. The score is computed as follows: at the end of each turn, each player's 
score gets augmented by the number of balls of the respective color sunk. 
This means that not only does B gain an extra turn if A messes up, but B 
potentially gains another point as well. This is because if B were to not gain 
a point for this, since the winner is determined by the score, B would not gain 
a real advantage due to A's mistake.

8. The model for the game keeps track of the number of balls of each color, the
velocities of each ball including the white ball, and handles sunk balls and
replacing balls.

9. The board stays static throughout the gameplay. This includes the pool table 
holes. The balls are rendered dynamically on frame.

10. The physics: at the beginning of each turn, each ball is stationary. 
The player is automatically locked to hit the white ball and the white ball 
only. The player launches the white ball by clicking with the left mouse button.
The initial speed is equal (changed this to equality) to the distance of the 
mouse click from the white ball, and the initial direction is the same as the 
direction of the directed line segment connecting the cursor position to the 
position of the white ball. We are assuming perfectly inelastic collisions on 
the board and no drag forces due to air resistance. At each time interval dt, 
the speed of the ball is decreased by a fixed amount. I will devote a separate 
point to collisions.

11. The collisions are handled as follows: two balls collide if the distance 
between their centers is less than the sum of their radii. Upon collision, the 
new velocities are computed according to conservation of momentum: the product 
of the masses of the balls in the horizontal and vertical directions must remain
the same before and after collision. I won't write down the general solution to 
such collision problems, but I will do the math out and include it in the
header. Note that at each time step in the program, each ball needs to be 
checked for collisions and their velocities updated appropriately. Collisions 
with the board are predetermined statically before compiling.
12. A ball dies if, according to the physics, it collides (see 11.) with one of 
the fixed-position holes on the board. (See also Open questions 1. for a 
potential refinement of this mechanism). When a ball dies, it is removed from 
the std::vector that contains the balls of a given color via the same mechanism 
that bricks are popped in brick_out, and thescore is implemented as before.
	
13. The 'AI' is not really an AI. At the beginning of the game, the player 
is set to start (changed this, too), and the program sets the color for the 
player and constructs a dummy player to have the other color (say colors 1 and 
2 resp.) At each of its turns, the dummy player hits the ball with a random 
initial velocity and lets the chips fall where they may (see
Open questions 2. about potential refinements).

## Open questions

1. In a regular game of pool, it is *possible* although rare that a ball can 
reflect off of the edge of a hole and not fall into it, not giving the player 
who cued it any points. Implemented in game.
2. One potential refinement is that the AI will choose the initial direction to 
be that pointing to the nearest ball of its own color (ignoring the possibility 
that this might cause it to hit a ball of the other color). Implemented.

3. Usually, there is an 8-ball in regular pool, and it really doesn't matter 
what your score is so long as you hit the 8-ball in before the other player. 
Implementing this would amount to keeping track of the 8-ball but not having any
kind of score functionality. Once a player empties out their set of balls, they 
have to hit the 8-ball into a fixed hole on the board, and a simplified version
of the rules might apply (this is analogous to the phases in reversi). Did not
end up implementing. 
	
4. The replacement of the white ball is either automatic, putting it right in 
front of the whole from which it was sunk, which is probably the most equitable 
solution, but there are versions of pool where the player whose turn it is gets 
to decide where to put the white ball after it has been sunk by the other 
player. Implementing this might be more difficult since there needs to be a 
separate UI functionality to place the white ball using the mouse that is 
distinct from the function of the mouse to launch balls. Implemented.