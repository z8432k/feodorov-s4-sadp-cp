#include <string.h>
#include "wordsearch.h"

gssize wordsearch(const wchar_t *target, const wchar_t *text)
{
  gsize target_pos = 0;
  gsize text_pos = 0;
  gsize text_view_pos = 0;

  wchar_t target_sym, text_sym;

  do {
    target_sym = target[target_pos];
    text_sym = text[text_view_pos];

    if (target_sym == text_sym) {
      if (target_sym == '\0') {
        return text_pos;
      }

      target_pos++;
      text_view_pos++;
    }
    else {
      if (target_sym == '\0') {
        if (target_pos > 0) {
          return text_pos;
        }
        else {
            break;
        }
      }

      target_pos = 0;
      text_view_pos = ++text_pos;
    }

  } while (text_sym != '\0');

  return -1;
}
