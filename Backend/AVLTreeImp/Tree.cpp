#include "Tree.h"
using namespace std;
int max(AVLNode* val1,AVLNode* val2){
    int a = val1?val1->height:0;
    int b = val2?val2->height:0;
    return a>b?a:b;
}

//BINARY TREE CODE



// Node** tree_search(Node** from,int cmp(Node*,void*),void* key){
//     Node* node = *from;
//     if(!node){
//         return NULL;
//     }
//     int val = cmp(node,key);
//     if(val<0){
//         return tree_search(&node->right,cmp,key);
//     }
//     else if(val>0){
//         return tree_search(&node->left,cmp,key);
//     }
//     else{
//         return from;
//     }
// }
// void tree_insert(Node** from,int cmp(Node*,void*),Node* val){
//     Node* node = *from;
//     if(!node){
//         return;
//     }

//     Node** value = tree_search(from,cmp,(void*)val->key);
//     value = &val;
//     cout<<"success\n";
// }
// Node* node_detach(Node* node){
//     if(!node->left){
//         return node->right;
//     }
//     Node** victim = &node->right;
//     while((*victim)->left){
//         victim = &(*victim)->left;
//     }
//     Node* to_victim = *victim;
//     *victim = to_victim->right;
//     to_victim->left = node->left;
//     to_victim->right = node->right;
//     return to_victim;
// }
// Node* search_and_delete(Node* tree,int cmp(Node*,void*),void* key){
//     if(!tree){
//         return NULL;
//     }
//     int val_compared = cmp(tree,key);
//     if(val_compared<0){
//         tree->right = search_and_delete(tree->right,cmp,key);
//     }
//     else if(val_compared>0){
//         tree->left = search_and_delete(tree->left,cmp,key);    
//     }
//     else{
//         return node_detach(tree);
//     }
//     return tree;
// }

AVLNode* rot_right(AVLNode* node){
    AVLNode *parent = node->parent;
    AVLNode *new_node = node->left;
    AVLNode *inner = new_node->right;
    node->left = inner;
    if (inner) {
        inner->parent = node;
    }
    new_node->parent = parent;
    new_node->right = node;
    node->parent = new_node;
    avl_update(node);
    avl_update(new_node);
    return new_node;
}
AVLNode* rot_left(AVLNode* node){
    AVLNode *parent = node->parent;
    AVLNode *new_node = node->right;
    AVLNode *inner = new_node->left;
    node->right = inner;
    if (inner) {
        inner->parent = node;
    }
    new_node->parent = parent;
    new_node->left = node;
    node->parent = new_node;
    avl_update(node);
    avl_update(new_node);
    return new_node;
}
static AVLNode* fix_right(AVLNode* node){
    if(get_heights(node->right->left)>get_heights(node->right->right)){
        node->right = rot_right(node->right);
    }
    return rot_left(node);
}
static AVLNode* fix_left(AVLNode * node){
    if(get_heights(node->left->left)<get_heights(node->left->right)){
        node->left = rot_left(node->left);
    }
    return rot_right(node);
}
AVLNode* avl_fix(AVLNode* node){
    while (true)
    {
        AVLNode** from = &node;
        AVLNode* parent = node->parent;

        if(parent){
            from = parent->left == node?&parent->left:&parent->right;
        }
        avl_update(node);
        size_t l,r;
        l = get_heights(node->left);
        r = get_heights(node->right);
        if(l == r+2){
            *from = fix_left(node);
        }

         if(l+2 == r){
            *from = fix_right(node);
        }

        if(!parent){
            return *from;
        }

        node = parent;
    }
    
}
static AVLNode* del_node_easy(AVLNode* node){
    assert(!node->left||!node->right);
    AVLNode* parent = node->parent;
    AVLNode* child = node->left?node->left:node->right;

    if(child){
        child->parent = parent;
    }
    if(!parent){
        return child;
    }
    AVLNode** from = (parent->left == node)?&parent->left:&parent->right;
    *from = child;
    return avl_fix(parent);
}
AVLNode* node_detachs(AVLNode* node){
    if(!node->left||!node->right){
        return del_node_easy(node);
    }
    AVLNode* victim = node;
    victim = victim->right;
    while(victim->left){
        victim = victim->left;
    }
    AVLNode* root = del_node_easy(victim);
    *victim = *node;
    if(victim->left){
        victim->left->parent = victim;
    }
    if(victim->right){
        victim->right->parent = victim;
    }
    AVLNode** from = &root;
    AVLNode* parent = node->parent;
    if(parent){
        from = (parent->left == node)?&parent->left:&parent->right;
    }
    *from = victim;
    return root;
}
void search_and_insert(AVLNode** root,AVLNode* new_node,bool* less(AVLNode*,AVLNode*)){
    AVLNode** from = root;
    AVLNode* parent = NULL;
    AVLNode* node = NULL;
    for(node = *root;node;){
        from = less(new_node,new_node)?&node->left:&node->right;
        parent = node;
        node = *from;
    
    }
    *from = new_node;
    new_node->parent = parent;
    *root = avl_fix(new_node);
}
AVLNode* search_and_deletes(AVLNode** root,void* key,int cmp(AVLNode*,void*)){
    AVLNode* node = *root;
    for(;node;){
        int val = cmp(node,key);
        if(val<0){
            node = node->right;
        }
        else if(val>0){
            node = node->left;
        }
        else{
            *root = node_detachs(node);
            return node;
        }
    }
    return NULL;
}


