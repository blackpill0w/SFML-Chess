#include "onlinePieceSprite.hpp"

OnlinePieceSprite::OnlinePieceSprite(sf::RenderWindow *window, vector<sf::Texture> *textures)
:  alive(true), pieceType( 'q' ), pos( "a1" ), legalMoves(),
   sprite{sf::Sprite()}, window(window), textures(textures)
{
   sprite.setScale(
      utils::pieceSize / sprite.getGlobalBounds().width,
      utils::pieceSize / sprite.getGlobalBounds().height
   );
   sprite.setPosition(utils::strToVectorf( pos ));
   changeTexture();

}

OnlinePieceSprite::OnlinePieceSprite(const OnlinePieceSprite &other)
:  alive(other.alive), pieceType(other.pieceType), pos( other.pos ), legalMoves(),
   sprite{sf::Sprite()}, window( other.window ), textures( other.textures )
{
   sprite.setTexture(*other.sprite.getTexture());
   sprite.setPosition(other.sprite.getPosition());
   sprite.setScale(utils::pieceSize/sprite.getGlobalBounds().width, utils::pieceSize/sprite.getGlobalBounds().height);
}

void OnlinePieceSprite::move(const sf::Vector2f &newPos) {
   sprite.setPosition(newPos);
}

bool OnlinePieceSprite::isContainPos(const sf::Vector2f &pos) const {
   return ( sprite.getGlobalBounds().contains( pos ) );
}

bool OnlinePieceSprite::isValidMove(string &move) const {
  for (auto& m: legalMoves) {
    if (m == move) {
      return true;
    }
  }
  return false;
}

void OnlinePieceSprite::draw() {
   if (alive) {
      window->draw(sprite);
   }
}

void OnlinePieceSprite::update(const bool &isAlive, const char type, const string &pos, const vector<string> &legalMoves) {
   if (isAlive) {
      alive = true;
      pieceType = type;
      changeTexture();

      this->pos = pos;
      sprite.setPosition(utils::strToVectorf( pos ));

      this->legalMoves.clear();
      this->legalMoves.reserve(legalMoves.size());
      for (auto& move: legalMoves) {
         this->legalMoves.emplace_back(move);
      }
   }
   else {
      alive = false;
   }
}

void OnlinePieceSprite::changeTexture() {
   unsigned txtrIndex{ utils::getTextureIndex(pieceType) };
   if (txtrIndex != utils::invalidIndex) {
      sprite.setTexture( (*textures)[txtrIndex] );
   }
}
