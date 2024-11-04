//
// Created by Eskild on 03.11.2024.
//

#include "Board.h"

#include <format>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <algorithm>

namespace DeepKnight {

   Board::Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {}

   Board::Board(std::string fenString) :
       m_fenString(std::move(fenString)), m_bitBoards(), m_colorToPlay(), m_whiteCastled(false), m_blackCastled(false),
       m_castleState(0), m_movesPlayed(0)
   {
      parseFenString(m_fenString);
   }

   std::string Board::printBitBoard() const { return printBitBoard(getAllPieces()); }

   std::string Board::printBitBoard(const Bitboard &b) const
   {
      std::string s = "+---+---+---+---+---+---+---+---+\n";
      for (Rank r = RANK_8; r >= RANK_1; --r) {
         for (File f = FILE_A; f <= FILE_H; ++f) {
            s += b & getBoardIndex(r, f) ? "| X " : "|   ";
         }
         s += "| " + std::to_string(1 + r) + "\n+---+---+---+---+---+---+---+---+\n";
      }
      s += "  a   b   c   d   e   f   g   h\n";
      return s;
   }

   void Board::parseFenString(const std::string &fenString)
   {
      std::array<std::string, 6> fenTokens;
      std::array<std::string, 8> boardPosition;
      std::stringstream ss(fenString);

      /* Read every token in the FEN string */
      for (auto &entry: fenTokens) {
         ss >> entry;
      }

      /* Extract every rank and set bitBoards */
      int i{0};
      std::string token;
      ss = std::stringstream(fenTokens[0]);
      while (std::getline(ss, token, '/')) {
         boardPosition[i++] = token;
      }

      for (Rank r = RANK_8; r >= RANK_1; --r) {
         setBitBoard(boardPosition[r], r);
      }

      /* Determine who's to play the next move */
      fenTokens[1] == "w" ? m_colorToPlay = WHITE : m_colorToPlay = BLACK;
   }

   void Board::setBitBoard(const std::string& rankString, const Rank& rank)
   {
      std::uint8_t space{0};
      for (const auto& piece : rankString) {
         if (isdigit(piece)) {
            space += piece - '0';
            continue;
         }
         const std::uint8_t index = 8 * rank + space;
         switch (piece) {
            case 'p': m_bitBoards[W_PAWN]   |= Bitboard{1} << index; break;
            case 'n': m_bitBoards[W_KNIGHT] |= Bitboard{1} << index; break;
            case 'b': m_bitBoards[W_BISHOP] |= Bitboard{1} << index; break;
            case 'r': m_bitBoards[W_ROOK]   |= Bitboard{1} << index; break;
            case 'q': m_bitBoards[W_QUEEN]  |= Bitboard{1} << index; break;
            case 'k': m_bitBoards[W_KING]   |= Bitboard{1} << index; break;
            case 'P': m_bitBoards[B_PAWN]   |= Bitboard{1} << index; break;
            case 'N': m_bitBoards[B_KNIGHT] |= Bitboard{1} << index; break;
            case 'B': m_bitBoards[B_BISHOP] |= Bitboard{1} << index; break;
            case 'R': m_bitBoards[B_ROOK]   |= Bitboard{1} << index; break;
            case 'Q': m_bitBoards[B_QUEEN]  |= Bitboard{1} << index; break;
            case 'K': m_bitBoards[B_KING]   |= Bitboard{1} << index; break;
            default: break;
         }
         space++;
      }
   }

} // namespace DeepKnight
