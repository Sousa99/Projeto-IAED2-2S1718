#include "btree.h"

/**	Function: NEW
 *	@param item (Item (link))
 *  @param l (link_b)
 *  @param r (link_b)
 *  @return x (link_b)
 *  Creates a new node for the binary tree
 */
link_b NEW(Item item, link_b l, link_b r) {
    link_b x = (link_b)malloc(sizeof(struct node_b));
    x->item = item;
    x->l = l;
    x->r = r;
    x->height = 1;
    return x;
}

/**	Function: height
 *	@param h (link_b)
 *  @return (int)
 *  Returns the height of the binary tree
 */
int height(link_b h) {
    if (h == NULL) return 0;
    return h->height;
}

/**	Function: max
 *	@param h (link_b)
 *  @return (link_b)
 *  Returns the max node
 */
link_b max(link_b h) {
    if (h==NULL || h->r==NULL) return h;
    else return max(h->r);
}

/**	Function: rotL
 *	@param h (link_b)
 *  @return x (link_b)
 *  Operation of rotation-left on tree
 */
link_b rotL(link_b h) {
    int hleft, hright, xleft, xright;
    link_b x = h->r;

    h->r = x->l;
    x->l = h;

    hleft = height(h->l);
    hright = height(h->r);
    h->height = hleft > hright ? hleft + 1 : hright + 1;

    xleft = height(x->l);
    xright = height(x->r);
    x->height = xleft > xright ? xleft + 1 : xright + 1;

    return x;
}

/**	Function: rotR
 *	@param h (link_b)
 *  @return x (link_b)
 *  Operation of rotation-right on tree
 */
link_b rotR(link_b h) {
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

/**	Function: rotLR
 *	@param h (link_b)
 *  @return (link_b)
 *  Operation of double rotation, left then right, on tree
 */
link_b rotLR(link_b h) /* rotação dupla esquerda direita */ {
    if (h == NULL) return h;
    h->l = rotL(h->l);
    return rotR(h);
}
/**	Function: rotRL
 *	@param h (link_b)
 *  @return (link_b)
 *  Operation of double rotation, right then left, on tree
 */
link_b rotRL(link_b h) /* rotação dupla direita esquerda */ {
    if (h == NULL) return h;
    h->r = rotR(h->r);
    return rotL(h);
}

/**	Function: balance
 *	@param h (link_b)
 *  @return (int)
 *  Operation of balance factor on tree
 */
int balance(link_b h) /* balance factor*/ {
    if(h == NULL) return 0;
    return height(h->l) - height(h->r);
}

/**	Function: AVLbalance
 *	@param h (link_list)
 *  @return h (link_b)
 *  Balances out the all tree
 */
link_b AVLbalance(link_b h) {
    int balanceFactor, hleft, hright;

    if (h == NULL) return h;
    balanceFactor = balance(h);
    if(balanceFactor > 1) { /* mais peso para a esquerda */
        if (balance(h->l) >= 0) h = rotR(h);
        else h = rotLR(h);
    }
    else if(balanceFactor < -1){ /* mais peso para a direita*/
        if (balance(h->r) <= 0) h = rotL(h);
        else h = rotRL(h);
    }
    else{
        hleft = height(h->l);
        hright = height(h->r);
        h->height = hleft > hright ? hleft + 1 : hright + 1;
    }
    return h;
}

/**	Function: insertR
 *	@param h (link_b)
 *  @param item (Item)
 *  @return h (link_b)
 *  Inserts a new node to the tree and then balances it
 */
link_b insertR(link_b h, Item item) {
    if (h == NULL) return NEW(item, NULL, NULL);

    if (less(key(item), key(h->item))) h->l = insertR(h->l, item);
    else h->r = insertR(h->r, item);
    h = AVLbalance(h);

    return h;
}

/**	Function: insertR
 *	@param h (link_b)
 *  @param k (Key)
 *  @return h (link_b)
 *  Removes a node from the tree and then balances it
 */
link_b deleteR(link_b h, Key k) {
    if (h == NULL) return h;
    else if (less(k, key(h->item))) h->l = deleteR(h->l,k);
    else if (less(key(h->item), k)) h->r = deleteR(h->r,k);
    else {
        if (h->l != NULL && h->r != NULL){
        link_b aux = max(h->l);
        {Item x; x = h->item; h->item = aux->item; aux->item = x;}
        h->l = deleteR(h->l, key(aux->item));
        }
        else {
            link_b aux = h;
            if (h->l == NULL && h->r == NULL) h = NULL;
            else if (h->l == NULL) h = h->r;
            else h = h->l;
            free(aux);
        }
    }
    h = AVLbalance(h);
    return h;
}

/**	Function: searchR
 *	@param h (link_b)
 *  @param v (Key)
 *  @return (Item)
 *  Searchs the all tree for a specific key and return the node's item
 */
Item searchR(link_b h, Key v) {
    if (h == NULL) return NULL;

    if (eq(v, key(h->item))) return h->item;

    if (less(v, key(h->item))) return searchR(h->l, v);
    else return searchR(h->r, v);
}

/**	Function: freeR
 *	@param h (link_b)
 *  Frees the allocated memory for the all tree, therefore of all nodes
 */
void freeR(link_b h) {
    if (h != NULL) {
        freeR(h->l);
        freeR(h->r);
        free(h);
    }
}

/*  Functions used in order to work with binary tree in an abstract way, only using */
/* pointers and not expecting, in most of them, anything returned */
void STinit(link_b *head) { *head = NULL; }
void STinsert(link_b *head, Item item) { *head = insertR(*head, item); }
Item STsearch(link_b head, Key v) { return searchR(head, v); }
void STdelete(link_b *head, Key k) { *head = deleteR(*head, k); }
void STfree(link_b *head) { freeR(*head); }
