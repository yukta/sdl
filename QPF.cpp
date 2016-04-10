// The Quick Pink Fox - SPRITE ANIMATION PROJECT

/*			 
  PROJECT BY : 
 
 * ABHAY RAJ MALHOTRA (1215)

*/

//header file initialization

#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_image.h"
#include <string>
#include <fstream>

using namespace std;

const int w=1000;	// width of window
const int h=600; 	// height of the window
int SpriteAnimationoff=0;	// SpriteAnimationing offset of the background
int size=8;			// size of the sprite array
int score=0;		// total hurdles cross


SDL_Rect CatRect[8]; 			 // array of sprites
SDL_Window* window=NULL; 	    // original window platform
SDL_Renderer* renderer = NULL; // the image rendering structure 
Mix_Music *music = NULL;
Mix_Music *go=NULL;

class SpriteAnimation 
{
	public:
	
	SpriteAnimation(); 			// default constructor 
	bool loadpng(string path); // loads a png image
	void render(int x, int y, SDL_Rect* clip = NULL); // rendering background
	void free(); // cleans up space occupied by SDL structures
	int getwidth(); //returns width of the current surface 
	void catrender(int x, int y, SDL_Rect* clip); // renders the cat texture
	void classification(); // assigns parametric values to CatRect[8] structure
	void GameLoads(); // starts the game
	void move(); // moves the background 
	int velocity; // velocity of the background in pix / frame
	int PosX; // current position of the obstacle
	void gameover(); // ends the game
	
	private:
	
	int s_width; // width of the surface
	int s_height; // width of the height
	SDL_Texture* texture; // the actual texture loaded
};

SpriteAnimation bkg; //object for background
SpriteAnimation sprite; // object for cat rendering
SpriteAnimation starting; // object for starting game 
SpriteAnimation obj; // object for collision
SpriteAnimation over; // object for ending game


SpriteAnimation::SpriteAnimation() // default constructor
{
	texture =NULL; 
	s_width=s_height=0; // the actual texture width and height
	velocity=-9; // speed of 9 pix/frame in west direction
	PosX=w + 10000; // current position of the obstacle
}

void SpriteAnimation::free() 
{
	if(texture!=NULL) // if texture is currently occupied by another
	{
		SDL_DestroyTexture( texture ); 
		
		// reinitialization of all the variables to default values
		
		texture = NULL; 
		s_width = 0;
		s_height = 0;
		velocity=0;
		PosX=w;
	}
}


bool init()
{
	// Setting up SDL2 environment
	
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) <0) 
	return false;
	
	else
	{
		window = SDL_CreateWindow( "Quick Pink Fox", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN ); // defining and creating window
		
		if(window == NULL) // if window initialization fails
		{
		cout<< SDL_GetError();	// throws SDL Error
		return false;
		}
		
		else
		{
		renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ); // creating a 2d rendering context
		
		if(renderer == NULL)
		{
		cout<< SDL_GetError();	// throws SDL Error
		return false;
		}
		
		else
			{
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF ); // setting colours for drawing operations

				int imgFlags = IMG_INIT_PNG; // png flags 
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					IMG_GetError(); // throws IMG Error
					return false;
				}
				
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) 
				{ 
					cout<<Mix_GetError(); 
					return false;
					}
				
			}
		}
		
		
	}
	
	return true;
} // end of init

//loadig individual images
bool load()
{	
	if(!bkg.loadpng("/home/lalit/Desktop/QPF-SAproject/data/ground.jpg")) // loads background image
	{
	IMG_GetError();
	return false;
	}
	if(!sprite.loadpng("/home/lalit/Desktop/QPF-SAproject/data/cat.png")) // loads cat image
	{
	IMG_GetError();
	return false;
	}
	
	if(!starting.loadpng("/home/lalit/Desktop/QPF-SAproject/data/strt.jpg")) // loads starting image
	{
	IMG_GetError();
	return false;
	}
	
	if(!obj.loadpng("/home/lalit/Desktop/QPF-SAproject/data/obj.png")) // loads collision object image
	{
	IMG_GetError();
	return false;
	}
	
	if(!over.loadpng("/home/lalit/Desktop/QPF-SAproject/data/over.gif")) // loads end image
	{
	IMG_GetError();
	return false;
	}
	
	 music = Mix_LoadMUS( "/home/lalit/Desktop/QPF-SAproject/data/music.mp3" );
	 go = Mix_LoadMUS( "/home/lalit/Desktop/QPF-SAproject/data/go.mp3" );
	  if( music == NULL || go == NULL )
	   { 
		   cout<<Mix_GetError();
		   return false;
	  } 
	
	return true;
}

// png loading function
bool SpriteAnimation::loadpng(string path)
{
	//defining the surface
	SDL_Surface* surface = NULL;
	
	surface = IMG_Load( path.c_str() ); //loading the image
	
	if(surface == NULL)
	{
		cout<<SDL_GetError();
		return false;
	}
	
	else
	{
		SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0, 0xFF, 0xFF ) ); // creating a trasnsparent surface
		
		//creating a texture from the given surface
		texture=SDL_CreateTextureFromSurface( renderer, surface );
		
		if(texture == NULL) 
		{
			cout<<SDL_GetError();
			return false;
		}
		
		else 
		{
			//initialsizing the width and height of the surface
			s_width=surface->w; 
			s_height=surface->h;
		}
		
		SDL_FreeSurface(surface); // deallocating memory taken by surface
	}
	return true;
}

// classifying the sprite sheet
void SpriteAnimation:: classification()
{
		// setting up x, y, width and height from original image
		CatRect[ 0 ].x =   0;
		CatRect[ 0 ].y =   0;
		CatRect[ 0 ].w =  512;
		CatRect[ 0 ].h = 256;

		CatRect[ 1 ].x =  512;
		CatRect[ 1 ].y =   0;
		CatRect[ 1 ].w =  512;
		CatRect[ 1 ].h = 256;
		
		CatRect[ 2 ].x =  0;
		CatRect[ 2 ].y =   256;
		CatRect[ 2 ].w =  512;
		CatRect[ 2 ].h = 256;

		CatRect[ 3 ].x =  512;
		CatRect[ 3 ].y =   256;
		CatRect[ 3 ].w =  512;
		CatRect[ 3 ].h = 256;
	
		CatRect[ 4 ].x =  0;
		CatRect[ 4 ].y =   512;
		CatRect[ 4 ].w =  512;
		CatRect[ 4 ].h = 256;
		
		CatRect[ 5 ].x =  512;
		CatRect[ 5 ].y =   512;
		CatRect[ 5 ].w =  512;
		CatRect[ 5 ].h = 256;
		
		CatRect[ 6 ].x =  0;
		CatRect[ 6 ].y =   768;
		CatRect[ 6 ].w =  512;
		CatRect[ 6 ].h = 256;
		
		CatRect[ 7 ].x =  512;
		CatRect[ 7 ].y =   768;
		CatRect[ 7 ].w =  512;
		CatRect[ 7 ].h = 256;

}

//renders background
void SpriteAnimation:: render(int x, int y, SDL_Rect* clip)
{
	//defining destination rect
	SDL_Rect renderQuad = { x, y, s_width, h };
	
	// copies current rect to rendering context
	SDL_RenderCopy( renderer, texture, clip , &renderQuad);
}

// returns width
int SpriteAnimation::getwidth()
{
	return s_width; // returns the width of the current surface
}

// rendering the cat
void SpriteAnimation :: catrender(int x, int y, SDL_Rect* clip)
{
	SDL_Rect show = { x, y, s_width, s_height};
	
	if(clip!=NULL)
	{
		// reducing the size of the cat sprite to 1/2
		show.w=256;
		show.h=128;
	}
	
	SDL_RenderCopy( renderer, texture, clip, &show );
}

//moving the object
void SpriteAnimation:: move()
{
	PosX += velocity; // adding pixel drag value to the left
	
	
	if((PosX + 110) <0) // till the time the collision object disappears
	{
	PosX=w; // setting it up again to width 
	score++; // incrementing score
	}
	SDL_Rect show = {PosX, h-117, 110,70}; // rect for hurdle
	SDL_RenderCopy( renderer, texture, NULL, &show ); // copying current texture to current rendering context at desired destination
	
}


//regulating bkg speed
void bkgSpriteAnimation(int speed) // moving the background
{
		// scrolling offset of background
		SpriteAnimationoff-=speed;	
	
		// reinitialization
		if(SpriteAnimationoff< -bkg.getwidth())
		SpriteAnimationoff=0;
				
		SDL_RenderClear( renderer );
		// renders background from both sides so as to give a continuity
		
		// calling rendering function
		bkg.render(SpriteAnimationoff,0);
		bkg.render(SpriteAnimationoff + bkg.getwidth(), 0);
}


// Bounding Box + Additional Changes
bool CollDet(int pos_c) // the function detects for a collision between rect of cat and rect of object
{
	// the hurdle object rect
	SDL_Rect object = {obj.PosX,h-117,110,70};
	SDL_Rect cat = {(w-550)/2 , pos_c , 256 , 128};
	
	//  computing the top of object and bottom measure of cat
	int top_obj = object.y;
	int bottom_cat = cat.y + cat.h;
	
	// computing the righ and left of the object and the cat
	int left_obj = object.x;
	int right_obj = object.x + object.w;
	int right_cat = cat.x + 179;
	int mid_cat = cat.x + 95;
	
	//checks for only full front collision and half back collision
	
	if((bottom_cat >= top_obj) && (right_cat>=left_obj) && (right_obj >=  mid_cat))
	return true;
	
	return false;
}

// loading the game
void SpriteAnimation::GameLoads()
{
	//setting up renderer
	SDL_RenderClear( renderer );
	SDL_RenderCopy( renderer, texture, NULL, NULL );
	SDL_RenderPresent(renderer);
	
	bool flag=true; // for ending event loop
	SDL_Event E2; // event declaration
	while(flag)
	{
		while( SDL_PollEvent( &E2 ) != 0 ) 
				{
					if( E2.type == SDL_KEYDOWN ) // when a key is pressed down
					{
					flag = false;
					break;
					}	
				}
	}
}	


//ending the game
void SpriteAnimation::gameover()
{
		//clears the Rendering Context
		SDL_RenderClear( renderer );
		
		SDL_RenderCopy( renderer, texture, NULL, NULL );
		
		// Presents the given render
		SDL_RenderPresent(renderer);
		
		// Delay for 1.5 seconds	
		SDL_Delay(1500);
		
}

//closing SDL systems
void close()
{
	// cleaning up spaces occupied by different objects
	
	bkg.free();
	sprite.free();
	over.free();
	starting.free();
	obj.free();
	
	// destroying rendering context and original window
	
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;
	 Mix_FreeMusic( music ); 
	 music = NULL;
	 Mix_FreeMusic( go );
	 go=NULL;
	
	// closing up IMG and SDL system
	
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	
}

// OUTPUTS THE SCORE AS READ BY THE TEXT FILE	
void checkscore()
{
	fstream myReadFile;
 myReadFile.open("highscore.txt");
 string output;
 
 if (myReadFile.is_open()) 
 { 
 while (!myReadFile.eof()) 
 {
    myReadFile >> output;
 }
}
 int tot=0; int rem=1;
 for(int i = output.length() - 1; i>=0 ; i--)
 {
	 tot+= (output.at(i) - 48) * rem;
	 rem*=10;
 }	
 
cout<<"\n\n\nYour score is "<<score<<endl;

if(score>=tot)
{
 ofstream myfile;
  myfile.open ("highscore.txt");
  myfile << score;
  myfile.close();	
cout<< "Congratulations!! High Score\n";
}

}

int main() // the main function
{

	
	int level,speed,flag;  // defining levels and speeds for game
	flag=1;
	
	cout<<"           THe Quick Pink Fox                \n\n\n";
	cout<<"          Please choose a Level             \n";
	cout<<"                1 : Easy                     \n";
	cout<<"                2 : Medium                    \n";
	cout<<"                3 : Difficult                  \n";
	cin>>level; 
	
	if(level == 1)
	{
	speed = 8; // sets scrolling to 8
	}
	
	else if(level==2)
	{
	speed = 11; // sets scrolling to 11
	}
	
	else if(level==3)
	{
	speed=14; //sets scrolling to 14
	}
	
	else
	{
	cout<<"Wrong Input....";
	flag=0;
	}
	
	if(flag)
	{
	if(init()) // if init function returs true
	{
		if(load()) // if all surfaces have been loaded successfully
			{
				
				 if( Mix_PlayingMusic() == 0 )
				  {
					   //Play the music 
					   Mix_PlayMusic( music, -1 ); 
					}
				
				//loading the game image
				starting.GameLoads();	
					 
				sprite.classification(); // classification of the image sprite
				int ind=0; // the current index in within the catRect array
				
				
				bool gameover =false;
				SDL_Event e; // SDL Event
			
				while(!gameover)
				{
					while( SDL_PollEvent( &e ) != 0 )
				{
					
					//checks if the user gave a cross input
					if( e.type == SDL_QUIT)
					{
						gameover = true;
					}
					}// end of pollevent
	
	
	const Uint8 *keys = SDL_GetKeyboardState(NULL); // getting into pointers to the input key		
							
	// checking if shift is pressed
	if (keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT]) 
	{
	//boosting up the speed
	bkgSpriteAnimation(speed + 5); // moves up the background
	size=5;
	obj.velocity=-(speed + 5);
	}
	
	else  
	{
	// normal speed
	bkgSpriteAnimation(speed);
	size=9;
	obj.velocity=-speed;
	}			
			
	obj.move(); // moving the hurdle 
	
	if(e.key.repeat == 0 && keys[SDL_SCANCODE_UP] )
    {		
				// if the pressed button is 'up'
				SDL_Rect* clip =&CatRect[0];
				
				// reducing the value of height
				sprite.catrender((w-550)/2 , (h)/2 , clip);
				
				//presenting the rendered context
				SDL_RenderPresent(renderer);

                ind=0;
                				
				// calling the CollDet function
				if(CollDet((h+50)/2))
				{ 
					
				SDL_Delay(1500);
				Mix_HaltMusic();
				
				if( Mix_PlayingMusic() == 0 )
   			    {
				   Mix_PlayMusic( go, -1 ); 
				}
				
				over.gameover();
				SDL_Delay(2000);
				Mix_HaltMusic();
				break;
				}
	   } 
	           
    
    else 
    {   
		 SDL_Rect* clip =&CatRect[ind/size];
	     sprite.catrender((w-550)/2 , (h+250)/2 , clip); 
	     SDL_RenderPresent(renderer);
		if(CollDet((h+250)/2))
		{
			SDL_Delay(2000);
			Mix_HaltMusic();
			
			if( Mix_PlayingMusic() == 0 )
   			    {
				   Mix_PlayMusic( go, -1 ); 
				}
				
			over.gameover();
			SDL_Delay(2000);
			Mix_HaltMusic();
			break;
		}
	}
				++ ind; // incrementing ind for next sprite 
			
				if(ind/size >=8 ) // bringing ind to start
				ind=0;	
			
				
				} // end of gameover 
			} // end of load
	}// end of init loop
	
	
	close();
	
	checkscore();
	} // end of if
	return 0; 
}
