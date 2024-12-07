#ifndef __OZGEAKOSA5177_DIMITRINEARCHOSDON5092_PLAYER__
#define __OZGEAKOSA5177_DIMITRINEARCHOSDON5092_PLAYER__

#include <vector>
#include <queue> // For managing priority moves
#include "common.h"
#include "board.h"
#include "player.h"

using namespace std;

// Represents the player logic for the game "Dots and Boxes."
// Implements strategic gameplay, managing the player's moves and reacting
// to game events such as adding lines or boxes on the board.
class OzgeAkosa5177_DimitriNearchosdon5092_Player : public IPlayer
{
private:
    Board board;              // Tracks the current state of the game board (dots, lines, boxes).
    char player_box;          // The character used to represent boxes captured by this player.
    char player_line;         // The character used to represent lines drawn by this player.
    int board_rows;           // The total number of rows on the board (including dots and spaces for lines).
    int board_cols;           // The total number of columns on the board (including dots and spaces for lines).
    queue<Loc> priorityMoves; // Stores high-priority moves, like completing a box or blocking the opponent.

    // Collect all legal moves available on the board.
    vector<Loc> CollectMoves(char displayCharacter) const;

    // Collect scorable boxes based on display character and yield type.
    vector<Loc> CollectLegalMoves() const { return CollectMoves(' '); };

    // Count surrounding lines for the "next" adjacent cell in the given direction.
    int CountAdjacentNextSurroundingLines(int row, int column) const { return CountAdjacentNextSurroundingLines(Loc(row, column)); };

    int CountAdjacentNextSurroundingLines(const Loc &location) const;

    // Count surrounding lines for the "previous" adjacent cell in the given direction.
    int CountAdjacentPreviousSurroundingLines(int row, int column) const { return CountAdjacentPreviousSurroundingLines(Loc(row, column)); };

    int CountAdjacentPreviousSurroundingLines(const Loc &location) const;

    // Count the number of lines around a given cell.
    int CountSurroundingLines(int row, int column) const;

    int CountSurroundingLines(const Loc &location) const { return CountSurroundingLines(location.row, location.col); };

    // Check if a box is a "prevention" move (1 surrounding line).
    bool DoesBoxYieldPrevention(const Loc &location) const { return DoesBoxYieldPrevention(location.row, location.col); };

    bool DoesBoxYieldPrevention(int row, int column) const;

    // Check if a move (line) will capture a box.
    bool DoesMoveYieldCapture(const Loc &location) const;

    bool DoesMoveYieldCapture(int row, int column) const { return DoesMoveYieldCapture(Loc(row, column)); };

    // Check if a move creates or continues a chain.
    bool DoesMoveYieldChain(const Loc &location) const;

    bool DoesMoveYieldChain(int row, int column) const { return DoesMoveYieldChain(Loc(row, column)); };

public:
    // Constructor: Initializes the player instance.
    // Prepares the player for gameplay by setting up necessary variables.
    OzgeAkosa5177_DimitriNearchosdon5092_Player();

    // Destructor: Cleans up resources used by the player.
    // Frees up memory or other resources allocated during gameplay.
    ~OzgeAkosa5177_DimitriNearchosdon5092_Player();

    // Returns metadata about the player, including names and email addresses.
    string PlayerInfo() { return "Ozge Selin Ak (osa5177@psu.edu) + Dimitri Orion Nearchos (don5092@psu.edu)"; }

    // Initializes the player at the start of the game.
    // Sets up the board dimensions, box character, and line character for the player.
    void Init(int _dots_in_rows, int _dots_in_cols, char _player_box, char _player_line);

    // Cleans up the player instance at the end of the game.
    // Frees any allocated resources, such as the game board.
    void Close();

    // Handles the event when a line is added to the board.
    // Updates the internal game state to reflect the new line and checks for opportunities.
    void EventAddLine(const char bar, const Loc &loc);

    // Handles the event when a box is completed on the board.
    // Updates the internal game state to reflect the captured box.
    void EventAddBox(const char box, const Loc &loc);

    // Selects the next move for the player.
    // Implements the player's strategy to determine the best line to draw.
    Loc SelectLineLocation();

    // Evaluates the potential value or risk of a given move.
    // Assigns a heuristic score to a move based on factors like box completion, chain creation, or safety.
    int EvaluateMove(const Loc &move);
};

#endif
