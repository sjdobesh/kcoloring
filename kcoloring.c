// ===================================//
//           K-COLOR TREES            //
// ===================================//
// Author: Samuel Dobesh              //
// Date: June 2021                    //
// Find a valid K-coloring of a tree  //
// and maximize the usage of 1 color  //
// in linear time.                    //
//====================================//======================================80

#include <stdio.h>
#include <stdlib.h>

// branching factor
#define M 3
// coloring factor
#define K 3
// nodes in test tree
#define V 10

// tree node
typedef struct node {
  // child ptrs
  struct node** children;
  int num_child;
  // subtree configurations
  int** configs;
  // coloring 1, 2 ,..., K
  int color;
  // label
  int label;
} node;

// Tree Utils //====================================

// print a tree structures data
void print_tree(node* root) {
  printf("Node :\n");
  printf("  Color: %d\n", root->color);
  printf("  Num Children: %d\n", root->num_child);
  for (int i = 0; i < K; i++) {
    printf("  Config %d :\n", i);
    for (int j = 0; j < K; j++)
      printf("  %d", root->configs[i][j]);
    printf("\n");
  }
  for (int i = 0; i < root->num_child; i++)
    print_tree(root->children[i]);
}

// print a tree structures data
void print_tree_colors(node* root) {
  printf("Node :%d\n", root->label);
  printf("  Color: %d\n", root->color);
  printf("  Children: %d\n", root->num_child);
  for (int i = 0; i < root->num_child; i++) {
    printf("  %d", root->children[i]->label);
  }
  printf("\n");
  for (int i = 0; i < root->num_child; i++)
    print_tree_colors(root->children[i]);
}

// make a valid test tree
node* get_test_tree() {
  node* arr[V];
  // malloc each node
  for (int i = 0; i < V; i++) {
    arr[i]          = malloc(sizeof(node));
    arr[i]->label   = i;
    arr[i]->configs = malloc(sizeof(int*) * K);
    // malloc each config
    for (int j = 0; j < K; j++) {
      arr[i]->configs[j] = malloc(sizeof(int) * K);
      for (int k = 0; k < K; k++)
        arr[i]->configs[j][k] = 0;
    }
    // malloc its child list
    if (i < 3) {
      arr[i]->children  = malloc(sizeof(node*) * M);
      arr[i]->num_child = M;
    } else {
      arr[i]->num_child = 0;
    }
  }
  // add hardcoded relations
  for (int i = 0; i < 3; i++) {
    arr[i]->children[0] = arr[(i*3)+1];
    arr[i]->children[1] = arr[(i*3)+2];
    arr[i]->children[2] = arr[(i*3)+3];
  }
  // return root
  return arr[0];
}

// free the tree bottom up
void destroy_tree(node* root) {
  // free children
  // free configs and then struct
  if (root->num_child != 0) {
    for (int i = 0; i < root->num_child; i++)
      destroy_tree(root->children[i]);
    free(root->children);
  }
  for (int i = 0; i < K; i++)
    free(root->configs[i]);
  free(root->configs);
  free(root);
}

// PHASE 1 //====================================

// find the best compatible config
int get_max_compatible(node** child, int exclude) {
  int max_c1  = -1;
  int max_idx = -1;
  for (int i = 0; i < K; i++)
    if ((i != exclude) &&
        (*child)->configs[i][0] > max_c1) {
      max_c1  = (*child)->configs[i][0];
      max_idx = i;
    }
  return max_idx;
}

// sum up optimal subtree configs
void config(node** root) {
  int max_compatible = 0; // defaults to index 0
  for (int i = 0; i < K; i++)
    (*root)->configs[i][i] = 1;
  // if not a leaf
  if ((*root)->num_child != 0)
    // for each config // (*root)->configs[i][]
    for (int i = 0; i < K; i++)
      // add up every childs max compatible value // (*root)->children[j]
      for (int j = 0; j < (*root)->num_child; j++) {
        // get best index to add
        max_compatible = get_max_compatible(&(*root)->children[j],i);
        // add each entry into this root config
        for (int k = 0; k < K; k++)
          (*root)->configs[i][k] += (*root)->children[j]
                                           ->configs[max_compatible][k];
      }
}

// post fix bottom up
// fill configs with optimal subtree
void post_fix(node** root) {
  // if not leaf, recurse
  if ((*root)->num_child != 0)
    for (int i = 0; i < (*root)->num_child; i++)
      post_fix(&((*root)->children[i]));
  // find optimal configurations
  for (int i = 0; i < (*root)->num_child; i++)
    config(root);
}

// PHASE 2 //====================================

// choose best compatible color configuration
int choose_best(node* root, int pcolor) {
  // track best so far
  int best_color_val = -1;
  int best_color_idx = -1;
  // get best config
  for (int i = 0; i < K; i++)
    if ((i != pcolor) && (root->configs[i][0] > best_color_val)) {
      best_color_val = root->configs[i][0];
      best_color_idx = i;
    }
  return best_color_idx;
}

// in fix top down
void in_fix(node** root, int pcolor) {
  // choose optimal
  (*root)->color = choose_best((*root), pcolor);
  // recursive calls
  if ((*root)->num_child != 0)
    for (int i = 0; i < (*root)->num_child; i++)
      in_fix(&((*root)->children[i]), (*root)->color);
}

// MAIN //====================================================================80
int main() {

  // Declared Variables
  printf("K = %d\n", K);
  printf("M = %d\n", M);
  printf("V = %d\n", V);

  printf("\nAcquiring test tree...\n");
  node* root = get_test_tree();
  printf("Post-fix...\n");
  post_fix(&root);
  printf("In-fix...\n");
  in_fix(&root,-1);
  printf("Completed.\n");
  print_tree_colors(root);
  destroy_tree(root);
}
