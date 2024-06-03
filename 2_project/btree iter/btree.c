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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value)
{
  bool search = false;
  bool loop; //pro pokracovani v while loopu
  if (tree == NULL)
    loop = true;
  else //v prazdnum stormu neni co hledat
    loop = false;
  while (!loop)
  {
    if (tree->key == key) //nasel se uzel
    {
      loop = true;
      search = true;
      *value = tree->value; //vraci se hodnota
    }
    else
    {
      //hledani dal v stomu podle velikosti key
      if (key < tree->key)
        tree = tree->left;
      else
        tree = tree->right;
      if (tree == NULL) //uz neni co prohledavat ve stomu, takze se uzel nenasel
        loop = true;
    }
  }
  return search;
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
void bst_insert(bst_node_t **tree, char key, int value)
{
  //v pripade, ze je strom jeste prazdny, tak se vytvori jeho root
  if (*tree == NULL)
  {
    bst_node_t *new = (bst_node_t *)malloc(sizeof(bst_node_t));
    if (new == NULL)
      return;
    new->left = NULL;
    new->right = NULL;
    new->key = key;
    new->value = value;
    *tree = new;
    return;
  }
  else //ve stromu uz neco je
  {
    bst_node_t *temp = *tree;
    bst_node_t *prev = NULL;
    bool prevLeft;
    while (true)
    {
      if (temp == NULL) //dojiti do pradnyho listu, kde se vztvori novy uzel
      {
        bst_node_t *new = (bst_node_t *)malloc(sizeof(bst_node_t));
        if (new == NULL)
          return;
        new->left = NULL;
        new->right = NULL;
        new->key = key;
        new->value = value;
        if (prevLeft == true) //napojeni na predesly uzel
          prev->left = new;   //novy uzel je levy syn predesleho
        else
          prev->right = new; //novy uzel je pravy syn predesleho
        return;
      }
      else if (temp->key == key) //uzel s danym key uz existuje
      {
        temp->value = value; //prepisu hodnoty
        return;
      }
      else if (temp->key < key) //hledani spravneho mista v pravem podstromu
      {
        prevLeft = false; //doslo se sem zprava
        prev = temp;
        temp = temp->right;
      }
      else if (temp->key > key) //hledani spravneho mista v levem podstromu
      {
        prev = temp;
        prevLeft = true; //doslo se sem zleva
        temp = temp->left;
      }
    }
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  if (target == NULL) //chybny vstup
    return;
  else
  {
    bool loop = true;
    bst_node_t *temp = (*tree);
    bst_node_t *prev = NULL;
    while (loop)
    {
      if (temp->right == NULL)
      {
        //prepsani hodnoty a key
        target->key = temp->key;
        target->value = temp->value;
        //napojeni podstrom nejpravejsiho uzlu z leve strany
        if(prev == NULL)
          *tree = temp->right;
        else
          prev->right = temp->left;
        free(temp);
        loop = false;
      }
      else
      {
        //posouni se dal v podstome
        prev = temp;
        temp = temp->right;
      }
    }
    return;
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
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key)
{
  //z prazdneho stromu neni co mazat
  if ((*tree == NULL))
    return;
  bst_node_t *temp2 = (*tree);
  bst_node_t *prev = NULL;
  //hledani uzlu
  while (true)
  {
    //nalezen uzel, hledani skoncilo
    if (temp2->key == key)
      break;
    prev = temp2;
    //v pripade ze hledam hodnotu ktera ve stome neni
    if ((temp2->left == NULL) && (temp2->right == NULL))
      return;
      //posouvani se do podstromu
    if (temp2->key > key)
      temp2 = temp2->left;
    else if (temp2->key < key)
      temp2 = temp2->right;
  }
  //pokud nema uzel ani jeden potomek
  if ((temp2->left == NULL) && (temp2->right == NULL))
  {
    //pro pripad, ze je zaroven korenem
    if (prev == NULL)
    {
      free(temp2);
      bst_init(&(*tree));
      return;
    }
    //odkud je jeho otec a ktery jeho ukazatel bude pak ukazovat na NULL
    if (prev->right == temp2)
      prev->right = NULL;
    else
      prev->left = NULL;
    free(temp2);
  }
  else if (temp2->left == NULL) //pokud ma pouze praveho potomka
  {
    //pokud je zaroven korenem
    if (prev != NULL)
    {
      //odkud je jeho otec a ktery jeho ukazatel bude pak ukazovat na potomka smazaneho uzlu
      if (prev->left == temp2)
        prev->left = temp2->right;
      else
        prev->right = temp2->right;
    }else
      *tree = temp2->right;
    free(temp2);
  }
  else if (temp2->right == NULL) //pokud ma pouze leveho potomka
  {
    //pokud je zaroven korenem
    if (prev != NULL)
    {
      //odkud je jeho otec a ktery jeho ukazatel bude pak ukazovat na potomka smazaneho uzlu
      if (prev->right == temp2)
        prev->right = temp2->left;
      else
        prev->left = temp2->left;
    }else
      *tree = temp2->left;
    free(temp2);
  }
  else
  {
    //pripad ze ma uzel oba dva potomky zleva i zprava
    bst_replace_by_rightmost((temp2), &(temp2)->left);
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
void bst_dispose(bst_node_t **tree)
{
  //y prazdneho stromu neni co mazat
  if((*tree) == NULL)
    return;
  //inicializace stacku
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_node_t *temp = (*tree);
  stack_bst_push(&stack, *tree);
  //dokud neni stack vyprazdneny
  for( ;!stack_bst_empty(&stack); )
  {
    //pop aby se postupne zasobnik vyprazdnil -> podminka loopu
    temp = stack_bst_pop(&stack);
    if(temp != NULL)
    {
      //zanorovani se do podstormu
      stack_bst_push(&stack, temp->left);
      stack_bst_push(&stack, temp->right);
      free(temp);
    }
  }
  //inicializace stromu do puvodniho stavu
  bst_init(tree);
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
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    bst_print_node(tree);
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
void bst_preorder(bst_node_t *tree)
{
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_preorder(tree, &stack);
  while (!(stack_bst_empty(&stack)))
  {
    tree = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_leftmost_preorder(tree->right, &stack);
  }
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
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree);
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
void bst_inorder(bst_node_t *tree)
{
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_inorder(tree, &stack);
  while (!(stack_bst_empty(&stack)))
  {
    tree = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right, &stack);
  }
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
                            stack_bool_t *first_visit)
{
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
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
void bst_postorder(bst_node_t *tree)
{
  bool firstLeft;
  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bool_t stackBool;
  stack_bool_init(&stackBool);
  bst_leftmost_postorder(tree, &stack, &stackBool);
  while (!(stack_bst_empty(&stack)))
  {
    tree = stack_bst_top(&stack);
    firstLeft = stack_bool_top(&stackBool);
    stack_bool_pop(&stackBool);
    if (firstLeft)
    {
      stack_bool_push(&stackBool, false);
      bst_leftmost_postorder(tree->right, &stack, &stackBool);
    }
    else
    {
      stack_bst_pop(&stack);
      bst_print_node(tree);
    }
  }
}
