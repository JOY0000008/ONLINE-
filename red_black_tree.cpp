#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;
class node
{
    public:
    int val;
    string color;
    node *left;
    node *right;
    node *par;
    node(int data)
    {
        this->val=data;
        color="red";
        left=right=par=NULL;

    }


};


class RBTree
{
    public:
    node *root=NULL;

    void rotate_left(node* z)
    {
        node * y=z->right;
        
        if(!y)
        return;

        z->right=y->left;

        if(y->left)
        {
            y->left->par=z;

        }

        y->par=z->par;

        if(!z->par)
        root=y;

        else if(z==z->par->left)
        z->par->left=y;

        else
        z->par->right=y;

        y->left=z;
        z->par=y;


    }
    void rotate_right(node *z)
    {
        node * y=z->left;
        
        if(!y)
        return;

        z->left=y->right;

        if(y->right)
        {
            y->right->par=z;

        }

        y->par=z->par;

        if(!z->par)
        root=y;

        else if(z==z->par->right)
        z->par->right=y;

        else
        z->par->left=y;

        y->right=z;
        z->par=y;


    }
    void insert(int data)
    {
        node * newnode=new node(data);
        node * prev=NULL;
        node *curr=root;
        while(curr)
        {
            prev=curr;
            if(curr->val>data)
            curr=curr->left;
            else
            curr=curr->right;
        }
        if(prev==NULL)
        {
            root=newnode;
            newnode->color="black";
            return;
        }
        if(prev->val>data)
        prev->left=newnode;
        else
        prev->right=newnode;
        newnode->par=prev;

        insert_fixer(newnode);


    }
    void insert_fixer(node *z)
    {
       
        while(z->par!=NULL && z->par->color!="black")
        {
            if(z->par==z->par->par->left)
            {
                node *unc=z->par->par->right;
                if(unc && unc->color=="red")
                {
                    z->par->color="black";
                    unc->color="black";
                    z->par->par->color="red";
                    z=z->par->par;
                }
                else
                {
                    if(z==z->par->right)
                    {
                        z=z->par;
                        rotate_left(z);
                        
                    }
                    z->par->color="black";
                    z->par->par->color="red";
                    rotate_right(z->par->par);
                }
                

            }
            else
            {
                node *unc=z->par->par->left;
                if(unc && unc->color=="red")
                {
                    z->par->color="black";
                    unc->color="black";
                    z->par->par->color="red";
                    z=z->par->par;
                }
                else
                {
                    if(z==z->par->left)
                    {
                        z=z->par;
                        rotate_right(z);
                        
                    }
                    z->par->color="black";
                    z->par->par->color="red";
                    rotate_left(z->par->par);
                }


            }
        }
        root->color="black";
    }

    node *find(int data)
    {
        node *temp=root;
        while(temp!=NULL && temp->val!=data)
        {
            if(temp->val>data)
            temp=temp->left;
            else
            temp=temp->right;

        }
        return temp;
    }

    node *find_successor(node *z)
    {
        node *temp=z->right;

        while(temp!=NULL)
        temp=temp->left;
        return temp;

    }

    void delete_node(int data)
    {
        node *z=find(data);

        if(z==NULL)
        return;

        node *y,*x;

        if(z->left==NULL || z->right==NULL)
        y=z;
        
        else 
        y=find_successor(z);

        if(y!=z)
        z->val=y->val;

        if(y->left)
        x=y->left;

        else
        x=y->right;

        x->par=y->par;

        if(!y->par)
        root=x;

        else if(y==y->par->left)
        y->par->left=x;

        else
        y->par->right=x;

        if(y->color=="black")
        delete_fixup(x);

    }

    void delete_fixup(node *x)
    {
        while(x->color=="black" && x!=root)
        {
            if(x==x->par->left)
            {
                node *w=x->par->right;
                
                if(w->color=="red")
                {
                    w->color="black";
                    x->par->color="red";
                    rotate_left(x->par);

                }

                else if(w->color=="black" && w->left->color=="black" && w->right->color=="black")
                {
                    w->color="red";
                    x=x->par;

                }

                else
                {
                    if(w->right->color=="black")
                    {
                        w->color="red";
                        w->left->color="black";
                        rotate_right(w);
                        w=x->par->right;
                    }

                    w->color=x->par->color;
                    w->right->color="black";
                    x->par->color="black";
                    rotate_left(x->par);
                    x=root;

                }
            }

            else
            {
                node *w=x->par->left;
                
                if(w->color=="red")
                {
                    w->color="black";
                    x->par->color="red";
                    rotate_right(x->par);

                }

                else if(w->color=="black" && w->right->color=="black" && w->left->color=="black")
                {
                    w->color="red";
                    x=x->par;

                }

                else
                {
                    if(w->left->color=="black")
                    {
                        x->par->color="red";
                        w->color="black";
                        rotate_left(x->par);
                        w=x->par->left;
                    }

                    w->left->color="black";
                    x->par->color="black";
                    rotate_right(x->par);
                    x=root;

                }
            }
        }

        x->color="black";

    }

    void printTree(node* root, string indent = "", bool isRight = true)
    {
        if (root == NULL)
            return;

        cout << indent;
        if (isRight)
        {
            cout << "R----";
            indent += "     ";
        }
        else
        {
            cout << "L----";
            indent += "|    ";
        }

        string colorCode = (root->color == "red") ? "R" : "B";
        cout << root->val << "(" << colorCode << ")" << endl;

        printTree(root->left, indent, false);
        printTree(root->right, indent, true);
    }

    void print()
    {
        if(!root)
        {
            cout << "Tree is empty!" << endl;
            return;
        }
        
        cout << "\n=== Tree Structure ===" << endl;
        cout << "Root: " << root->val << "(" << (root->color == "red" ? "R" : "B") << ")" << endl;
        printTree(root->left, "", false);
        printTree(root->right, "", true);
        cout << "=====================\n" << endl;
    }
    
    // Level order print (your original style, but fixed)
    void printLevelOrder()
    {
        if(!root) {
            cout << "Tree is empty" << endl;
            return;
        }
        
        queue<node*> q;
        q.push(root);

        while(!q.empty())
        {
            node * temp = q.front();
            q.pop();  // FIX: Added this!

            if(temp == NULL)
                cout << "NULL(B) ";
            else
            {
                string colorCode = (temp->color == "red") ? "R" : "B";
                cout << temp->val << "(" << colorCode << ") ";
            }

            if(temp)
            {
                q.push(temp->left);
                q.push(temp->right);
            }
        }
        cout << endl;
    }
};

int main()
{
    RBTree rbt;
    
    cout << "Inserting: 1" << endl;
    rbt.insert(1);
    rbt.print();
    
    cout << "Inserting: 2" << endl;
    rbt.insert(2);
    rbt.print();
    
    cout << "Inserting: 3" << endl;
    rbt.insert(3);
    rbt.print();
    
    cout << "Inserting: 4" << endl;
    rbt.insert(4);
    rbt.print();
    
    cout << "Inserting: 5" << endl;
    rbt.insert(5);
    rbt.print();
    
    cout << "Inserting: 6" << endl;
    rbt.insert(6);
    rbt.print();

    cout << "Inserting: 7" << endl;
    rbt.insert(7);
    rbt.print();

    cout << "Inserting: 8" << endl;
    rbt.insert(8);
    rbt.print();

    cout << "Inserting: 9" << endl;
    rbt.insert(9);
    rbt.print();

    return 0;
}