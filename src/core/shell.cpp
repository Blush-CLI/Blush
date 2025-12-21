#include <blush.hpp>
#include <commands/base.hpp>
#include <core/history.hpp>
#include <core/job.hpp>
#include <core/shell.hpp>
#include <print>
#include <tui/colors.hpp>
#include <utils/file.hpp>
#include <utils/input.hpp>
extern Audio audio;
History *Shell::currentHistory = nullptr;

void Shell::Init() {
  this->audio = &::audio;
  currentHistory = &history;
  std::println("{}[  OK  ]{} Shell initialized!{}", Color::Green, Color::Purple,
               Color::Reset);
  int rcode = run();
  D_PRINTLN("Shell process exited with code {}", rcode);
}

History *Shell::getHistory() { return currentHistory; }

void printPrompt() {
  std::print("{} [{}] {} > {}", Color::Purple, File::lcwd(), Color::Blue,
             Color::Reset);
  // std::cout
  //         << Color::Purple
  //         << "["
  //         << File::lcwd()
  //         << "]"
  //         << " "
  //         << Color::Blue
  //         << "> "
  //         << Color::Reset
  //         << std::flush;
}

int Shell::run() {
  audio->play("./music/jump.wav");
  while (true) {
    std::string command = Input::readLine(history, printPrompt);

    if (command.empty())
      continue;

    Command cmd(command);
    if (cmd.mainCommand == "exit")
      break;

    if (commands_map.find(cmd.mainCommand) != commands_map.end()) {
      commands_map[cmd.mainCommand].func(cmd);
    } else {
      Job job(command);
      bool result = job.run();
      if (!result) {
        audio->play("jump.wav");
        std::println("Error: '{}' | Exit code: {}", cmd.mainCommand,
                     job.exitCode);
      }
    }
  }
  return 0;
}

Shell::~Shell() { std::println("Goodbye!"); }
