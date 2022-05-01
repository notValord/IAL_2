/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
    if (table != NULL){
        for (int i = 0; i < HT_SIZE; ++i) {
            (*table)[i] = NULL;                 // nastavi kazdy prvok pola na NULL
        }
    }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    if (table == NULL){
        return NULL;
    }

    int hash = get_hash(key);                   // ziskame index v poli
    for (ht_item_t* find = (*table)[hash]; find != NULL ; find = find->next) { // iterujeme cez zretazeny zoznam
        if (find->key == key){
            return find;                        // nasli sme prvok
        }
    }
    return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    if (table == NULL){
        return;
    }

    ht_item_t* look_up = ht_search(table, key);
    if (look_up != NULL){                   // prvok existuje
        look_up->value = value;             // nahradime jeho hodnotu
        return;
    }

    int hash = get_hash(key);
    ht_item_t* add = malloc( sizeof(ht_item_t));            // alokujeme novy prvok
    add->key = key;                     // nastavime jeho hodnoty
    add->value = value;
    add->next = (*table)[hash];
    (*table)[hash] = add;               // pridame ho do tabulky na zaciatok zoznamu

}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    ht_item_t *look_up = ht_search(table, key);
    if (look_up == NULL){
        return NULL;
    }
    return (float*) &(look_up->value);                  // vracia pointer na float
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    if (table == NULL){
        return;
    }

    int hash = get_hash(key);
    ht_item_t* first = (*table)[hash];
    if (first == NULL){                     // polozka neexistuje
        return;
    }

    if (first->key == key){                 // hladana polozka je prva polozka
        (*table)[hash] = first->next;       // zmenime prvy prvok tabulky
        free(first);
        return;
    }

    for (ht_item_t* after = first->next; after != NULL ; after = after->next) {     // iterujeme cez zoznam
        if (after->key == key){
            first->next = after->next;              // napojime predchadzjuci prvok na nasledujuci
            free(after);
            return;
        }

        first = after;
    }
    return;
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
    if (table == NULL){
        return;
    }

    ht_item_t *tmp = NULL;              // pomocna premenna
    for (int i = 0; i < HT_SIZE; ++i) {
        tmp = (*table)[i];

        while (tmp != NULL){            // pokym sa nepremaze cely zoznam
            ht_delete(table, tmp->key);         //vyuzitie pomocnej funkcie
            tmp = (*table)[i];              // zostavame na prvom prvku
        }
    }
    return;
}
