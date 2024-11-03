
//collision detection code

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#define MAX_BUB_COUNT 100 
#define MAX_TREE_DEPTH 4

#define MAX_NODES 4

#define SIM_STEPS 10


// Struct for a bubble - each bubble with position, velocity, radius, color, and popped status

typedef struct{
    float posX;
    float posY;
    float velX;
    float velY;
    float size;   // radius
    int hue;        // color
    int isPopped;   // 1 if popped, 0 if not
}Bubble;



// Struct for each quadtree node (helps with organizing bubbles in 2D space)
typedef struct TreeNode{
    float posX,posY;
    float width,height;
    Bubble* bubbleList[MAX_NODES];
    int bubbleCount;
    struct TreeNode* nwNode,*neNode,*swNode,*seNode;
}TreeNode;


// Function declarations
TreeNode* initNode(float x,float y,float w,float h);
int isCollision(Bubble* b1,Bubble* b2);

void splitNode(TreeNode* node);

void loadBubblesFromFile(Bubble** bubbleArray,int* bubbleCount,const char* filePath);
void searchNearby(TreeNode* node,Bubble* bubble,Bubble** results,int* resCount);
void refreshPosition(Bubble* bubble);

void deleteNode(TreeNode* node);

int addBubble(TreeNode* node,Bubble* bubble,int depth);



TreeNode* initNode(float x,float y,float w,float h) {
    TreeNode* node=(TreeNode*)malloc(sizeof(TreeNode));
    node->posX=x;
    node->posY=y;
    node->width=w;
    node->height=h;
    node->bubbleCount=0;    // no bubbles yet
    node->nwNode=node->neNode=node->swNode=node->seNode=NULL;  
    // no children yet
    return node;
}



int isCollision(Bubble* b1,Bubble* b2){
    // Calculate distance between two bubbles
    
    float dist=sqrt((b1->posX-b2->posX)*(b1->posX-b2->posX)+(b1->posY-b2->posY)*(b1->posY-b2->posY));
    return dist<=(b1->size+b2->size); 
    // if they overlap, we got a collision
    
}

void loadBubblesFromFile(Bubble** bubbleArray,int* bubbleCount,const char* filePath){
    FILE* file=fopen(filePath,"r");
    
    if(!file) {
        perror("Failed to open file");
        
        exit(EXIT_FAILURE);
    }
    
    *bubbleArray=(Bubble*)malloc(MAX_BUB_COUNT*sizeof(Bubble)); 
    // Allocate memory for bubbles
    
    int i=0;
    while(i<MAX_BUB_COUNT&&fscanf(file,"%f%f%f%f%f%d",
          &(*bubbleArray)[i].posX,&(*bubbleArray)[i].posY,&(*bubbleArray)[i].velX,
          &(*bubbleArray)[i].velY,&(*bubbleArray)[i].size,&(*bubbleArray)[i].hue)==6){
              
        (*bubbleArray)[i].isPopped=0; 
        // Not popped
        i++;
        
        
    }
    
    *bubbleCount=i; 
    // stores how many bubbles were loaded
    
    fclose(file);
    
}

void splitNode(TreeNode* node) {
    float halfW=node->width/2;
    float halfH=node->height/2;

    // Splits a node into 4 quadrants
    node->nwNode=initNode(node->posX,node->posY,halfW,halfH);
    node->neNode=initNode(node->posX+halfW,node->posY,halfW,halfH);
    
    node->swNode=initNode(node->posX,node->posY+halfH,halfW,halfH);
    node->seNode=initNode(node->posX+halfW,node->posY+halfH,halfW,halfH);
}

int addBubble(TreeNode* node,Bubble* bubble,int depth) {
    
    if(depth>=MAX_TREE_DEPTH) {
        return 0;
        
    }

    // If bubble is outside bounds
    if(bubble->posX-bubble->size<node->posX||
       bubble->posX+bubble->size>node->posX+node->width||
       bubble->posY-bubble->size<node->posY||
       bubble->posY+bubble->size>node->posY+node->height){
        return 0;
        
    }

    // If we can add more bubbles
    if(node->bubbleCount<MAX_NODES){
        node->bubbleList[node->bubbleCount++]=bubble;
        
        return 1;
    }

    // If we can't - we need to split if not already done
    if(node->nwNode==NULL) splitNode(node);

    // Adding to one of the children nodes
    if(addBubble(node->nwNode,bubble,depth+1)) return 1;
    if(addBubble(node->neNode,bubble,depth+1)) return 1;
    if(addBubble(node->swNode,bubble,depth+1)) return 1;
    if(addBubble(node->seNode,bubble,depth+1)) return 1;

    return 0;
    
    
}



void refreshPosition(Bubble* bubble){
    bubble->posX+=bubble->velX;
    bubble->posY+=bubble->velY;

    // Bounce back if it hits wall
    if(bubble->posX-bubble->size<0){
        bubble->velX=-bubble->velX;
        
        bubble->posX=bubble->size; 
        // reset to edge
    }
    
    else if(bubble->posX+bubble->size>100){
        
        bubble->velX=-bubble->velX;
        bubble->posX=100-bubble->size;
        
    }
    // Check top-bottom boundary collisions
    if(bubble->posY-bubble->size<0){
        bubble->velY=-bubble->velY;
        bubble->posY=bubble->size;
    }
    
    else if(bubble->posY+bubble->size>100){
        bubble->velY=-bubble->velY;
        
        bubble->posY=100-bubble->size;
        
    }
    
    
}



void searchNearby(TreeNode* node,Bubble* bubble,Bubble** results,int* resCount){
    if(bubble->posX-bubble->size<node->posX||bubble->posX+bubble->size>node->posX+node->width||bubble->posY-bubble->size<node->posY||bubble->posY+bubble->size>node->posY+node->height){
        return;
        
    }

    for(int i=0;i<node->bubbleCount;i++){
        
        if(*resCount>=MAX_BUB_COUNT) return; 
        // Stop if results array is full
        results[(*resCount)++]=node->bubbleList[i];
        
    }

    // If no subdivisions, we're done here
    if(node->nwNode==NULL) return;

    // Recursive check on each child node
    searchNearby(node->nwNode,bubble,results,resCount);
    
    searchNearby(node->neNode,bubble,results,resCount);
    searchNearby(node->swNode,bubble,results,resCount);
    searchNearby(node->seNode,bubble,results,resCount);
}


void deleteNode(TreeNode* node){
    if(node==NULL){
        return;
        
    }
    deleteNode(node->nwNode);
    deleteNode(node->neNode);
    deleteNode(node->swNode);
    deleteNode(node->seNode);
    
    free(node);
    
}

int main(){
    
    Bubble* bubbles; 
    // Pointer for dynamic allocation
    int bubbleCount;
    
    loadBubblesFromFile(&bubbles,&bubbleCount,"case.txt");
    // Load bubbles from file
    TreeNode* root;
    
    int score=0;

    for(int step=0;step<SIM_STEPS;step++){
        
        printf("Simulation Step %d:\n",step);
        
        root=initNode(0,0,100,100); 
        // Initialize the root node

        for(int i=0;i<bubbleCount;i++){
            if(!bubbles[i].isPopped) addBubble(root,&bubbles[i],0);
            // Add bubble if not popped
            
        }

        for(int i=0;i<bubbleCount;i++){
            if(bubbles[i].isPopped) continue; 
            // Skip if already popped

            Bubble* found[MAX_BUB_COUNT];
            int foundCount=0;
            
            searchNearby(root,&bubbles[i],found,&foundCount);

            for(int j=0;j<foundCount;j++){
                
                // Collision and color check - both bubbles gotta match color to pop
                if(found[j]!=&bubbles[i]&&!found[j]->isPopped&&isCollision(&bubbles[i],found[j])&&bubbles[i].hue==found[j]->hue){
                    printf("Bubbles %d and %d collided and popped (same hue)!\n",i,(int)(found[j]-bubbles));
                    bubbles[i].isPopped=1;
                    
                    found[j]->isPopped=1;
                    score++;
                    
                }
                
                
                
            }
        }

        for(int i=0;i<bubbleCount;i++){
            
            if(!bubbles[i].isPopped){
                
                refreshPosition(&bubbles[i]);
                // Update bubble position if still active
                printf("Bubble %d at (%.2f,%.2f) Color: %d\n",i,bubbles[i].posX,bubbles[i].posY,bubbles[i].hue);
            }
            
            
        }
        

        printf("Score: %d\n\n",score);
        
        deleteNode(root); // Clear quadtree for next step
        
        
    }

    printf("Final Score: %d\n",score);
    
    free(bubbles); // Free allocated memory for bubbles
    
    return 0;
    
    
}
