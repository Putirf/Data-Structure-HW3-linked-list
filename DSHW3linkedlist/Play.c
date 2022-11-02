#include "shogi.h"
#include "Play.h"
#include "Print.h"
#include "Rule.h"
int side = X;//which side now X Y 
bool Gamecontinue = 1;
int Rownow = 0,Columnnow = 0,Rowtogo = 0,Columntogo = 0;
int GraveXlast = 0, GraveYlast = 0;
bool Domove = 0;
char YN[2] = " ";
int yorn  = 0;
int GraveX[20],GraveY[20] ;
FILE *fp;
int stepcount = 0;
int Save = 1;
ev_io stdin_watcher;
ev_timer timeout_watcher1;
ev_timer timeout_watcher2;
ev_tstamp now;
ev_tstamp clk1;
ev_tstamp clk2;
Node *first,*current,*previous,*node;
Node *deleteNode(Node *,Node*);//刪除節點函數 
Node *searchNode(Node *,int); //搜尋節點函數 


const Chess InitialBroad[9][9]= {//初始化棋盤
    {{0,0,X,Car},{0,0,X,Horse},{0,0,X,Silver},{0,0,X,Gold},{0,0,X,King},{0,0,X,Gold},{0,0,X,Silver},{0,0,X,Horse},{0,0,X,Car},},
    {{0,0,NONE,Empty},{0,0,X,FlyingCar},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,X,Horn},{0,0,NONE,Empty},},
    {{0,0,X,Foot},{0,0,X,Foot},{0,0,X,Foot},{0,0,X,Foot},{0,0,X,Foot},{0,0,X,Foot},{0,0,X,Foot},{0,0,X,Foot},{0,0,X,Foot}},
    {{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},},
    {{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},},
    {{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},},
    {{0,0,Y,Foot},{0,0,Y,Foot},{0,0,Y,Foot},{0,0,Y,Foot},{0,0,Y,Foot},{0,0,Y,Foot},{0,0,Y,Foot},{0,0,Y,Foot},{0,0,Y,Foot}},
    {{0,0,NONE,Empty},{0,0,Y,FlyingCar},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,NONE,Empty},{0,0,Y,Horn},{0,0,NONE,Empty},},
    {{0,0,Y,Car},{0,0,Y,Horse},{0,0,Y,Silver},{0,0,Y,Gold},{0,0,Y,King},{0,0,Y,Gold},{0,0,Y,Silver},{0,0,Y,Horse},{0,0,Y,Car},},
    };;
Chess BroadSeat[9][9];

static void stdin_cb (EV_P_ ev_io *w, int revents)//透過timer重啟來執行遊戲
{   
    MOVETO(13,0);
	ev_timer_stop (loop, &timeout_watcher1);
    ev_timer_stop (loop, &timeout_watcher2);
    Enter_move();
    Print_Broad();
    
    if(side == X&&Gamecontinue ==1){
        side = Y;
    }      
    else if(side == Y&&Gamecontinue ==1){
        side = X;

    }  

	if(side == X){	
		ev_timer_stop (loop, &timeout_watcher2);
		ev_timer_start (loop, &timeout_watcher1);
	}
	else if(side == Y){
		ev_timer_stop (loop, &timeout_watcher1);
		ev_timer_start (loop, &timeout_watcher2);
	}
    if(Gamecontinue == 0){
        ev_break(loop,-1);
        ev_io_stop (loop, &stdin_watcher);
        ev_timer_stop (loop, &timeout_watcher1);
        ev_timer_stop (loop, &timeout_watcher2);
    }
}

static void timeout1_cb (EV_P_ ev_timer *w, int revents)//X的計時器
{   
    clk2 = ev_now(loop)-now-clk1;//更新Y輸入時所耗費的時間
    MOVETO(0,11);
    SET_FRONT_COLOR(FRONT_BLUE);
	printf("Time:%.0f \n",clk1);
    MOVETO(12,11);
    SET_FRONT_COLOR(FRONT_RED);
	printf("Time:%.0f \n",clk2); 
	clk1 = clk1 +1;
    SET_FRONT_COLOR(FRONT_WHITE);
    printf("Now is %c turns.(-1:surrander  0:regret)\n",(side == X)?('X'):('Y'));
    
}

static void timeout2_cb (EV_P_ ev_timer *w, int revents)//Y的計時器
{
    clk1 = ev_now(loop)-now-clk2;//更新X輸入時所耗費的時間
    MOVETO(0,11);
    SET_FRONT_COLOR(FRONT_BLUE);
	printf("Time:%.0f \n",clk1);
    MOVETO(12,11);
    SET_FRONT_COLOR(FRONT_RED);
	printf("Time:%.0f \n",clk2);
    clk2 = clk2 +1;
    SET_FRONT_COLOR(FRONT_WHITE);
    printf("Now is %c turns.(-1:surrander  0:regret)\n",(side == X)?('X'):('Y'));
}


int Play_Chess(){
    

    clk1 = 0;
    clk2 = 0;
    side = X;
    Gamecontinue = 1;
    for(int i =0;i<=8;i++){
        for(int j =0;j<=8;j++){
            BroadSeat[i][j] = InitialBroad[i][j];
        }
    }
    Save = 1;
    Print_Broad();
    struct ev_loop *loop = EV_DEFAULT;
    now = ev_now(loop);
    ev_io_init (&stdin_watcher, stdin_cb,0, EV_READ);
    ev_io_start (loop, &stdin_watcher);
    ev_timer_init (&timeout_watcher1, timeout1_cb, 0, 1);//以秒為間隔 印出計時
    ev_timer_init (&timeout_watcher2, timeout2_cb, 0, 1);
    if(side == X)
    {
        ev_timer_start (loop, &timeout_watcher1);
    }
    else if(side == Y)
    {
        ev_timer_start (loop, &timeout_watcher2);
    }
    ev_timer_start (loop, &timeout_watcher1);
    ev_run (loop, 0);
    

    Save = 0;
    fp = fopen(fNAME,"w");
    rewind(fp);
    current=first; //設current為第一個節點 
    while(current!=NULL){
        fprintf(fp,"%d %d %d %d %d %d %d ",current->rownow,current->columnnow,current->rowtogo,current->columntogo,current->chess.kind,current->chess.side,current->stepcount);  
        current=current->next;  //將ptr指向下一個節點 
    }
    
    fclose(fp);
    printf("Player%c win\n",side);
    sleep(2);
    Save = 0;
    return 0;
}


int Enter_move(){ //Enter move and check does it follow the Chess_Move_Rule
    Columnnow:
    MOVETO(14,1);
    //CLEARLINE();
    printf("Column now:");
    scanf("%d",&Columnnow);
    Columnnow --;
    switch(Columnnow){
        case -1:
        if(stepcount == 0){
            printf("this is the begining");
            fflush(stdout);
            sleep(2);
            
            goto Columnnow;
        }
        else{
            Chess tmp;
            Rowtogo = current->rownow;
            Columntogo = current->columnnow;
            Rownow = current->rowtogo;
            Columnnow = current->columntogo;
            tmp = current->chess;
            Save =0;
            move(0);
            BroadSeat[Rownow][Columnnow].alivechange = 0;
            BroadSeat[Rownow][Columnnow].change = 0;
            BroadSeat[Rownow][Columnnow].side = tmp.side;
            BroadSeat[Rownow][Columnnow].kind = tmp.kind;
            first=deleteNode(first,current); //刪除當前一步
            if(stepcount>0){
                stepcount--;
                previous = searchNode(first,stepcount); //指到上一步
            }
            Save = 1;
            return 0;
        }    
            break;
        
        case 0 ... 8:
            break;
        
        case -2:
            Exitgame:
            MOVETO(14,0);
            printf("Do you sure to surrender and exit?[y/n]");
            scanf("%s",YN);
            if(strcmp(YN,"Y")==0||strcmp(YN,"y")==0){
                Gamecontinue =0;
                if(side == X){
                side = Y;
                }      
                else if(side == Y){
                side = X;
                }  
                return 0;
            }
            else if(strcmp(YN,"N")==0||strcmp(YN,"n")==0){
                goto Columnnow;
            }
            else {
                goto Exitgame;
            }
            break;
        
        case 9:
            if(side == X){
                if(Enter_Grave(GraveX)){
                    return 0;
                }
            }
            else if(side == Y){
                if(Enter_Grave(GraveY)){
                    return 0;
                }
            }
            else{
            goto Columnnow;
            }
            break;

        default:
            goto Columnnow;
            break;
    }
    Rownow:
    MOVETO(15,0);
    printf("Row now:");
    scanf("%d",&Rownow);
    Rownow --;
    switch(Rownow){
        case -1:
            goto Columnnow;
            break;
        case 0 ... 8:
            break;
        default:
            goto Rownow;
            break;
    } 

    Columntogo:
    MOVETO(16,0);
    printf("Row to go:");
    scanf("%d",&Columntogo);
    Columntogo --;
    switch(Columntogo){
        case -1:
            goto Rownow;
            break;
        case 0 ... 8:
            break;
        default:
            goto Columntogo;
            break;
    } 

    Rowtogo:
    MOVETO(17,0);
    printf("Column to go:");
    scanf("%d",&Rowtogo);
    Rowtogo --;
    switch(Rowtogo){
        case -1:
            goto Columntogo;
            break;
        case 0 ... 8:
            break;
        default:
            goto Rowtogo;
            break;
    } 
    if(Chess_Move_Rule(BroadSeat[Rownow][Columnnow],Rownow,Columnnow,Rowtogo,Columntogo)&&BroadSeat[Rownow][Columnnow].side!=BroadSeat[Rowtogo][Columntogo].side){
        move(Save);
        //Change_Rule(BroadSeat[Rowtogo][Columntogo],Rowtogo);
    }
    else{
        printf("It is illegal.Please enter again.");
        goto Columnnow;
    }
    return 0;
}

bool Enter_Grave(int Grave[]){ //enter grave to relive chess 待完成
    int number = 0;
    GraveStart:
    printf("Which chess you want to relive?(please enter the number) :");
    scanf("%d",&number);
    
    if (number == 0){
        return 0;
    }
    else if(number >20||number<0||Grave[number-1] == Empty){
        printf("Please enter the correct number.");
        goto GraveStart;
    }
    else{
        GraveRowtogo:
        printf("Where do you want to put this chess?\n");
        printf("Row to go:");
        scanf("%d",&Rowtogo);
        switch(Rowtogo){
            case 0:
                goto GraveStart;
                break;
            case 1 ... 9:
                break;
            default:
                goto GraveRowtogo;
                break;
        }
        GraveColumntogo:
        printf("Column to go:");
        scanf("%d",&Columntogo);
        switch(Columntogo){
            case 0:
                goto GraveRowtogo;
                break;
            case 1 ... 9:
                break;
            default:
                goto GraveColumntogo;
                break;
        } 

        if(Relive_Rule(BroadSeat[Rowtogo][Columntogo],Rowtogo,Columntogo)){
            BroadSeat[Rowtogo][Columntogo].kind = Grave[number];
            BroadSeat[Rowtogo][Columntogo].side = side;
            if(side == X){
                for(int counter = number;GraveX[counter+1] == Empty || counter<= 19; counter++){
                    GraveX[counter]=GraveX[counter+1];
                }
                GraveX[20] = Empty;
                GraveXlast --;
            }
            else if(side == Y){
                for(int counter = number;GraveY[counter+1] == Empty || counter<= 19; counter++){
                    GraveY[counter]=GraveY[counter+1];
                }
                GraveY[20] = Empty;
                GraveYlast --;
            }
        }
        else {
            printf("That is illegal.Please enter again.");
            goto GraveRowtogo;
        }
        return 1;
    }

}

int move(int save){//移動
        if(BroadSeat[Rowtogo][Columntogo].kind == Empty){
            if(side ==X){
                GraveX[GraveXlast] = BroadSeat[Rowtogo][Columntogo].kind;
                GraveXlast++;
            }
            else if(side == Y){
                GraveY[GraveYlast] = BroadSeat[Rowtogo][Columntogo].kind;
                GraveYlast++;
            }
        }
        else if(BroadSeat[Rowtogo][Columntogo].kind == King){
            Gamecontinue = 0;
        }
        if(save ==1){
            current=(Node *) malloc(sizeof(Node));//建立新節點
            current->rownow = Rownow;
            current->columnnow = Columnnow;
            current->rowtogo = Rowtogo;
            current->columntogo = Columntogo;
            current->chess = BroadSeat[Rowtogo][Columntogo];
            current->stepcount = stepcount;
            if(stepcount==0){
		        first=current;  //如果是第一個成員把指標frist指向目前的節點 
            }else{
                previous->next=current;//把前一個的next指向目前的節點 
            }
            current->next=NULL; //把目前的節點的next指向NULL
            previous=current; //把前一個節點設成目前的節點 
            stepcount ++;
        }
        BroadSeat[Rowtogo][Columntogo] = BroadSeat[Rownow][Columnnow];
        BroadSeat[Rownow][Columnnow].alivechange = 0;
        BroadSeat[Rownow][Columnnow].change = 0;
        BroadSeat[Rownow][Columnnow].side = NONE;
        BroadSeat[Rownow][Columnnow].kind = Empty;
    return 0;
}

int set_disp_mode(int option)  //0:Hide input 1:Show input顯示即隱藏游標
{  
   int err;  
   int fd = STDIN_FILENO;
   struct termios term;  
   if(tcgetattr(fd,&term)==-1){  
     perror("Cannot get the attribution of the terminal");  
     return 1;  
   }  
   if(option)  
        term.c_lflag|=ECHOFLAGS;  
   else  
        term.c_lflag &=~ECHOFLAGS;  
   err=tcsetattr(fd,TCSAFLUSH,&term);  
   if(err==-1 && err==EINTR){  
        perror("Cannot set the attribution of the terminal");  
        return 1;  
   }  
   return 0;  
}  

 //搜尋節點函數 
Node *searchNode(Node *first,int item){
	Node *node=first;
	while(node!=NULL){
		if(node->stepcount==item){ //如果node的data等於item 
			return node;  //傳回node為該節點的位址 
		}else{
			node=node->next; //否則將指標指向下一個節點 
		}
	}
	return NULL; //如果找不到符合的節點，則傳回NULL 
}

//刪除節點函數 
Node *deleteNode(Node *first,Node *node){
	Node* ptr=first;
	if(first==NULL){ //如果串列是空的，則印出Nothing to delete! */ 
		printf("Nothing to delete!\n");
		return NULL;
	}
	if(node==first){  //如果刪除的是第一個節點 
		first=first->next;//把first指向下一個節點(NULL) 
	}else{
		while(ptr->next!=node){ //找到要刪除之節點的前一個節點 
			ptr=ptr->next;
		}
		ptr->next=node->next; //重新設定ptr的next成員 
	}
	free(node);
	return first;
}

