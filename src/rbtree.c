#include "rbtree.h"

#include <stdlib.h>

rbtree* new_rbtree(void) {
  rbtree* p = (rbtree *)calloc(1, sizeof(rbtree)); // rbtree의 인자를 1개 메모리 할당(할당된 공간의 값을 모두 0으로 초기화)

  // TODO: initialize struct if needed
  p -> nil = (node_t *)calloc(1, sizeof(node_t)); // nil 메모리 할당
  p -> nil -> color = RBTREE_BLACK; // nil 색상
  p -> root = p -> nil; // root의 부모는 nil
  return p;
}

node_t * _delete(rbtree *t, node_t * curr){
  if(curr -> left == t -> nil && curr -> right == t -> nil){
    return curr;
  }
  if(curr -> left != t -> nil){
    free(_delete(t, curr -> left));
    curr == NULL;
  }
  if(curr -> right != t -> nil){
    free(_delete(t, curr -> right));
    curr == NULL;
  }
  return curr;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if(t -> root != t -> nil){
    free(_delete(t, t -> root));
  }
  free(t -> nil);
  t -> nil = NULL;
  free(t);
  t = NULL;
}


void right_rotate(rbtree* tree, node_t* x) {
  node_t* y = x -> left;
  x -> left = y -> right; // x의 왼쪽 서브 트리는 y의 오른쪽 서브 트리가 된다.
  if (y -> right != tree -> nil) { // y의 서브 트리가 nil 노드가 아니면
    y -> right -> parent = x; // y의 서브 트리의 부모는 x
  }
  y -> parent = x -> parent; // y의 부모를 x의 부모에 연결
  if (x -> parent == tree -> nil) { // x의 부모가 nil 노드이면
    tree -> root = y; // tree의 root는 y다
  }
  else if (x == x -> parent -> left) { // x가 x의 부모의 왼쪽 서브트리면
    x -> parent -> left = y; // x의 부모의 왼쪽 서브트리에 y를
  }
  else {
    x -> parent -> right = y; // 아니면 x의 부모의 오른쪽 서브트리에 y를 이동
  }
  y -> right = x; // y의 오른쪽 서브트리는 x가 된다.
  x -> parent = y; // x의 부모는 y다
}

void left_rotate(rbtree* tree, node_t* x) {
  node_t* y = x -> right; // y를 설정
  x -> right = y -> left; // x의 오른쪽 서브 트리는 y의 왼쪽 서브 트리가 된다.
  if (y -> left != tree -> nil) { // y의 서브 트리가 nil 노드가 아니면
    y -> left -> parent = x; // y의 서브 트리의 부모는 x
  }
  y -> parent = x -> parent; // y의 부모를 x의 부모에 연결
  if (x -> parent == tree -> nil) { // x의 부모가 nil 노드이면
    tree -> root = y; // tree의 root는 y다
  }
  else if (x == x -> parent -> left) { // x가 x의 부모의 왼쪽 서브트리면
    x -> parent -> left = y; // x의 부모의 왼쪽 서브트리에 y다
  }
  else {
    x -> parent -> right = y; // 아니면 x의 부모의 오른쪽 서브트리에 y를 이동
  }
  y -> left = x; // y의 왼쪽 서브트리는 x가 된다.
  x -> parent = y; // x의 부모는 y다
}

void _rb_insert_fixup(rbtree *t, node_t *curr){
  while (curr -> parent -> color == RBTREE_RED) { // 현재 부모의 컬러가 레드면, while
    // (1) 현재 부모의 노드가 현재 조상 노드의 왼쪽 노드면
    if (curr -> parent == curr -> parent -> parent-> left) {
      // case 1
      node_t * uncle = curr -> parent -> parent -> right; // 현재 조상의 오른쪽 노드는 삼촌으로
      if (uncle -> color == RBTREE_RED) { // 삼촌 노드 컬러가 레드면,
        curr -> parent -> color = uncle -> color = RBTREE_BLACK; // 현재 부모와 삼촌 노드의 색상을 블랙으로 변경,
        curr -> parent -> parent -> color = RBTREE_RED; // 그리고 현재 조상의 색상을 레드로 변경
        curr = curr -> parent -> parent; // 현재 조상 노드를 기준으로 다시 반복.
      }
      else {
        // case 2
        if (curr == curr -> parent -> right) { // 현재 노드가 현재 부모의 오른쪽이라면,
          curr = curr -> parent; // 현재 노드를 현재 변환? 갱신?.
          left_rotate(t, curr); // 그리고 left rotate
        }
        // case 3
        curr -> parent -> color = RBTREE_BLACK; // 현재 부모의 컬러 블랙으로,
        curr -> parent -> parent -> color = RBTREE_RED; // 현재 조상의 색상 레드로
        right_rotate(t, curr -> parent -> parent); // 그리고  right rotate
      }
    }
    // (2) 현재 부모 노드가 현재 조상 노드의 오른쪽 노드면,
    else{
      node_t * uncle = curr -> parent -> parent -> left; // 현재 조상의 왼쪽 노드를 현재 삼촌으로

      if (uncle -> color == RBTREE_RED) { // 현재 삼촌 노드 컬러가 레드면,
        curr -> parent -> color = uncle -> color = RBTREE_BLACK; // 현재 부모와 현재 삼촌 컬러 블랙으로
        curr -> parent -> parent -> color = RBTREE_RED; // 그리고 현재 조상 컬러를 레드로,
        curr = curr -> parent -> parent; // 다시 반복
      }
      else {
        if (curr == curr -> parent -> left) { // 현재 노드가 현재 부모 노드의 왼쪽 노드라면,
          curr = curr -> parent; // 현재 노드 변환?갱신?
          right_rotate(t, curr); // right rotate
        }
        curr -> parent -> color = RBTREE_BLACK; // 현재 부모 컬러 블랙으로
        curr -> parent -> parent -> color = RBTREE_RED; // 현재 조상 컬러 레드로
        left_rotate(t, curr -> parent -> parent); // left roate
      }
    }
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t * z = (node_t *)calloc(1, sizeof(node_t)); // 새로운 노드 생성
  node_t * y = t -> nil; // 임시 부모
  node_t * x = t -> root; // 임시 자식

  // 새 노드를 삽입할 수 있는 위치를 찾는다.
  while (x != t -> nil) {
    y = x;
    if (key < x -> key) {
      x = x -> left;
    }
    else {
      x = x -> right;
    }
  }

  z -> parent = y; // 새로운 노드를 y에 연결

  if (y == t -> nil) {
    t -> root = z;
  }
  else if (key < y -> key) {
    y -> left = z;
  }
  else {
    y -> right = z;
  }

  z -> key = key; // 새로운 노드 값 갱신
  z -> color = RBTREE_RED;
  // z.left, z.right t.nil
  z -> left = z -> right = t -> nil;
  // z를  적색으로 칠함으로, 특성 위반될 수 있으므로 fixup 진행
  _rb_insert_fixup(t, z);
  t -> root -> color = RBTREE_BLACK;

  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  if (t -> root == t -> nil) {
      return NULL;
  }
  node_t * curr = t -> root;
  while (curr != t -> nil) {
    if (key < curr -> key) {
        curr = curr -> left;
    }
    else if (key > curr -> key) {
        curr = curr -> right;
    }
    else {
        return curr;
    }
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  if (t -> root == t -> nil) {
    return NULL;
  }
  node_t * curr = t -> root;
  while (curr -> left != t -> nil) {
    curr = curr -> left;
  }
  return curr;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if (t -> root == t -> nil) {
    return NULL;
  }
  node_t * curr = t -> root;
  while (curr -> right != t -> nil) {
    curr = curr -> right;
  }
  return curr;
}

// rbtree의 특성 유지 노드의 위치를 이동시키는 함수
void _rb_transplant(rbtree *t, node_t *u, node_t *v){
  if (u -> parent == t -> nil) {
    t -> root = v;
  }
  else if (u == u -> parent -> left) {
    u -> parent -> left = v;
  }
  else {
    u -> parent -> right = v;
  }
  v -> parent = u -> parent;
}

void _rb_delete_fixup(rbtree * t, node_t *x){
  // x가 이중 흑색일 경우
  while (x != t -> root && x -> color == RBTREE_BLACK) {

    if (x == x -> parent -> left) { // x가 x의 부모의 왼쪽이면,
      node_t * w = x -> parent -> right;
      if (w -> color == RBTREE_RED) { // w가 레드면,
        // case 1
        w -> color = RBTREE_BLACK; // w를 블랙으로 변환
        x -> parent -> color = RBTREE_RED; // x의 부모 컬러를 레드로 변환 후,
        left_rotate(t, x -> parent); // left rotate
        w = x -> parent -> right; // w는 x의 부모의 오른쪽 노드
      }
      if (w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK) { // w의 모든 자식들이 black인 경우
        // case 2
        w -> color = RBTREE_RED; // w의 컬러를 레드로 변환
        x = x -> parent;
      }
      else {
        // case 3
        // w 컬러가 블랙이면서, w왼쪽 자식 컬러는 레드, 오른쪽 자식은 블랙인 경우
        if (w -> right -> color == RBTREE_BLACK) {
          w -> left -> color = RBTREE_BLACK;
          w -> color = RBTREE_RED;
          right_rotate(t, w);
          w = x -> parent -> right;
        }
        // case 4
        // w 컬러가 블랙이면서 w의 오른쪽 자식 컬러만 레드로 확정된 경우, (왼쪽 자식은 ?)
        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> right -> color = RBTREE_BLACK;
        left_rotate(t, x -> parent);
        x = t -> root;
      }
    }
    // x가 x의 부모의 오른쪽이면, 위와 반대
    else {
      node_t * w = x -> parent -> left;
      if (w -> color == RBTREE_RED){
        w -> color = RBTREE_BLACK;
        x -> parent -> color = RBTREE_RED;
        right_rotate(t, x -> parent);
        w = x -> parent -> left;
      }
      if (w -> right -> color == RBTREE_BLACK && w -> left -> color == RBTREE_BLACK) {
        w -> color = RBTREE_RED;
        x = x -> parent;
      }
      else{
        if (w -> left -> color == RBTREE_BLACK){
          w -> right -> color = RBTREE_BLACK;
          w -> color = RBTREE_RED;
          left_rotate(t, w);
          w = x -> parent -> left;
        }
        w -> color = x -> parent -> color;
        x -> parent -> color = RBTREE_BLACK;
        w -> left -> color = RBTREE_BLACK;
        right_rotate(t, x -> parent);
        x = t -> root;
      }
    }
  }
    x -> color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  if (z == NULL) {
    return 0;
  }
  node_t * y = z; // 삭제되거나 이동될 노드
  node_t * x; // y의 원래 위치(삭제 or 이동되기 전 위치)
  color_t y_original = y -> color; // y의 원래 색

  // case 1
  // 노드에 왼쪽 자식이 없으면, 오른쪽 자식으로 바꾼다.
  if(z -> left == t -> nil) {
    x = z -> right; //
    _rb_transplant(t, z, z -> right);
  }
  // case 2
  // 노드에 오른쪽 자식이 없으면, 왼쪽 자식으로 바꾼다.
  else if(z -> right == t -> nil) {
    x = z -> left;
    _rb_transplant(t, z, z -> left);
  }
  // case 3
  else {
    y = z -> right;
    while (y -> left != t -> nil) {
      y = y -> left;
    }
    y_original = y -> color;
    x = y -> right;
    if (y -> parent == z) {
      x -> parent = y;
    }
    else {
      _rb_transplant(t, y, y -> right);
      y -> right = z -> right;
      y -> right -> parent = y;
    }
    _rb_transplant(t, z, y);
    y -> left = z -> left;
    y -> left -> parent = y;
    y -> color = z -> color;
  }
  if (y_original == RBTREE_BLACK) {
    _rb_delete_fixup(t, x);
  }
  free(z);
  return 0;
}

void _rbtree_to_array(const rbtree *t, node_t * curr, key_t *arr, size_t *pcnt, const size_t n) {
    if(curr == t -> nil){
        return;
    }
    _rbtree_to_array(t, curr -> left, arr, pcnt, n);
    if( *pcnt < n ) {
        arr[(*pcnt)++] = curr -> key;
    }
    else{
        return;
    }
    _rbtree_to_array(t, curr -> right, arr, pcnt, n);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // TODO: implement to_array
    if (t -> root == t -> nil) {
        return 0;
    }
    else{
        size_t cnt = 0;
        _rbtree_to_array(t, t -> root, arr, &cnt, n);
    }
    return 0;
}