char** tokenize(char* str, const char delim);
void print_all_tokens(char** tokens);
char *copy_str(char *inStr, short len);
int count_tokens(char* str, const char delim);
char *end_word(char* str, const char delim);
char *word_start(char* str, const char delim);
bool non_delim_character(char c, const char delim);
bool delim_character (char c, const char delim);
