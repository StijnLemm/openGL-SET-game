#include <vector>
#include <cstdlib>
#include "deck.h"

Deck::Deck()
{
	refreshDeck();
}

Card Deck::getNextCard()
{
	Card* card = this->cards.top();
	this->cards.pop();
	return *card;
}

unsigned int Deck::getCardsAmountLeft()
{
	return (unsigned int) this->cards.size();
}

void Deck::refreshDeck()
{
	// empty cards
	while (!cards.empty())
		cards.pop();

	std::vector<Card*> allUniqueCards;

	// fill vector with all unique combinations
	for (int shapeCountI = 1; shapeCountI < 4; shapeCountI++) {

		for (int shapeI = 1; shapeI < 4; shapeI++) {

			for (int colorI = 1; colorI < 4; colorI++) {

				for (int fillI = 1; fillI < 4; fillI++) {

					allUniqueCards.push_back(new Card(static_cast<Fill>(fillI), 
												static_cast<Color>(colorI), 
												static_cast<Shape>(shapeI), shapeCountI));
				}
			}
		}
	}
	
	// fill our final stack with random order
	for (int i = 0; i < DECK_SIZE; i++) {
		int randomIndex = rand() % allUniqueCards.size();
		this->cards.push(allUniqueCards[randomIndex]);
		allUniqueCards.erase(allUniqueCards.begin() + randomIndex);
	}
}
