/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

//xcesko00 Simona Ceskova 22.10.2021

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error()
{
    printf("*ERROR* The program has performed an illegal operation.\n");
    error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init(DLList *list)
{
    //inicializovany prazdny seznam nema zadny prvek a tak firstElement ukazuje na NULL
    list->firstElement = list->lastElement = list->activeElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose(DLList *list)
{
    //pokud je seznam prazdny, tak nema zadne prvky co by zrusil
    if ((list->firstElement == NULL) && (list->lastElement == NULL))
        return;

    //definovani pomocne promenne
    DLLElementPtr tempElement = list->firstElement;
    //ruseni prvku postupne od prvniho, prazdny seznam ma prvni (a i posledni) prvel s hodnotou NULL
    while ((list->firstElement != NULL))
    {
        //posunu prvni prvek na dalsi a puvodni prvni uvolnim v pameti
        list->firstElement = list->firstElement->nextElement;
        free(tempElement);
        //pomocnou promennou posunu na aktualni prvni prvek
        tempElement = list->firstElement;
    }

    //stav po inicializaci
    list->firstElement = NULL;
    list->lastElement = NULL;
    list->activeElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst(DLList *list, int data)
{
    //novy vkladany prvek
    DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement));

    //v pripade nedostatku pameti
    if (newElement == NULL)
    {
        DLL_Error();
        return;
    }

    //predam noveho prvku data
    newElement->data = data;
    //tim, ze bude novy firstElement, tak bude ukazovat na puvodni
    newElement->nextElement = list->firstElement;
    //firstElement->previousElement vzdy ukazuje na NULL
    newElement->previousElement = NULL;

    //v pripade, ze seznam je prazdny
    if (list->firstElement == NULL)
    {
        list->lastElement = newElement;
    }
    else
    {
        //puvodni firstElement musi nazpet odkazovat na novy firstElement
        list->firstElement->previousElement = newElement;
    }
    //oficialne novy prvek dostane vlastnost firstElement
    list->firstElement = newElement;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast(DLList *list, int data)
{
    //novy vkladany prvek
    DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement));

    //v pripade nedostatku pameti
    if (newElement == NULL)
    {
        DLL_Error();
        return;
    }

    //predam data noveho prvku
    newElement->data = data;
    //tim, ze bude novy lastElement, tak bude ukazovat na puvodni
    newElement->previousElement = list->lastElement;
    //lastElement->nextElement vzdy ukazuje na NULL
    newElement->nextElement = NULL;

    //v pripade, ze seznam je prazdny
    if (list->lastElement == NULL)
    {
        list->firstElement = newElement;
    }
    else
    {
        //puvodni lastElement musi nazpet odkazovat na novy lastElement
        list->lastElement->nextElement = newElement;
    }
    //priradim novemu prvku, ze je lastElement
    list->lastElement = newElement;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First(DLList *list)
{
    //activni prvek bude mit stejne parametry jako prvni element seznamu
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last(DLList *list)
{
    //activni prvek bude mit stejne parametry jako posledni element seznamu
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst(DLList *list, int *dataPtr)
{
    //v pripade, ze je seznam prazdny
    if (list->firstElement == NULL)
        DLL_Error();
    else
        *dataPtr = list->firstElement->data;
    //vratim data prvniho prvku
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast(DLList *list, int *dataPtr)
{
    //v pripade, ze je seznam prazdny
    if (list->firstElement == NULL)
        DLL_Error();
    else
        *dataPtr = list->lastElement->data;
    //vratim data posledniho prvku
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst(DLList *list)
{
    //v pripade, ze je seznam prazdny
    if ((list->lastElement == NULL) || (list->firstElement == NULL))
        return;
    else
    {

        //pokud je to jednoprvkovy seznam, tak se smaze smazanim prvniho prvku cely a dojde do stavu po inicializaci
        if (list->lastElement == list->firstElement)
        {
            free(list->lastElement);
            //stav po inicializaci
            list->lastElement = NULL;
            list->activeElement = NULL;
            list->firstElement = NULL;
        }
        else
        { 
            //pokud je aktivni prvek zaroven firstElement, tak se aktivni prvek zrusi
            if (list->activeElement == list->firstElement)
                list->activeElement = NULL;
            //pomocna promenna, do ktere ulozim 2. prvek v poradi seznamu
            DLLElementPtr tempElement = list->firstElement->nextElement;
            free(list->firstElement);
            //tempElement je novy firstElement
            tempElement->previousElement = NULL;
            list->firstElement = tempElement;
        }
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast(DLList *list)
{
    //v pripade, ze je seznam prazdny
    if ((list->firstElement == NULL) || (list->lastElement == NULL))
        return;
    else
    {
        //pokud je to jednoprvkovy seznam, tak se smaze smazanim posledniho prvku cely a dojde do stavu po inicializaci
        if (list->firstElement == list->lastElement)
        {
            free(list->firstElement);
            //stav po inicializaci
            list->firstElement = NULL;
            list->lastElement = NULL;
            list->activeElement = NULL;
        }
        else
        {
            //pokud je aktivni prvek zaroven lastElement, tak se aktivni prvek zrusi
            if (list->activeElement == list->lastElement)
                list->activeElement = NULL;
            //pomocna promenna, do ktere ulozim 2. prvek v poradi seznamu od konce
            DLLElementPtr tempElement = list->lastElement->previousElement;
            free(list->lastElement);
            //tempElement je novy lastElement
            tempElement->nextElement = NULL;
            list->lastElement = tempElement;
        }
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter(DLList *list)
{
    //v pripade, ze neni aktivni prvek inicializovany a nebo je posledni prvek
    if ((list->activeElement == NULL) || (list->activeElement == list->lastElement))
        return;
    else if (list->activeElement->nextElement == list->lastElement)
    {
        //pokud je prvek za aktiv. prvkem posledni, tak se aktivni prvek stane zaroven i lastElement
        //smazeme prvek za aktivnim prvkem
        free(list->lastElement);
        list->activeElement->nextElement = NULL;
        list->lastElement = list->activeElement;
    }
    else
    {
        //pomocny prvek, aby se ukazatel nextElement aktivniho prvku spojil s prvkem po smazanem prvku
        DLLElementPtr tempElement = list->activeElement->nextElement->nextElement;
        free(list->activeElement->nextElement);
        //pospojovani jejich ukazatelu na prvky
        tempElement->previousElement = list->activeElement;
        list->activeElement->nextElement = tempElement;
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore(DLList *list)
{
    //v pripade, ze neni aktivni prvek inicializovany a nebo je prvni prvek
    if ((list->activeElement == NULL) || (list->activeElement == list->firstElement))
        return;
    else if (list->activeElement->previousElement == list->firstElement)
    {
        //pokud je prvek pred aktiv. prvkem prvni, tak se aktivni prvek stane zaroven i firstElement
        //smazeme prvek pred aktivnim prvkem
        free(list->firstElement);
        list->activeElement->previousElement = NULL;
        list->firstElement = list->activeElement;
    }
    else
    {
        //pomocny prvek, aby se ukazatel previousElement aktivniho prvku spojil s prvkem po smazanem prvku
        DLLElementPtr tempElement = list->activeElement->previousElement->previousElement;
        free(list->activeElement->previousElement);
        //pospojovani jejich ukazatelu na prvky
        tempElement->nextElement = list->activeElement;
        list->activeElement->previousElement = tempElement;
    }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter(DLList *list, int data)
{
    //v pripade, ze neni aktivni prvek
    if (list->activeElement == NULL)
        return;
    else
    {
        //novy vkladany prvek
        DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement));
        //pri nedostatku pameti
        if (newElement == NULL)
        {
            DLL_Error();
            return;
        }
        else if (list->activeElement == list->lastElement)
        {
            //v pripade, ze je aktivni prvek zaroven posledni bude ukazovat na novy
            list->activeElement->nextElement = newElement;
            //pridani novemu prvku data a odkaz na aktivni predchozi element
            newElement->previousElement = list->activeElement;
            newElement->data = data;
            //posledni prvek vzdy ukazuje na NULL
            newElement->nextElement = NULL;
            list->lastElement = newElement;
        }
        else
        {
            //postupne prirazovani spravnych ukazatelu
            //  ACTIVE NEW -> NEXT
            newElement->nextElement = list->activeElement->nextElement;
            //  ACTIVE NEW <--> NEXT
            list->activeElement->nextElement->previousElement = newElement;
            //  ACTIVE -> NEW <--> NEXT
            list->activeElement->nextElement = newElement;
            //  ACTIVE <--> NEW <--> NEXT
            newElement->previousElement = list->activeElement;
            //prirazeni dat novemu elementu
            newElement->data = data;
        }
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore(DLList *list, int data)
{
    //v pripade, ze neni aktivni prvek inicializovany
    if (list->activeElement == NULL)
        return;
    else
    {
        //novy vkladany prvek
        DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement));
        //pri nedostatku pameti
        if (newElement == NULL)
        {
            DLL_Error();
            return;
        }
        else if (list->activeElement == list->firstElement)
        {
            //v pripade, ze je aktivni prvek zaroven prvni bude ukazovat na novy
            list->activeElement->previousElement = newElement;
            //pridani novemu prvku data a odkaz na aktivni dalsi element
            newElement->nextElement = list->activeElement;
            newElement->data = data;
            //prvni prvek vzdy ukazuje na NULL
            newElement->previousElement = NULL;
            list->firstElement = newElement;
        }
        else
        {
            //postupne prirazovani spravnych ukazatelu
            //  PREVIOUS <- NEW  ACTIVE
            newElement->previousElement = list->activeElement->previousElement;
            //  PREVIOUS <--> NEW  ACTIVE
            list->activeElement->previousElement->nextElement = newElement;
            //  PREVIOUS <--> NEW <-- ACTIVE
            list->activeElement->previousElement = newElement;
            //  PREVIOUS <--> NEW <--> ACTIVE
            newElement->nextElement = list->activeElement;
            //prirazeni dat novemu elementu
            newElement->data = data;
        }
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue(DLList *list, int *dataPtr)
{
    //prikad, kdy list neni aktivni
    if (list->activeElement == NULL)
    {
        DLL_Error();
        return;
    }
    //predani dat
    *dataPtr = list->activeElement->data;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue(DLList *list, int data)
{
    //pripad,ze seznam neni aktivni
    if (list->activeElement == NULL)
        return;
    else
    {
        //prepsani dat
        list->activeElement->data = data;
    }
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next(DLList *list)
{
    //pripad, ze list neni aktivni
    if (list->activeElement == NULL)
        return;
    else if (list->activeElement == list->lastElement)
    {
        //pokud je aktivni prvek zaroven posledni, tak se aktivita ztraci
        list->activeElement = NULL;
    }
    else
        //DLLElementPtr tempElement = list->activeElement->nextElement;
    {
        //predani aktivity
        list->activeElement = list->activeElement->nextElement;
    }
}

/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous(DLList *list)
{
    //pokud je seznam neaktivni
    if (list->activeElement == NULL)
        return;
    else if (list->activeElement == list->firstElement)
    {
        //pokud je aktivni prvek zaroven prvni, tak se aktivita ztraci
        list->activeElement = NULL;
    }
    else
    {
        //predani aktivity
        list->activeElement = list->activeElement->previousElement;
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive(DLList *list)
{
    //je sezanam aktivni?: return TRUE, je neaktivni?: return FALSE
    return ((list->activeElement != NULL) ? TRUE : FALSE);
}

/* Konec c206.c */
