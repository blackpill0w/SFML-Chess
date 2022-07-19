#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

#include "../Chess/board.hpp"

#include "sfmlColors.hpp"
#include "button.hpp"

using std::string;
using std::vector;

namespace utils {

   //** Just a helper
   extern const unsigned int invalidIndex;
   //** FPS
   extern const unsigned int fps;

   //** Length of piece's side
   extern const int pieceSize;
   //** Length of the board (pSize * 8)
   extern const int boardSize;
   //** Width of window
   extern const unsigned int wWidth;
   //** Height of window
   extern const unsigned int wHeight;
   //** Game title
   extern const sf::String gameTitle;
   //** Initial position of window
   extern const sf::Vector2i wInitialPos;
   //** Font
   extern const string fontFile;
   //** Path to texture of the board
   extern const string boardTexture;
   //** Path to textures of the pieces
   extern string textures[12];
   //** To access the pieces' textures
   enum : unsigned int {wKing = 0u, bKing, wQueen, bQueen, wRook, bRook, wBishop, bBishop, wKnight, bKnight, wPawn, bPawn};
   //** Sound files
   extern string soundFiles[6];
   //** To access the sound files
   enum : unsigned int {
      gameStart = 0u, move,
      capture, check,
      castle, gameEnd
   };

   // Server settings
   extern const unsigned port;
   extern const string startGameStr;
   extern const string disconnectStr;

   //********************************************
   //**
   //** Given the mouse's position, it returns the top
   //** left point of the square containing
   //** the mouse (doesn't matter if there is a piece).
   //**
   //********************************************
   sf::Vector2f adjustMousePos(const sf::Vector2f &mousePos);

   //********************************************
   //**
   //** Converts an sf::Vector2f to a chess position
   //**
   //********************************************
   string posToStr(const sf::Vector2f &mousePos);

   //********************************************
   //**
   //** The opposite. A chess position to the top left
   //** corner of the corresponding square.
   //**
   //********************************************
   sf::Vector2f strToVectorf(const string &pos);

   //********************************************
   //**
   //** Highlights the moves of a piece.
   //**
   //********************************************
   void highlightMoves(sf::RenderWindow *window, vector<string> &moves);

   //********************************************
   //**
   //** Given a vector of textures, load the textures.
   //** Texture are assumed to be in this order:
   //**   wKing, bKing,
   //**   wQueen, bQueen,
   //**   wRook, bRook,
   //**   wBishop, bBishop,
   //**   wKnight, bKnight,
   //**   wPawn, bPawn
   //**
   //********************************************
   void loadTextures(vector< sf::Texture > &textureList);

   //********************************************
   //**
   //** Returns the index of the texture depending
   //** on the piece's (the 'Piece' class) type
   //**
   //********************************************
   unsigned getTextureIndex(const char pieceType);

   //********************************************
   //**
   //** Keeps a piece inside the board while being
   //** dragged.
   //**
   //********************************************
   void keepPieceInsideBoard(sf::Vector2f &pos);

   //********************************************
   //**
   //** Given a vector of PieceSprite/OnlinePieceSprite,
   //** return the index of the sprite at a given
   //** position (sf::Vector2f).
   //**
   //********************************************
   template<class T>
   unsigned getSpriteIndexAt(const vector< T > &sprites, const sf::Vector2f &pos) {
     unsigned index{ invalidIndex };
     for (unsigned j=0 ; j < sprites.size(); j++ ) {
       if (sprites[j].alive) {
         if (sprites[j].isContainPos(pos)) {
           index = j;
           break;
         }
       }
     }
     return index;
   };

   //********************************************
   //**
   //** Draws a vector of PieceSprite/OnlinePieceSprite.
   //**
   //********************************************
   template<class T>
   void drawSprites(vector<T> &sprites, const unsigned &spritePressedIndex = invalidIndex) {
      for (auto& sprite: sprites) {
         sprite.draw();
      }
      if (spritePressedIndex != invalidIndex) {
         sprites[spritePressedIndex].draw();
      }
   };


  template <class T>
  char getPieceToPromoteTo(
      sf::RenderWindow *window,
      sf::Sprite &boardSprite,
      vector< T > *sprites,
      const unsigned &pieceToBePromotedIndex
  ) {
    char piece{ 0 };
    // Get mouse's position
    sf::Vector2f mousePos;

    sf::Vector2f rectSize(utils::boardSize - 200.f, 100.f);

    sf::RectangleShape rects[4] = {
      sf::RectangleShape(rectSize),
      sf::RectangleShape(rectSize),
      sf::RectangleShape(rectSize),
      sf::RectangleShape(rectSize)
    };
    sf::Font font;
    font.loadFromFile(utils::fontFile);
    sf::Text texts[4] = {
      sf::Text("Queen", font),
      sf::Text("Rook", font),
      sf::Text("Bishop", font),
      sf::Text("Knight", font)
    };

    for (int i=0; i<4; i++) {
      sf::Vector2f pos(100.f, i*110.f + 80.f);
      rects[i].setFillColor(sf::Color(0u, 0u, 0u, 255u));
      rects[i].setPosition(pos);
      texts[i].setFillColor(sf::Color(255u, 255u, 255u, 255u));
      pos.x += (rects[i].getSize().x / 2.f) - (texts[i].getGlobalBounds().width / 2.f);
      pos.y += 30.f;
      texts[i].setPosition(pos);
    }

    while (piece == 0 && window->isOpen()) {
      sf::Event event;

      while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window->close();
          return 0;
        }
        if (event.type == sf::Event::MouseButtonPressed) {
          for (int i=0; i < 4; i++) {
            if ( rects[i].getGlobalBounds().contains( mousePos )) {
              switch (i) {
              case 0:
                piece = 'q';
                break;
              case 1:
                piece = 'r';
                break;
              case 2:
                piece = 'b';
                break;
              case 3:
                piece = 'n';
                break;
              }
              // Break from the for loop
              break;
            }
          }
        }
      }

      // Get mouse's position
      mousePos = sf::Vector2f( sf::Mouse::getPosition(*window) );

      window->clear();

      for (int i=0; i < 4; i++) {
        if ( rects[i].getGlobalBounds().contains( mousePos )) {
          rects[i].setFillColor(sf::Color(45u, 25u, 52u, 255u));
        }
        else {
          rects[i].setFillColor(sf::Color(0u, 0u, 0u, 255u));
        }
      }

      window->draw(boardSprite);
      // Draw pieces
      utils::drawSprites(*sprites);
      // Draw promotion menu
      for (int i=0; i<4; i++) {
        window->draw(rects[i]);
        window->draw(texts[i]);
      }

      window->display();
    }
    // Match case of the choice with the piece (make them have the same color)
    if ( isupper( (*sprites)[pieceToBePromotedIndex].pieceType ) ) {
      piece = toupper(piece);
    }
    return piece;
  }

  template <class T>
  char checkPromotion(
    sf::RenderWindow *window,
    vector< T > &sprites,
    sf::Sprite &boardSprite,
    unsigned &spritePressedIndex,
    string &to
  ) {
    if ( tolower(sprites[spritePressedIndex].pieceType) == 'p' && (to[1] == '1' || to[1] == '8')) {
     // Get the type of piece to promote to
      return getPieceToPromoteTo<T>(window, boardSprite, &sprites, spritePressedIndex);
   }
   return 'q';
  }
}

#endif
