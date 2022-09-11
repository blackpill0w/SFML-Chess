#include "./button.hpp"

namespace utils {
   Button::Button(const sf::Vector2f &pos, const string &text, sf::Font *font, const function< void() > &funcToRun)
   :  bgColor(utils::blackColor), bgHoverColor(utils::purpleColor),
      fgColor(utils::whiteColor),
      m_rect(), m_text(), m_funcToRun(funcToRun)
   {
      m_text.setFont(*font);
      m_text.setString(text);
      m_text.setPosition(
         pos.x + xPadding,
         pos.y + yPadding - m_text.getGlobalBounds().height/2.f
      );

      m_rect.setSize({
         m_text.getGlobalBounds().width + xPadding*2.f,
         m_text.getGlobalBounds().height + yPadding*2.f
      });
      m_rect.setPosition(pos);
      m_rect.setFillColor(bgColor);
   }

   void Button::update(sf::RenderWindow &window, const sf::Vector2f &mousePos, const bool isMouseClicked) {
      draw(window);
      if (isHovered(mousePos)) {
         m_rect.setFillColor(bgHoverColor);
         if (isMouseClicked) {
            m_funcToRun();
         }
      }
      else {
         m_rect.setFillColor(bgColor);
      }
   }

   bool Button::isHovered(const sf::Vector2f &mousePos) {
      return ( m_rect.getGlobalBounds().contains(mousePos) );
   }
   void Button::draw(sf::RenderWindow &window) {
      window.draw(m_rect);
      window.draw(m_text);
   }
   void Button::run() {
      m_funcToRun();
   }
   void Button::changeBgColor(const sf::Color &color) {
      m_rect.setFillColor(color);
   }
   void Button::changeFgColor(const sf::Color &color) {
      m_text.setFillColor(color);
   }
   void Button::setPosition(const sf::Vector2f &newPos) {
      m_rect.setPosition(newPos);
      m_text.setPosition(
         newPos.x + xPadding,
         newPos.y + yPadding
      );
   }

}
