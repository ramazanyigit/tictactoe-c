#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
//#include <SDL/SDL_ttf.h>
#include <windows.h>

static void GM_Init();
static void GM_Draw();
static SDL_Surface* GM_LoadIMG(char*);
static void GM_ShowSurface(SDL_Surface*,SDL_Surface*,int,int);
static void GM_KeyControl();
static void GM_Close();

// Screen Details
const int SCREEN_W     = 600;
const int SCREEN_H     = 640;
const int SCREEN_BPP   = 32;
static SDL_Surface* GM_Screen = NULL;
SDL_Surface* imageb;
SDL_Surface* imagem;
SDL_Surface* imageo;
SDL_Surface* imagex;

// Game Variables
int quit=0;
int sira=0;
int blocks=0;
int game_ai=1;
int end=0;
int pause=0;
int bloklar[3][3];
SDL_Event GM_Event;
short koseler1[4] = {0,0,2,2};
short koseler2[4] = {0,2,0,2};

short combinations[8][6] = {
      {0,0,1,1,2,2},
      {0,2,1,1,2,0},
      {0,0,1,0,2,0},
      {0,1,1,1,2,1},
      {0,2,1,2,2,2},
      {0,0,0,1,0,2},
      {1,0,1,1,2,1},
      {2,0,2,1,2,2}


};

int main(int argc, char **argv) {
    GM_Init();
    while(!quit) {
        GM_KeyControl();
        GM_Draw();
    }
    GM_Close();
}

static void GM_Init() {
    if( SDL_Init(SDL_INIT_EVERYTHING) == -1 ) {
        MessageBox(0,"SDL Grafik Kütüphanesi yüklenemedi!","Hata",MB_OK);
        exit(1);
    }
        
    //MessageBox(0,"..:: Bilgi ::..\nBu oyun deneme aþamasýndadýr!\n\nOyun Adý: Tic Tac Toe\nSürüm: 1.0.0\nYapýmcý: RamazanYigit.", "SDL Game Test", MB_OK);
    
    GM_Screen = SDL_SetVideoMode( SCREEN_W,SCREEN_H,SCREEN_BPP,SDL_SWSURFACE | SDL_DOUBLEBUF);
    if(GM_Screen == NULL) {
        MessageBox(0,"SDL Grafik Ekraný oluþturulamadý!","Hata",MB_OK);
        exit(1);             
    }
    SDL_WM_SetCaption("TicTacToe - RamazanYigit.", "./res/icon.bmp");
    Uint32          colorkey;
    SDL_Surface     *image;

    image = SDL_LoadBMP("./res/icon.bmp");
    image = SDL_DisplayFormatAlpha(image);
    SDL_WM_SetIcon(image,NULL);
    
    // Loads Resources
    imageb = GM_LoadIMG("./res/back.png");
    imagem = GM_LoadIMG("./res/menu.png");
    imagex = GM_LoadIMG("./res/x.png");
    imageo = GM_LoadIMG("./res/o.png");
    
    int i = 0;
    for(i=0; i < 3; i++) {
        bloklar[i][0] = 0;
        bloklar[i][1] = 0;
        bloklar[i][2] = 0;
    }
    
    // Main Background
    GM_ShowSurface(imagem,GM_Screen,0,0);
    GM_ShowSurface(imageb,GM_Screen,0,40);
    
}

static void GM_ResetRound() {
    int i = 0;
    for(i=0; i < 3; i++) {
        bloklar[i][0] = 0;
        bloklar[i][1] = 0;
        bloklar[i][2] = 0;
    }
    sira = 0;
    end = 0;
    blocks = 0;
    SDL_FillRect(GM_Screen,NULL,0x000000);
    GM_ShowSurface(imagem,GM_Screen,0,0);
    GM_ShowSurface(imageb,GM_Screen,0,40);
}

static void GM_Draw() {
    if( SDL_Flip(GM_Screen) == -1 ) {
		MessageBox(0,"Uygulamada bir problem oluþtu. Uygulama durduruldu!", "Hata", MB_OK);
        exit(1);    
    }
}
static void GM_ControlGame() {
    int block = sira == 0 ? 1 : 2;
        if(!end)  {
             int durum = (bloklar[0][0] == block && bloklar[1][0] == block && bloklar[2][0] == block) || (bloklar[0][1] == block && bloklar[1][1] == block && bloklar[2][1] == block) || (bloklar[0][2] == block && bloklar[1][2] == block && bloklar[2][2] == block) || (bloklar[0][0] == block && bloklar[0][1] == block && bloklar[0][2] == block) || (bloklar[1][0] == block && bloklar[1][1] == block && bloklar[1][2] == block) || (bloklar[2][0] == block && bloklar[2][1] == block && bloklar[2][2] == block) || (bloklar[0][0] == block && bloklar[1][1] == block && bloklar[2][2] == block) || (bloklar[0][2] == block && bloklar[1][1] == block && bloklar[2][0] == block); 
             if(durum) {
                      if( SDL_Flip(GM_Screen) == -1 ) {
                          exit(1);                    
                      }
                      end = 1;
                      if( block == 1 ) MessageBox(0,"Oyuncu 1 kazandý!","Bilgi",MB_OK);
                      else MessageBox(0,"Oyuncu 2 kazandý!","Bilgi",MB_OK);
             }
        }     
    if(end == 0 && blocks > 8) {
           if( SDL_Flip(GM_Screen) == -1 ) {
             exit(1);                    
           }
           end = 1;
           MessageBox(0,"Oyun berabere!","Bilgi",MB_OK);
    }
}
static void GM_CompRand() {
    int rand_block[2];
    int ix; int ix2; int durum=0; int bloksayi=0,randn=0;
    
    for(ix2 = 0; ix2 < 8; ix2++) {
            bloksayi=0;
            
            if(bloklar[combinations[ix2][0]][combinations[ix2][1]] != 0 && bloklar[combinations[ix2][0]][combinations[ix2][1]] == 2) bloksayi++;
            if(bloklar[combinations[ix2][2]][combinations[ix2][3]] != 0 && bloklar[combinations[ix2][2]][combinations[ix2][3]] == 2) bloksayi++;
            if(bloklar[combinations[ix2][4]][combinations[ix2][5]] != 0 && bloklar[combinations[ix2][4]][combinations[ix2][5]] == 2) bloksayi++;
            if(bloklar[combinations[ix2][0]][combinations[ix2][1]] != 0 && bloklar[combinations[ix2][0]][combinations[ix2][1]] == 1) bloksayi--;
            if(bloklar[combinations[ix2][2]][combinations[ix2][3]] != 0 && bloklar[combinations[ix2][2]][combinations[ix2][3]] == 1) bloksayi--;
            if(bloklar[combinations[ix2][4]][combinations[ix2][5]] != 0 && bloklar[combinations[ix2][4]][combinations[ix2][5]] == 1) bloksayi--;
            if(bloksayi == 2 || bloksayi == -2) {
                 randn = rand() % 3;
                 while(bloklar[combinations[ix2][2*randn]][combinations[ix2][(randn*2)+1]] > 0) randn = rand() % 3;
                 rand_block[0] = combinations[ix2][2*randn];
                 rand_block[1] = combinations[ix2][(2*randn)+1]; 
                 durum = 1;
                 break;
            }
            
    }
    
    if(durum == 0) { 
        for(ix=0; ix < 4; ix++) {
            if(bloklar[koseler1[ix]][koseler2[ix]] < 1) {
                rand_block[0] = koseler1[ix];
                rand_block[1] = koseler2[ix];
                durum = 1;
                break;
            } 
        }
    }
    
    while(bloklar[rand_block[0]][rand_block[1]] > 0) {
        rand_block[0] = rand() % 3;
        rand_block[1] = rand() % 3;
    }
    bloklar[rand_block[0]][rand_block[1]] = sira == 0 ? 1 : 2;
    GM_ShowSurface(sira == 0 ? imagex : imageo,GM_Screen,(rand_block[0]*200),(rand_block[1]*200)+40);
    GM_ControlGame();
    if(sira == 0) sira = 1; else sira = 0;
    blocks++;
}
static void GM_KeyControl() {
    SDL_EnableKeyRepeat(0,0);
    SDL_PollEvent(&GM_Event);
    if( GM_Event.type == SDL_QUIT ) quit = 1;
    else if( GM_Event.type == SDL_MOUSEBUTTONDOWN) {
        if(GM_Event.button.button == SDL_BUTTON_LEFT && !end) {
            int blocknum1,blocknum2;
            blocknum1 = (GM_Event.button.x)/200 == 3 ? 2 : (int)(GM_Event.button.x)/200;
            blocknum2 = (GM_Event.button.y - 40)/200 == 3 ? 2 : (int)(GM_Event.button.y-40)/200;
            if(bloklar[blocknum1][blocknum2] < 1) {
                 bloklar[blocknum1][blocknum2] = sira == 0 ? 1 : 2;
                 GM_ShowSurface(sira == 0 ? imagex : imageo,GM_Screen,blocknum1*200,(blocknum2*200)+40);  
                 blocks++;
                 GM_ControlGame();
                 if(sira == 0) sira = 1; else sira = 0; 
                 if(blocks < 9 && game_ai == 1 && !end) GM_CompRand();
                 if(blocks > 8 || end) {
                           if( SDL_Flip(GM_Screen) == -1 ) {
                                 exit(1);                    
                           }
                           GM_ResetRound();
                 }                            
            }
        }
    }
    else if( GM_Event.type == SDL_KEYUP ) {
		switch( GM_Event.key.keysym.sym ) {
			default: { break; }
			case SDLK_ESCAPE: {
				if(!pause) { 
					pause = 1;
				} else { 
					pause = 0;
				}
				break;
			}
			case SDLK_F2: {
				GM_ResetRound();
				break;
			}
			case SDLK_F4: {
                 if(game_ai == 1) { game_ai = 0; MessageBox(0,"AI deactivated!","Bilgi",MB_OK); } else { game_ai = 1; MessageBox(0,"AI activated!","Bilgi",MB_OK); }
                 GM_ResetRound();
            }
		}
	}
}

static void GM_Close() {
       SDL_Quit();
       exit(0);       
} 

static SDL_Surface* GM_LoadIMG(char* src) {
	SDL_Surface* imgtemp = NULL; SDL_Surface* imgretn = NULL;
	imgtemp = IMG_Load(src);
	if(imgtemp != NULL) {
		imgretn = SDL_DisplayFormatAlpha(imgtemp);
        SDL_FreeSurface(imgtemp);          
	}
    return imgretn;
}

static void GM_ShowSurface(SDL_Surface* src, SDL_Surface* dst, int x, int y) {
	SDL_Rect the_rect;
	the_rect.x = x;
	the_rect.y = y;
	SDL_BlitSurface(src, NULL, dst, &the_rect);
}
