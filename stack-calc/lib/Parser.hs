module Parser
  ( parseCalculation
  ) where

import Control.Applicative
  ((<|>), many)
import Control.Monad.Identity
  (Identity)
import Text.Parsec
  (ParsecT, anyChar, try, noneOf, spaces, eof)
import Text.Read
  (readMaybe)

import Math
  (fact)
import Token
  (Token(..))

type Parser = ParsecT String () Identity

parseToken :: Parser String
parseToken = do
  spaces
  s <- many $ noneOf " \t\n"
  spaces
  return s

parseOperation :: (Num a, Integral a) => Parser (Token a)
parseOperation = try $ do
  c <- parseToken
  case c of
       "+" -> return . BinOp "+" $ (+)
       "-" -> return . BinOp "-" $ (-)
       "*" -> return . BinOp "*" $ (*)
       "/" -> return . BinOp "/" $ div
       "^" -> return . BinOp "^" $ (^)
       "!" -> return . UnaryOp "!" $ fact
       _   -> fail $ "could not parse binary operation: " ++ show c

parseInteger :: (Read a, Integral a) => Parser (Token a)
parseInteger = try $ do
  t <- parseToken
  case readMaybe t of
       Nothing -> fail $ "could not parse integer: " ++ show t
       Just x  -> return . Numeral $ x

parseCalculation :: (Read a, Num a, Integral a) => Parser [Token a]
parseCalculation = many parseSingle <* eof -- ensure we parse the entire line
  where parseSingle = parseOperation <|> parseInteger
