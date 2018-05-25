#include "farm.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

const Color Farm::s_color = FARM_COLOR;
const Texture* Farm::s_texture = new Texture();
const char Farm::s_pathButtonTexture[] = FARM_PATH_BUTTON_TEXTURE;
const char Farm::s_pathButtonTexture2[] = FARM_PATH_BUTTON_TEXTURE2;

void Farm::setting()
{
       Farm::setTexture();
       Element::elements()[FARM_TYPE] = new Farm();
};



