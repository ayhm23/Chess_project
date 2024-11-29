#include <bits/stdc++.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <winsock2.h> // Windows socket library
#include <utility>



using namespace std;

#define PORT 8080
#define maxVal 255

bool turn = true;

#define changeTurn turn = !turn

// class for a general piece
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
};

// class for each tile/square in the 8*8 chess board
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

// a derived class for each specific piece
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
};

class Pawn : public Piece{
    public:

    Pawn(int x, int y, int c): Piece("Pawn", 1, c){
        coords = make_pair(x, y);
    }

    bool isValidMove(pair <int, int> target){
        if(colour == 0 && coords.second - target.second == 1 && abs(target.first - coords.first) < 2){
            return true;
        }
        if(colour == 1 && target.second - coords.second == 1 && abs(target.first - coords.first) < 2){
            return true;
        }
        if(colour == 0 && coords.second - target.second == 2 && target.first == coords.first && coords.second == 6){
            return true;
        }
        if(colour == 1 && target.second - coords.second == 2 && target.first == coords.first && coords.second == 1){
            return true;
        }
        return false;
    }
};

// a dummy class to represent an empty square
class Empty : public Piece{
    public:

    Empty(): Piece("Empty", 0, -1){
        coords = make_pair(-1, -1);
    }

    bool isValidMove(pair <int, int> target){
        return true;
    }
};

// initialising an empty square object
Piece * emptySquare = new Empty();

// class for the 8*8 chess board on which the game will be played
class Board{
    public:

    Square board[8][8];
    Piece * blackKing,  * whiteKing;
    vector <Piece *> blackPieces;
    vector <Piece *> whitePieces;
    string prevMove;
    int adv;

    Board(){
        adv = 0;
        prevMove = "";
        // initialising the board to the starting position
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
                    board[i][j].occupant = emptySquare;
                }
            }
        }
    }

    // creates another Board object with identical data
    Board * copy(){
        Board * temp = new Board();
        temp->blackKing = this->blackKing;
        temp->whiteKing = this->whiteKing;
        temp->adv = this->adv;
        temp->prevMove = this->prevMove;
        temp->whitePieces = this->whitePieces;
        temp->blackPieces = this->blackPieces;
        for(int i = 0; i < 8; i ++){
            for(int j = 0; j < 8; j ++){
                temp->board[i][j].occupant = this->board[i][j].occupant;
            }
        }
        return temp;
    }

    // method for printing the board on the console
    void print(){
       for(int j = 7; j > -1; j --){
            for(int i = 0; i < 8; i ++){
                if(this->board[i][j].occupant == emptySquare){
                    cout << "0 ";
                }
                else if(this->board[i][j].occupant->name == "Pawn"){
                    cout << (this->board[i][j].occupant->colour == 0 ? "p " : "P ");
                }
                else if(this->board[i][j].occupant->name == "Rook"){
                    cout << (this->board[i][j].occupant->colour == 0 ? "r " : "R ");
                }
                else if(this->board[i][j].occupant->name == "Knight"){
                    cout << (this->board[i][j].occupant->colour == 0 ? "k " : "K ");
                }
                else if(this->board[i][j].occupant->name == "Bishop"){
                    cout << (this->board[i][j].occupant->colour == 0 ? "b " : "B ");
                }
                else if(this->board[i][j].occupant->name == "Queen"){
                    cout << (this->board[i][j].occupant->colour == 0 ? "q " : "Q ");
                }
                else if(this->board[i][j].occupant->name == "King"){
                    cout << (this->board[i][j].occupant->colour == 0 ? "l " : "L ");
                }
                else{
                    cout << "0 ";
                }
            }
            cout << "\n";
        }
    }

    bool isBCheck();
    bool isWCheck();
    bool isStalemate(bool turn);
    bool isCheckmate(bool turn);
};

// function to check if the desired path of the piece is blocked by some other piece
bool isBlocked(Piece * piece, Board * board, pair <int, int> target){
    // knights and kings cannot be blocked
    if(piece->name == "Knight" || piece->name == "King"){
        return false;
    }
    // checking the straight paths
    if(piece->name == "Rook" || piece->name == "Queen" || piece->name == "Pawn"){
        if(piece->coords.first == target.first){
            int smalll = (piece->coords.second > target.second ? target.second : piece->coords.second);
            int big = (piece->coords.second < target.second ? target.second : piece->coords.second);
            for(int i = 1; i < abs(big - smalll); i ++){
                if(board->board[target.first][smalll + i].occupant != emptySquare){
                    return true;
                }
            }
        }
        else if(piece->coords.second == target.second){
            int smalll = (piece->coords.first > target.first ? target.first : piece->coords.first);
            int big = (piece->coords.first < target.first ? target.first : piece->coords.first);
            for(int i = 1; i < abs(big - smalll); i ++){
                if(board->board[smalll + i][target.second].occupant != emptySquare){
                    return true;
                }
            }
        }
        if(piece->name != "Rook"){
            return false;
        }
    }
    // checking the diagonal paths
    if(piece->name == "Bishop" || piece->name == "Queen"){
        if(abs(piece->coords.first - target.first) == abs(piece->coords.second - target.second)){
            int diff = abs(piece->coords.first - target.first);
            int low2 = (piece->coords.second > target.second ? target.second : piece->coords.second);
            int low1 = (piece->coords.first > target.first ? target.first : piece->coords.first);
            for(int i = 1; i < diff; i ++){
                if(board->board[piece->coords.first + i * (piece->coords.first > target.first ? -1 : 1)][piece->coords.second + i * (piece->coords.second > target.second ? -1 : 1)].occupant != emptySquare){
                    cout <<"check"<<low1 + i << " " << low2 + i << endl;
                    return true;
                }
            }
        }
        return false;
    }
    return false;
}

// function to check if the desired destination is within bounds of the chess board
bool isValidSquare(pair <int, int> coords){
    int x = coords.first, y = coords.second;
    return !(x < 0 || x > 7 || y < 0 || y > 7);
}

// function to check if the desired move is a castling move
bool isCastling(Board * board, Piece * piece, pair <int, int> target){
    if(piece->colour == 1 && dynamic_cast<King *>(board->whiteKing)->castle){
        if(board->isWCheck()){
            return false;
        }
        if(target == make_pair(2, 0) && board->board[1][0].occupant == emptySquare && board->board[2][0].occupant == emptySquare && board->board[3][0].occupant == nullptr && board->board[0][0].occupant != nullptr){
            if(board->board[0][0].occupant->name == "Rook"){
                return true;
            }
            return false;
        }
        if(target == make_pair(6, 0) && board->board[5][0].occupant == emptySquare && board->board[6][0].occupant == emptySquare && board->board[7][0].occupant != emptySquare){
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
        if(target == make_pair(2, 7) && board->board[1][7].occupant == emptySquare && board->board[2][7].occupant == emptySquare && board->board[3][7].occupant == emptySquare && board->board[0][7].occupant != emptySquare){
            if(board->board[0][7].occupant->name == "Rook"){
                return true;
            }
            return false;
        }
        if(target == make_pair(6, 7) && board->board[5][7].occupant == emptySquare && board->board[6][7].occupant == emptySquare && board->board[7][7].occupant != emptySquare){
            if(board->board[7][7].occupant->name == "Rook"){
                return true;
            }
            return false;
        }
    }
    return false;
}

// function to castle
void castle(Board * board, Piece * piece, pair <int, int> target){
    if(target == make_pair(2, 0)){
        Piece * temp = board->board[0][0].occupant;
        temp->coords = make_pair(3, 0);
        board->board[0][0].occupant = emptySquare;
        board->board[3][0].occupant = temp;
        board->board[4][0].occupant = emptySquare;
        board->board[2][0].occupant = piece;
        piece->coords = target;
        return;
    }
    if(target == make_pair(6, 0)){
        Piece * temp = board->board[7][0].occupant;
        temp->coords = make_pair(5, 0);
        board->board[7][0].occupant = emptySquare;
        board->board[5][0].occupant = temp;
        board->board[4][0].occupant = emptySquare;
        board->board[6][0].occupant = piece;
        piece->coords = target;
        return;
    }
    if(target == make_pair(2, 7)){
        Piece * temp = board->board[0][7].occupant;
        temp->coords = make_pair(3, 7);
        board->board[0][7].occupant = emptySquare;
        board->board[3][7].occupant = temp;
        board->board[4][7].occupant = emptySquare;
        board->board[2][7].occupant = piece;
        piece->coords = target;
        return;
    }
    if(target == make_pair(6, 7)){
        Piece * temp = board->board[7][7].occupant;
        temp->coords = make_pair(5, 7);
        board->board[7][7].occupant = emptySquare;
        board->board[5][7].occupant = temp;
        board->board[4][7].occupant = emptySquare;
        board->board[6][7].occupant = piece;
        piece->coords = target;
        return;
    }
}

// function to remove a piece from the board upon capture
void removePiece(vector <Piece *> & pieces, Piece * piece){
    int i = 0;
    for(Piece * p : pieces){
        if(p->name == piece->name && p->coords == piece->coords){
            break;
        }
        i ++;
    }
    if(i = pieces.size()){
        return;
    }
    pieces.erase(pieces.begin() + i);
    return;
}

// function to format the previous move into a string for storage
string moveFormat(const string & piece, pair <int, int> initial, pair <int, int> target){
    string ans = "";
    ans += piece + " ";
    ans += to_string(initial.first) + to_string(initial.second) + " ";
    ans += to_string(target.first) + to_string(target.second);
    return ans;
}

Board * nextMove(Board * boardOriginal, Piece * piece, pair <int, int> target);
bool goesToCheck(Board * board, Piece * piece, pair <int, int> target);

// functions to check if any of the kings are Checked
bool Board::isBCheck(){
    pair <int, int> coords = this->blackKing->coords;
    for(Piece * piece : this->whitePieces){
        if(!isBlocked(piece, this, coords) && piece->isValidMove(coords)){
            return true;
        }
    }
    return false;
}

bool Board::isWCheck(){
    pair <int, int> coords = this->whiteKing->coords;
    for(Piece * piece : this->blackPieces){
        if(!isBlocked(piece, this, coords) && piece->isValidMove(coords)){
            return true;
        }
    }
    return false;
}

// function to check if the game has ended by stalemate
bool Board::isStalemate(bool turn){
    // If the player's king is in check, it's not a stalemate
    if((turn && this->isWCheck()) || (!turn && this->isBCheck())){
        return false;
    }

    // Loop through each piece of the current player
    vector<Piece*>& playerPieces = turn ? this->whitePieces : this->blackPieces;
    
    for(Piece* piece : playerPieces){
        // Try all possible moves for this piece
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                pair<int, int> target = make_pair(i, j);
                Board* temp = nextMove(this, piece, target);
                
                if(!temp) continue; // Skip invalid moves
                
                // If any valid move does not leave the king in check, it's not stalemate
                if((turn && !temp->isWCheck()) || (!turn && !temp->isBCheck())){
                    delete temp;
                    return false; // Not stalemate if there's a valid move
                }

                delete temp;
            }
        }
    }

    // If no valid moves exist and the king is not in check, it's stalemate
    return true;
}

// function to check if the game has ended by checkmate
bool Board::isCheckmate(bool turn){
    // Check if the player's king is in check
    if((turn && !this->isWCheck()) || (!turn && !this->isBCheck())){
        return false; // If the king is not in check, it's not checkmate
    }
    
    // Loop through each piece of the current player
    vector<Piece*>& playerPieces = turn ? this->whitePieces : this->blackPieces;
    
    for(Piece* piece : playerPieces){
        // Try all possible moves for this piece
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                pair<int, int> target = make_pair(i, j);
                Board* temp = nextMove(this, piece, target);
                
                if(temp) continue; // Skip if the move is invalid
                
                if(temp == nullptr){
                    return true;
                }

                // If the move results in the king not being in check, it's not checkmate
                if((turn && !temp->isWCheck()) || (!turn && !temp->isBCheck())){
                    delete temp;
                    return false; // Not checkmate if the king is no longer in check
                }
                
                delete temp;
            }
        }
    }
    
    // If all moves lead to the king still being in check, it's checkmate
    return true;
}

// function that returns a board that is the result of the desired move
// this function is used to check if a move leads to Check on your king, which would make it an illegal move
Board * nextMove(Board * boardOriginal, Piece * piece, pair <int, int> target){
    Board * board;
    board = boardOriginal->copy();
    if(piece->name == "Pawn" && piece->coords.first != target.first && board->board[target.first][target.second].occupant == emptySquare){
        if(board->prevMove.substr(0, 4) == "Pawn"){
            if(stoi(board->prevMove.substr(5, 2)) % 10 == 6 && stoi(board->prevMove.substr(8, 2)) % 10 == 4){
                if(target.second == 5 && target.first == (stoi(board->prevMove.substr(5, 2)) - stoi(board->prevMove.substr(5, 2)) % 10) / 10){
                    board->board[target.first][target.second].occupant = piece;
                    //piece->coords = target;
                    removePiece(board->blackPieces, board->board[target.first][4].occupant);
                    board->board[target.first][4].occupant = emptySquare;
                }
                return board;
            }
            else if(stoi(board->prevMove.substr(5, 2)) % 10 == 1 && stoi(board->prevMove.substr(8, 2)) % 10 == 3){
                if(target.second == 2 && target.first == (stoi(board->prevMove.substr(5, 2)) - stoi(board->prevMove.substr(5, 2)) % 10) / 10){
                    board->board[target.first][target.second].occupant = piece;
                    //piece->coords = target;
                    removePiece(board->whitePieces, board->board[target.first][3].occupant);
                    board->board[target.first][3].occupant = emptySquare;
                }
                return board;
            }
            else{
                return nullptr;
            }
        }
    }
    if(piece->name == "Pawn" && piece->coords.first == target.first && board->board[target.first][target.second].occupant != emptySquare){
        return nullptr;
    }
    if(!isValidSquare(target) || isBlocked(piece, board, target) || !piece->isValidMove(target)){
        return nullptr;
    }
    if(piece->coords == target){
        return nullptr;
    }
    if(isCastling(board, piece, target)){
        castle(board, piece, target);
        board->prevMove = moveFormat("Castle", piece->coords, target);
        return board;
    }
    if(board->board[target.first][target.second].occupant == emptySquare){
        board->board[piece->coords.first][piece->coords.second].occupant = emptySquare;
        //piece->coords = target;
        board->board[target.first][target.second].occupant = piece;
        board->prevMove = moveFormat(piece->name, piece->coords, target);
        return board;
    }
    if(board->board[target.first][target.second].occupant->colour == piece->colour){
        return nullptr;
    }
    if(board->board[target.first][target.second].occupant->colour != piece->colour){
        board->board[piece->coords.first][piece->coords.second].occupant = emptySquare;
        if(piece->colour == 0){
            removePiece(board->whitePieces, board->board[target.first][target.second].occupant);
        }
        else{
            removePiece(board->blackPieces, board->board[target.first][target.second].occupant);
        }
        board->board[target.first][target.second].occupant = piece;
        board->prevMove = moveFormat(piece->name, piece->coords, target);
        return board;
    }
}

// function to check if the move puts your own king in check, making it an illegal move
bool goesToCheck(Board * board, Piece * piece, pair <int, int> target){
    pair <int, int> coord = piece->coords;
    Board * temp = nextMove(board, piece, target);
    if(!temp){
        piece->coords = coord;
        return false;
    }
    if(turn && temp->isWCheck()){
        piece->coords = coord;
        return true;
    }
    if(!turn && temp->isBCheck()){
        piece->coords = coord;
        return true;
    }
    piece->coords = coord;
    return false;
}

// function to execute the desired move
bool move(Board * board, Piece * piece, pair <int, int> target){
    // checking for en passant
    if(piece->name == "Pawn" && piece->coords.first != target.first && board->board[target.first][target.second].occupant == emptySquare){
        if(board->prevMove.substr(0, 4) == "Pawn"){
            if(stoi(board->prevMove.substr(5, 2)) % 10 == 6 && stoi(board->prevMove.substr(8, 2)) % 10 == 4){
                if(target.second == 5 && target.first == (stoi(board->prevMove.substr(5, 2)) - stoi(board->prevMove.substr(5, 2)) % 10) / 10){
                    board->board[target.first][target.second].occupant = piece;
                    piece->coords = target;
                    removePiece(board->blackPieces, board->board[target.first][4].occupant);
                    board->board[target.first][4].occupant = emptySquare;
                }
                return true ;
            }
            else if(stoi(board->prevMove.substr(5, 2)) % 10 == 1 && stoi(board->prevMove.substr(8, 2)) % 10 == 3){
                if(target.second == 2 && target.first == (stoi(board->prevMove.substr(5, 2)) - stoi(board->prevMove.substr(5, 2)) % 10) / 10){
                    board->board[target.first][target.second].occupant = piece;
                    piece->coords = target;
                    removePiece(board->whitePieces, board->board[target.first][3].occupant);
                    board->board[target.first][3].occupant = emptySquare;
                }
                return true;
            }
            else{
                return false;
            }
        }
    }
    // checking if pawn is going straight yet capturing
    else if(piece->name == "Pawn" && piece->coords.first == target.first && board->board[target.first][target.second].occupant != emptySquare){
        return false;
    }
    // checking if the path and destination are valid
    else if(!isValidSquare(target) || isBlocked(piece, board, target) || !piece->isValidMove(target)){
        
        return false;
    }
    // checking if target is same as current location
    else if(piece->coords == target){
        
        return false;
    }
    else if(goesToCheck(board, piece, target)){
    
        return false;
    }
    else if(isCastling(board, piece, target)){
        castle(board, piece, target);
        board->prevMove = moveFormat("Castle", piece->coords, target);
        changeTurn;
        return true;
    }
    // checking if the move is not a capture
    else if(board->board[target.first][target.second].occupant == emptySquare){
        board->board[piece->coords.first][piece->coords.second].occupant = emptySquare;
        piece->coords = target;
        board->board[target.first][target.second].occupant = piece;
        board->prevMove = moveFormat(piece->name, piece->coords, target);
        changeTurn;
        return true;
    }
    // checking if the move leads to capturing your own piece
    else if(board->board[target.first][target.second].occupant->colour == piece->colour){
        return false;
    }
    // checking if the move is a capture
    else if(board->board[target.first][target.second].occupant->colour != piece->colour){
        board->board[piece->coords.first][piece->coords.second].occupant = emptySquare;
        if(piece->colour == 0){
            removePiece(board->whitePieces, board->board[target.first][target.second].occupant);
        }
        else{
            removePiece(board->blackPieces, board->board[target.first][target.second].occupant);
        }
        board->board[target.first][target.second].occupant = piece;
        piece->coords = target;
        board->prevMove = moveFormat(piece->name, piece->coords, target);
        changeTurn;
        return true;
    }
}
// promoting the pawn 
void promote(Board * board, Piece * piece, string name){
    int x = piece->coords.first, y = piece->coords.second, c = piece->colour;
    if(name == "Queen"){
        Queen * ptr = new Queen(x, y, c);
        board->adv += (c == 1 ? 8 : -8);
        if(c == 0){
            board->blackPieces.push_back(ptr);
            removePiece(board->blackPieces, piece);
            delete piece;
        }
        else{
            board->whitePieces.push_back(ptr);
            removePiece(board->whitePieces, piece);
            delete piece;
        }
    }
    else if(name == "Rook"){
        Rook * ptr = new Rook(x, y, c);
        board->adv += (c == 1 ? 4 : -4);
        if(c == 0){
            board->blackPieces.push_back(ptr);
            removePiece(board->blackPieces, piece);
            delete piece;
        }
        else{
            board->whitePieces.push_back(ptr);
            removePiece(board->whitePieces, piece);
            delete piece;
        }
    }
    else if(name == "Knight"){
        Knight * ptr = new Knight(x, y, c);
        board->adv += (c == 1 ? 2 : -2);
        if(c == 0){ 
            board->blackPieces.push_back(ptr);
            removePiece(board->blackPieces, piece);
            delete piece;
        }
        else{
            board->whitePieces.push_back(ptr);
            removePiece(board->whitePieces, piece);
            delete piece;
        }
    }
    else if(name == "Bishop"){
        Bishop * ptr = new Bishop(x, y, c);
        board->adv += (c == 1 ? 2 : -2);
        if(c == 0){
            board->blackPieces.push_back(ptr);
            removePiece(board->blackPieces, piece);
            delete piece;
        }
        else{
            board->whitePieces.push_back(ptr);
            removePiece(board->whitePieces, piece);
            delete piece;
        }
    }
}



void send_message(SOCKET client_socket, const string& message) {
    send(client_socket, message.c_str(), message.length(), 0);
}

string receive_message(SOCKET client_socket) {
    char buffer[1024] = {0};
    int valread = recv(client_socket, buffer, 1024, 0);
    if (valread > 0) {
        return string(buffer, valread);
    }
    return "";
}

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket[2];
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size = sizeof(client_addr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed: " << WSAGetLastError() << endl;
        return 1;
    }

    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cerr << "Socket creation failed: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // Bind socket to the port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "Bind failed: " << WSAGetLastError() << endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen for connections
    if (listen(server_socket, 2) == SOCKET_ERROR) {
        cerr << "Listen failed: " << WSAGetLastError() << endl;
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    cout << "Waiting for two clients to connect..." << endl;

    // Accept two clients
    for (int i = 0; i < 2; ++i) {
        client_socket[i] = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
        if (client_socket[i] == INVALID_SOCKET) {
            cerr << "Accept failed: " << WSAGetLastError() << endl;
            closesocket(server_socket);
            WSACleanup();
            return 1;
        }
        cout << "Client " << i + 1 << " connected." << endl;
    }

    Board* board = new Board();
    int turn = 1; // 0 for white, 1 for black
    int turncount=0;
    while (true) {
        
        board->print();

        if (board->isCheckmate(turn)) {
            cout << "Checkmate! Player " << turn + 1 << " loses!" << endl;
            send_message(client_socket[turn], "Checkmate! You lose.");
            send_message(client_socket[1 - turn], "Checkmate! You win.");
            break;
        }

        if (board->isStalemate(turn)) {
            cout << "Stalemate!" << endl;
            send_message(client_socket[0], "Stalemate! It's a draw.");
            send_message(client_socket[1], "Stalemate! It's a draw.");
            break;
        }

        // Notify current player to send a move
        send_message(client_socket[turn], "Your move:\n");
        send_message(client_socket[1 - turn], "Waiting for opponent...\n");

        // Receive move
        string move_input = receive_message(client_socket[turn]);
        cout << move_input << endl;
        
        stringstream ss(move_input);
        string name, initial, target;
        ss >> name >> initial >> target;
         pair<int, int> i;
          pair<int, int> t;
          cout<<turncount<<endl;
        if(turncount%2==0){
         i = make_pair(stoi(initial.substr(0, 1)), stoi(initial.substr(1, 1)));
         t = make_pair(stoi(target.substr(0, 1)), stoi(target.substr(1, 1)));
        }
        else{
              i = make_pair(stoi(initial.substr(0, 1)), 7-stoi(initial.substr(1, 1)));
             t = make_pair(stoi(target.substr(0, 1)), 7-stoi(target.substr(1, 1)));
        }
         
      
    
        if (board->board[i.first][i.second].occupant->name == name && !(board->board[i.first][i.second].occupant->colour ^ turn)) {
            if (move(board, board->board[i.first][i.second].occupant, t)) {
                send_message(client_socket[turn], "MOVE_VALID\n");
                send_message(client_socket[1 - turn], "Opponent move: " + move_input + "\n");
                turn = 1 - turn;
                turncount++;
                cout<<turncount<<endl;
            } else {
                send_message(client_socket[turn], "-1 Invalid move.\n");
            }
        } else {
            send_message(client_socket[turn], "-1 Error: Invalid piece or move.\n");
        }
    }

    // Close sockets
    for (int i = 0; i < 2; ++i) {
        closesocket(client_socket[i]);
    }
    closesocket(server_socket);
    WSACleanup();

    delete board;
    return 0;
}
