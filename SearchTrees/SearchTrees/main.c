#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


//////// FastRandom stuff ////////////////
typedef struct FastRandom_s {
    unsigned long long rnd;
} FastRandom;

FastRandom *FastRandom_create(unsigned long long seed) {
  FastRandom *t = (FastRandom *)malloc(sizeof (FastRandom));
  t->rnd = seed;
  return t;
}

unsigned long long
FastRandom_rand(FastRandom *t) {
  t->rnd ^= t->rnd << 21;
  t->rnd ^= t->rnd >> 35;
  t->rnd ^= t->rnd << 4;
  return t->rnd;
}

void FastRandom_delete(FastRandom *r) {
  free(r);
}


//////// BST stuff ///////////////////////
typedef unsigned long long BSTT;


typedef struct BSTNode_s {
    struct BSTNode_s *left, *right;
    BSTT key;
} BSTNode;

static void BSTNode_print(BSTNode const *t, int lev);
 

static BSTNode *BSTNode_create(BSTT key) {
  BSTNode *t = (BSTNode *)calloc(1, sizeof (BSTNode));
  t->key = key;
  return t;
}

static BSTNode *BSTNode_insert(BSTNode *this, BSTT key) {
  if (key < this->key)
    this->left = this->left   == NULL ? BSTNode_create(key) : BSTNode_insert(this->left,  key);
  else
    this->right = this->right == NULL ? BSTNode_create(key) : BSTNode_insert(this->right, key);
  return this;
}

#if 0
//static BSTNode *BSTNode_insert_to_root(BSTNode *this, BSTT key) {
BSTNode *BSTNode_insert_to_root(node* head, BSTT key) {
  if (head == NULL) {
    return BSTNode_create(key);
  }
  if (key < head->key) {
    head->left = BSTNode_insert_to_root(head->left, key);
    return rotateRight(head);
  } else {
    head->right = BSTNode_insert_to_root(head->right, key);
    return rotateLeft(head);
  }
}
#endif

static BSTNode *findMinimum(BSTNode *this) {
  return this->left != NULL ? findMinimum(this->left) : this;
}

static BSTNode *removeMinimum(BSTNode *this) {
  if (this->left == NULL) return this->right;
  this->left = removeMinimum(this->left);
  return this;
}

static BSTNode *BSTNode_remove(BSTNode *p, BSTT key) {
  if (p == NULL) return NULL;
  if (key < p->key) {
    p->left = BSTNode_remove(p->left, key);
    return p;
  } else if (key > p->key) {
    p->right = BSTNode_remove(p->right, key);
    return p;
  } else {
    BSTNode *l = p->left;
    BSTNode *r = p->right;
    free(p);
    if (r == NULL) return l;
    BSTNode *min = findMinimum(r);
    min->right = removeMinimum(r);
    min->left = l;
    return min;
  }
}

static void BSTNode_print(BSTNode const *t, int lev) {
  if (t->right != NULL) BSTNode_print(t->right, lev+1);
  for (int i = 0; i < lev; i++) {
    printf("  ");
  }
  printf("[%llu]\n", t->key);
  if (t->left != NULL) BSTNode_print(t->left, lev+1);
}

static void BSTNode_delete_recursive(BSTNode *t) {
  if (t == NULL) return;
  BSTNode_delete_recursive(t->left);
  BSTNode_delete_recursive(t->right);
  free(t);
}

///   BST public stuff //////////
typedef struct BSTTree_s {
  BSTNode *root;
} BST;
    
BST *BST_create() {
  BST *t = (BST *)calloc(1, sizeof(BST));
  t->root = NULL;
  return t;
}

void BST_destroy(BST *t) {
  if (t->root != NULL) {
    BSTNode_delete_recursive(t->root);
  }
  free(t);
}

void BST_print(BST const *t) {
  if (t->root != NULL) BSTNode_print(t->root, 0);
}

int BST_insert(BST *t, BSTT key) {
  t->root = t->root == NULL? BSTNode_create(key): BSTNode_insert(t->root, key);
  return 0;
}

int BST_remove(BST *t, BSTT key) {
  t->root = BSTNode_remove(t->root, key);
  return 0;
}


    
int main() {
    const int SIZE = 1000001;
    FastRandom *r = FastRandom_create(1); // time(NULL));
    BSTT *pat = malloc(SIZE * sizeof(BSTT));
    for (int i = 0; i < SIZE; i++) {
        pat[i] = FastRandom_rand(r);
    }
    BST *t = BST_create();
    clock_t s1 = clock();
    for (int i = 0; i < SIZE; i++) {
      //printf("===== insert %lld\n", pat[i]);
      BST_insert(t,pat[i]);
      //printf("after insertion %lld\n", pat[i]);
      //BST_print(t);
      //printf("--------\n");
    }
    clock_t s12 = clock();
    for (int i = 0; i < SIZE; i++) {
      BST_insert(t,pat[i]);
    }
    clock_t e1 = clock();
    BST_destroy(t);
#if 0
    multiset<BSTT> se1;
    clock_t s2 = clock();
    for (auto const &s: pat) {
        se1.insert(s);
    }
    clock_t s22 = clock();
    for (auto const &s: pat) {
      se1.erase(s);
    }
    clock_t e2 = clock();
#endif
    printf("BST_INS=%.3f BST_REM=%.3f\n",
      (double)(s12 - s1) / CLOCKS_PER_SEC,
      (double)(e1  - s12) / CLOCKS_PER_SEC);
    FastRandom_delete(r);
#if 0
      printf("BST_INS=%.3f BST_REM=%.3f SET_INS=%.3f SET_REM=%.3f\n",
        (double)(s12 - s1) / CLOCKS_PER_SEC,
        (double)(e1  - s12) / CLOCKS_PER_SEC,
        (double)(s22 - s2) / CLOCKS_PER_SEC,
        (double)(e2 - s22) / CLOCKS_PER_SEC);
#endif
   free(pat);
}
