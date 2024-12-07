Name: Ozge Selin Ak + Dimitri Orion Nearchos
Email: osa5177@psu.edu + don5092@psu.edu
Class: CMPSC 330
Section: 001

Assignment: Homework 5
Due Date: December 5th, 2024

Build Commands:

    * `...directory.../compile-player.sh`

Run Commands:

    /* SELECTIVE BOARD SIZES */

    * `...directory.../run-test-run.sh`

    /* CUSTOM BOARD SIZES */

    * `./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./strategic_player.so <<< "ROWS COLUMNS"`

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