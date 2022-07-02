#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <functional>
#include <SFML/Graphics.hpp>

#include "sfmlColors.hpp"

using std::string;
using std::function;

namespace utils {

class Button {
   public:
      sf::Color bgColor;
      sf::Color bgHoverColor;
      sf::Color fgColor;
   protected:
      sf::RectangleShape m_rect;
      sf::Text m_text;
      function< void() > m_funcToRun;

      static constexpr float xPadding{ 14.f };
      static constexpr float yPadding{ 12.f };
   public:
      Button(const sf::Vector2f &pos, const string &text, sf::Font *font, const function< void() > &funcToRun);
      bool isHovered(const sf::Vector2f &mousePos);
      void run();
      void draw(sf::RenderWindow &window);
      void changeBgColor(const sf::Color &color);
      void changeFgColor(const sf::Color &color);
      void setPosition(const sf::Vector2f &newPos);
      void update(sf::RenderWindow &window, const sf::Vector2f &mousePos, const bool isMouseClicked);
};

}


#endif
