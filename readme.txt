Name: Ozge Selin Ak + Dimitri Orion Nearchos
Email: osa5177@psu.edu + don5092@psu.edu
Class: CMPSC 330
Section: 001

Assignment: Homework 5
Due Date: December 5th, 2024

Build Commands:

    * `...directory.../compile-player.sh`

Run Commands:

    > SELECTIVE BOARD SIZES

    * `...directory.../run-test-run.sh`

    > CUSTOM BOARD SIZES

    * `...directory.../dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./strategic_player.so <<< "ROWS COLUMNS"`

Acknowledgments:

    > See each individual source file for their individual acknowledgments, if any.

    1. Generally, we (Ozge and Dimitri) used C++ reference documentation provided by:
       https://cplusplus.com/reference

    2. Unless stated otherwise, we (Ozge and Dimitri) implemented everything in the
       source code from scratch.

Homework #2 and #4 to Homework #5 Refactoring Log:

(Dimitri)

Generally, we did not reuse the code from either assignment. They had highly
structured APIs and thus were not really compatible with Dr. Na's base project
set up.

That being said, we did reuse a lot of the logic and strategizing implementions
in homework #4. Particularly, we translated over a lot of the optimizied grid
walking loops for only traversing lines on the board. And we translated over
our original strategic player as a test bed for trying out Dr. Na's APIs. After
that, we experimented with various players and got the code you see now.

(Ozge)

While developing my player for "Dots and Boxes," I gained valuable insights into 
implementing decision-making algorithms and designing strategies for competitive
games. I focused on creating a system that prioritizes completing boxes by 
leveraging game state evaluations, such as counting surrounding lines with methods 
like 'CountSurroundingLines' and checking move outcomes using functions like
'DoesMoveYieldCapture' and 'DoesMoveYieldChain'. These functions taught me the 
importance of modular and reusable logic when analyzing board states.

I also learned to use data structures like std::queue to manage priority moves, 
enabling my player to track and act on high-value opportunities, such as completing 
boxes with three lines. Implementing strategic fallbacks, such as avoiding chains or 
choosing random moves when no optimal strategy exists, highlighted the role of 
fail-safe mechanisms in AI design. Debugging issues with line and box placement using 
assertions and careful boundary checks emphasized the importance of input validation, 
especially when accessing board indices.

Additionally, working with methods like SelectLineLocation required balancing 
offensive tactics (prioritizing box completions) and defensive considerations 
(avoiding risky chains) while iterating through legal moves collected using 
'CollectLegalMoves'. By assigning heuristic scores to moves in 'EvaluateMove', 
I refined my understanding of scoring systems to weigh the pros and cons of potential
actions. This project not only strengthened my algorithm design skills but also 
deepened my ability to analyze and optimize decision trees in real-time game scenarios.