#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define MAX 100

typedef struct node 
{
    int id; //ID of user
    int numfren; //number of friends of user
    char name[MAX]; //name of user
    int* friends; //friends of user as an array
    struct node* right;  //user to the right
    struct node* left; //user to the left
} node;

struct node* retUser(char str[MAX])
{
    char name[MAX];
    char ID[MAX];
    char strfriends[MAX];

    //copied ID
    char*token=strtok(str,",");
    strcpy(ID,token);

    //copied Name
    token=strtok(NULL,",");
    strcpy(name,token);

    //copied friends' ID
    token=strtok(NULL,",");
    strcpy(strfriends,token);
    //printf("%s\n",(strfriends));

    //creating user nodes
    int id=atoi(ID);
    struct node *user = malloc(sizeof(struct node));
    user->id=id;
    user->friends=(int*)malloc(MAX * sizeof(int));
    strcpy(user->name,name);


    //adding user's friend's IDs
    token = strtok(strfriends,"|");  
    int i=0;
    while( token != NULL ) 
    {

        int temp=atoi(token);
        user->friends[i]=temp;
        i++;
        token = strtok(NULL,"|");
    }
    user->numfren=i;
    if(i==0){user->friends[i]=-1;}
    return user;
}


//search for user with id=key
struct node* search(int key, struct node *users)
{
    //CODE HERE
    node* temp = users;

    while(temp != NULL)
    {
        if(key == temp->id)
        {
            return temp;
        }
        else if(key < temp->id)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }

    return NULL;
}

//see document for explanattion
struct node* refineUser(struct node *user, struct node *users)
{
    //CODE HERE
    while(search(user->id, users) != NULL)
    {
        user->id ++;
    }
    for(int i = 0; i < user->numfren; )
    {
        struct node *temp = search(user->friends[i], users);
        if(temp == NULL)
        {
            for(int j = i; j < user->numfren - 1; j++) 
            {
                user->friends[j] = user->friends[j+1];
            } 
            user->numfren --;
            if (user->numfren == 0)
            {
                user->friends[0] = -1;       
            } 
        }
        else
        {
            temp->friends[temp->numfren] = user->id;
            temp->numfren ++;
            i++;
        }
    }
    return user;
}

//insert user with id
struct node* insertUser(struct node* root, int id, struct node* user)
{
    //CODE HERE
    node* temp;
    node* prev;
    user->left = NULL;
    user->right = NULL;

    // if the root is empty then make a new node as the root node
    if (root == NULL)
    {
        root = user;
    }
    else
    {
        temp = root;
        prev = NULL;
        // traverse through the BST to get the correct position for insertion
        while (temp != NULL)
        {
            prev = temp;
            if (id < temp->id)
            {
                temp = temp->left;
            }
            else
            {
                temp = temp->right;
            }
        }
        // found the last node where the new node should insert
        if (id < prev->id)
        {
            prev->left = user;
        }
        else
        {
            prev->right = user;
        }
    }
    return root;
}

//prints friends list
void friends(int id, struct node *users) 
{
    //CODE HERE
    node *temp = search(id, users);
    if(temp->numfren == 0)
    {
        printf("-1\n");
    }
    else
    {
        for(int i = 0; i < temp->numfren; i++)
        {  
            printf("%d\n",temp->friends[i]);
        }
    }
}

//find child node with minimum value (inorder successor) - helper to delete node
struct node *minValueNode(struct node *node) 
{
    //CODE HERE
    struct node* temp = node;
    while((temp) && (temp->left != NULL))
    {
        temp = temp->left;
    }
    return temp;
}

//deletes itself from its friend's nodes
struct node *deleteFriends(int key, struct node* users)
{
    //CODE HERE
    node* user = search(key, users);
    for(int i = 0; i < user->numfren; i++)
    {
        node *temp = search(user->friends[i], users);
        if(temp != NULL)
        {
            for(int j = 0; j < temp->numfren; j++)
            {
                if(temp->friends[j] == user->id)
                {
                    for(int k = j; k < (temp->numfren-1); k++) 
                    {
                        temp->friends[k] = temp->friends[k+1];
                    }
                    temp->numfren--;
                }
            }
        }
    }
    return user;
}

// Deleting a node
struct node *deleteNode(struct node *root, int key) 
{
    //CODE HERE
    if (root == NULL)
    {
        return NULL;
    }
    else if (key < root->id)
    {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->id)
    {
        root->right = deleteNode(root->right, key);
    }
    else 
    {
        if (root->left==NULL && root->right==NULL)  
        {
            free(root);
            return NULL;
        }
        else if (root->left == NULL) 
        {
            struct node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) 
        {
            struct node* temp = root->left;
            free(root);
            return temp;
        }
        node* min_node = minValueNode(root->right);
        root->id = min_node->id;
        root->numfren = min_node->numfren;
        strcpy(root->name, min_node->name);
        for(int i = 0; i < root->numfren; i++)
        {
            root->friends[i] = min_node->friends[i];
        }
        root->right = deleteNode(root->right, min_node->id);
    }
    return root;
}

//Print USER's IDs in ascending order
void printInOrder(node* myusers) 
{
    //CODE HERE
    if(myusers != NULL)
	{
		printInOrder(myusers->left);
		printf("%d %s\n", myusers->id, myusers->name);
		printInOrder(myusers->right);
	}
}


int main(int argc, char **argv)
{
    node *users = NULL;   
    while(1)
    {

        int opt, id;
        fflush(stdin);
        scanf("%d",&opt);
        char str[MAX];
        switch(opt)
        {
            case 1:
      
                scanf("%s",str);
                struct node* tbins=retUser(str);
                tbins=refineUser(tbins, users);
                users=insertUser(users,tbins->id,tbins);
                break;

            case 2:
           
                scanf("%d", &id);
                deleteFriends(id, users);
                users=deleteNode(users, id);
                break;

            case 3:
        
                scanf("%d", &id);
                node* result=search(id,users);
                if(result==NULL) 
                    printf("USER NOT IN TREE.\n");
                else{
                    printf("%d\n",result->id);
                }
                break;

            case 4:
                scanf("%d", &id);
                friends(id, users);
                break;

            case 5:
                printInOrder(users);
                break;

            case 6:
                exit(0);
                break;

            default: 
                printf("Wrong input! \n");
                break;
        }

    }
    return 0;
}
