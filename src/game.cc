/*
 * Tic-Tac-Toe Game Implementation in C++
 *
 * This program implements a complete tic-tac-toe game with an AI opponent
 * that has three difficulty levels: Novice (random moves), Intermediate
 * (defensive/offensive strategy), and Experienced (optimal play with center
 * control and edge strategies).
 *
 * Author: [Your Name]
 * Date: [Current Date]
 */

// Standard library includes for I/O operations, random number generation,
// string manipulation, and dynamic array functionality
#include <iostream> // For console input/output (cin, cout)
#include <random>   // For random number generation (AI moves)
// Note: string.h removed as it's not directly used (string operations handled
// by <iostream>)
#include <vector> // For dynamic arrays to store game tiles

using namespace std;

// Game piece constants - visual representation of different game states
const string EMPTY_PIECE = "*";  // Represents an unoccupied tile
const string PLAYER_PIECE = "O"; // Represents the human player's piece
const string AI_PIECE = "X";     // Represents the AI opponent's piece

// All possible winning combinations in tic-tac-toe (8 total)
// Each sub-array contains the tile indices that form a winning line
// Indices represent positions: 0,1,2 (top row), 3,4,5 (middle row), 6,7,8
// (bottom row)
const int WINNING_COMBINATIONS[8][3]{{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
                                     {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
                                     {0, 4, 8}, {2, 4, 6}}; // Diagonals

// Random number generation setup for AI move selection
random_device rd;  // Hardware-based random number generator
mt19937 rng(rd()); // Mersenne Twister engine seeded with random device

/**
 * Piece class represents a game piece on the tic-tac-toe board
 * Each piece has a visual representation (sprite), an owner, and an empty state
 */
class Piece {
private:
  string sprite; // Visual character representing the piece ("*", "O", or "X")
  string owner;  // Who owns this piece ("player", "AI", or "" for empty)
  bool isEmpty;  // Flag indicating if this tile is unoccupied

public:
  /**
   * Constructor to create a new piece
   * @param sprite Visual representation of the piece
   * @param owner Owner of the piece ("player", "AI", or "")
   * @param isEmpty Whether this piece represents an empty tile
   */
  Piece(string sprite, string owner, bool isEmpty) {
    this->sprite = sprite;
    this->owner = owner;
    this->isEmpty = isEmpty;
  };

  // Getter methods to access private member variables
  string getSprite() { return this->sprite; }; // Returns the visual character
  string getOwner() { return this->owner; };   // Returns the owner string
  bool empty() { return this->isEmpty; };      // Returns true if tile is empty

  // Setter methods to modify private member variables
  void setSprite(string sprite) {
    this->sprite = sprite;
  } // Updates visual character
  void setOwner(string owner) { this->owner = owner; } // Updates owner
  void setEmpty(bool empty) { this->isEmpty = empty; } // Updates empty state
};

/**
 * Tile class represents a single position on the tic-tac-toe board
 * Each tile contains a piece and knows its position (column, row)
 */
class Tile {
private:
  Piece piece; // The game piece occupying this tile
  int column;  // Column position (0-2)
  int row;     // Row position (0-2)

public:
  /**
   * Constructor to create a new tile at a specific position
   * @param p The piece to place on this tile
   * @param column Column coordinate (0-2)
   * @param row Row coordinate (0-2)
   */
  Tile(Piece p, int column, int row) : piece(p) {
    this->piece = p;
    this->column = column;
    this->row = row;
  };

  // Getter methods to access tile properties
  Piece getPiece() { return this->piece; }; // Returns the piece on this tile
  int getColumn() { return this->column; }; // Returns column coordinate
  int getRow() { return this->row; };       // Returns row coordinate

  // Setter method to update the piece on this tile
  void setPiece(Piece piece) { this->piece = piece; }
};

/**
 * Board class manages the 3x3 tic-tac-toe game board
 * Handles board initialization, tile management, win detection, and display
 */
class Board {
private:
  vector<Tile> tiles; // Container holding all 9 tiles of the board

public:
  /**
   * Constructor initializes the board with empty tiles
   * Reserves space for 9 tiles and generates the initial board state
   */
  Board() {
    this->tiles.reserve(9); // Pre-allocate space for 9 tiles for efficiency
    this->genBoard();       // Create the initial empty board
  };

  /**
   * Adds a new tile to the board
   * @param tile The tile to add to the board
   */
  void addTile(Tile tile) { this->tiles.push_back(tile); };

  /**
   * Generates the initial 3x3 board with empty tiles
   * Creates 9 tiles in a grid pattern, each initialized as empty
   */
  void genBoard() {
    // Nested loops to create a 3x3 grid
    for (int col = 0; col < 3; col++) {
      for (int row = 0; row < 3; row++) {
        // Create an empty piece for each tile
        Piece piece(EMPTY_PIECE, "", true);
        // Create tile with the piece and its coordinates
        Tile tile(piece, col, row);
        // Add the tile to the board
        this->tiles.push_back(tile);
      }
    }
  };

  /**
   * Retrieves a tile by its column and row coordinates
   * @param col Column coordinate (0-2)
   * @param row Row coordinate (0-2)
   * @return The tile at the specified coordinates
   */
  Tile getTile(int col, int row) {
    // Search through all tiles to find matching coordinates
    for (int p = 0; p < this->tiles.size(); p++) {
      if (this->tiles.at(p).getColumn() == col &&
          this->tiles.at(p).getRow() == row) {
        return this->tiles.at(p);
      }
    }
    // This should never happen in a properly initialized board, but return
    // first tile as fallback
    return this->tiles.at(0);
  }

  /**
   * Retrieves a tile by its index position in the tiles vector
   * @param num Index position (0-8)
   * @return The tile at the specified index
   */
  Tile getTileByIndex(int num) { return this->tiles.at(num); }

  /**
   * Checks if a tile exists at the specified coordinates
   * @param col Column coordinate (0-2)
   * @param row Row coordinate (0-2)
   * @return True if tile exists, false otherwise
   */
  bool hasTile(int col, int row) {
    // Search through all tiles to find matching coordinates
    for (int p = 0; p < this->tiles.size(); p++) {
      if (this->tiles.at(p).getColumn() == col &&
          this->tiles.at(p).getRow() == row) {
        return true;
      }
    }
    return false; // Tile not found
  }

  /**
   * Updates a tile on the board by replacing it with a new tile
   * @param tile The new tile to place on the board
   */
  void setTile(Tile tile) {
    // Find the tile with matching coordinates and replace it
    for (int i = 0; i < this->tiles.size(); i++) {
      if (this->tiles.at(i).getColumn() == tile.getColumn() &&
          this->tiles.at(i).getRow() == tile.getRow()) {
        this->tiles.at(i) = tile; // Replace the old tile with the new one
      }
    }
  }

  /**
   * Displays the current state of the board to the console
   * Shows a 3x3 grid with pieces separated by lines
   */
  void draw() {
    // Iterate through each row of the board
    for (int col = 0; col < 3; col++) {
      // Iterate through each column in the current row
      for (int row = 0; row < 3; row++) {
        if (this->hasTile(col, row)) {
          // Get the tile and display its piece
          Tile tile = this->getTile(col, row);
          cout << tile.getPiece().getSprite();

          // Add vertical separator between columns (except after last column)
          if (row < 2) {
            cout << "|";
          }
        } else {
          // Error handling - this should never happen in a properly initialized
          // board
          cout << "Tile doesn't exist\n";
          cout << "Column: " << col << "\n";
          cout << "Row: " << row << "\n";
          exit(1); // Terminate program on error
        }
      }

      // Add horizontal separator between rows (except after last row)
      if (col < 2) {
        cout << "\n-----\n";
      }
    }
    cout << "\n\n\n"; // Add extra spacing after board display
  }

  /**
   * Checks if the specified player has won the game
   * @param owner The player to check for victory ("player" or "AI")
   * @return True if the player has won, false otherwise
   */
  bool hasWon(string owner) {
    // Check each of the 8 possible winning combinations
    for (int y = 0;
         y < sizeof(WINNING_COMBINATIONS) / sizeof(WINNING_COMBINATIONS[0]);
         y++) {
      int count = 0; // Count pieces belonging to the owner in this combination

      // Check each position in the current winning combination
      for (int x = 0; x < sizeof(WINNING_COMBINATIONS[y]) /
                              sizeof(WINNING_COMBINATIONS[y][0]);
           x++) {
        int num = WINNING_COMBINATIONS[y][x]; // Get tile index from combination
        Tile tile = this->getTileByIndex(num); // Get the actual tile

        // If this tile belongs to the owner, increment the count
        if (tile.getPiece().getOwner() == owner) {
          count += 1;
        }
      }

      // If all 3 positions in this combination belong to the owner, they won
      if (count == 3) {
        return true;
      }
    }

    return false; // No winning combination found
  }

  /**
   * Finds an open tile that would complete a winning combination for the owner
   * Used by AI to find winning moves or blocking moves
   * @param owner The player to find a strategic move for ("player" or "AI")
   * @return Vector containing [column, row] of the strategic tile, or [-1, -1]
   * if none found
   */
  vector<int> getOpenTile(string owner) {
    vector<int> vect; // Return vector for coordinates
    int column = -1;  // Default invalid column
    int row = -1;     // Default invalid row

    // Check each winning combination for strategic opportunities
    for (int y = 0;
         y < sizeof(WINNING_COMBINATIONS) / sizeof(WINNING_COMBINATIONS[0]);
         y++) {
      int count = 0; // Count pieces belonging to the owner in this combination

      // Count how many pieces the owner has in this combination
      for (int x = 0; x < sizeof(WINNING_COMBINATIONS[y]) /
                              sizeof(WINNING_COMBINATIONS[y][0]);
           x++) {
        int num = WINNING_COMBINATIONS[y][x];
        Tile tile = this->getTileByIndex(num);

        if (tile.getPiece().getOwner() == owner) {
          count += 1;
        }
      }

      // If owner has 2 pieces in this combination, find the empty third
      // position
      if (count == 2) {
        for (int x = 0; x < sizeof(WINNING_COMBINATIONS[y]) /
                                sizeof(WINNING_COMBINATIONS[y][0]);
             x++) {
          int num = WINNING_COMBINATIONS[y][x];
          Tile tile = this->getTileByIndex(num);

          // If this position is empty, it's the strategic move
          if (tile.getPiece().empty() == true) {
            column = tile.getColumn();
            row = tile.getRow();
            vect.push_back(column);
            vect.push_back(row);
            return vect; // Return immediately when found
          }
        }
      }
    }

    // No strategic move found, return invalid coordinates
    vect.push_back(column);
    vect.push_back(row);
    return vect;
  }

  /**
   * Checks if the game has ended in a draw (all tiles filled, no winner)
   * @return True if all 9 tiles are occupied and no one has won, false
   * otherwise
   */
  bool isADraw() {
    int count = 0; // Count of occupied tiles

    // Count how many tiles are occupied (not empty)
    for (int x = 0; x < this->tiles.size(); x++) {
      if (this->getTileByIndex(x).getPiece().empty() == false) {
        count += 1;
      }
    }

    // If all 9 tiles are occupied, it's a draw
    if (count == 9) {
      return true;
    }

    return false; // Game is not a draw yet
  }
};

/**
 * AI class implements the computer opponent with three difficulty levels
 * Each difficulty level uses different strategies for move selection
 */
class AI {
private:
  int diff; // Difficulty level: 1=Novice, 2=Intermediate, 3=Experienced

public:
  /**
   * Constructor to create an AI opponent with specified difficulty
   * @param difficulty Difficulty level (1-3)
   *                  1: Novice (random moves)
   *                  2: Intermediate (defensive/offensive strategy)
   *                  3: Experienced (optimal play with center control)
   */
  AI(int difficulty) { this->diff = difficulty; };

  /**
   * Main AI move selection method based on difficulty level
   * @param board Pointer to the game board
   */
  void play(Board *board) {
    // Create list of players to check for strategic moves
    vector<string> owners;
    owners.reserve(2);
    owners.push_back("player"); // Check for blocking moves first
    owners.push_back("AI");     // Then check for winning moves

    switch (this->diff) {
    case 1: { // Novice difficulty - completely random moves
      this->chooseRandomSpot(board);
    } break;

    case 2: { // Intermediate difficulty - defensive and offensive strategy
      // Check for strategic moves (blocking opponent or winning)
      for (int i = 0; i < owners.size(); i++) {
        string owner = owners.at(i);
        const vector<int> check = board->getOpenTile(owner);
        int column = check.at(0);
        int row = check.at(1);

        // If a strategic move is found, make it
        if (column >= 0 && row >= 0) {
          if (board->hasTile(column, row)) {
            Piece piece(AI_PIECE, "AI", false);
            Tile tile = board->getTile(column, row);
            tile.setPiece(piece);
            board->setTile(tile);
            return; // Move made, exit function
          }
        }
      }
      // No strategic move found, make a random move
      chooseRandomSpot(board);
    } break;

    case 3: { // Experienced difficulty - optimal play strategy
      // First priority: Take center if available (tile index 4)
      Tile middle = board->getTileByIndex(4);
      if (middle.getPiece().empty()) {
        Piece piece(AI_PIECE, "AI", false);
        middle.setPiece(piece);
        board->setTile(middle);
        return; // Center taken, exit function
      }

      // Second priority: Check for strategic moves (blocking or winning)
      for (int i = 0; i < owners.size(); i++) {
        string owner = owners.at(i);
        const vector<int> check = board->getOpenTile(owner);
        int column = check.at(0);
        int row = check.at(1);

        if (column >= 0 && row >= 0) {
          if (board->hasTile(column, row)) {
            Piece piece(AI_PIECE, "AI", false);
            Tile tile = board->getTile(column, row);
            tile.setPiece(piece);
            board->setTile(tile);
            return; // Strategic move made, exit function
          }
        }
      }

      // Third priority: Take an edge position (corners are indices 0, 2, 6, 8)
      const int edges[4] = {0, 2, 6, 8}; // Corner tile indices
      bool done = false;

      while (!done) {
        // Randomly select one of the four corners
        uniform_int_distribution<int> uni(0,
                                          3); // Fixed: should be 0-3, not 0-4
        Tile tile = board->getTileByIndex(edges[uni(rng)]);

        // If the selected corner is empty, take it
        if (tile.getPiece().empty()) {
          Piece piece(AI_PIECE, "AI", false);
          tile.setPiece(piece);
          board->setTile(tile); // Update the board with the new piece
          done = true;          // Exit the loop
        }
      }
    } break;
    }
  }

  /**
   * Chooses a random empty spot on the board for AI move
   * Used by novice AI and as fallback for other difficulties
   * @param board Pointer to the game board
   */
  void chooseRandomSpot(Board *board) {
    bool done = false;

    // Keep trying until an empty spot is found
    while (!done) {
      // Generate random coordinates (0-2 for both column and row)
      uniform_int_distribution<int> uni(0, 2);
      int column = uni(rng);
      int row = uni(rng);

      // Check if the randomly selected position exists on the board
      if (board->hasTile(column, row)) {
        Piece piece(AI_PIECE, "AI", false);
        Tile tile = board->getTile(column, row);

        // If the tile is empty, place the AI piece there
        if (tile.getPiece().empty() == true) {
          tile.setPiece(piece);
          board->setTile(tile); // Update the board
          done = true;          // Successfully placed piece, exit loop
        }
      }
    }
  }
};

/**
 * Utility function to clear the console screen
 * Uses ANSI escape sequences to clear screen and move cursor to top-left
 */
void clrscr() { cout << "\033[2J\033[1;1H"; };

/**
 * Main function - entry point of the tic-tac-toe game
 * Handles game initialization, difficulty selection, and main game loop
 * @return 0 on successful program termination
 */
int main() {
  Board board;    // Create the game board
  int difficulty; // Store selected difficulty level

  // Difficulty selection loop - ensure valid input
  while (true) {
    cout << "Choose a difficulty [1-3]: ";
    cin >> difficulty;
    cout << "\n";

    // Validate difficulty input (must be 1, 2, or 3)
    if (difficulty >= 1 && difficulty <= 3) {
      break; // Valid input, exit loop
    } else {
      cout << "Please choose a valid difficulty\n";
    }
  }

  AI ai(difficulty); // Create AI opponent with selected difficulty
  bool done = false; // Game completion flag

  // Main game loop - continues until game ends
  while (done == false) {
    int column; // Player's column choice
    int row;    // Player's row choice

    // Clear screen and display current board state
    clrscr();
    board.draw();

    bool check = false; // Valid move flag

    // Player input loop - ensure valid move
    while (!check) {
      // Get player's column choice (1-3, displayed as user-friendly)
      cout << "Enter the column [1-3]: ";
      cin >> column;
      cout << "\n";

      // Get player's row choice (1-3, displayed as user-friendly)
      cout << "Enter the row [1-3]: ";
      cin >> row;
      cout << "\n\n";

      // Convert user input (1-3) to array indices (0-2)
      column--;
      row--;

      // Validate that the chosen position exists on the board
      if (board.hasTile(column, row)) {
        Tile tile = board.getTile(column, row);

        // Check if the chosen tile is empty
        if (tile.getPiece().empty() == true) {
          // Place player's piece on the chosen tile
          Piece piece(PLAYER_PIECE, "player", false);
          tile.setPiece(piece);
          board.setTile(tile);

          // Check for draw condition after player move
          if (board.isADraw()) {
            board.draw();
            cout << "Its a tie";
            exit(0); // End game on draw
          }

          // AI makes its move
          ai.play(&board);

          // Check for win conditions after AI move
          if (board.hasWon("player")) {
            board.draw();
            cout << "Player has won!";
            exit(0); // End game on player victory
          } else if (board.hasWon("AI")) {
            board.draw();
            cout << "AI has won!";
            exit(0); // End game on AI victory
          }

          check = true; // Valid move completed, exit input loop
        } else {
          // Tile is already occupied
          cout << "Place you chose is not empty\n";
        }
      } else {
        // Invalid coordinates
        cout << "Please choose a valid column/row\n";
      }
    }
  }
  return 0; // Program completed successfully
}
