#include <bits/stdc++.h>
#include <string>


using namespace std;

#define maxVal 255

class Piece{
    public:

    string name;
    int val;
    pair<int, int> coords;
    int colour;

    Piece(): name(""), val(0){}

    Piece(string name, int val, int colour): name(name), val(val), colour(colour){}

    virtual bool isValidMove(pair<int, int> target){
        return true;
    }

    virtual bool isBlocked(Board * board, pair<int, int> target){
        return false;
    }
};

class Square{
    public:

    pair<int, int> coords;
    Piece * occupant;

    Square(){}

    Square(int x, int y): occupant(nullptr){
        coords= make_pair(x, y);
    }

    Square(int x, int y, Piece * ptr): occupant(ptr){
        coords = make_pair(x, y);
    }
};

class Board{
    public:

    Square board[8][8];
    Piece * blackKing,  * whiteKing;
    vector <Piece *> blackPieces;
    vector <Piece *> whitePieces;
    int adv;

    Board(){
        adv = 0;
        for(int i = 0; i < 8; i ++){
            for(int j = 0; j < 8; j ++){
                board[i][j].coords = make_pair(i, j);
                if(j == 1){
                    board[i][j].occupant = new Pawn(i, j, 1);
                    whitePieces.push_back(board[i][j].occupant);
                }
                else if(j == 6){
                    board[i][j].occupant = new Pawn(i, j, 0);
                    blackPieces.push_back(board[i][j].occupant);
                }
                else if(j == 0 && (i == 0 || i == 7)){
                    board[i][j].occupant = new Rook(i, j, 1);
                    whitePieces.push_back(board[i][j].occupant);
                }
                else if(j == 0 && (i == 1 || i == 6)){
                    board[i][j].occupant = new Knight(i, j, 1);
                    whitePieces.push_back(board[i][j].occupant);
                }
                else if(j == 0 && (i == 2 || i == 5)){
                    board[i][j].occupant = new Bishop(i, j, 1);
                    whitePieces.push_back(board[i][j].occupant);
                }
                else if(j == 0 && (i == 3)){
                    board[i][j].occupant = new Queen(i, j, 1);
                    whitePieces.push_back(board[i][j].occupant);
                }
                else if(j == 0 && (i == 4)){
                    board[i][j].occupant = new King(i, j, 1);
                    whitePieces.push_back(board[i][j].occupant);
                    whiteKing = board[i][j].occupant;
                }
                else if(j == 7 && (i == 0 || i == 7)){
                    board[i][j].occupant = new Rook(i, j, 0);
                    blackPieces.push_back(board[i][j].occupant);
                }
                else if(j == 7 && (i == 1 || i == 6)){
                    board[i][j].occupant = new Knight(i, j, 0);
                    blackPieces.push_back(board[i][j].occupant);
                }
                else if(j == 7 && (i == 2 || i == 5)){
                    board[i][j].occupant = new Bishop(i, j, 0);
                    blackPieces.push_back(board[i][j].occupant);
                }
                else if(j == 7 && (i == 3)){
                    board[i][j].occupant = new Queen(i, j, 0);
                    blackPieces.push_back(board[i][j].occupant);
                }
                else if(j == 7 && (i == 4)){
                    board[i][j].occupant = new King(i, j, 0);
                    blackPieces.push_back(board[i][j].occupant);
                    blackKing = board[i][j].occupant;
                }
                else{
                    board[i][j].occupant = nullptr;
                }
            }
        }
    }

    bool isBCheck(){
        pair <int, int> coords = this->blackKing->coords;
        for(Piece * piece : this->whitePieces){
            if(!piece->isBlocked(this, coords) && piece->isValidMove(coords)){
                return true;
            }
        }
        return false;
    }

    bool isWCheck(){
        pair <int, int> coords = this->whiteKing->coords;
        for(Piece * piece : this->blackPieces){
            if(!piece->isBlocked(this, coords) && piece->isValidMove(coords)){
                return true;
            }
        }
        return false;
    }
};

class King : public Piece{
    public:

    bool castle;

    King(int x, int y, int c): Piece("King", maxVal, c){
        coords = make_pair(x, y);
        castle = true;
    }

    bool isValidMove(pair <int, int> target){
        if(abs(coords.first - target.first) < 2 && abs(coords.second - target.second) < 2){
            return true;
        }
        if(castle){
            if(colour == 0 && (target == make_pair(2, 7) || target == make_pair(6, 7))){
                return true;
            }
            if(colour == 1 && (target == make_pair(2, 0) || target == make_pair(6, 0))){
                return true;
            }
        }
        return false;
    }

    bool isBlocked(Board * board, pair<int, int> target){
        return false;
    }
};

class Queen : public Piece{
    public:

    Queen(int x, int y, int c): Piece("Queen", 9, c){
        coords = make_pair(x, y);
    }

    bool isValidMove(pair <int, int> target){
        if(abs(coords.first - target.first) == 0 || abs(coords.second - target.second) == 0 || abs(coords.first - target.first) == abs(coords.second - target.second)){
            return true;
        }
        return false;
    }

    bool isBlocked(Board * board, pair<int, int> target){
        if(abs(coords.first - target.first) == 0){
            int x = coords.first;
            int low = (coords.second > target.second ? target.second : coords.second);
            int high = (coords.second < target.second ? target.second : coords.second);
            for(int i = low + 1; i < high; i ++){
                if(board->board[x][i].occupant != nullptr){
                    return true;
                }
            }
        }
        if(abs(coords.second - target.second) == 0){
            int y = coords.second;
            int low = (coords.first > target.first ? target.first : coords.first);
            int high = (coords.first < target.first ? target.first : coords.first);
            for(int i = low + 1; i < high; i ++){
                if(board->board[i][y].occupant != nullptr){
                    return true;
                }
            }
        }
        if(abs(coords.first - target.first) == abs(coords.second - target.second)){
            int diff = abs(coords.first - target.first);
            int low2 = (coords.second > target.second ? target.second : coords.second);
            int low1 = (coords.first > target.first ? target.first : coords.first);
            for(int i = 1; i < diff; i ++){
                if(board->board[low1 + i][low2 + i].occupant != nullptr){
                    return true;
                }
            }
        }
        return false;
    }
};

class Rook : public Piece{
    public:

    Rook(int x, int y, int c): Piece("Rook", 5, c){
        coords = make_pair(x, y);
    }

    bool isValidMove(pair <int, int> target){
        if(abs(coords.first - target.first) == 0 || abs(coords.second - target.second) == 0){
            return true;
        }
        return false;
    }

    bool isBlocked(Board * board, pair<int, int> target){
        if(abs(coords.first - target.first) == 0){
            int x = coords.first;
            int low = (coords.second > target.second ? target.second : coords.second);
            int high = (coords.second < target.second ? target.second : coords.second);
            for(int i = low + 1; i < high; i ++){
                if(board->board[x][i].occupant != nullptr){
                    return true;
                }
            }
        }
        if(abs(coords.second - target.second) == 0){
            int y = coords.second;
            int low = (coords.first > target.first ? target.first : coords.first);
            int high = (coords.first < target.first ? target.first : coords.first);
            for(int i = low + 1; i < high; i ++){
                if(board->board[i][y].occupant != nullptr){
                    return true;
                }
            }
        }
    }
};

class Bishop : public Piece{
    public:

    Bishop(int x, int y, int c): Piece("Bishop", 3, c){
        coords = make_pair(x, y);
    }

    bool isValidMove(pair <int, int> target){
        if(abs(coords.first - target.first) == abs(coords.second - target.second)){
            return true;
        }
        return false;
    }

    bool isBlocked(Board * board, pair<int, int> target){
        if(abs(coords.first - target.first) == abs(coords.second - target.second)){
            int diff = abs(coords.first - target.first);
            int low2 = (coords.second > target.second ? target.second : coords.second);
            int low1 = (coords.first > target.first ? target.first : coords.first);
            for(int i = 1; i < diff; i ++){
                if(board->board[low1 + i][low2 + i].occupant != nullptr){
                    return true;
                }
            }
        }
        return false;
    }
};

class Knight : public Piece{
    public:

    Knight(int x, int y, int c): Piece("Knight", 3, c){
        coords = make_pair(x, y);
    }

    bool isValidMove(pair <int, int> target){
        if((abs(coords.first - target.first) == 2 && abs(coords.second - target.second) == 1) || (abs(coords.first - target.first) == 1 && abs(coords.second - target.second) == 2)){
            return true;
        }
        return false;
    }

    bool isBlocked(Board * board, pair<int, int> target){
        return false;
    }
};

class Pawn : public Piece{
    public:

    Pawn(int x, int y, int c): Piece("Pawn", 1, c){
        coords = make_pair(x, y);
    }

    bool isBlocked(Board * board, pair<int, int> target){
        if(abs(coords.first - target.first) == 0){
            int x = coords.first;
            int low = (coords.second > target.second ? target.second : coords.second);
            int high = (coords.second < target.second ? target.second : coords.second);
            for(int i = low + 1; i < high; i ++){
                if(board->board[x][i].occupant != nullptr){
                    return true;
                }
            }
        }
        return false;
    }
};

bool turn = true;

#define changeTurn turn = !turn

bool isValidSquare(pair <int, int> coords){
    int x = coords.first, y = coords.second;
    return !(x < 0 || x > 7 || y < 0 || y > 7);
}

bool isCastling(Board * board, Piece * piece, pair <int, int> target){
    if(piece->colour == 1 && dynamic_cast<King *>(board->whiteKing)->castle){
        if(board->isWCheck()){
            return false;
        }
        if(target == make_pair(2, 0) && board->board[1][0].occupant == nullptr && board->board[2][0].occupant == nullptr && board->board[3][0].occupant == nullptr && board->board[0][0].occupant != nullptr){
            if(board->board[0][0].occupant->name == "Rook"){
                return true;
            }
            return false;
        }
        if(target == make_pair(6, 0) && board->board[5][0].occupant == nullptr && board->board[6][0].occupant == nullptr && board->board[7][0].occupant != nullptr){
            if(board->board[7][0].occupant->name == "Rook"){
                return true;
            }
            return false;
        }
    }
    if(piece->colour == 0 && dynamic_cast<King *>(board->blackKing)->castle){
        if(board->isBCheck()){
            return false;
        }
        if(target == make_pair(2, 7) && board->board[1][7].occupant == nullptr && board->board[2][7].occupant == nullptr && board->board[3][7].occupant == nullptr && board->board[0][7].occupant != nullptr){
            if(board->board[0][7].occupant->name == "Rook"){
                return true;
            }
            return false;
        }
        if(target == make_pair(6, 7) && board->board[5][7].occupant == nullptr && board->board[6][7].occupant == nullptr && board->board[7][7].occupant != nullptr){
            if(board->board[7][7].occupant->name == "Rook"){
                return true;
            }
            return false;
        }
    }
    return false;
}

void move(Board * board, Piece * piece, pair <int, int> target){
    if(!isValidSquare(target) || piece->isBlocked(board, target) || !piece->isValidMove(target)){
        return;
    }
    if(piece->coords == target){
        return;
    }
    if(isCastling(board, piece, target)){
        castle(board, piece, target);
        changeTurn;
        return;
    }
    if(board->board[target.first][target.second].occupant == nullptr){
        board->board[piece->coords.first][piece->coords.second].occupant = nullptr;
        piece->coords = target;
        board->board[target.first][target.second].occupant = piece;
        changeTurn;
        return;
    }
    if(board->board[target.first][target.second].occupant->colour == piece->colour){
        return;
    }
    if(board->board[target.first][target.second].occupant->colour != piece->colour){
        board->board[piece->coords.first][piece->coords.second].occupant = nullptr;
        board->board[target.first][target.second].occupant->coords = make_pair(-1, -1);
        board->board[target.first][target.second].occupant = piece;
        changeTurn;
        return;
    }
}
