#include<stdio.h>
#include<stdlib.h>
#include "bst.h"

struct animal {
	char *name;
};

struct Node {
	TYPE         val;
	struct Node *left;
	struct Node *right;
};

struct BSTree {
	struct Node *root;
	int          cnt;
};

struct BSTree *buildBSTTree() {

    struct BSTree *tree = (struct BSTree *)malloc(sizeof(struct BSTree));
	tree->cnt  = 0;
	tree->root = 0;

	struct animal *fly = (struct animal *) malloc(sizeof(struct animal));
	fly->name = "Does it have wings?";
    struct Node *flynode = (struct Node *) malloc(sizeof(struct Node));
    flynode->val = fly;

	struct animal *four = (struct animal *) malloc(sizeof(struct animal));
    four->name = "Does it use four legs for walking?";
    struct Node *fournode = (struct Node *) malloc(sizeof(struct Node));
    fournode->val = four;

	struct animal *primate = (struct animal *) malloc(sizeof(struct animal));
    primate->name = "primate";
    struct Node *primatenode = (struct Node *) malloc(sizeof(struct Node));
    primatenode->val = primate;

	struct animal *dog = (struct animal *) malloc(sizeof(struct animal));
    dog->name = "dog";
    struct Node *dognode = (struct Node *) malloc(sizeof(struct Node));
    dognode->val = dog;

	struct animal *eatmeat = (struct animal *) malloc(sizeof(struct animal));
    eatmeat->name = "Does it eat meat?";
    struct Node *meatnode = (struct Node *) malloc(sizeof(struct Node));
    meatnode->val = eatmeat;

	struct animal *parrot = (struct animal *) malloc(sizeof(struct animal));
    parrot->name = "parrot";
    struct Node *parrotnode = (struct Node *) malloc(sizeof(struct Node));
    parrotnode->val = parrot;

	struct animal *falcon = (struct animal *) malloc(sizeof(struct animal));
    falcon->name = "falcon";
    struct Node *falconnode = (struct Node *) malloc(sizeof(struct Node));
    falconnode->val = falcon;

    tree->root = flynode;
    flynode->left = fournode;
    fournode->left = primatenode;
    fournode->right = dognode;
    flynode->right = meatnode;
    meatnode->left = parrotnode;
    meatnode->right = falconnode;

    return tree;
}

int main(int argc, char *argv[]){

    printf("Welcome to Dr. Dolittle's Animal Guessing Game.\n\n");
    
    printf("To prove my knowledge of the animal kingdom, think of any animal and I'll guess it!!\n\n");
   

    struct BSTree *tree = buildBSTTree();
    struct Node *current = tree->root;
    int answer = 0;
   
    while (current->left != 0 && current->right != 0){
        struct animal *animal = current->val;
        printf("%s", animal->name);
        scanf("%d", &answer);
        if (answer == 1){
            current = current->right;
        } else {
            current = current->left;
        }
    }
    struct animal *currAnimal = current->val;
    printf("Your animal is %s.\n", currAnimal->name);

}



