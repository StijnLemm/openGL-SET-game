#pragma once

enum Fill {
	Empty = 1,
	Filled = 2,
	Stripes = 3
};

enum Shape {
	Oval = 1,
	Swirl = 2,
	Rhombus = 3
};

enum Color {
	Green = 1,
	Purple = 2,
	Red = 3
};

class Card {
public:
	
	Fill fill;
	Color color;
	Shape shape;

	unsigned int shapeCount;

	Card(Fill fill, Color color, Shape shape, unsigned int shapeCount){
		this->fill = fill;
		this->color = color;
		this->shape = shape;
		this->shapeCount = shapeCount;
	}

	bool compare(Card* c1, Card* c2);
};