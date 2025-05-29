// GameException.hpp
#pragma once

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message)
        : std::runtime_error(message) {}
};

class InvalidNameException : public GameException {
public:
    explicit InvalidNameException(const std::string& name)
        : GameException("Nume invalid: " + name) {}
};

class NotEnoughGoldException : public GameException {
public:
    explicit NotEnoughGoldException(int gold)
        : GameException("Nu ai destui banuti, cont curent: " + std::to_string(gold)) {}
};

class InvalidActionException : public GameException {
public:
    explicit InvalidActionException(const std::string& action)
        : GameException("Actiune invaliday: " + action) {}
};
