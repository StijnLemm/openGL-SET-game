#pragma once
#include <stack>
#include "card.h"

#define DECK_SIZE 81

class Deck {
public:
	Deck();
	Card getNextCard();
	unsigned int getCardsAmountLeft();
	void refreshDeck();
private:
	std::stack<Card*> cards;
};