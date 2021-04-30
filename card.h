#pragma once

enum Fill {
	Empty = 1,
	Stripes = 2,
	Filled = 3
};

enum Shape {
	Diamond = 1,
	Rhombus= 2,
	Swirl = 3
};

enum Color {
	Red = 1,
	Green = 2,
	Blue = 3
};

class Card {
public:
	
	Fill fill;
	Color color;
	Shape shape;

	unsigned int shapeCount;

	// Empty constructor for empty array of cards
	Card() {	}

	Card(Fill fill, Color color, Shape shape, unsigned int shapeCount){
		this->fill = fill;
		this->color = color;
		this->shape = shape;
		this->shapeCount = shapeCount;
	}

	bool compare(Card* c1, Card* c2);
};