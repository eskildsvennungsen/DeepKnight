//
// Created by Eskild on 03.11.2024.
//

#ifndef BITBOARD_H
#define BITBOARD_H

#include <array>
#include <string>
#include <unordered_map>

#include "Types.h"

namespace DeepKnight {

class Board {
   public:
      explicit Board();
      explicit Board(std::string fenString);

      /* White pieces, indexed 0-5 */
      [[nodiscard]] Bitboard getWhitePawns() const { return m_bitBoards[W_PAWN]; }
      [[nodiscard]] Bitboard getWhiteKnights() const { return m_bitBoards[W_KNIGHT]; }
      [[nodiscard]] Bitboard getWhiteBishops() const { return m_bitBoards[W_BISHOP]; }
      [[nodiscard]] Bitboard getWhiteRooks() const { return m_bitBoards[W_ROOK]; }
      [[nodiscard]] Bitboard getWhiteQueens() const { return m_bitBoards[W_QUEEN]; }
      [[nodiscard]] Bitboard getWhiteKing() const { return m_bitBoards[W_KING]; }

      /* Black pieces, indexed 6-11 */
      [[nodiscard]] Bitboard getBlackPawns() const { return m_bitBoards[B_PAWN]; }
      [[nodiscard]] Bitboard getBlackKnights() const { return m_bitBoards[B_KNIGHT]; }
      [[nodiscard]] Bitboard getBlackBishops() const { return m_bitBoards[B_BISHOP]; }
      [[nodiscard]] Bitboard getBlackRooks() const { return m_bitBoards[B_ROOK]; }
      [[nodiscard]] Bitboard getBlackQueens() const { return m_bitBoards[B_QUEEN]; }
      [[nodiscard]] Bitboard getBlackKing() const { return m_bitBoards[B_KING]; }

      [[nodiscard]] Bitboard getWhitePieces() const { return m_bitBoards[W_PAWN] | m_bitBoards[W_KNIGHT] |\
                   m_bitBoards[W_BISHOP] | m_bitBoards[W_ROOK] | m_bitBoards[W_QUEEN] | m_bitBoards[W_KING]; }

      [[nodiscard]] Bitboard getBlackPieces() const { return m_bitBoards[B_PAWN] | m_bitBoards[B_KNIGHT] |\
                   m_bitBoards[B_BISHOP] | m_bitBoards[B_ROOK] | m_bitBoards[B_QUEEN] | m_bitBoards[B_KING]; }

      [[nodiscard]] Bitboard getAllPieces() const { return getWhitePieces() | getBlackPieces(); }

      std::string printBitBoard() const;
      std::string printBitBoard(const Bitboard &) const;

   private:
      void parseFenString(const std::string &);
      void setBitBoard(const std::string &, const Rank &);

   private:
      std::string m_fenString;
      std::unordered_map<Piece, char> m_fenPieces =\
         {{W_PAWN, 'p'}, {W_KNIGHT, 'n'}, {W_BISHOP, 'b'},{W_ROOK, 'r'},\
         {W_QUEEN, 'q'}, {W_KING, 'k'}, {B_PAWN, 'P'}, {B_KNIGHT, 'N'},\
         {B_BISHOP, 'B'}, {B_ROOK, 'R'}, {B_QUEEN, 'Q'}, {B_KING, 'K'}};

      static constexpr int m_numBitBoards{15};
      std::array<Bitboard, m_numBitBoards> m_bitBoards;

      Color m_colorToPlay;

      bool m_whiteCastled;
      bool m_blackCastled;

      std::uint8_t m_castleState; /* Four bits representing castle state, same order as fen */
      std::uint16_t m_movesPlayed;
      std::uint16_t m_halfMoveClock{}; /* Keeps track of 50-move rule */
   };
} // DeepKnight

#endif //BITBOARD_H
