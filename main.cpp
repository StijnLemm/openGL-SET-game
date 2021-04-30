#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stack>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "fpsCam.h"

#include <iostream>
#include "card.h"
#include "deck.h"

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <list>

GLFWwindow* window;

void init();
void update();
void draw();

int windowWidth = 1400;
int windowHeight = 800;
bool ribbonMode = false;
bool flyMode = false;
FpsCam* camera;

// Card selection
int selectedCard = 1;
int amountOfSelectedCards = 0;
int previouslySelectedCard[2];

// Deck logic
int amountOfCards = 0;
const int maxAmountOfCards = 18;
Card cardsOnTable[maxAmountOfCards];
Deck* deck;

int main(void)
{
	if (!glfwInit())
		throw "Could not initialize glwf";

	window = glfwCreateWindow(windowWidth, windowHeight, "Set - Card game", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	tigl::init();

	init();

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();


	return 0;
}


void init()
{

#pragma region keyCallBacks

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE) {
				glfwSetWindowShouldClose(window, true);
			}
			if (key == GLFW_KEY_M)
			{
				if (action == GLFW_PRESS)
				{
					ribbonMode = !ribbonMode;
					if (ribbonMode)
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						std::cout << "Ribbon mode enabled" << std::endl;
					}
					else
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						std::cout << "Fill mode enabled" << std::endl;
					}
				}
			}
			if (key == GLFW_KEY_N)
			{
				if (action == GLFW_PRESS)
				{
					flyMode = !flyMode;

					if (flyMode)
					{
						camera->enable(window);
						std::cout << "Fly mode enabled" << std::endl;
					}
					else
					{
						camera->disable(window);
						std::cout << "Fly mode disabled" << std::endl;
					}
				}
			}

			if (key == GLFW_KEY_DOWN)
			{
				if (action == GLFW_PRESS)
				{
					if (selectedCard > 1)
					{
						selectedCard -= 1;
					}
				}
			}
			if (key == GLFW_KEY_UP)
			{
				if (action == GLFW_PRESS)
				{
					if (selectedCard != amountOfCards)
					{
						selectedCard += 1;
					}
				}
			}
			if (key == GLFW_KEY_RIGHT)
			{
				if (action == GLFW_PRESS)
				{
					// Amount of cards starts at 0 so +1
					if (selectedCard < amountOfCards + 1 - 3)
					{
						selectedCard += 3;
					}
				}
			}
			if (key == GLFW_KEY_LEFT)
			{
				if (action == GLFW_PRESS)
				{
					if (selectedCard > 3)
					{
						selectedCard -= 3;
					}
				}
			}
			if (key == GLFW_KEY_B)
			{
				if (action == GLFW_PRESS)
				{
					if (amountOfCards<maxAmountOfCards)
					{
						cardsOnTable[amountOfCards] = deck->getNextCard();
						amountOfCards++;
					}
				}
			}
			if (key == GLFW_KEY_ENTER)
			{
				if (action == GLFW_PRESS)
				{
					if (previouslySelectedCard[0]==selectedCard)
					{
						if (amountOfSelectedCards == 1) {
							previouslySelectedCard[0] = 0;
							amountOfSelectedCards--;
						}
						else
						{
							previouslySelectedCard[0] = previouslySelectedCard[1];
							previouslySelectedCard[1] = 0;
							amountOfSelectedCards--;
						}
					}
					else if(previouslySelectedCard[1] == selectedCard)
					{
						previouslySelectedCard[1] = 0;
						amountOfSelectedCards--;
					}
					else if (amountOfSelectedCards==2)
					{
						// 3 cards have been selected
						// Check if the cards are correct
						Card card1 = cardsOnTable[selectedCard-1];
						Card card2 = cardsOnTable[previouslySelectedCard[0]-1];
						Card card3 = cardsOnTable[previouslySelectedCard[1]-1];
						if (card1.compare(&card2, &card3)) {
							std::cout << "You have got a Set" << std::endl;
							cardsOnTable[selectedCard-1] = deck->getNextCard();
							cardsOnTable[previouslySelectedCard[0]-1] = deck->getNextCard();
							cardsOnTable[previouslySelectedCard[1]-1] = deck->getNextCard();
						}
						else {
							std::cout << "That was not a correct Set" << std::endl;
						}

						amountOfSelectedCards = 0;
						previouslySelectedCard[0] = 0;
						previouslySelectedCard[1] = 0;
					}else {
						previouslySelectedCard[amountOfSelectedCards] = selectedCard;
						amountOfSelectedCards++;
					}
				}
			}
		});
#pragma endregion

	// Add camera for fly mode
	camera = new FpsCam();

	// Initialise deck
	deck = new Deck();

#pragma region texture loading

	tigl::shader->enableTexture(true);

	glEnable(GL_TEXTURE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	tigl::shader->enableColor(true);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("Resources/texture_atlas.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
#pragma endregion

}


void update()
{
	if (flyMode)
	{
		camera->update(window);
	}

	while (amountOfCards < 12)
	{
		cardsOnTable[amountOfCards] = deck->getNextCard();
		amountOfCards++;
	}
}

#pragma region Calculation of card data

glm::vec2 calculateTexturePosition(Card card) {
	// Calculate the x position on the texture atlas, this will be where the texture starts
	float x = (card.shape) * 3.0f;
	x += card.shapeCount;

	// Calculate the y position on the texture atlas, this will be where the texture starts
	float y = (card.color) * 3.0f;
	y += card.fill;

	return glm::vec2(x, y);
}

glm::vec3 calculatePosition(int index) {
	// start at position 0,0 with index 0
	int x = (index / 3);
	int y = index % 3 - 1;

	return glm::vec3(x, y * 2, 0);
}
#pragma endregion

#pragma region Matrix

glm::mat4 getProjectionMatrix() {
	constexpr float fieldOfView = glm::radians(90.0f);
	float aspectRatio = (float)windowWidth / (float)windowHeight;
	return glm::perspective(fieldOfView, aspectRatio, 0.1f, 200.0f);
}

glm::mat4 getViewMatrix() {
	// Position of the camera in the space
	glm::vec3 cameraPosition(2, 0, 5);

	// Where we want to point the camera to
	glm::vec3 cameraTarget = glm::vec3(2.0f, 0.5f, 0.0f);

	// The positive X-axis of the camera
	glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	return glm::lookAt(cameraPosition, cameraTarget, upVector);
}
#pragma endregion



void createCard(glm::mat4 modelMatrix, glm::vec2 textureLocation, glm::vec4 color) {
	glm::vec3 a(-0.5, -1, -0.5);
	glm::vec3 b(0.5, -1, -0.5);
	glm::vec3 c(0.5, 1, -0.5);
	glm::vec3 d(-0.5, 1, -0.5);

	// floats used by texture calculations
	float multiplier = 1.0f / 9.0f;
	float textureX = textureLocation.x;
	float textureY = textureLocation.y;

	tigl::shader->setModelMatrix(modelMatrix);
	tigl::begin(GL_QUADS);
	tigl::addVertex(Vertex::PTC(a, glm::vec2((textureX - 1.0f) * multiplier, (textureY - 1.0f) * multiplier), color));
	tigl::addVertex(Vertex::PTC(b, glm::vec2((textureX - 1.0f) * multiplier, textureY * multiplier), color));
	tigl::addVertex(Vertex::PTC(c, glm::vec2(textureX * multiplier, textureY * multiplier), color));
	tigl::addVertex(Vertex::PTC(d, glm::vec2(textureX * multiplier, (textureY - 1.0f) * multiplier), color));
	tigl::end();
}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (flyMode)
	{
		tigl::shader->setProjectionMatrix(getProjectionMatrix());
		tigl::shader->setViewMatrix(camera->getMatrix());
	}
	else {

		tigl::shader->setProjectionMatrix(getProjectionMatrix());
		tigl::shader->setViewMatrix(getViewMatrix());
	}


	for (int cardIterator = 0; cardIterator < amountOfCards; cardIterator++)
	{
		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, calculatePosition(cardIterator));

		if (cardIterator + 1 == selectedCard)
		{
			createCard(modelMatrix, calculateTexturePosition(cardsOnTable[cardIterator]), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		}
		else if(cardIterator + 1 == previouslySelectedCard[0] || cardIterator + 1 == previouslySelectedCard[1])
		{
			createCard(modelMatrix, calculateTexturePosition(cardsOnTable[cardIterator]), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		}
		else
		{
			createCard(modelMatrix, calculateTexturePosition(cardsOnTable[cardIterator]), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}
