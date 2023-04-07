#include <iostream>
#include <string.h>
#include <vector>
#include <random>

using namespace std;

const string EMPTY_PIECE = "*";
const string PLAYER_PIECE = "O";
const string AI_PIECE = "X";

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
            this->genBoard();
        };

        void addTile(Tile tile){
            this->tiles.push_back(tile);
        };

        void genBoard(){
            for(int col = 0; col < 3; col++){
                for(int row = 0; row < 3; row++){
                    Piece piece(EMPTY_PIECE,"",false);
                    Tile tile(piece,col,row);

                    this->tiles.push_back(tile);
                }
            }
        };

        Tile getTile(int col, int row){
            for(int p = 0; p < 3; p++){
                if(this->tiles.at(p).getColumn() == col && this->tiles.at(p).getRow() == row){
                    return this->tiles.at(p);
                }
            }
        }

        bool hasTile(int col, int row){
            for(int p = 0; p < 3; p++){
                if(this->tiles.at(p).getColumn() == col && this->tiles.at(p).getRow() == row){
                    return true;
                }
            }

            return false;
        }

        void draw(){
            for(int col = 0; col < 3; col++){
                for(int row = 0; row < 3; row++){
                    Tile tile = this->getTile(col,row);

                    cout << tile.getPiece().getSprite();

                    if(row < 2){
                        cout << "|";
                    }
                }

                if(col < 2){
                    cout << "\n-----\n";
                }
            }
            cout << "\n\n\n";
        }
};

class AI {
    private:
        string diff;
    public:
        AI(string difficulty){
            this->diff = difficulty;
        };
    
        void play(){

        }
};

void clrscr(){
    cout << "\033[2J\033[1;1H";
};

int main()
{
    random_device rd;
    Board board;

    bool done = false;

    while(done == false){
        int column;
        int row;

        clrscr();
        board.draw();

        cout << "Enter the column: ";
        cin >> column;
        
        cout << "\n";

        cout << "Enter the row: ";
        cin >> row;
        cout << "\n\n";

        if(board.hasTile(column,row)){
            Tile tile = board.getTile(column,row);
            if(tile.getPiece().empty() == true){
                Piece piece(PLAYER_PIECE,"player",false);

                tile.setPiece(piece);
            }
        }
    }
    return 0;
}