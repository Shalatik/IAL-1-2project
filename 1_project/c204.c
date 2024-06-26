
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

//xcesko00 Simona Ceskova 22.10.2021

#include "c204.h"

int solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar(Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength)
{
    char temp;
    // pomocna promenna, kam budu ukladat vrchol zasobniku
    for (; !Stack_IsEmpty(stack);)
    {
        //dokud neni zasobnik prazdny, tak budu z nej ukladat operatory do Postfixu, protoze jsou v zavorce a maji prednost
        Stack_Top(stack, &temp);
        Stack_Pop(stack);
        //smazu prvek na topu stacku
        if (temp == '(')
            break;
        //pouzivam temp abych minimalizovala pristup k postfixExpression
        //skoncim u leve "(" protoze zasobnik je LIFO

        postfixExpression[*postfixExpressionLength] = temp;
        *postfixExpressionLength += 1;
        //vlozim operator do postfix zapisu jehoz delku zvysim o 1
    }
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation(Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength)
{
    char tempTop;
    //pomocna promenna na ukladani hodnoty vrcholu zasobniku
    if (Stack_IsEmpty(stack) == TRUE)
    {
        Stack_Push(stack, c);
        //jestli je zasobnik prazdny, tak operator do nej vlozim a neresim jeste jeho prioritu
    }
    else
    {
        Stack_Top(stack, &tempTop);
        if (((tempTop == '+') || (tempTop == '-')) && ((c == '*') || (c == '/')))
        {
            Stack_Push(stack, c);
            //diky priorite muzu dat na zasobnik nove prichozi operator
        }
        else if (tempTop == '(')
        {
            Stack_Push(stack, c);
            //vlozim do zasobniku, protoze jsem zacatku zavorky a mam prioritu
        }
        else
        {
            postfixExpression[*postfixExpressionLength] = tempTop;
            *postfixExpressionLength += 1;
            //vkladam na postfix zapis operatory a smazu je ze zasobniku
            Stack_Pop(stack);
            doOperation(stack, c, postfixExpression, postfixExpressionLength);
            //opekuji v rekurzi, protoze na zasobniku muzu mit vice prvku zaraz
            //abych dodrzela prioritu a posloupnost operatoru, tak si ho musim udrzovat co nejvic prazdny
        }
    }
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression Znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns Znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix(const char *infixExpression)
{
    unsigned PostfixLen = 0;
    //delka postfix  retezce

    char *Postfix = (char *)malloc(sizeof(char) * MAX_LEN);
    if (Postfix == NULL)
        return NULL;
    //allocovani mista pro postfix retezec

    Stack *PostfixStack = (Stack *)malloc(sizeof(Stack));
    if (PostfixStack == NULL)
    {
        free(Postfix);
        return NULL;
    }
    Stack_Init(PostfixStack);
    //allocovani mista pro zasobnik a jeho inicializace

    unsigned i = 0;
    while (infixExpression[i] != '\0')
    {
        //while cyklus dokud nenarazim na znak ukoncujici retezec

        char temp = infixExpression[i];
        //promenna temp na zpracovavani znaku na prevod do postfix

        //              <A;Z>              ||              <a;z>             ||             <0;9>
        if (((temp < '[') && (temp > '@')) || ((temp < '{') && (temp > '`')) || ((temp < ':') && (temp > '/')))
        {
            Postfix[PostfixLen] = temp;
            PostfixLen += 1;
            //vlozim operand na postfix, protoze ty nemaji prioritu
        }
        else if (temp == '(')
        {
            Stack_Push(PostfixStack, temp);
            //na zasobnik, aby se zavorka spracovala spravne
        }
        else if (temp == ')')
        {
            untilLeftPar(PostfixStack, Postfix, &PostfixLen);
            //zpracovavani vyrazu zavorky
        }
        else if (temp == '=')
        {
            //to znamena zakonceni retezce postfix
            while (!Stack_IsEmpty(PostfixStack))
            {
                char operator;
                Stack_Top(PostfixStack, &operator);
                Postfix[PostfixLen] = operator;
                PostfixLen += 1;
                Stack_Pop(PostfixStack);
                //nez vlozim =, tak musim vlozit do postfixu vsechny zbyle operatory co zbyly na zasobniku
            }
            Postfix[PostfixLen] = temp;
            PostfixLen += 1;
            // = je vzdycky posledni operator
        }
        else
        {
            doOperation(PostfixStack, temp, Postfix, &PostfixLen);
            // temp je operator, ktery poslu na zpracovani
        }
        i += 1;
    }
    free(PostfixStack);
    Postfix[PostfixLen] = '\0';
    //znak ukonceni retezce
    return Postfix;
}

/* Konec c204.c */
