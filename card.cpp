#include "card.h"

bool Card::compare(Card* c1, Card* c2)
{
	/*
	CHECKED - They all have the same number or have three different numbers.
	CHECKED - They all have the same shape or have three different shapes.
	CHECKED - They all have the same shading or have three different shadings.
	CHECKED - They all have the same color or have three different colors.
	*/

	// check if any of the cards are equal to NULL
	

	// if any of the card values is equal to 0, return false. 0 == NULLCARD
	if (this->shapeCount == 0
		|| c1->shapeCount == 0
		|| c2->shapeCount == 0
		|| this->shape == 0
		|| c1->shape == 0
		|| c2->shape == 0
		|| this->fill == 0
		|| c1->fill == 0
		|| c2->fill == 0
		|| this->color == 0
		|| c1->color == 0
		|| c2->color == 0) {
		return false;
	}

	// check the shape count
	unsigned int totalCount = (this->shapeCount 
								+ c1->shapeCount
								+ c2->shapeCount);

	if (totalCount % 3 != 0) {
		return false;
	}

	// check the shape
	totalCount = (this->shape + c1->shape + c2->shape);

	if (totalCount % 3 != 0) {
		return false;
	}
	//check the fillament
	totalCount = (this->fill + c1->fill + c2->fill);

	if (totalCount % 3 != 0) {
		return false;
	}
	//check the color
	totalCount = (this->color + c1->color + c2->color);

	if (totalCount % 3 != 0) {
		return false;
	}

	return true;
}
