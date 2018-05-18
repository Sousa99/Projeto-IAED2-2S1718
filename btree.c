#include "btree.h"

/**	Function: new_node
 *	@param item (Item (link))
 *  @param l (link_b)
 *  @param r (link_b)
 *  @return x (link_b)
 *  Creates a new node for the binary tree
 */
link_b new_node(Item item, link_b left, link_b right) {
    link_b new_node = (link_b)malloc(sizeof(struct node_b));
    new_node->item = item;
    new_node->l = left;
    new_node->r = right;
    new_node->height = 1;
    return new_node;
}

/**	Function: height
 *	@param h (link_b)
 *  @return (int)
 *  Returns the height of the binary tree
 */
int height(link_b head) {
    if (head == NULL) return 0;
    return head->height;
}

/**	Function: max
 *	@param h (link_b)
 *  @return (link_b)
 *  Returns the max node
 */
link_b max(link_b head) {
    if (head == NULL || head->r == NULL) return head;
    else return max(head->r);
}

/**	Function: rotationLeft
 *	@param h (link_b)
 *  @return x (link_b)
 *  Operation of rotation-left on tree
 */
link_b rotationLeft(link_b head) {
    int head_left, head_right, new_node_left, new_node_right;
    link_b new_node = head->r;

    head->r = new_node->l;
    new_node->l = head;

    head_left = height(head->l);
    head_right = height(head->r);
    head->height = head_left > head_right ? head_left + 1 : head_right + 1;

    new_node_left = height(new_node->l);
    new_node_right = height(new_node->r);
    new_node->height = new_node_left > new_node_right ? new_node_left + 1 : new_node_right + 1;

    return new_node;
}

/**	Function: rotationRight
 *	@param h (link_b)
 *  @return x (link_b)
 *  Operation of rotation-right on tree
 */
link_b rotationRight(link_b h) {
    int hleft, hright, xleft, xright;
    link_b x = h->l;

    h->l = x->r;
    x->r = h;

    hleft = height(h->l);
    hright = height(h->r);
    h->height = hleft > hright ? hleft + 1 : hright + 1;

    xleft = height(x->l);
    xright = height(x->r);
    x->height = xleft > xright ? xleft + 1 : xright + 1;

    return x;
}

/**	Function: rotLeftRight
 *	@param h (link_b)
 *  @return (link_b)
 *  Operation of double rotation, left then right, on tree
 */
link_b rotationLeftRight(link_b head) /* rotação dupla esquerda direita */ {
    if (head == NULL) return head;
    head->l = rotationLeft(head->l);
    return rotationRight(head);
}
/**	Function: rotationRightLeft
 *	@param h (link_b)
 *  @return (link_b)
 *  Operation of double rotation, right then left, on tree
 */
link_b rotationRightLeft(link_b head) /* rotação dupla direita esquerda */ {
    if (head == NULL) return head;
    head->r = rotationRight(head->r);
    return rotationLeft(head);
}

/**	Function: balance
 *	@param h (link_b)
 *  @return (int)
 *  Operation of balance factor on tree
 */
int balance(link_b head) /* balance factor*/ {
    if(head == NULL) return 0;
    return height(head->l) - height(head->r);
}

/**	Function: AVLbalance
 *	@param h (link_list)
 *  @return h (link_b)
 *  Balances out the all tree
 */
link_b AVLbalance(link_b head) {
    int balanceFactor, head_left, head_right;

    if (head == NULL) return head;
    balanceFactor = balance(head);
    if(balanceFactor > 1) { /* mais peso para a esquerda */
        if (balance(head->l) >= 0) head = rotationRight(head);
        else head = rotationLeftRight(head);
    }
    else if(balanceFactor < -1){ /* mais peso para a direita*/
        if (balance(head->r) <= 0) head = rotationLeft(head);
        else head = rotationRightLeft(head);
    }
    else{
        head_left = height(head->l);
        head_right = height(head->r);
        head->height = head_left > head_right ? head_left + 1 : head_right + 1;
    }
    return head;
}

/**	Function: insertNode
 *	@param h (link_b)
 *  @param item (Item)
 *  @return h (link_b)
 *  Inserts a new node to the tree and then balances it
 */
link_b insertNode(link_b head, Item item) {
    if (head == NULL) return new_node(item, NULL, NULL);

    if (less(key(item), key(head->item))) head->l = insertNode(head->l, item);
    else head->r = insertNode(head->r, item);
    head = AVLbalance(head);

    return head;
}

/**	Function: deleteNode
 *	@param h (link_b)
 *  @param k (Key)
 *  @return h (link_b)
 *  Removes a node from the tree and then balances it
 */
link_b deleteNode(link_b head, Key k) {
    if (head == NULL) return head;
    else if (less(k, key(head->item))) head->l = deleteNode(head->l,k);
    else if (less(key(head->item), k)) head->r = deleteNode(head->r,k);
    else {
        if (head->l != NULL && head->r != NULL){
        link_b aux = max(head->l);
        {Item x; x = head->item; head->item = aux->item; aux->item = x;}
        head->l = deleteNode(head->l, key(aux->item));
        }
        else {
            link_b aux = head;
            if (head->l == NULL && head->r == NULL) head = NULL;
            else if (head->l == NULL) head = head->r;
            else head = head->l;
            free(aux);
        }
    }
    head = AVLbalance(head);
    return head;
}

/**	Function: searchTree
 *	@param h (link_b)
 *  @param v (Key)
 *  @return (Item)
 *  Searchs the all tree for a specific key and return the node's item
 */
Item searchTree(link_b head, Key v) {
    if (head == NULL) return NULL;

    if (eq(v, key(head->item))) return head->item;

    if (less(v, key(head->item))) return searchTree(head->l, v);
    else return searchTree(head->r, v);
}

/**	Function: freeAll
 *	@param h (link_b)
 *  Frees the allocated memory for the all tree, therefore of all nodes
 */
void freeAllTree(link_b head) {
    if (head != NULL) {
        freeAllTree(head->l);
        freeAllTree(head->r);
        free(head);
    }
}

/*  Functions used in order to work with binary tree in an abstract way, only using */
/* pointers and not expecting, in most of them, anything returned */
void STinit(link_b *head) { *head = NULL; }
void STinsert(link_b *head, Item item) { *head = insertNode(*head, item); }
Item STsearch(link_b head, Key v) { return searchTree(head, v); }
void STdelete(link_b *head, Key k) { *head = deleteNode(*head, k); }
void STfree(link_b *head) { freeAllTree(*head); }
