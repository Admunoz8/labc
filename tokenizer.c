#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return true (non-zero) if c is a whitespace character
   ('\t', ' ').
   Zero terminators are not printable (therefor false).
 */
bool delim_character (char c, const char delim){
  if (c == '\t' || c == delim)
    return true;
  else
    return false;
}

/* Return true (non-zero) if c is a non-whitespace character
   (not tab or space). Zero terminators are not printable
   (therefore false).
*/
bool non_delim_character(char c, const char delim){
  if (c == '\t' || c == '\0' || c == delim)
    return false;
  else
    return true;
}

/* Returns a pointer to the first character of the next
   space-separated word.
*/
char *word_start(char* str, const char delim){
  while(delim_character(*str, delim))
    str++;
  return str;
}

/* Returns a pointer to the first space character of the zero
   terminated string.
*/
char *end_word(char* str, const char delim){
  while(non_delim_character(*str, delim))
    str++;
  return str;
}

/* counts the number of words or tokens*/
int count_tokens(char* str, const char delim){
  int count = 0;
  while(*str != '\0'){
    if (delim_character(*str, delim))
      count++;
    str++; // moves pointer along the str array.
  }
  count++; // accounts for the terminator.
  return count;
}

/* Returns a freshly allocated zero-terminated vector of freshly 
   allocated space-seperated tokens from zero-terminated str.
   For example, tokenize("hello world string") would result in:
     tokens[0] = "hello"
     tokens[1] = "world"
     tokens[2] = "string"
     tokens[3] = 0
*/
char *copy_str(char *inStr, short len){
  /*Allocates enough memory for an array of chars the size of inStr*/
  char *copy = (char*)malloc((len)*sizeof(char)); 
  for (int i = 0; i < len; i++)
    *(copy+i) = *(inStr+i);
  *(copy+len) = '\0';
  return copy;
}

char** tokenize(char* str, const char delim){
  int count = count_tokens(str, delim);
  /*Allocates enough memory for an array containing pointers to other arrays.*/
  char **arrayOfPointers = (char**)malloc((count+1)*sizeof(char*));
  char *end;
  char *start = word_start(str, delim);
  int i;
  for (i = 0; i < count; i++){
    end = end_word(start, delim);
    arrayOfPointers[i] = copy_str(start, (end-start)); //end-start finds the offset for the token
    start = word_start(end, delim);
  }
  arrayOfPointers[i] = NULL; // makes the last item in arrayOfPointers = NULL.
  return arrayOfPointers;
}

void print_all_tokens(char** tokens){
  for (int i = 0; *tokens; i++){
    printf("Tokens[%d]: %s\n", i, *tokens);
    *tokens++;
  }
}
