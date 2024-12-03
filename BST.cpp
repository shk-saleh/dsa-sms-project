#include<iostream>
#include<queue>
#include <cstdlib> 
#include <ctime>  
using namespace std;


const int MAP_ROW = 20;
const int MAP_COL = 20;


// Inventory management - BST
class Inventory{
    
    public:

    int rewardId;
    char rewardName;    // will be unique J, P, W
    int rewardScore;
    Inventory* left;
    Inventory* right;

    Inventory(int rewardId, char rewardName, int rewardScore){

        this -> rewardId = rewardId;
        this -> rewardName = rewardName;
        this -> rewardScore = rewardScore;
        this -> left = NULL;
        this -> right = NULL;

    }

};


class InventoryManag{

    
    public:
    
    Inventory* root;

    InventoryManag(){
        root = NULL;
    }
    

    // inventory type function to build BST tree
    Inventory* buildTree(Inventory* root, int rewardId, char rewardName, int rewardScore){

        // base case
        if(root == NULL){       // if there will no node then node will created
            root = new Inventory(rewardId, rewardName, rewardScore);
            return root;
        }

        // If the ID matches an existing node, generate a new one
        if (root->rewardId == rewardId) {
            rewardId = rand() % 201; // Generate a new ID
            return buildTree(root, rewardId, rewardName, rewardScore);
        }

        if(root->rewardId > rewardId){
            root -> left = buildTree(root->left, rewardId, rewardName, rewardScore);
        }
        else{
            root -> right = buildTree(root->right, rewardId, rewardName, rewardScore);
        }

        return root;

    }

    // Level order traversal of tree
    void printInventory(){

        queue<Inventory*> q;
        q.push(root);       // pushed root element
        q.push(NULL);       // after root we have null
        int totalScore = 0;
        int jewel = 0;
        int potion = 0;
        int weapon = 0;


        while(!q.empty()){      // printing the element until queue got empty

            Inventory* temp = q.front();
            q.pop();

            if(temp == NULL){
                cout<<endl;         // if NULL found in queue then enter 
                if(!q.empty()){
                    q.push(NULL);
                }
            }
            else{

                totalScore += temp->rewardScore;

                if(temp -> rewardName == 'P')  potion++;
                if(temp -> rewardName == 'J')  jewel++;
                if(temp -> rewardName == 'W')  weapon++;
                

                // cout<<"ID: "<<temp -> rewardId<<", ";
                // cout<<"Name: "<<temp -> rewardName<<", ";
                // cout<<"Score: "<<temp -> rewardScore<<endl;


                if(temp -> left){       // if temp left is non-null then it push 
                    q.push(temp->left);
                }

                if(temp -> right){      // if temp right is non-null then it push 
                    q.push(temp->right);
                }

            }

        }


        cout<<"You have collected : "<<jewel<<" Jewel , "<<potion<<" Potion , "<<weapon<<" Weapon.";
        cout<<"Total score : "<<totalScore;

    }


    // deletion of node 
        
    // min value of tree
    Inventory* minValue(Inventory* root){

        Inventory* temp = root;

        while(root->left != NULL){
            temp = temp -> left;
        }

        return temp;
    }

    // delete Node using preorder/NLR -
    Inventory* deleteNode(Inventory* root, char rewardName, bool &isDeleted){

        // base case
        if(root == NULL || isDeleted){
            return root;
        }

        // if value found
        if(root->rewardName == rewardName){

            isDeleted = true;

            // cases 
            if(root -> left == NULL && root -> right == NULL){
                // if there is leaf node
                delete root;
                return NULL; 
            }
            
            if(root->left != NULL && root->right == NULL){
                // if node have only one left child
                Inventory* temp = root -> left;
                delete root;
                return temp;
            }

            if(root->left == NULL && root->right != NULL){
                // if node have only one right child
                Inventory* temp = root -> right;
                delete root;
                return temp;
            }

            if(root->left != NULL && root->right != NULL){
                // if there are both left and right child
                int mini = minValue(root->right) -> rewardId;
                root -> rewardId = mini;    // store the min value in root
                root -> right = deleteNode(root->right, mini, isDeleted);      // call the func to delete that copied node
                return root;
            }
        
        }
        
        root -> left = deleteNode(root->left, rewardName, isDeleted);
        root -> right = deleteNode(root->right, rewardName, isDeleted);
            
        return root;
        
    }



    void traverseMap(char matrix[MAP_ROW][MAP_COL]){

        for(int i = 0; i<20; i++){
            for(int j = 0; j<20; j++){
                if(matrix[i][j] == 'J'){
                    root = buildTree(root, rand() % 201, 'J', 50);
                }
                else if(matrix[i][j] == 'W'){
                    root = buildTree(root, rand() % 201, 'W', 60);
                }
                else if(matrix[i][j] == 'P'){
                    root = buildTree(root, rand() % 201, 'P', 70);
                }
                else if(matrix[i][j] == '@'){
                    bool isDeleted = false;
                    deleteNode(root, 'W', isDeleted);
                }
                else if(matrix[i][j] == '$'){
                    bool isDeleted = false;
                    deleteNode(root, 'P', isDeleted);
                }
                else if(matrix[i][j] == '&'){
                    bool isDeleted = false;
                    deleteNode(root, 'J', isDeleted);
                }
                else if(matrix[i][j] == '*'){
                    cout<<"You found the crystal! Ya ho..";
                    break;
                }
            }
            cout<<endl;
        }

    }


};




int main(){

    srand(time(0));

    char matrix[MAP_ROW][MAP_COL] = {
                            {'H', 'P', 'C', '#', 'C', 'C', 'C', 'C', 'C', 'J', '#', 'C', 'C', 'C', 'C', 'C', '%', 'C', 'C', 'C'},
                            {'C', 'C', '$', 'C', 'C', 'C', '%', 'C', 'C', 'C', 'J', '#', 'W', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                            {'C', '#', 'J', 'C', 'C', 'C', 'W', 'C', 'C', '#', 'C', 'C', 'C', 'C', 'C', 'C', '#', 'C', 'C', 'C'},
                            {'%', '#', 'P', 'C', 'C', 'C', '#', 'C', 'C', 'J', 'C', 'C', 'C', 'C', 'C', '$', '%', '@', 'C', '&'},
                            {'C', 'C', 'C', 'C', '$', 'C', '#', 'C', 'C', 'C', 'C', '%', 'C', '#', 'C', 'J', 'C', 'C', 'C', 'C'},
                            {'C', 'C', '%', '-', 'C', 'C', 'C', 'C', 'C', 'C', 'C', '#', '#', 'C', 'C', 'C', 'C', 'C', 'C', 'C'},
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
 

    InventoryManag I;

    I.traverseMap(matrix);
    I.printInventory();

}