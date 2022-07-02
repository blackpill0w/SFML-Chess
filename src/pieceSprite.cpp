#include "pieceSprite.hpp"

// Copy constructor
PieceSprite::PieceSprite(const PieceSprite& other)
:  pieceIndex{ other.pieceIndex }, board{ other.board },
   alive{ other.alive }, pos{ other.pos }, pieceType{ other.pieceType }, sprite{ sf::Sprite( *other.sprite.getTexture() ) },
   textures{ other.textures }, window{ other.window }
{
   sprite.setPosition(other.sprite.getPosition());
   sprite.setScale(utils::pieceSize/sprite.getGlobalBounds().width, utils::pieceSize/sprite.getGlobalBounds().height);
}

PieceSprite::PieceSprite(const vector<sf::Texture> *textures, const unsigned &pieceIndex, Chess::Board *board, sf::RenderWindow *window)
:  pieceIndex{ pieceIndex },
   board{ board }, alive{ true }, pos{ (*board).pieces[pieceIndex]->pos },
   pieceType{ (*board).pieces[pieceIndex]->type }, sprite{ sf::Sprite() },
   textures{ textures }, window{ window }
{
   sprite.setScale(
      utils::pieceSize / sprite.getGlobalBounds().width,
      utils::pieceSize / sprite.getGlobalBounds().height
   );

   sprite.setPosition(utils::strToVectorf( pos ));
   // Set texture
   changeTexture();
}

void PieceSprite::move(const sf::Vector2f &pos) {
   sprite.setPosition(pos);
}

bool PieceSprite::isContainPos(const sf::Vector2f &pos) const {
   return ( sprite.getGlobalBounds().contains( pos ) );
}

void PieceSprite::draw() {
   if (alive) {
      window->draw(sprite);
   }
}

void PieceSprite::changeTexture() {
   unsigned txtrIndex{ utils::getTextureIndex(pieceType) };
   sprite.setTexture((*textures)[txtrIndex]);
}

void PieceSprite::update() {
   // If corresponding piece is still alive
   if ( (*board).pieces[pieceIndex]->alive ) {
      // If piece's position changed
      if (pos != (*board).pieces[pieceIndex]->pos) {
         pos = (*board).pieces[pieceIndex]->pos;
         // Move the sprite
         sprite.setPosition(utils::strToVectorf( pos ));
      }
      if (!alive) {
         alive = true;
      }
      // If a pieces is promoted
      if (pieceType != (*board).pieces[pieceIndex]->type) {
         pieceType = (*board).pieces[pieceIndex]->type;
         changeTexture();
      }
   }
   else {
      alive = false;
   }
}
