#include "farm.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

const Color Farm::s_color = FARM_COLOR;
const Texture* Farm::s_texture = new Texture();
const char Farm::s_pathButtonTexture[] = FARM_PATH_BUTTON_TEXTURE;

void Farm::setting()
{
       Farm::setTexture();
       Element::elements()[FARM_TYPE] = new Farm();
};



