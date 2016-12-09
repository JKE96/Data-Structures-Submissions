

void die (FRESULT rc) ;

int play(char *name);

//Display the current files 
void display_and_store();

//Displays the selector for the file to play from the SD card based on the offset you pass it, which should be the current file to play
void selector_display(int offset) ;

//checks if the current file has a .wav extension
int check_extension();
int playmain(void);
