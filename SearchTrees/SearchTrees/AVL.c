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


//////// AVL stuff ///////////////////////
typedef unsigned long long AVLT;


typedef struct AVLNode_s {
    struct AVLNode_s *left, *right;
    AVLT key;
    int height;
} AVLNode;

static void AVLNode_print(AVLNode const *t);
static void AVLNode_debug(const char *msg, AVLNode const *t, int lev, const char *lr);
 

static AVLNode *AVLNode_create(unsigned long long key) {
  AVLNode *t = (AVLNode *)calloc(1, sizeof (AVLNode));
  t->key = key;
  t->height = 1;
  //printf("create: %p with key %lld\n", t, key);
  return t;
}

static int getBalanceFactor(AVLNode const *t) {
  int r = t->right == NULL ? 0 : t->right->height;
  int l = t->left  == NULL ? 0 : t->left->height;
  return r - l;
}

static void fix(AVLNode *t) {
  int r = t->right == NULL ? 0 : t->right->height;
  int l = t->left  == NULL ? 0 : t->left->height;
  t->height = (r > l ? r : l) + 1;
}

static AVLNode *AVLNode_insert(AVLNode *this, AVLT key) {
  if (key < this->key)
    this->left = this->left   == NULL ? AVLNode_create(key) : AVLNode_insert(this->left,  key);
  else
    this->right = this->right == NULL ? AVLNode_create(key) : AVLNode_insert(this->right, key);
  return balance(this);
}

static AVLNode *findMinimum(AVLNode *this) {
  return this->left != NULL ? findMinimum(this->left) : this;
}

static AVLNode *removeMinimum(AVLNode *this) {
  if (this->left == NULL) return this->right;
  this->left = removeMinimum(this->left);
  return balance(this);
}

static AVLNode *AVLNode_remove(AVLNode *p, AVLT key) {
  if (p == NULL) return NULL;
  if (key < p->key) {
    p->left = AVLNode_remove(p->left, key);
    return balance(p);
  } else if (key > p->key) {
    p->right = AVLNode_remove(p->right, key);
    return balance(p);
  } else {
    AVLNode *l = p->left;
    AVLNode *r = p->right;
    free(p);
    if (r == NULL) return l;
    AVLNode *min = findMinimum(r);
    min->right = removeMinimum(r);
    min->left = l;
    return balance(min);
  }
}

static void AVLNode_delete_recursive(AVLNode *t) {
  if (t == NULL) return;
  AVLNode_delete_recursive(t->left);
  AVLNode_delete_recursive(t->right);
  free(t);
}

///   AVL public stuff //////////
typedef struct AVLTree_s {
  AVLNode *root;
} AVL;
    
AVL *AVL_create() {
  AVL *t = (AVL *)calloc(1, sizeof(AVL));
  t->root = NULL;
  return t;
}

void AVL_destroy(AVL *t) {
  if (t->root != NULL) {
    AVLNode_delete_recursive(t->root);
  }
  free(t);
}

void AVL_print(AVL const *t) {
  if (t->root != NULL) AVLNode_print(t->root);
}

int AVL_insert(AVL *t, AVLT key) {
  t->root = t->root == NULL? AVLNode_create(key): AVLNode_insert(t->root, key);
  return 0;
}

int AVL_remove(AVL *t, AVLT key) {
  t->root = AVLNode_remove(t->root, key);
  return 0;
}


/////// main stuff ///////
    
int main() {
    const int SIZE = 1000001;
    FastRandom *r = FastRandom_create(1); // time(NULL));
    AVLT *pat = malloc(SIZE * sizeof(AVLT));
    for (int i = 0; i < SIZE; i++) {
        pat[i] = FastRandom_rand(r);
    }
    AVL *t = AVL_create();
    clock_t s1 = clock();
    for (int i = 0; i < SIZE; i++) {
      AVL_insert(t,pat[i]);
    }
    clock_t s12 = clock();
    for (int i = 0; i < SIZE; i++) {
      AVL_remove(t,pat[i]);
    }
    clock_t e1 = clock();
    AVL_destroy(t);
#if 0
    multiset<AVLT> se1;
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
    printf("AVL_INS=%.3f AVL_REM=%.3f\n",
      (double)(s12 - s1) / CLOCKS_PER_SEC,
      (double)(e1  - s12) / CLOCKS_PER_SEC);
    FastRandom_delete(r);
#if 0
      printf("AVL_INS=%.3f AVL_REM=%.3f SET_INS=%.3f SET_REM=%.3f\n",
        (double)(s12 - s1) / CLOCKS_PER_SEC,
        (double)(e1  - s12) / CLOCKS_PER_SEC,
        (double)(s22 - s2) / CLOCKS_PER_SEC,
        (double)(e2 - s22) / CLOCKS_PER_SEC);
#endif
   free(pat);
}
