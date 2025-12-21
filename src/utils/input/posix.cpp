#include <cstdio>
#include <termios.h>
#include <unistd.h>
#include <utils/input.hpp>
#include <functional>
#include <print>

class TermiosGuard {
public:
  TermiosGuard() { tcgetattr(STDIN_FILENO, &oldTerm); }

  ~TermiosGuard() { tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm); }

  void setRawMode() {
    struct termios newTerm = oldTerm;
    newTerm.c_lflag &= ~(ICANON | ECHO | ISIG);
    newTerm.c_iflag &= ~(IXON | IXOFF | IXANY);
    newTerm.c_cc[VMIN] = 1;
    newTerm.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);
  }

private:
  struct termios oldTerm;
};

char Input::readchar() {
  TermiosGuard guard;
  guard.setRawMode();
  return getchar();
}

std::string Input::readamount(int n) {
  TermiosGuard guard;
  guard.setRawMode();

  std::string result;
  for (int i = 0; i < n; ++i) {
    char ch = getchar();
    if (ch == EOF)
      break;
    result += ch;
  }
  return result;
}

Char Input::readkey() {
  TermiosGuard guard;
  guard.setRawMode();

  Char result;
  char ch = getchar();

  if (ch == 27) {
    struct termios newTerm;
    tcgetattr(STDIN_FILENO, &newTerm);
    newTerm.c_lflag &= ~(ICANON | ECHO | ISIG);
    newTerm.c_iflag &= ~(IXON | IXOFF | IXANY);
    newTerm.c_cc[VMIN] = 0;
    newTerm.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);

    char next = getchar();
    if (next == '[') {
      char arrow = getchar();
      switch (arrow) {
      case 'A':
        result.escape = EscapeKey::UpArrow;
        break;
      case 'B':
        result.escape = EscapeKey::DownArrow;
        break;
      case 'C':
        result.escape = EscapeKey::RightArrow;
        break;
      case 'D':
        result.escape = EscapeKey::LeftArrow;
        break;
      case '1':
        if (getchar() == '~')
          result.escape = EscapeKey::Home;
        break;
      case '3':
        if (getchar() == '~')
          result.escape = EscapeKey::Delete;
        break;
      case '4':
        if (getchar() == '~')
          result.escape = EscapeKey::End;
        break;
      case '5':
        if (getchar() == '~')
          result.escape = EscapeKey::PgUp;
        break;
      case '6':
        if (getchar() == '~')
          result.escape = EscapeKey::PgDn;
        break;
      }
    } else {
      result.escape = EscapeKey::Escape;
    }
  } else {
    result.character = ch;
    if (ch == 127 || ch == 8)
      result.escape = EscapeKey::Backspace;
  }

  return result;
}


std::string Input::readLine(History &history, const std::function<void()>& printPrompt) {
  std::string command;
  int curPos = 0;
  bool done = false;

  printPrompt();
  std::print("");

  while (!done) {
    Char c = Input::readkey();

    switch (c.escape) {
    case EscapeKey::Escape:
      break;

    case EscapeKey::UpArrow: {
      std::string histCmd = history.getPrevious();
      if (!histCmd.empty()) {
        command = histCmd;
        curPos = command.size();
        std::print("\r\x1b[2K");
        printPrompt();
        std::print("{}", command);
      }
    } break;

    case EscapeKey::DownArrow: {
      std::string histCmd = history.getNext();
      command = histCmd;
      curPos = command.size();
      std::print("\r\x1b[2K");
      printPrompt();
      std::print("{}", command);
    } break;

    case EscapeKey::LeftArrow:
      if (curPos > 0) {
        std::print("\x1b[D");
        curPos--;
      }
      break;

    case EscapeKey::RightArrow:
      if (curPos < (int)command.size()) {
        std::print("\x1b[C");
        curPos++;
      }
      break;

    case EscapeKey::Backspace:
      if (curPos > 0) {
        command.erase(curPos - 1, 1);
        curPos--;

        std::print("\r\x1b[2K");
        printPrompt();
        std::print("{}", command);

        std::print("\r");
        printPrompt();
        std::print("{}", command.substr(0, curPos));
      }
      break;

    default:
      if (c.character == '\n' || c.character == '\r') {
        std::println("");
        done = true;
        break;
      }

      if (c.character) {
        command.insert(curPos, 1, c.character);
        curPos++;

        std::print("\r\x1b[2K");
        printPrompt();
        std::print("{}", command);

        std::print("\r");
        printPrompt();
        std::print("{}", command.substr(0, curPos));
      }
      break;
    }
  }

  if (!command.empty()) {
    history.add(command);
    history.reset();
  }

  return command;
}
