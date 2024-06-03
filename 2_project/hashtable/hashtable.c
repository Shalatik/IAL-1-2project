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

//Simona Ceskova xcesko00 14.11.

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key)
{
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++)
  {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table)
{
  //inicializace vsech pozic do maxima moznych indexu
  for (int i = 0; i < MAX_HT_SIZE; i++)
    (*table)[i] = NULL;
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key)
{
  ht_item_t *temp = (*table)[get_hash(key)]; //najdu tabulku s indexem
  while (temp != NULL)
  {
    if (temp->key == key)
      return temp; //ukazatel na najity prvek
    else
      temp = temp->next; //prochazim prvky a hledam dany prvek
  }
  return NULL; //hledani se nezdarilo
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value)
{
  if (ht_search(table, key) != NULL) //prvek uz v tabulce je -> prepise se jeho hodnota
  {
    ht_item_t *temp;
    temp = ht_search(table, key);
    temp->value = value;
  }
  else //vkladany prvek v tabulce jeste neni
  {
    if (table[get_hash(key)] != NULL) //kontrola ze je jeste misto na novy prvek
    {
      //zalozim novy prvek a dam mu jeho hodnotu a klic
      ht_item_t *new = (ht_item_t *)malloc(sizeof(ht_item_t));
      if (new == NULL)
        return;
      new->value = value;
      new->key = key;
      new->next = (*table)[get_hash(key)];
      (*table)[get_hash(key)] = new;
    }
  }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key)
{
  if (ht_search(table, key) != NULL) //prvek se nasel
  {
    ht_item_t *temp = (*table)[get_hash(key)];
    while (temp != NULL)
    {
      if (temp->key == key) //prvek nalezen a vratim ukazatel na jeho hodnotu prvku
        return &(temp->value);
      else
        temp = temp->next; //hledam prvek s danym key
    }
  }
  return NULL; //prvek v tabulce neni
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key)
{
  ht_item_t *itemWhereDelete = (*table)[get_hash(key)];
  ht_item_t *temp;
  if ((*table)[get_hash(key)]) //pokud takovy prvek vubec existuje na smazani
  {
    while (itemWhereDelete->key != key) //dokud se nenasel prvek s hledanym klicem
    {
      temp = itemWhereDelete;
      itemWhereDelete = itemWhereDelete->next; //posunu se dal
      if(itemWhereDelete->key == key) //prvek nalezen
      {
        if(temp == NULL) //kdyz je temp null tak nemam jaky prvek mazat
          break;
        //prvky se musi spojit, aby dal na sebe navazovaly
        temp->next = itemWhereDelete->next;
        free(itemWhereDelete);
        return;
      }
    }
    //v pripade, ze se prvek nasel hled
    (*table)[get_hash(key)] = itemWhereDelete->next;
    free(itemWhereDelete);
  }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table)
{
  int j = 0;
  while (j < MAX_HT_SIZE) //prochazim vsechnz indexy v tabulce
  {
    ht_item_t *temp = (*table)[j];
    while ((*table)[j]) //kdyz index neni prazdny, tak uvolnim vsechny jeho prvky
    {
      temp = (*table)[j]->next;
      free((*table)[j]);
      (*table)[j] = temp;
    }
    j++;
  }
  //inicializace tabulky
  ht_init(table);
}
