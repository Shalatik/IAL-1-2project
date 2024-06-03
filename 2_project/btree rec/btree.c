/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

//Simona Ceskova xcesko00 14.11.

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree)
{
  *tree = NULL;
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
bool bst_search(bst_node_t *tree, char key, int *value)
{
  if (tree == NULL)
    return false;
  else if (tree->key == key) //prepsani uzlu, ktery uz s danym key existuje
  {
    *value = tree->value;
    return true; //uzel uspesne nalezen
  }
  else //uzel s key jeste neexistuje
  {
    if (key > tree->key) //dal v stromu doprava
      return bst_search(tree->right, key, value);
    else if (key < tree->key) //dal v stromu doleva
      return bst_search(tree->left, key, value);
  }
  return false; //uzel nebyl nalezen
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
void bst_insert(bst_node_t **tree, char key, int value)
{
  //pokud jeste neni zadny uzel ve stromu, tzv jeste neni ani root
  if (*tree == NULL)
  {
    bst_node_t *new = (bst_node_t *)malloc(sizeof(bst_node_t));
    if (new == NULL)
      return;
    new->left = NULL; //jeho syni budou zatim null
    new->right = NULL;
    new->key = key;
    new->value = value;
    *tree = new; //vlozim ho do struktury
    return;
  }
  else
  {
    if ((*tree)->key == key) //uz uzel s key existuje a tak se jen prepise jeho hodnota
    {
      (*tree)->value = value;
      return;
    }
    else if ((*tree)->key < key)
      bst_insert(&(*tree)->right, key, value);
    else if ((*tree)->key > key)
      bst_insert(&(*tree)->left, key, value);
    //dale postup stromem, podle hodnoty key
  }
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
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  if (target == NULL) //neplatny vstup
    return;
  else if ((*tree)->right == NULL) //pokud je potomek listem, tak to bude ten kterym se bude nahrazovat
  {
    target->key = (*tree)->key; //nahradim nejpravejsim potomkem
    target->value = (*tree)->value;
    bst_node_t *temp = *tree;
    *tree = (*tree)->left; //musim napojit podstrom nejpravejsiho uzlu z leve strany
    free(temp); //uvolnim potomek kterym se nahrazoval odstraneny uzel
    return;
  }
  else
  {
    bst_replace_by_rightmost(target, &(*tree)->right); //hledam dokud nenajdu potomka co je list
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
void bst_delete(bst_node_t **tree, char key)
{
  if ((*tree == NULL)) //z prazdneho stromu neni co smazat
    return;
  else
  {
    //hledani uzlu ktery se ma smazat
    if ((*tree)->key > key)
    {
      bst_delete(&(*tree)->left, key);
    }
    else if ((*tree)->key < key)
    {
      bst_delete(&(*tree)->right, key);
    }
    else
    {
      bst_node_t *temp = (*tree);
      //pokud uzel nema zadne potomky, neni nicim nahrazeny
      if (((*tree)->left == NULL) && ((*tree)->right == NULL))
      {
        bst_init(&(*tree)); //stane se z nej NULL
        free(temp);
        return;
      }
      else if ((*tree)->left == NULL)
      {
        //nahrazeny svym jedinnym synem zprava
        (*tree) = (*tree)->right;
        free(temp);
      }
      else if ((*tree)->right == NULL)
      {
        //nahrazeny svym jedinnym synem zleva
        (*tree) = (*tree)->left;
        free(temp);
      }
      else
      {
        //v priprade ze ma oba potomky
        bst_replace_by_rightmost((*tree), &(*tree)->left); 
      }
    }
  }
  return;
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
void bst_dispose(bst_node_t **tree)
{
  if (*tree != NULL)
  {
    //zanorim se do vsech podstromu a postupne mazu
    bst_dispose(&(*tree)->left);
    bst_dispose(&(*tree)->right);
    free(*tree);
    //inicializace
    bst_init(&(*tree));
  }
  else
    return;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree)
{
  //uzly se resi v poslouposti: root, root->left, root->right 
  if (tree != NULL)
  {
    bst_print_node(tree);
    bst_preorder(tree->left);
    bst_preorder(tree->right);
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree)
{
  //uzly se resi v poslouposti:  root->left, root, root->right 
  if (tree != NULL)
  {
    bst_inorder(tree->left);
    bst_print_node(tree);
    bst_inorder(tree->right);
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree)
{
  //uzly se resi v poslouposti:  root->left, root->right , root 
  if (tree != NULL)
  {
    bst_postorder(tree->left);
    bst_postorder(tree->right);
    bst_print_node(tree);
  }
}
