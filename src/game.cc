#include <iostream>
#include <string.h>
#include <vector>
#include <random>

using namespace std;

const string EMPTY_PIECE = "*";
const string PLAYER_PIECE = "O";
const string AI_PIECE = "X";

const int WINNING_COMBINATIONS[8][3] {
    {0, 1, 2},
	{3, 4, 5},
	{6, 7, 8},
	{0, 3, 6},
	{1, 4, 7},
	{2, 5, 8},
	{0, 4, 8},
	{2, 4, 6}
};

random_device rd;
mt19937 rng(rd());
uniform_int_distribution<int> uni(0,2);

class Piece {
    private:
        string sprite;
        string owner;
        bool isEmpty;
    public:
        Piece(string sprite, string owner, bool isEmpty){
            this->sprite = sprite;
            this->owner = owner;
            this->isEmpty = isEmpty;
        };

        string getSprite(){
            return this->sprite;
        };

        string getOwner(){
            return this->owner;
        };

        bool empty(){
            return this->isEmpty;
        };

        void setSprite(string sprite){
            this->sprite = sprite;
        }

        void setOwner(string owner){
            this->owner = owner;
        }

        void setEmpty(bool empty){
            this->isEmpty = empty;
        }
};

class Tile {
    private:
        Piece piece;
        int column;
        int row;
    public:
        Tile(Piece p, int column, int row): piece(p) {
            this->piece = p;
            this->column = column;
            this->row = row;
        };

        Piece getPiece(){
            return this->piece;
        };

        int getColumn(){
            return this->column;
        };

        int getRow(){
            return this->row;
        };

        void setPiece(Piece piece){
            this->piece = piece;
        }
};

class Board {
    private:
        vector<Tile> tiles;
    public:
        Board(){
            this->tiles.reserve(9);
            this->genBoard();
        };

        void addTile(Tile tile){
            this->tiles.push_back(tile);
        };

        void genBoard(){
            for(int col = 0; col < 3; col++){
                for(int row = 0; row < 3; row++){
                    Piece piece(EMPTY_PIECE,"",true);
                    Tile tile(piece,col,row);

                    this->tiles.push_back(tile);
                }
            }
        };

        Tile getTile(int col, int row){
            for(int p = 0; p < this->tiles.size(); p++){
                if(this->tiles.at(p).getColumn() == col && this->tiles.at(p).getRow() == row){
                    return this->tiles.at(p);
                }
            }
        }

        Tile getTileByIndex(int num){
            return this->tiles.at(num);
        }

        bool hasTile(int col, int row){
            for(int p = 0; p < this->tiles.size(); p++){
                if(this->tiles.at(p).getColumn() == col && this->tiles.at(p).getRow() == row){
                    return true;
                }
            }

            return false;
        }

        void setTile(Tile tile){
            
            for(int i = 0; i < this->tiles.size(); i++){
                if(this->tiles.at(i).getColumn() == tile.getColumn() && this->tiles.at(i).getRow() == tile.getRow()){
                    this->tiles.at(i) = tile;
                }
            }
            
        }

        void draw(){
            for(int col = 0; col < 3; col++){
                for(int row = 0; row < 3; row++){
                    if(this->hasTile(col,row)){
                        Tile tile = this->getTile(col,row);

                        cout << tile.getPiece().getSprite();
                        
                        if(row < 2){
                            cout << "|";
                        }
                    } else {
                        cout << "Tile doesn't exist\n";
                        cout << "Column: " << col << "\n";
                        cout << "Row: " << row << "\n"; 
                        exit(1);
                    }
                }

                if(col < 2){
                    cout << "\n-----\n";
                }
            }
            cout << "\n\n\n";
        }

        bool hasWon(string owner){
            for(int y = 0; y < sizeof(WINNING_COMBINATIONS)/sizeof(WINNING_COMBINATIONS[0]); y++){
                int count = 0;
                for(int x = 0; x < sizeof(WINNING_COMBINATIONS[y])/sizeof(WINNING_COMBINATIONS[y][0]); x++){
                    int num = WINNING_COMBINATIONS[y][x];
                    Tile tile = this->getTileByIndex(num);
                    
                    if(tile.getPiece().getOwner() == owner){
                        count += 1;
                    }
                }

                if(count == 3){
                    return true;
                }
            }

            return false;
        }

        vector<int> getOpenTile(string owner){
            vector<int> vect;
            int column = -1;
            int row = -1;

            for(int y = 0; y < sizeof(WINNING_COMBINATIONS)/sizeof(WINNING_COMBINATIONS[0]); y++){
                int count = 0;
                for(int x = 0; x < sizeof(WINNING_COMBINATIONS[y])/sizeof(WINNING_COMBINATIONS[y][0]); x++){
                    int num = WINNING_COMBINATIONS[y][x];
                    Tile tile = this->getTileByIndex(num);
                    
                    if(tile.getPiece().getOwner() == owner){
                        count += 1;
                    }
                }

                if(count == 2){
                    for(int x = 0; x < sizeof(WINNING_COMBINATIONS[y])/sizeof(WINNING_COMBINATIONS[y][0]); x++){
                        int num = WINNING_COMBINATIONS[y][x];
                        Tile tile = this->getTileByIndex(num);
                        
                        if(tile.getPiece().empty() == true){

                            column = tile.getColumn();
                            row = tile.getRow();
                            vect.push_back(column);
                            vect.push_back(row);

                            return vect;
                        }
                    }
                }
            }

            vect.push_back(column);
            vect.push_back(row);

            return vect;
        }

        bool isADraw(){
            int count = 0;
            for(int x = 0; x < this->tiles.size(); x++){
                if(this->getTileByIndex(x).getPiece().empty() == false){
                    count += 1;
                }
            }

            if(count == 9){
                return true;
            }

            return false;
        }
};

class AI {
    private:
        int diff;
    public:
        AI(int difficulty){
            this->diff = difficulty; // Novice, Intermediate, Expert, Experienced
        };
    
        void play(Board *board){
            switch(this->diff){
                case 1: {
                    this->chooseRandomSpot(board);
                }break;
                case 2: {
                    vector<string> owners;
                    owners.reserve(2);

                    owners.push_back("player");
                    owners.push_back("AI");

                    for(int i = 0; i < owners.size(); i++){
                        string owner = owners.at(i);
                        const vector<int> check = board->getOpenTile(owner);
                        int column = check.at(0);
                        int row = check.at(1);

                        if(column >= 0 && row >= 0){
                            if(board->hasTile(column,row)){
                                Piece piece(AI_PIECE,"AI",false);
                                Tile tile = board->getTile(column,row);
                                tile.setPiece(piece);
                                board->setTile(tile);
                                return;
                            }
                        }
                    }
                    chooseRandomSpot(board);
                }break;
            }
            
        }

        void chooseRandomSpot(Board *board){
            bool done = false;
                while(!done){
                    int column = uni(rng);
                    int row = uni(rng);

                    if(board->hasTile(column,row)){
                        
                        Piece piece(AI_PIECE,"AI",false);
                        Tile tile = board->getTile(column,row);

                        if(tile.getPiece().empty() == true){
                            tile.setPiece(piece);
                            board->setTile(tile);
                            done = true;
                        }

                    }
                }
        }
};

void clrscr(){
    cout << "\033[2J\033[1;1H";
};

int main()
{
    Board board;
    AI ai(2);

    bool done = false;

    while(done == false){
        int column;
        int row;

        clrscr();
        board.draw();


        bool check = false;

        while(!check){
            cout << "Enter the column [1-3]: ";
            cin >> column;
            
            cout << "\n";

            cout << "Enter the row [1-3]: ";
            cin >> row;
            cout << "\n\n";

            column--;
            row--;

            if(board.hasTile(column,row)){
                Tile tile = board.getTile(column,row);
                
                if(tile.getPiece().empty() == true){
                    Piece piece(PLAYER_PIECE,"player",false);

                    tile.setPiece(piece);
                    board.setTile(tile);

                    if(board.isADraw()){
                        board.draw();
                        cout << "Its a tie";
                        exit(0);
                    }

                    ai.play(&board);

                    if(board.hasWon("player")){
                        board.draw();
                        cout << "Player has won!";
                        exit(0);
                    } else if(board.hasWon("AI")){
                        board.draw();
                        cout << "AI has won!";
                        exit(0);
                    }

                    check = true;
                } else {
                    cout << "Place you chose is not empty\n";
                }
            } else {
                cout << "Please choose a valid column/row\n";
            }
        }
    }
    return 0;
}