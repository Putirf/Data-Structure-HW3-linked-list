#include "shogi.h"
#include "Print.h"
#include "Play.h"
#include "Rule.h"
char fNAME[50] = "GAME.txt";
int choose;
int Read_Old_Game();

int main(int argc, char * argv[]){
    int ch;

    while ((ch = getopt(argc, argv, "s:nl:")) != -1){//判斷cmd的輸入
        switch (ch) {
            case 's':
                strcpy(fNAME,optarg);
                break;
            case 'n':
                Play_Chess();
                break;

            case 'l':
                strcpy(fNAME,optarg);
                Read_Old_Game();
                break;
            abort();
            break;
        }
    }
    
    return 0;        
}

int Read_Old_Game(){
    side = X;
    FILE *fp;
    int countstep = 0;
    char fb[2];
    Node *first,*current,*previous,*node;
    Chess tmp;
    CLEAR();
    SHOW_CURSOR();
    set_disp_mode(1);
    fp = fopen(fNAME,"r");
    if(fp == NULL){
            printf("FILEOPENERROR1");
    }
    else{
        for(int i =0;i<=8;i++){
            for(int j =0;j<=8;j++){
                BroadSeat[i][j] = InitialBroad[i][j];
            }
        }
        rewind(fp);
        while(!feof(fp)){
            current=(Node *) malloc(sizeof(Node));//建立新節點
            fscanf(fp,"%d %d %d %d %d %d %d ",&current->rownow,&current->columnnow,&current->rowtogo,&current->columntogo,&current->chess.kind,&current->chess.side,&current->stepcount);
            if(current->stepcount==0){
            first=current;  //如果是第一個成員把指標frist指向目前的節點 
            }
            else{
                previous->next=current;//把前一個的next指向目前的節點 
            }
            current->next=NULL; //把目前的節點的next指向NULL
            previous=current; //把前一個節點設成目前的節點 
        }
        fclose(fp);
        current = first;
    }
    Option:
    Print_Broad();
    printf("enter(f:forward b:backward 0:exit):");
    fflush(stdin);
    scanf("%s",fb);
    if(strcmp(fb,"b")==0||strcmp(fb,"B")==0){
        if(countstep == 0){
            printf("this is the begining");
            sleep(2);
            goto Option;
        }
        else{
            Chess tmp;
            
            node = searchNode(first,countstep-1); //指到上一步
            Rowtogo = node->rownow;
            Columntogo = node->columnnow;
            Rownow = node->rowtogo;
            Columnnow = node->columntogo;
            tmp = node->chess;
            move(0);
            BroadSeat[Rownow][Columnnow].alivechange = 0;
            BroadSeat[Rownow][Columnnow].change = 0;
            BroadSeat[Rownow][Columnnow].side = tmp.side;
            BroadSeat[Rownow][Columnnow].kind = tmp.kind;
            if(side == X){
            side = Y;
            }      
            else if(side == Y){
            side = X;
            }  
            countstep--;
            goto Option;
        }

    }
    else if(strcmp(fb,"F")==0||strcmp(fb,"f")==0){ 
        if(current!= NULL){
        Chess tmp;
        Rownow = current->rownow;
        Columnnow = current->columnnow;
        Rowtogo = current->rowtogo;
        Columntogo = current->columntogo;
        move();
        if(side == X){
            
        }      
        else if(side == Y){
            
        }  
            countstep++;
            current = current->next;
        }
        goto Option;
    }
    else if(strcmp(fb,"0")==0){
            return 0;
    }
    else{
        goto Option;
    }
}



//gcc -o main main.c Play.c Play.h Print.c Print.h Rule.c Rule.h shogi.h -lm -lev

