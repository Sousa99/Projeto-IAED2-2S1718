#include "btree.h"

link_b max(link_b h) {
    if (h==NULL || h->r==NULL) return h;
    else return max(h->r);
}

link_b NEW(Item item, link_b l, link_b r) {
    link_b x = (link_b)malloc(sizeof(struct node_b));
    x->item = item;
    x->l = l;
    x->r = r;
    x->height=1;
    return x;
}

int height(link_b h) {
    if (h == NULL) return 0;
    return h->height;
}

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

link_b rotLR(link_b h) /* rotação dupla esquerda direita */ {
    if (h == NULL) return h;
    h->l = rotL(h->l);
    return rotR(h);
}
link_b rotRL(link_b h) /* rotação dupla direita esquerda */ {
    if (h == NULL) return h;
    h->r = rotR(h->r);
    return rotL(h);
}
int balance(link_b h) /* balance factor*/ {
    if(h == NULL) return 0;
    return height(h->l) - height(h->r);
}

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

link_b insertR(link_b h, Item item) {
    if (h == NULL)
    return NEW(item, NULL, NULL);
    if (less(key(item), key(h->item)))
    h->l = insertR(h->l, item);
    else
    h->r = insertR(h->r, item);
    h = AVLbalance(h);
    return h;
}

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

Item searchR(link_b h, Key v) {
    if (h == NULL)
    return NULL;
    if (eq(v, key(h->item)))
    return h->item;
    if (less(v, key(h->item)))
    return searchR(h->l, v);
    else
    return searchR(h->r, v);
}

link_b freeR(link_b h) {
    if (h==NULL)
    return h;
    h->l=freeR(h->l);
    h->r=freeR(h->r);
    return deleteR(h,key(h->item));
}

void STinit(link_b *head) { *head = NULL; }
void STinsert(link_b *head, Item item) { *head = insertR(*head, item); }
Item STsearch(link_b head, Key v) { return searchR(head, v); }
void STdelete(link_b *head, Key k) { *head = deleteR(*head, k); }
void STfree(link_b *head) { *head=freeR(*head); }
