{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE FunctionalDependencies #-}
{-# LANGUAGE MultiParamTypeClasses #-}

module Stack
  ( Stack
  , StackT(..)
  , MonadStack(..)
  , runStack
  , runStackT
  ) where

import Control.Monad.Identity
  (Identity, runIdentity)
import Control.Monad.State
  (StateT, runStateT, get, put, modify)
import Data.Maybe
  (fromJust)

type Stack s = StackT s Identity
newtype StackT s m a = StackT { unStack :: StateT [s] m a }

class Monad m => MonadStack s m | m -> s where
  push :: s -> m ()
  peek :: m (Maybe s)
  pop :: m (Maybe s)

  peek' :: m s
  peek' = fromJust <$> peek
  pop' :: m s
  pop' = fromJust <$> pop

instance Monad m => Functor (StackT s m) where
  fmap f = StackT . fmap f . unStack

instance Monad m => Applicative (StackT s m) where
  pure = StackT . pure
  f <*> x = StackT $ unStack f <*> unStack x

instance Monad m => Monad (StackT s m) where
  return = StackT . return
  x >>= f = StackT $ unStack x >>= unStack . f

instance Monad m => MonadStack s (StackT s m) where
  push = StackT . modify . (:)
  peek = StackT $ do
    s <- get
    return $ case s of
                  (x:_) -> Just x
                  _     -> Nothing
  pop = StackT $ do
    s <- get
    case s of
         (x:s') -> put s' >> return (Just x)
         _     -> return Nothing

runStackT :: StackT s m a -> m (a, [s])
runStackT = flip runStateT [] . unStack

runStack :: Stack s a -> (a, [s])
runStack = runIdentity . runStackT
