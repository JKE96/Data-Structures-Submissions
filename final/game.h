  
void die(FRESULT rc);

int play(char *name);

void transition();
void deleteOld();

void initMapOne();
void initMapTwo();

void createMapOne();

void drawLife();
void coinCheck();
void gravityMon();


void jump();
void jumpMon();

void boxes(int xValue,int yValue,int zValue,int xPrev,int yPrev,int zPrev);

void drawMonsterOne();
void drawMonsterWhite();
void drawDoor();

void drawCoins();
void drawPlayer();
void drawPlayerWhite();

void onTick();


void runLevelOne();
void startGame();
void runLevelTwo();

void runLevelThree();

void monsterAI();
void moveMonster();

void checkCoins();

void movePlayer();

int hitObject(int ex1,int ex2,int ey1,int ey2);
int hitObjectMon(int ex1,int ex2, int ey1,int ey2);

int xMovCheck();

int xMovCheckMon();

int yMovCheck();
int yMovCheckMon();


void death();

void gravity();