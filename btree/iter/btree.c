/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    (*tree) = NULL;                     // pradzny strom
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    while (tree != NULL){       // pokym sa nedostanem na koniec stromu
        if (tree->key == key){      // nasiel som
            *value = (*tree).value;
            return true;
        }
        else if (key > tree->key){          // pokracuj v pravom podstrome
            tree = tree->right;
        }
        else if (key < tree->key){          // pokracuj v lavom podstrome
            tree = tree->left;
        }
    }
  return false;             // nenasiel som
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if (*tree == NULL){     // prazdny strom, vloz polozku
        bst_node_t *tmp = malloc(sizeof(bst_node_t));
        tmp->key = key;
        tmp->value = value;
        tmp->right = NULL;
        tmp->left = NULL;
        (*tree) = tmp;          // vytvor koren
        return;
    }

    bst_node_t *father = (*tree);
    bst_node_t *son = (*tree);
    do {
        if (father->key == key){        // hladany prvok existuje, aktualizuj hodnotu
            father->value = value;
            return;
        }
        father = son;               // posun otca
        if (key < father->key){         // chod do laveho podstromu
            son = father->left;
        }
        else if (key > father->key){        // chod do praveho podstromu
            son = father->right;
        }
    } while (son != NULL);               // otec nema dalsieho spravneho potomka, vytvorime noveho

    bst_node_t *tmp = malloc(sizeof(bst_node_t));
    tmp->key = key;
    tmp->value = value;
    tmp->right = NULL;
    tmp->left = NULL;
    if (key < father->key){         // novy prvok prirad ako laveho potomka
        father->left = tmp;
    }
    else {                          // novy prvok prirad ako praveho potomka
        father->right = tmp;
    }
    return;
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if (*tree == NULL){
        return;
    }

    if ((*tree)->right == NULL){
        target->key = (*tree)->key;
        target->value = (*tree)->value;
        //target->left = (*tree)->left;
        //target->right = (*tree)->right;
        free(*tree);
        *tree = NULL;
        return;
    }

    bst_node_t* g_father = (*tree);
    bst_node_t *father = g_father->right;
    while (father->right != NULL){
        g_father = father;
        father = father->right;
    }
    g_father->right = father->left;
    target->key = father->key;
    target->value = father->value;
    free(father);
    father = NULL;
    return;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (*tree == NULL){
        return;
    }

    bst_node_t* g_father = (*tree);
    bst_node_t* father = NULL;
    if (g_father->key == key){ //todo ak je lavy podstrom null?
        if (g_father->left == NULL){
            (*tree) = g_father->right;
            free(g_father);
        }
        else if (g_father->right == NULL){
            (*tree) = g_father->left;
            free(g_father);
        }
        else {
            bst_replace_by_rightmost(g_father, &g_father->left);
        }
        return;
    }
    else if (g_father->key < key){
        father = g_father->right;
    }
    else {
        father = g_father->left;
    }

    while(father != NULL){
        if (father->key < key){
            g_father = father;
            father = father->right;
        }
        else if (father->key > key){
            g_father = father;
            father = father->left;
        }
        else if (father->left != NULL && father->right != NULL){
            bst_replace_by_rightmost(father, &father->left);
            return;
        }
        else if (father->left == NULL && father->right != NULL){
                if (father == g_father->right){
                    g_father->right = father->right;
                }
                else {
                    g_father->left = father->right;
                }
                free(father);
                return;
            }
        else {
            if (father == g_father->right){
                g_father->right = father->left;
            }
            else {
                g_father->left = father->left;
            }
            free(father);
            return;
        }
        }
    }


/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    stack_bst_init(stack);
    bst_node_t *tmp = NULL;
    do {
        if (*tree == NULL){
            if (!stack_bst_empty(stack)){
                *tree = stack_bst_pop(stack);
            }
        }
        else {
            if ((*tree)->right != NULL){
                stack_bst_push(stack, (*tree)->right);
            }
            tmp = *tree;
            (*tree) = (*tree)->left;
            free(tmp);
        }
    } while((*tree) != NULL || !stack_bst_empty(stack));
    free(stack);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree != NULL){
        bst_print_node(tree);
        if (to_visit != NULL){
            stack_bst_push(to_visit, tree);
        }
        tree = tree->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    stack_bst_t *to_visit = malloc(sizeof (stack_bst_t));
    if (to_visit == NULL){
        return;
    }
    stack_bst_init(to_visit);

    do {
        bst_leftmost_preorder(tree, to_visit);
        tree = stack_bst_pop(to_visit);
        tree = tree->right;
    } while (!stack_bst_empty(to_visit) || tree != NULL);

    free(to_visit);
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree != NULL){
        if (to_visit != NULL){
            stack_bst_push(to_visit, tree);
        }
        tree = tree->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    stack_bst_t *to_visit = malloc(sizeof (stack_bst_t));
    if (to_visit == NULL){
        return;
    }
    stack_bst_init(to_visit);

    do {
        bst_leftmost_inorder(tree, to_visit);
        tree = stack_bst_pop(to_visit);
        bst_print_node(tree);
        tree = tree->right;
    } while (!stack_bst_empty(to_visit) || tree != NULL);

    free(to_visit);
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    while (tree != NULL){
        if (to_visit != NULL){
            stack_bst_push(to_visit, tree);
        }
        if (first_visit != NULL){
            stack_bool_push(first_visit, true);
        }
        tree = tree->left;
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    stack_bst_t *to_visit = malloc(sizeof (stack_bst_t));
    if (to_visit == NULL){
        return;
    }
    stack_bst_init(to_visit);

    stack_bool_t *bool_stack = malloc(sizeof (stack_bool_t));
    if (bool_stack == NULL){
        return;
    }
    stack_bool_init(bool_stack);
    bool from_left = false;
    bst_leftmost_postorder(tree, to_visit, bool_stack);

    while (!stack_bst_empty(to_visit)) {
        tree = stack_bst_top(to_visit);
        from_left = stack_bool_pop(bool_stack);
        if (from_left){
            stack_bool_push(bool_stack, false);
            tree = tree->right;
            bst_leftmost_postorder(tree, to_visit, bool_stack);
        }
        else {
            stack_bst_pop(to_visit);
            bst_print_node(tree);
        }

    }

    free(to_visit);
    free(bool_stack);
}
