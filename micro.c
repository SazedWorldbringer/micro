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
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag &= ~(CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();

  // read input from the terminal
  while (1) {
    char c = '\0';
    read(STDIN_FILENO, &c, 1);
    if (iscntrl(c)) {
      // print the ascii code of the character pressed
      printf("%d\r\n", c);
    } else {
      // print the character it represents if it isn't a control character
      printf("%d ('%c')\r\n", c, c);
    }
    if(c == 'q') break;
  }

  return 0;
}
