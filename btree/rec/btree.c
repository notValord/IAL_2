/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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
    (*tree) = NULL;                 // prazdny strom
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    if (tree == NULL){
        return false;                   // prazdny strom
    }
    if (tree->key == key){              // nasiel sa hladany prvok
        *value = tree->value;
        return true;
    }
    if (key < tree->key && tree->left != NULL){         // prehladavame lavy podstrom
         return bst_search(tree->left, key, value);
    }
    else if (key > tree->key && tree->right != NULL){       // prehldavame pravy podstrom
         return bst_search(tree->right, key, value);
    }
    else {                  // dostali sme sa do listu a nenasli sme hladany prvok
        return false;
    }
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if (*tree == NULL){         // strom je prazdny, vlozime koren
        bst_node_t *tmp = malloc(sizeof(bst_node_t));
        tmp->key = key;
        tmp->value = value;
        tmp->left = NULL;
        tmp->right = NULL;
        (*tree) = tmp;          // nastavime koren
        return;
    }
    if (key == (*tree)->key){       // prvok uz existuje, nastavime jeho hodnotu
        (*tree)->value = value;
        return;
    }
    if (key < (*tree)->key){            // ideme do laveho podstromu
        if ((*tree)->left == NULL){     // neexistuje dalsi lavy podstrom, vlozime prvok
            bst_node_t *tmp = malloc(sizeof(bst_node_t));
            tmp->key = key;
            tmp->value = value;
            tmp->left = (*tree)->left;
            tmp->right = NULL;
            (*tree)->left = tmp;
            return;
        }
        else{
            bst_insert(&(*tree)->left, key, value);     // pokracujeme nizsie do laveho podstromu
        }
    }
    else if (key > (*tree)->key){           // ideme do praveho podstromu
        if ((*tree)->right == NULL){        // neexistuje dalsi pravy podstrom, vlozime prvok
            bst_node_t *tmp = malloc(sizeof(bst_node_t));
            tmp->key = key;
            tmp->value = value;
            tmp->right = (*tree)->right;
            tmp->left = NULL;
            (*tree)->right = tmp;
            return;
        }
        else{
            bst_insert(&(*tree)->right, key, value);        // pokracujeme nozsie do praveho podstromu
        }
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if (*tree == NULL){     // prazdny podstrom
        return;
    }

    bst_node_t* father = (*tree);
    bst_node_t* l_grandson = father;
    if ((*tree)->right == NULL){        // neexisuje dalsi pravy potomok, nastavime hodnotu a uvolnime
        target->key = (*tree)->key;
        target->value = (*tree)->value;
        free((*tree));
        (*tree) = NULL;
        //todo ak ma tento element este lavy podstrom tak chyba
    }
    else {
        if (father->right != NULL && father->right->right == NULL){ //najpravejsi
            l_grandson = father->right->left;
        }
        bst_replace_by_rightmost(target, &(*tree)->right);      // pokracujeme nizsie do praveho podstromu
        if ( father != l_grandson){
            father->right = l_grandson;
        }
    }
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (*tree == NULL){
        return;
    }

    bst_node_t* father = *tree;
    bst_node_t* r_grandson = father;
    bst_node_t* l_grandson = father;
    if (father->key > key){
        if (father->left != NULL && father->left->key == key){ //rusime jeho syna
            r_grandson = father->left->right;
            l_grandson = father->left->left;
        }
        bst_delete(&father->left, key);
        if (r_grandson != NULL && l_grandson == NULL){
            father->left = r_grandson;
        }
        else if (l_grandson != NULL && r_grandson == NULL){
            father->left = l_grandson;
        }
        else if (l_grandson == NULL){
            father->left = NULL;
        }
        return;
    }
    else if (father->key < key){
        if (father->right != NULL && father->right->key == key){ //rusime jeho syna
            r_grandson = father->right->right;
            l_grandson = father->right->left;
        }
        bst_delete(&(*tree)->right, key);
        if (r_grandson != NULL && l_grandson == NULL){
            father->right = r_grandson;
        }
        else if (l_grandson != NULL && r_grandson == NULL){
            father->right = l_grandson;
        }
        else if (l_grandson == NULL){
            father->right = NULL;
        }
        
        return;
    }
    else { //nasli sme ruseny uzol
        if (father->left == NULL){
            *tree = father->right;
            free(father);
        }
        else if (father->right == NULL){
            *tree = father->left;
            free(father);
        }
        else { // ma dvoch synov
            bst_replace_by_rightmost(father, &(father->left));
        }
        return;
    }

}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    if (*tree != NULL){
        bst_dispose(&(*tree)->right);
        bst_dispose(&(*tree)->left);
        free(*tree);
        (*tree) = NULL;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    if (tree == NULL){      // neexistuje dalsi podstrom
        return;
    }
    bst_print_node(tree);       // vypis otca, laveho potomka, praveho potomka
    bst_preorder(tree->left);       // spracuj laveho syna
    bst_preorder(tree->right);      // spracuj praveho syna
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    if (tree == NULL){          // neexistuje dalsi podstrom
        return;
    }
    bst_inorder(tree->left);        // spracuj laveho syna
    bst_print_node(tree);           // vypis uzol, najlavejsi potomok, otec, najpravejsi potomok
    bst_inorder(tree->right);       // spracuj praveho syna
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    if (tree == NULL){          // neexistuje dalsi podstrom
        return;
    }
    bst_postorder(tree->left);          // spracuj laveho syna
    bst_postorder(tree->right);         // spracuj praveho syna
    bst_print_node(tree);           // vypis uzol, najlavejsi potomok, najpravejsi potomok, otec
}
