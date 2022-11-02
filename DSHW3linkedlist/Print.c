#include "shogi.h"
#include "Print.h"
int Print_Broad(){
    CLEAR();
    HIDE_CURSOR();
    set_disp_mode(0);
    MOVETO(0,0);
    SET_FRONT_COLOR(FRONT_BLUE);
    printf("Player X    \n");
    SET_FRONT_COLOR(FRONT_WHITE);
    printf("1 2 3 4 5 6 7 8 9\n");
    for(int i =0;i<=8;i++){
        for(int j =0;j<=8;j++){
            if(BroadSeat[i][j].side == X){
                SET_FRONT_COLOR(FRONT_BLUE);
            }
            else if(BroadSeat[i][j].side == Y){
                SET_FRONT_COLOR(FRONT_RED);
            }
            //MOVETO(4+i,4 +j);
            switch(BroadSeat[i][j].kind){
                case Foot:
                    printf("步");
                    break;
                case FlyingCar:
                    printf("飛");
                    break;
                case Horse:
                    printf("桂");
                    break;
                case Silver:
                    printf("銀");
                    break;
                case Gold:
                    printf("金");
                    break;
                case King:
                    printf("王");
                    break;
                case Horn:
                    printf("角");
                    break;
                case Car:
                    printf("香");
                    break;
                case Empty:
                    printf("  ");
                    break;
                /*case DragonHorse:
                    printf("龍馬");
                    break;
                case DragonKing:
                    printf("龍王");
                    break;*/
            }//end switch if
        }//for endif
        SET_FRONT_COLOR(FRONT_WHITE);
        printf("%d",i+1);
        printf("\n");
    }
    SET_FRONT_COLOR(FRONT_RED);
    printf("Player Y\n");
    SET_FRONT_COLOR(FRONT_WHITE);
    SHOW_CURSOR();
    set_disp_mode(1);
    return 0;
}
