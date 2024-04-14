#include <iostream>
#include <bits/stdc++.h>

std::vector<std::vector<char>> board;
int dim;
int DEPTH;

bool turn1 = true;
int playerType1, playerType2;

int posx, posy;

void newBoard(){
    for(int i = 0; i < dim; i++){
        std::vector<char> v1;
        for(int j = 0; j < dim; j++) {
            v1.push_back('_');
        }
        board.push_back(v1);
    }
}

void buildBoard(){
    for(int i = 1; i <= dim; i++){
        std::cout<<" "<< i;
    }
    for(int i = 1; i <= dim; i++){
        std::cout<<"\n";
        std::cout<< i;
        for(int j = 1; j <= dim; j++){
            std::cout<<board[i-1][j-1];
            if(j < dim){
                std::cout<<"|";
            }
        }
        if(i-1 == dim/2) std::cout<<" X";
    }
    std::cout<<"\n";
    for(int i = 1; i <= dim; i++){
        std::cout<<" ";
        if(i == dim) std::cout<<"Y";
    }
    std::cout<<"\n";
}

void init(){
    std::cout<<"Please select the dimentionality of the board 3x3, 4x4, 5x5\n";
    std::cout<<"Type (3) for 3x3, (4) for 4x4, (5) for 5x5: ";
    std::cin>>dim;
    while(1){
        if(dim == 3 or dim == 4 or dim == 5) {
            break;
        } else{
            std::cout <<"Invalid dimension! Type again.\n";
            std::cout<<"Type (3) for 3x3, (4) for 4x4, (5) for 5x5: ";
            std::cin >> dim;
        }
    }
    //depth values are heuristically derived for alpha beta pruning
    if(dim == 3) DEPTH = 1000;
    if(dim == 4) DEPTH = 8;
    if(dim == 5) DEPTH = 6;


    newBoard();
    buildBoard();

    std::cout<<"Person vs Bot\n";
    std::cout<<"Person vs Person\n";
    std::cout<<"Bot vs Bot\n";
    std::cout<<"Type (1) for Person, (2) for Bot\n";
    std::cout<<"First Player is (Person or Bot): ";
    std::cin>>playerType1;
    while(1){
        if(playerType1 == 1 or playerType1 == 2) {
            break;
        } else{
            std::cout <<"Invalid Player Type! Type again.\n";
            std::cout<<"Type (1) for Person, (2) for Bot\n";
            std::cout<<"First Player is (Person or Bot): ";
            std::cin >> playerType1;
        }
    }

    std::cout<<"Type (1) for Person, (2) for Bot\n";
    std::cout<<"Second Player is (Person or Bot): ";
    std::cin>>playerType2;
    while(1){
        if(playerType2 == 1 or playerType2 == 2) {
            break;
        } else{
            std::cout <<"Invalid Player Type! Type again.\n";
            std::cout<<"Type (1) for Person, (2) for Bot\n";
            std::cout<<"Second Player is (Person or Bot): ";
            std::cin >> playerType2;
        }
    }
}


bool isGoing(){
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            if(board[i][j] == '_') return true;
        }
    }
    return false;
}

void putPos(){
    char turnChar = turn1 ? 'x' : 'o';

    while(1){
        std::cin>>posx>>posy;
        if((posx < 1 or posx > dim) or (posy < 1 or posy > dim)){
            std::cout<<"Positions are out of range! Try again!\n";
            std::cout<<"Put your position on X and Y axes:";
        }
        else if(board[posx - 1][posy - 1] == 'x' or board[posx - 1][posy - 1] == 'o'){
            std::cout<<"Invalid position! Already has element! Try again!\n";
            std::cout<<"Put your position on X and Y axes:";
        }
        else{
            board[posx-1][posy-1] = turnChar;
            break;
        }
    }
}

bool winTest(char turnChar){
    //horizontal check
    for(int i = 0; i < dim; i++){
        bool hFlag = true;
        for(int j = 0; j < dim; j++){
            if(board[i][j] != turnChar){
                hFlag = false;
                break;
            }
        }
        if(hFlag) return true;
    }

    //vertical check
    for(int j = 0; j < dim; j++){
        bool vFlag = true;
        for(int i = 0; i < dim; i++) {
            if(board[i][j] != turnChar){
                vFlag = false;
                break;
            }
        }
        if(vFlag) return true;
    }

    //diagonal check
    bool d1Flag = true;
    for(int i = 0; i < dim; i++){
        if(board[i][i] != turnChar){
            d1Flag = false;
            break;
        }
    }
    if(d1Flag) return true;

    bool d2Flag = true;
    for(int i = 0; i < dim; i++){
        if(board[i][dim - 1 - i] != turnChar){
            d2Flag = false;
            break;
        }
    }
    if(d2Flag) return true;

    return false;
}

int moveEval(){
    char turnChar = turn1 ? 'x' : 'o';
    char opChar = (turnChar == 'x') ? 'o' : 'x';

    if(winTest(turnChar)) return 1;
    if(winTest(opChar)) return -1;
    return 0;
}

int alphaBetaP(char turnChar, int depth, bool isMax, int alpha, int beta){ // alpha beta
    char opChar = (turnChar == 'x') ? 'o' : 'x';

    if(!isGoing() || depth == 0) return 0;

    int score = moveEval();

    if(score == 1) return score;
    if(score == -1) return score;



    if(isMax){
        int best = -10;
        for(int i = 0; i < dim; i++){
            for(int j = 0; j < dim; j++){
                if(board[i][j] == '_'){
                    board[i][j] = turnChar;

                    best = std::max(best, alphaBetaP(turnChar, depth - 1, !isMax, alpha, beta));
                    alpha = std::max(alpha, best);

                    board[i][j] = '_';

                    if(beta <= alpha) break;
                }
            }
        }
        return best;
    }
    else{
        int best = 10;
        for(int i = 0; i < dim; i++){
            for(int j = 0; j < dim; j++){
                if(board[i][j] == '_'){
                    board[i][j] = opChar;

                    best = std::min(best, alphaBetaP(opChar, depth - 1, !isMax, alpha, beta));
                    beta = std::min(beta, best);

                    board[i][j] = '_';

                    if(beta <= alpha) break;

                }
            }
        }
        return best;
    }
}

void botMove(){
    char turnChar = turn1 ? 'x' : 'o';

    int bestVal = -1000;

    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            if(board[i][j] == '_'){
                board[i][j] = turnChar;

                int moveVal = alphaBetaP(turnChar, DEPTH, false, -1000, 1000);

                board[i][j] = '_';

                if(moveVal > bestVal){
                    posx = i;
                    posy = j;
                    bestVal = moveVal;
                }
            }
        }
    }

    board[posx][posy] = turnChar;
}

int main() {

    init();


    if(playerType1 == 1 and playerType2 == 1){
        while(isGoing()){
            char turnChar = turn1 ? 'x' : 'o';
            std::cout<<"Put your position on X and Y axes:";
            putPos();

            buildBoard();


            if(winTest(turnChar)){
                break;
            }
            turn1 = !turn1;
        }
    }
    else if(playerType1 == 2 and playerType2 == 2){
        while(isGoing()){
            char turnChar = turn1 ? 'x' : 'o';
            botMove();

            buildBoard();

            if(winTest(turnChar)){
                break;
            }
            turn1 = !turn1;
        }
    }
    else{
        bool humanTurn;
        bool humanBool = playerType1 == 1;

        while(isGoing()){
            char turnChar = turn1 ? 'x' : 'o';

            humanTurn = humanBool == turn1;

            if(humanTurn){
                std::cout<<"Put your position on X and Y axes:";
                putPos();
            }else{
                std::cout<<"Bot's turn...\n";
                botMove();
            }

            buildBoard();

            if(winTest(turnChar)){
                break;
            }
            turn1 = !turn1;
        }
    }

    std::cout<<"==========GAME OVER==========\n";

    if(isGoing()){
        if(turn1){
            std::cout<<"FIRST (1st) Player WON!";
        }else{
            std::cout<<"SECOND (2nd) Player WON!";
        }
    }
    else{
        std::cout<<"DRAW!\n";
    }



    system("pause");
    return 0;
}

