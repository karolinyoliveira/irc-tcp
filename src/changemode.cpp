#include "../lib/utils.hpp"

int clear_icanon(void)
{
  struct termios settings;
  int result;
  result = tcgetattr (STDIN_FILENO, &settings);
  if (result < 0)
    {
      perror ("error in tcgetattr");
      return 0;
    }

  settings.c_lflag &= ~ICANON;
  result = tcsetattr (STDIN_FILENO, TCSANOW, &settings);
  if (result < 0)
    {
      perror ("error in tcsetattr");
      return 0;
   }
  return 1;
}