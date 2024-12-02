#include<iostream>
#include<queue>
#include <cstdlib> 
#include <ctime>  
using namespace std;

// Inventory management - BST
class Inventory{
    
    public:

    int rewardId;
    char rewardName;    // will be unique J, P, W
    // bool cnt;
    int rewardScore;
    Inventory* left;
    Inventory* right;

    Inventory(int rewardId, char rewardName, int rewardScore){

        this -> rewardId = rewardId;
        this -> rewardName = rewardName;
        // this -> cnt = true;
        this -> left = NULL;
        this -> right = NULL;

    }

};

// inventory type function to build BST tree
Inventory* buildTree(Inventory* root, int rewardId, char rewardName, int rewardScore){

    // base case
    if(root == NULL){       // if there will no node then node will created
        root = new Inventory(100, rewardName, rewardScore);
        return root;
    }

    if(root->rewardId > rewardId){
        root -> left = buildTree(root->left, rewardId, rewardName, rewardScore);
    }
    else{
        root -> right = buildTree(root->right, rewardId, rewardName, rewardScore);
    }
    // else{       // if id will same to previous one 
    //     buildTree(root, rand() % 201, rewardName, rewardScore);
    // }

    return root;

}


void printRecord(Inventory* root){


    if(root == NULL)
        return;

    printRecord(root->left);

    cout<<root -> rewardId<<endl;

    printRecord(root->right);

}



// Level order traversal of tree
// void levelOrderTraversal(Inventory* root){

//     queue<Inventory*> q;
//     q.push(root);       // pushed root element
//     q.push(NULL);       // after root we have null


//     while(!q.empty()){      // printing the element until queue got empty

//         Inventory* temp = q.front();
//         q.pop();

//         if(temp == NULL){
//             cout<<endl;         // if NULL found in queue then enter 
//             if(!q.empty()){
//                 q.push(NULL);
//             }
//         }
//         else{

//             cout<<temp -> rewardId<<endl;
//             cout<<temp -> rewardName<<endl;
//             cout<<temp -> rewardScore<<endl;

//             if(temp -> left){       // if temp left is non-null then it push 
//                 q.push(temp->left);
//             }

//             if(temp -> right){      // if temp right is non-null then it push 
//                 q.push(temp->right);
//             }

//         }

//     }

// }




class Game{

    private:

    char matrix[20][20] = {
                            {'H', '#', 'C', '#', 'C', 'C', 'C', 'C', 'C', 'J', '#', 'C', 'C', 'C', 'C', 'C', '%', 'C', 'C', 'C'},
                            {'C', 'C', '$', 'C', 'C', 'C', '%', 'C', 'C', 'C', 'J', '#', 'W', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                            {'C', '#', 'C', 'C', 'C', 'C', 'W', 'C', 'C', '#', 'C', 'C', 'C', 'C', 'C', 'C', '#', 'C', 'C', 'C'},
                            {'%', '#', 'P', 'C', 'C', 'C', '#', 'C', 'C', 'J', 'C', 'C', 'C', 'C', 'C', '$', '%', '@', 'C', '&'},
                            {'C', 'C', 'C', 'C', '$', 'C', '#', 'C', 'C', 'C', 'C', '%', 'C', '#', 'C', 'J', 'C', 'C', 'C', 'C'},
                            {'C', 'C', '%', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '#', '#', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                            {'C', 'C', 'C', '$', '&', 'P', 'J', 'C', 'C', 'C', 'C', 'C', 'C', '#', 'C', '&', '$', '%', 'C', '@'},
                            {'C', 'C', 'C', 'C', '#', 'W', 'C', '#', 'C', 'C', 'C', 'C', 'C', 'C', '&', 'S', 'W', 'C', '%', 'C'},
                            {'C', 'C', '@', 'C', 'C', 'J', 'C', 'W', 'C', 'C', 'W', 'C', '#', 'C', 'C', '#', 'C', '%', 'C', 'C'},
                            {'C', 'J', 'C', 'C', 'C', 'C', 'C', 'P', 'C', '#', '&', '$', 'W', 'C', '%', 'C', 'C', 'C', 'C', 'C'},
                            {'%', 'C', '&', 'C', '#', '#', 'C', 'C', 'C', '%', 'C', 'C', 'C', '#', '#', 'C', 'J', 'C', 'C', 'C'},
                            {'C', 'P', 'C', 'C', '$', 'C', '#', 'C', 'C', 'C', 'C', 'C', '%', 'C', '#', 'C', 'C', 'C', '@', 'C'},
                            {'C', 'C', '%', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                            {'C', '#', '#', 'C', 'C', 'C', 'C', '#', 'C', 'C', 'C', '#', 'C', '#', '@', '#', '%', 'C', '@', 'C'},
                            {'#', 'C', 'C', '@', 'C', 'C', 'C', '#', 'C', 'C', '$', 'C', '@', 'C', 'C', 'P', 'C', '%', 'C', '@'},
                            {'C', '%', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'W', 'C', 'C'},
                            {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '%', 'C', 'W', 'C', 'C', '$', 'C', 'C', 'C', 'C'},
                            {'C', '#', '*', 'C', 'C', 'W', 'C', '%', 'C', 'C', 'C', '$', 'C', 'C', 'C', 'C', 'C', '@', 'C', 'C'},
                            {'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '@', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                            {'#', 'C', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '#', 'C', 'C', 'C', 'C', 'J', 'C', 'C', 'C', 'C', '%'}
                        };

    public:


    void printMap(){

        for(int i = 0; i<20; i++){
            for(int j = 0; j<20; j++){
                cout<<matrix[i][j]<<"  ";
            }
            cout<<endl;
        }

    }

};


int main(){

    srand(time(0));

    Game g;
    Inventory* root = NULL;

    // cout<<"Random number : "<<rand() % 201;

    buildTree(root, rand() % 201, 'J', 50);
    buildTree(root, rand() % 201, 'P', 60);
    buildTree(root, rand() % 201, 'J', 50);
    buildTree(root, rand() % 201, 'W', 70);

    printRecord(root);

    // g.printMap();

}