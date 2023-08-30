#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// original terminal attributes
struct termios orig_termios;

void disableRawMode() {
  // reset terminal to original attributes when disabling raw mode
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
  // disable raw mode when exiting (from main, or from the exit function)
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);

  // update original attributes, disable echoing input and read input
  // byte-by-byte
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(IXON);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();

  // read input from the terminal
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
    if (iscntrl(c)) {
      // print the ascii code of the character pressed
      printf("%d\n", c);
    } else {
      // print the character it represents if it isn't a control character
      printf("%d ('%c')\n", c, c);
    }
  }

  return 0;
}
