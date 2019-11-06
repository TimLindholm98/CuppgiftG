#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "safeinput.h"

typedef struct
{

    int cardNumber;
    bool access;
    struct tm *dateAdded;
}CARD;
typedef struct
{
	CARD* allCards;
	int AmountOfCards;
}SYSTEM_STATE;
void clrscr()
{
    system("@cls||clear");
}
void RemoveOrAddingAccess(SYSTEM_STATE *state, int accessNumber)
{

    while(true)
    {
        printf("\n---Alter card access---\n");
        printf("Remove card access(1) Give card access(2)\n");
        int selection;
        GetInputInt("Ange val:> ", &selection);
        switch(selection)
        {
        case 1: // Removing access
            for (int i = 0; i < state->AmountOfCards; i++)
            {
                CARD A = state->allCards[i];
                if(A.cardNumber == accessNumber)
                {
                    state->allCards[i].access = false;
                    break;
                }
            }
            break;
        case 2: // Giving access
            for (int i = 0; i < state->AmountOfCards; i++)
            {
                CARD B = state->allCards[i];
                if(B.cardNumber == accessNumber)
                {
                    state->allCards[i].access = true;
                    break;
                }
            }
            break;
        }
        return;
    }
}
void openDoor(SYSTEM_STATE *state)
{
    clrscr();
    int accessNumber;
    bool openingDoor = false;
    GetInputInt("Access Code: ", &accessNumber);

    if(state->AmountOfCards == 0)
    {
        printf("No cards in system...\n");
        return;
    }
    for (int i = 0; i < state->AmountOfCards; i++)
    {
        CARD p = state->allCards[i];

        if(p.access == true && p.cardNumber == accessNumber)
        {
                openingDoor = true;
                break;
        }
    }
    if(openingDoor == true)
        printf("\nOpening door... The lamp is green:)\n");
    else if (openingDoor == false)
        printf("\nPremission denied... The lamp is red\n");
}
void ListPlayers(SYSTEM_STATE *state)
{
    clrscr();
    printf("\n---List registerd cards---\n");
    for (int i = 0; i < state->AmountOfCards; i++)
    {
        CARD p = state->allCards[i];
        printf("%d ", p.cardNumber);
        printf("%s", p.access? "Acess to system:     ":"No access to system: ");
        printf("%d-%02d-%02d\n",
               state->allCards[i].dateAdded->tm_year +1900,
               state->allCards[i].dateAdded->tm_mon +1,
               state->allCards[i].dateAdded->tm_mday);
    }
    printf("\nPress any key to continue...");
    getchar();
}
void NewCard(SYSTEM_STATE *state, int accessNumber)
{
    clrscr();
    int indexForTheNewOne;
    for (int i = 0; i < state->AmountOfCards; i++)
    {
        CARD c = state->allCards[i];
        if(accessNumber == c.cardNumber)
        {
            RemoveOrAddingAccess(state, accessNumber);
            return;
        }
    }
    printf("\n---New card---\n");

	if (state->AmountOfCards == 0)
	{
		state->allCards = malloc(sizeof(CARD));
		state->AmountOfCards = 1;
		indexForTheNewOne = 0;
	}
	else
    {
        state->allCards = realloc(state->allCards, sizeof(CARD)*(state->AmountOfCards + 1));
        indexForTheNewOne = state->AmountOfCards;
        state->AmountOfCards++;
    }

    state->allCards[indexForTheNewOne].cardNumber = accessNumber;

    printf("Give card access yes(1) no(2)\n");
    int selection;
    GetInputInt("Ange val:> ", &selection);
    switch (selection)
    {
    case 1:
        state->allCards[indexForTheNewOne].access = true;
        break;
    case 2:
        state->allCards[indexForTheNewOne].access = false;
        break;
    }

    time_t now;
    struct tm *ts;
    now = time(NULL);
    ts = localtime(&now);

    state->allCards[indexForTheNewOne].dateAdded = ts;
}
void AdminMenu(SYSTEM_STATE *state)
{

    while(true)
    {
        clrscr();
        printf("\n---Admin Menu---\n");
        printf("1. Remote open door\n2. List all cards in system\n3. Add/remove access\n4. Exit \n5. Test fake card\n");

        int selection;
		if (!GetInputInt("Ange val:> ", &selection))
			continue;
        int accessNumber;
		switch (selection)
		{
		case 1:
			openDoor(state);
			break;
		case 2:
			ListPlayers(state);
			break;
        case 3:
            GetInputInt("Access Code: ", &accessNumber);
            NewCard(state, accessNumber);
			break;
        case 4:
			return;
        case 5:
            openDoor(state);
			break;
		}
    }

}
void MainMenu(SYSTEM_STATE *state)
{
    while(true)
    {
        printf("\n---Main Menu---\n");
        printf("1. Manual access\n2. Admin\n3. Exit\n");
        int selection;
        if (GetInputInt("Ange val:>", &selection) == false)
            continue;
        switch(selection)
        {
        case 1:
            openDoor(state);
            break;
        case 2:
            AdminMenu(state);
            break;
        case 3:
            return;
        }
    }
}
int main()
{
    SYSTEM_STATE state;
    state.allCards = NULL;
	state.AmountOfCards = 0;
    MainMenu(&state);

    return 0;
}

