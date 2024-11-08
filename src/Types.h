//
// Created by Eskild on 03.11.2024.
//

#ifndef TYPES_H
#define TYPES_H
#include <cstdint>

namespace DeepKnight {

   using Bitboard = uint64_t;

   constexpr uint8_t MAX_MOVES = 255;

   enum Color {
      WHITE,
      BLACK,
      COLOR_NONE = 2
   };

   enum CastlingRights {
      NO_CASTLING,
      WHITE_OO,
      WHITE_OOO = WHITE_OO << 1,
      BLACK_OO  = WHITE_OO << 2,
      BLACK_OOO = WHITE_OO << 3,

      KING_SIDE      = WHITE_OO | BLACK_OO,
      QUEEN_SIDE     = WHITE_OOO | BLACK_OOO,
      WHITE_CASTLING = WHITE_OO | WHITE_OOO,
      BLACK_CASTLING = BLACK_OO | BLACK_OOO,
      ANY_CASTLING   = WHITE_CASTLING | BLACK_CASTLING
   };

   enum File : int {
      FILE_A,
      FILE_B,
      FILE_C,
      FILE_D,
      FILE_E,
      FILE_F,
      FILE_G,
      FILE_H,
      FILE_NONE
   };

   enum Rank : int {
      RANK_1,
      RANK_2,
      RANK_3,
      RANK_4,
      RANK_5,
      RANK_6,
      RANK_7,
      RANK_8,
      RANK_NONE
   };

   enum Square : int {
      SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
      SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
      SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
      SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
      SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
      SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
      SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
      SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
      SQ_NONE,

      SQUARE_ZERO = 0,
      SQUARE_NB   = 64
   };

   enum PieceType : int {
      PAWN,
      KNIGHT,
      BISHOP,
      ROOK,
      QUEEN,
      KING,
      PIECETYPE_NONE
   };

   enum Piece : int {
      W_PAWN = PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
      B_PAWN = PAWN + 6, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
      PIECE_NONE
   };

#define ENABLE_INCR_OPERATORS_ON(T) \
      inline T& operator++(T& d) { return d = T(int(d) + 1); } \
      inline T& operator--(T& d) { return d = T(int(d) - 1); }

   ENABLE_INCR_OPERATORS_ON(Square)
   ENABLE_INCR_OPERATORS_ON(File)
   ENABLE_INCR_OPERATORS_ON(Rank)
   ENABLE_INCR_OPERATORS_ON(Piece)
#undef ENABLE_INCR_OPERATORS_ON

   constexpr Bitboard getBoardIndex(const Square& sq) { return Bitboard{1} << sq; }
   constexpr Bitboard getBoardIndex(const Rank& r, const File& f) { return Bitboard{1} << ((r << 3) + f); }
   constexpr Piece    makePiece(const Color& c, const PieceType& pt) { return Piece{(c * 6) + pt}; }
   constexpr Square   makeSquare(const Rank& r, const File& f) { return Square{(r << 3) + f}; }
   constexpr bool     validSquare(const Square& sq) { return sq >= SQ_A1 && sq <= SQ_H8; }

}

#endif //TYPES_H
