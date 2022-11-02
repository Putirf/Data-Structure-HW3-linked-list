#ifndef RULE_H_
#define RULE_H_
#include "shogi.h"

bool Chess_Move_Rule(Chess chess,int rownow,int columnnow,int rowtogo,int columntogo);
bool Relive_Rule(Chess chess,int rowrelive,int columnrelive );
Chess Change_Rule(Chess chess,int rownow);

#endif