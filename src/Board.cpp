//
// Created by Eskild on 03.11.2024.
//

#include "Board.h"

#include <algorithm>
#include <bitset>
#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace DeepKnight {

   Board::Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {}

   Board::Board(std::string fenString) :
       m_fenString(std::move(fenString)), m_bitBoards(), m_colorToPlay(WHITE), m_whiteCastled(false), m_blackCastled(false),
       m_passantTargetSquare(), m_castleState(0b0000), m_movesPlayed(0), m_halfMoveClock()
   {
      parseFenString(m_fenString);

      /* Set "good to have" bitboards */
      m_bitBoards[13] = m_bitBoards[W_PAWN] | m_bitBoards[W_KNIGHT] | m_bitBoards[W_BISHOP] | m_bitBoards[W_ROOK] |\
                        m_bitBoards[W_QUEEN] | m_bitBoards[W_KING];
      m_bitBoards[14] = m_bitBoards[B_PAWN] | m_bitBoards[B_KNIGHT] | m_bitBoards[B_BISHOP] | m_bitBoards[B_ROOK] |\
                        m_bitBoards[B_QUEEN] | m_bitBoards[B_KING];
      m_bitBoards[15] = m_bitBoards[13] | m_bitBoards[14];


   }
   Board::~Board()
   {
      std::cout << "Color to play: " << m_colorToPlay << "\nCastle state: "  << m_castleState  <<\
         "\nPassant index: " << m_passantTargetSquare << "\nCloks: <"<< m_halfMoveClock << ", " << m_movesPlayed << ">" << std::endl;
   }

   std::string Board::printBitBoard() const { return printBitBoard(getAllPieces()); }

   std::string Board::printBitBoard(const Bitboard &b) const /* TODO: fix input usage */
   {
      std::string s = "+---+---+---+---+---+---+---+---+\n";
      for (Rank r = RANK_8; r >= RANK_1; --r) {
         for (File f = FILE_A; f <= FILE_H; ++f) {
            const Square square = makeSquare(r, f);
            const Piece piece = getPieceOnSquare(square);
            const char fenChar = piece != PIECE_NONE ? m_fenPieces[piece] : ' ';
            s += std::string{"|  "}.insert(2, &fenChar, 1);
         }
         s += "| " + std::to_string(1 + r) + "\n+---+---+---+---+---+---+---+---+\n";
      }
      s += "  a   b   c   d   e   f   g   h\n";
      return s;
   }

   void Board::parseFenString(const std::string &fenString)
   {
      std::array<std::string, 6> fenTokens;
      std::stringstream ss(fenString);

      /* Read every token in the FEN string */
      for (auto &entry: fenTokens) {
         ss >> entry;
      }

      setBitBoard(fenTokens[0]);
      fenTokens[1] == "w" ? m_colorToPlay = WHITE : m_colorToPlay = BLACK;
      setCastleState(fenTokens[2]);
      fenTokens[3] == "-" ? m_passantTargetSquare = SQ_NONE : m_passantTargetSquare = getSquareFromString(fenTokens[3]);
      m_halfMoveClock = std::stoi(fenTokens[4]);
      m_movesPlayed = std::stoi(fenTokens[5]);
   }

   void Board::setBitBoard(const std::string& token)
   {
      /* Extract every rank and set bitBoards */
      Rank i{RANK_8};
      std::string rankString;
      std::stringstream ss{token};
      std::array<std::string, 8> boardPosition;

      while (std::getline(ss, rankString, '/')) {
         boardPosition[i] = rankString;
         --i;
      }

      for (Rank r = RANK_8; r >= RANK_1; --r) {
         setBitBoard(boardPosition[r], r);
      }
   }

   void Board::setBitBoard(const std::string &rankString, const Rank &rank)
   {
      std::uint8_t space{0};
      for (const auto &piece: rankString) {
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

   Piece Board::getPieceOnSquare(const Square &sq) const
   {
      const Bitboard index = getBoardIndex(sq);
      for (Piece piece{W_PAWN}; piece <= B_KING; ++piece) {
         if (index & m_bitBoards[piece]) return piece;
      }
      return PIECE_NONE;
   }

   void Board::setCastleState(const std::string &fenString)
   {
      for (const auto &entry: fenString) {
         switch (entry) {
            case ('K'):
               m_castleState |= 0x1;
               break;
            case ('Q'):
               m_castleState |= 0x2;
               break;
            case ('k'):
               m_castleState |= 0x4;
               break;
            case ('q'):
               m_castleState |= 0x8;
               break;
            default:
               return;
         }
      }
   }

   Square Board::getSquareFromString(const std::string& token) const {
      return Square{(token[0] - 'a') + ((token[1] - '0' - 1) * 8)};
   }

   std::string Board::getStringFromSquare() const
   { /* TODO: implement */
      return "yoyo";
   }

   std::ostream &operator<<(std::ostream &strm, const Board &brd) { return strm << brd.printBitBoard(); }
} // namespace DeepKnight
