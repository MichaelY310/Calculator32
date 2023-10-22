#include "ParserB.h"

// ParserB::~ParserB(){
//     Node * curr = mRoot;
//     While(curr->leftchild!=nullptr )
//     curr = nullptr;
// }

ParserB::ParserB(std::vector<Token> expression){
    std::vector<Token> EXP;
    int index = 0;

    for (int i = (int)expression.size()-2; i >= 0; i--){  // neglect the end token
        if (expression.at(i).type == TokenType::equals){
            index = i;
            break;
        }
        EXP.insert(EXP.begin() , expression.at(i));
    }
    Node * curr = new Node();
    curr = Build(EXP , 0, new Node());
    mRoot = curr;

    while(index >= 0){
        if (expression.at(index).type == TokenType::equals){ // when the element is "="
            curr = new Node(expression.at(index));
            curr->rightchild = mRoot;
            mRoot = curr;
        }
        else if (expression.at(index).type == TokenType::variable){   // when the element is variable
            curr = new Node(expression.at(index));
            mRoot->leftchild = curr;
        }
        index -= 1;
    }
    curr = nullptr;
}


Node * ParserB::Build(std::vector<Token> EXP, int index, Node * root) {
    if (this->maxindex < index) {
        this->maxindex = index;
    }
    Node * temp = new Node();

    if (IsSigns(EXP.at(index))==0 && IsSigns(EXP.at(index))!=2){             // when the element is a number
        if (root->value.content==""){          // when root is empty
            temp = new Node(EXP.at(index));
            root = Build(EXP, index + 1, temp);  //1 4 7
        }

        if(level > 0){
            level -= 1;
            return root;
        }
        
    }
    else if (IsSigns(EXP.at(index))==1){        // when the element is a sign 

        temp = new Node (EXP.at(index));
        temp->leftchild = root;
        root = temp;        

        if (index >= (int)EXP.size()-2 || EXP.at(index+1).type != TokenType::number){
            //error situation because every sign should have a number behind it
        }

        if (nextSign(EXP, index+1) == 0){       // there is no sign behind, so this is the last sign
            root->rightchild = new Node(EXP.at(index+1)); 
            return root;
        }

        else if (precedence(EXP.at(index)) < nextSign(EXP,index+1)){ // the nect signs precendence is greater than current sign
            level += 1;
            root->rightchild = Build(EXP, index + 1, new Node());  //3 6 8
        }

        else if (precedence(EXP.at(index)) == nextSign(EXP,index+1)){ // the next signs precendence is equal to current sign
            root->rightchild = new Node(EXP.at(index+1));
            root = Build(EXP, index + 2, root); //2 
        }
        else if (precedence(EXP.at(index)) > nextSign(EXP,index+1)){ // the nect signs precendence is less than current sign
            root->rightchild = new Node(EXP.at(index+1)); 
            return root;
        }
    }
    else if (IsSigns(EXP.at(index))==2){        // when there exist a Parentheses
        int leftB = left_bound(EXP,index);
        if (index > (int)EXP.size()-5){ 
            // there is error
        }
        temp = Build_Para(EXP,index, leftB);
        if (root->value.content==""){
            if (left_bound(EXP,index) == (int)EXP.size()-1){
                maxindex = leftB;
                return temp;
            }
            root = temp;
        }
        else if(IsSigns(root->value) == 1) {
            root->rightchild = temp;
        }
        else{
            // error 
        }
        maxindex = leftB;
    }

    temp = nullptr;
    while (maxindex < (int)EXP.size()-1){
        root = Build(EXP, maxindex + 1, root);  //5
    }
    return root;
}

Node * ParserB::Build_Para(std::vector<Token> EXP, int index, int leftBound){
    std::vector<Token> TempVect;
    for (int i = index + 1; i < leftBound; i++){
        TempVect.push_back(EXP.at(i));
    }
    return Build(TempVect, 0, new Node());
}

int ParserB::IsSigns(Token input){ // if the input is a sign it will return 1, if it is parenthesis it will return 2, else it will return 0
    if (input.type == TokenType::plus || input.type == TokenType::minus || input.type == TokenType::multiply || input.type == TokenType::divide){
        return 1;
    }
    else if (input.type == TokenType::leftParenthesis || input.type == TokenType::rightParenthesis){
        return 2;
    }
    return 0;
}

int ParserB::precedence(Token input){
    if (input.type == TokenType::plus ||input.type == TokenType::minus){
        return 1;
    }
    else if (input.type == TokenType::multiply ||input.type == TokenType::divide){
        return 2;
    }
    else if (input.type == TokenType::rightParenthesis ||input.type == TokenType::leftParenthesis){
        return 3;
    }
    else {
        return 0;  // should not return 0
    }
}

int ParserB::nextSign(std::vector<Token> EXP,int index) {
    while (index < (int)EXP.size()){
        if (EXP.at(index).type == TokenType::plus || EXP.at(index).type == TokenType::minus){
            return 1;
        }
        if (EXP.at(index).type == TokenType::multiply || EXP.at(index).type == TokenType::divide){
            return 2;
        }
        if (EXP.at(index).type == TokenType::leftParenthesis){
            return 3;
        }
        index += 1;
    }
    return 0;
}

int ParserB::left_bound(std::vector<Token> EXP, int index){
    int Para_balance = 1;
    while(Para_balance != 0){
        index += 1;
        if (EXP.at(index).type == TokenType::leftParenthesis){
            Para_balance += 1;
        }
        if (EXP.at(index).type == TokenType::rightParenthesis){
            Para_balance -= 1;
        }
    }
    return index;
}
