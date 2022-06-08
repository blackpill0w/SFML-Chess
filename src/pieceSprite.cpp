#include "pieceSprite.hpp"

PieceSprite::PieceSprite(sf::Texture &txtr, unsigned &pieceIndex, Board *board, sf::RenderWindow *window)
:  sprite{ sf::Sprite(txtr) }, window{ window }, pieceIndex{ pieceIndex },
   board{ board }, alive{ true }, pos{ (*board).pieces[pieceIndex]->pos }
{
   sprite.setScale(70/sprite.getGlobalBounds().width, 70/sprite.getGlobalBounds().height);
   sprite.setPosition(utils::strToVectorf( pos ));
}

PieceSprite::PieceSprite(const PieceSprite& other)
: sprite{ sf::Sprite() }, window{ other.window }, pieceIndex{ other.pieceIndex },
  board{ other.board }, alive{ other.alive }, pos{ other.pos }
{
   sprite.setTexture(*other.sprite.getTexture());
   sprite.setPosition(other.sprite.getPosition());
   sprite.setScale(70/sprite.getGlobalBounds().width, 70/sprite.getGlobalBounds().height);
}

void PieceSprite::move(const sf::Vector2f &pos) {
   sprite.setPosition(pos);
}

void PieceSprite::draw() {
   if (alive) {
      window->draw(sprite);
   }
}

void PieceSprite::update() {
   // If corresponding piece is still alive
   if ( (*board).pieces[pieceIndex]->alive ) {
      sprite.setPosition(utils::strToVectorf( (*board).pieces[pieceIndex]->pos ));
      pos = (*board).pieces[pieceIndex]->pos;
   }
   else {
      alive = false;
   }
}