
/* ******************************* c202.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c202 - Zásobník znaků v poli                                        */
/*  Referenční implementace: Petr Přikryl, 1994                               */
/*  Vytvořil: Václav Topinka, září 2005                                       */
/*  Úpravy: Kamil Jeřábek, září 2019                                          */
/*          Daniel Dolejška, září 2021                                        */
/* ************************************************************************** */
/*
** Implementujte datový typ zásobník znaků ve statickém poli. ADT zásobník je
** reprezentován záznamem typu Stack. Statické pole 'array' je indexováno
** do maximální hodnoty STACK_SIZE. Položka záznamu 'topIndex' ukazuje na prvek
** na vrcholu zásobníku. Prázdnému zásobníku odpovídá hodnota topIndex == -1,
** zásobníku s jedním prvkem hodnota 0, atd. Přesnou definici typů neleznete
** v hlavičkovém souboru c202.h.
**
** Implementujte následující funkce:
**
**    Stack_Init .... inicializace zásobníku
**    Stack_IsEmpty ... test na prázdnost zásobníku
**    Stack_IsFull .... test na zaplněnost zásobníku
**    Stack_Top ..... přečte hodnotu z vrcholu zásobníku
**    Stack_Pop ..... odstraní prvek z vrcholu zásobníku
**    Stack_Push .... vloží prvku do zásobníku
**
** Své řešení účelně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

//xcesko00 Simona Ceskova 22.10.2021

#include "c202.h"

int STACK_SIZE = MAX_STACK;
int error_flag;
int solved;


/**
 * Vytiskne upozornění, že došlo k chybě při práci se zásobníkem.
 *
 * TUTO FUNKCI, PROSÍME, NEUPRAVUJTE!
 *
 * @param error_code Interní identifikátor chyby
 */
void Stack_Error(int error_code)
{
	static const char *SERR_STRINGS[MAX_SERR + 1] = {
		"Unknown error",
		"Stack error: INIT",
		"Stack error: PUSH",
		"Stack error: TOP"};

	if (error_code <= 0 || error_code > MAX_SERR)
		error_code = 0;
	printf("%s\n", SERR_STRINGS[error_code]);
	error_flag = 1;
}

/**
 * Provede inicializaci zásobníku - nastaví vrchol zásobníku.
 * Hodnoty ve statickém poli neupravujte - po inicializaci zásobníku
 * jsou nedefinované.
 *
 * V případě, že funkce dostane jako parametr stack == NULL,
 * volejte funkci Stack_Error(SERR_INIT).
 * U ostatních funkcí pro zjednodušení předpokládejte, že tato situace
 * nenastane.
 *
 * @param stack Ukazatel na strukturu zásobníku
 */
void Stack_Init(Stack *stack)
{
	if (stack == NULL)
		Stack_Error(SERR_INIT);
	else
		stack->topIndex = -1;
	// -1 protoze jeste neni zadny prvek v zasobniku a zasobnik je v tomto momente prazdny
}

/**
 * Vrací nenulovou hodnotu, je-li zásobník prázdný, jinak vrací hodnotu 0.
 * Funkci implementujte jako jediný příkaz.
 * Vyvarujte se zejména konstrukce typu "if ( cond ) b=true else b=false".
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 *
 * @returns Nenulovou hodnotu v případě, že je zásobník prázdný, jinak nulu
 */
int Stack_IsEmpty(const Stack *stack)
{
	return ((stack->topIndex == -1) ? TRUE : FALSE);
	//pokud je zasobnik prazdny, tak TRUE vraci 1, FALSE vraci 0
}

/**
 * Vrací nenulovou hodnotu, je-li zásobník plný, jinak vrací hodnotu 0.
 * Dejte si zde pozor na častou programátorskou chybu "o jedničku" a dobře se
 * zamyslete, kdy je zásobník plný, jestliže může obsahovat nejvýše STACK_SIZE
 * prkvů a první prvek je vložen na pozici 0.
 *
 * Funkci implementujte jako jediný příkaz.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 *
 * @returns Nenulovou hodnotu v případě, že je zásobník plný, jinak nulu
 */
int Stack_IsFull(const Stack *stack)
{
	return ((stack->topIndex == STACK_SIZE - 1) ? TRUE : FALSE);
	//tim ze prvni index je 0, tak STACK_SIZE - 1 je nejvyssi index na kterem muze byt posledni prvek zasobniku
}

/**
 * Vrací znak z vrcholu zásobníku prostřednictvím parametru dataPtr.
 * Tato operace ale prvek z vrcholu zásobníku neodstraňuje.
 * Volání operace Top při prázdném zásobníku je nekorektní a ošetřete ho voláním
 * procedury Stack_Error(SERR_TOP).
 *
 * Pro ověření, zda je zásobník prázdný, použijte dříve definovanou funkci
 * Stack_IsEmpty.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void Stack_Top(const Stack *stack, char *dataPtr)
{
	if (Stack_IsEmpty(stack) == FALSE)
	{
		*dataPtr = stack->array[stack->topIndex];
		//pokud neni zasobnik prazdny, tak vracim hodnotu na vrcholu diky strukture Stack
	}
	else
		Stack_Error(SERR_TOP);
}

/**
 * Odstraní prvek z vrcholu zásobníku. Pro ověření, zda je zásobník prázdný,
 * použijte dříve definovanou funkci Stack_IsEmpty.
 *
 * Vyvolání operace Pop při prázdném zásobníku je sice podezřelé a může
 * signalizovat chybu v algoritmu, ve kterém je zásobník použit, ale funkci
 * pro ošetření chyby zde nevolejte (můžeme zásobník ponechat prázdný).
 * Spíše než volání chyby by se zde hodilo vypsat varování, což však pro
 * jednoduchost neděláme.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 */
void Stack_Pop(Stack *stack)
{
	if (Stack_IsEmpty(stack) == FALSE)
	{
		stack->topIndex -= 1;
		//jestlize neni zasobnik prazdny, snizim hodnotu topIndexu o 1 a tim odstranim puvodni topIndex a vytvorim novy
	}
}

/**
 * Vloží znak na vrchol zásobníku. Pokus o vložení prvku do plného zásobníku
 * je nekorektní a ošetřete ho voláním procedury Stack_Error(SERR_PUSH).
 *
 * Pro ověření, zda je zásobník plný, použijte dříve definovanou
 * funkci Stack_IsFull.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param data Znak k vložení
 */
void Stack_Push(Stack *stack, char data)
{

	if (Stack_IsFull(stack) == TRUE)
	{
		Stack_Error(SERR_PUSH);
		//jestli je zasobnik plny, nelze do nej vlozit dalsi prvek
	}
	else
	{
		stack->array[stack->topIndex + 1] = data;
		stack->topIndex += 1;
		//nactu data do noveho prvku s indexem topIndex+1 a zvysim hodnotu topIndexu
	}
}

/* Konec c202.c */
