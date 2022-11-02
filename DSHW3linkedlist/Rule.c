#include "shogi.h"
#include "Rule.h"

bool Chess_Move_Rule(Chess chess,int rownow,int columnnow,int rowtogo,int columntogo){
    if(chess.change == 1){
        switch(chess.kind){ //這些升變後走法和金將一樣
            case Foot:
            case Car:
            case Horse:
            case Silver:
            chess.kind = Gold;
            break;
        }
    }
    int chessside;
    if(chess.side == X){
        chessside = 1;
    }
    else {
        chessside = -1;
    }
    
    switch(chess.kind){//各棋子規則
    case Foot://每次只可向前行1格，不能後退。
        if(columntogo == columnnow && rowtogo - rownow == 1*(chessside)){
            
            return 1;
        }
        else{
            return 0;
        }
        break;

    case Car://每次可向前行任何1格，但不能後退。
        if(columntogo == columnnow && rowtogo*(chessside) > rownow*(chessside) ){
            return 1;
        }
        else{
            return 0;
        }
        break;

    case Horse://每次行右上格或左上格對上的1格，換言之行正「日」字，沒有絆馬腳，但不能後退。
        if(columntogo - columnnow == abs(1) && rowtogo - rownow == 2*(chessside)){
            return 1;
        }
        else{
            return 0;
        }
        break;

     case Silver://每次行前面、右上、右下、左上、左下的一格。
        if((columntogo - columnnow <= abs(1) && rowtogo - rownow == 1*(chessside))||(columntogo - columnnow == abs(1) && rowtogo - rownow == -1*(chessside))){
            return 1;
        }
        else{
            return 0;
        }
        break;

     case Gold://每次行前面、右上、右面、左上、左面、下面的一格。
        if((columntogo - columnnow <= abs(1) && (rowtogo - rownow == 0,1*(chessside)))||(columntogo == columnnow  && rowtogo - rownow == -1*(chessside))){
            return 1;
        }
        else{
            return 0;
        }
        break;

     case King://每次行前面、右上、右面、右下、左上、左面、左下、下面的一格。
        if(columntogo - columnnow <= abs(1) && rowtogo - rownow <= abs(1)){
            return 1;
        }
        else{
            return 0;
        }
        break;

     case Horn://每次可行對角對開的任何1格
        if(abs(columntogo - columnnow) == abs(rowtogo - rownow) ){
            return 1;
        }
        else{
            return 0;
        }
        break;

     case DragonHorse://行英文字母「X」外，更加上行上下左右的一格。
        if((abs(columntogo - columnnow) == abs(rowtogo - rownow) )||(columntogo - columnnow <= abs(1) && rowtogo - rownow <= abs(1))){
            return 1;
        }
        else{
            return 0;
        }
        break;

     case FlyingCar://可行上下左右對開的任何1格，換言之行中文「十」字。
        if((columntogo == columnnow)||(rowtogo == rownow)){
            return 1;
        }
        else{
            return 0;
        }
        break;

     case DragonKing://行「十」字外，更加上行右上、右下、左上、左下的一格
        if((columntogo == columnnow)||(rowtogo == rownow)||(columntogo - columnnow <= abs(1) && rowtogo - rownow <= abs(1))){
            return 1;
        }
        else{
            return 0;
        }
        break;

    default:
        return 0;   
    }
}



Chess Change_Rule(Chess chess,int rownow){//待完成
    rownow = (chess.side == X )?(rownow):(10-rownow);
    if(chess.kind == King);
    else {
        chess.change = (rownow >= 7)?(1):(0);
    }
    if(chess.alivechange == 1){
        chess.change = 1;
        chess.alivechange = 0;
    }
    return chess;
}


bool Relive_Rule(Chess chess,int rowrelive,int columnrelive ){//復活棋子 待完成
    if(BroadSeat[rowrelive][columnrelive].kind == Empty){ 
        return 0;
    }
    rowrelive = (chess.side == X )?(rowrelive):(10-rowrelive);
    //步兵跟香車跟桂馬不能放在最後面
    if(chess.kind == Foot||chess.kind == Car){
        if(rowrelive == 9){
            return 0;
        }
    }
    else if(chess.kind == Horse){
        if(rowrelive == 9,8){
            return 0;
        }
    }

    //同一列不能有二兵
    if(chess.kind == Foot){
        for(int counter = 0;counter<=8;counter++){
            if(BroadSeat[counter][columnrelive].side == chess.side && BroadSeat[counter][columnrelive].kind == Foot){
                return 0;
            }
        }
        /*if(seat[rowrelive + side][columnrelive].kind == 'King'){ //兵不能將死王
            if(seat[rowrelive + side][columnrelive].kind)
        }*/
    }
    else{
        return 1;
    }
}



