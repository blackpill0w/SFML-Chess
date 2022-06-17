#include "pieceSprite.hpp"

PieceSprite::PieceSprite(const vector<sf::Texture> *textures, unsigned &pieceIndex, Chess::Board *board, sf::RenderWindow *window)
:  textures{ textures }, sprite{ sf::Sprite() }, window{ window },
   pieceIndex{ pieceIndex }, board{ board }, alive{ true },
   pos{ (*board).pieces[pieceIndex]->pos }, pieceType{ (*board).pieces[pieceIndex]->type }
{
   sprite.setScale(70/sprite.getGlobalBounds().width, 70/sprite.getGlobalBounds().height);
   sprite.setPosition(utils::strToVectorf( pos ));
   // Set texture
   changeTexture();
}

PieceSprite::PieceSprite(const PieceSprite& other)
: textures{ other.textures }, sprite{ sf::Sprite() }, window{ other.window }, pieceIndex{ other.pieceIndex },
  board{ other.board }, alive{ other.alive }, pos{ other.pos }, pieceType{ other.pieceType }
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

void PieceSprite::changeTexture() {
   unsigned txtrIndex{ utils::getTextureIndex((*board).pieces[pieceIndex]->type) };
   sprite.setTexture((*textures)[txtrIndex]);
}

void PieceSprite::update() {
   // If corresponding piece is still alive
   if ( (*board).pieces[pieceIndex]->alive ) {
      sprite.setPosition(utils::strToVectorf( (*board).pieces[pieceIndex]->pos ));
      pos = (*board).pieces[pieceIndex]->pos;
      alive = true;
      if (pieceType != (*board).pieces[pieceIndex]->type) {
         pieceType = (*board).pieces[pieceIndex]->type;
         changeTexture();
      }
   }
   else {
      alive = false;
   }
}
