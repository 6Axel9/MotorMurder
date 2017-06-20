//
//! \file hello_demo.c
//!   Screen-entry text demo
//! \date 20060228 - 20080416
//! \author cearn
//
// === NOTES ===

#include <stdio.h>
#include <stdlib.h>
#include <tonc.h>
#include <time.h>
#include <string.h>
#include "Background.h"
#include "SpriteSheet.h"

OBJ_ATTR obj_buffer[2048];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;
//==================================================================== Declare Functions
void initialize();		//Initialize Sprite
void inputDetect();		//Input Detection
void objectsDisplay();	//Set Object To Draw
void gameBorders();		//In-Game Borders & Limiters
void collisionDetect();	//Collision Detection
void resetGameplay();	//Reset All Variables
//========================================================================================================= Collider Function
bool collider(int A1,int B1,int C1,int D1,int A2,int B2,int C2,int D2);					//Collider
//========================================================================================================= Declare UI Functions
void damagebar(u32 ID,u32 Pal,OBJ_ATTR* Name,OBJ_ATTR* Name2,int X,int Y,int Damage,int Number);		//Damage Bar
void scorebar(u32 ID,u32 Pal,OBJ_ATTR* Name,OBJ_ATTR* Name2,int X,int Y,int Number);					//Score
void bestTry(u32 ID,u32 Pal,OBJ_ATTR* Name,OBJ_ATTR* Name2,int X,int Y,int Last,int New,int Number);	//Comment
//========================================================================================================= Declare Objects Functions
void player(u32 ID,u32 Pal,OBJ_ATTR* Name,int X,int Y,bool Shield,int Number);				//Player Car
void traffic(u32 ID,u32 Pal,OBJ_ATTR* Name,int X,int Y,bool Big,int Speed,int Number);		//Traffic Cars
void pickups(u32 ID,u32 Pal,OBJ_ATTR* Name,int X,int Y,bool Chance,int Speed,int Number);	//Pick-Ups
void weapons(u32 ID,u32 Pal,OBJ_ATTR* Name,int X,int Y,bool Wield,int Side,int Number);	//Weapons
void bullets(u32 ID,u32 Pal,OBJ_ATTR* Name,int X,int Y,bool Shot,int Speed,int Number);	//Bullets
void explosions(u32 ID,u32 Pal,OBJ_ATTR* Name,int X,int Y,bool Boom,int Speed,int Number);	//Explosions
//========================================================================================================= Objects Names
OBJ_ATTR* Player;		OBJ_ATTR* Racecar;		OBJ_ATTR* DamageBar2;
OBJ_ATTR* Motorcicle;	OBJ_ATTR* Flashycar;	OBJ_ATTR* ScoreBar;
OBJ_ATTR* Truck;		OBJ_ATTR* Pickup;		OBJ_ATTR* ScoreBar2;
OBJ_ATTR* Weapon;		OBJ_ATTR* Bullet;		OBJ_ATTR* BestTry;
OBJ_ATTR* Explosion;	OBJ_ATTR* DamageBar;	OBJ_ATTR* BestTry2;
//==================================================================== Declare Variables
int i,k,j;										//Looping Variables
int timer;
//==================================================================== Game Data
int gamestate=0,		offScreen=-64;			//Gamestate//Vanish
int scroll_X=8,			scroll_Y=0;				//Scroll BackGround X & Y
bool restart;			char HighScore[50];		//Printable Score
//==================================================================== Damage Bar Data
u32 damagebar_ID=0, 	damagebar_Pal=0;
int damagebar_X=0, 		damagebar_Y=92;	
int damage=0;
//==================================================================== Score Bar Data
u32 scorebar_ID=20, 	scorebar_Pal=0;
int scorebar_X=7, 		scorebar_Y=92+48;
int newScore=0,			bestScore=0;
//==================================================================== Comment Data
u32 bestTry_ID=148, 	bestTry_Pal=0;
int bestTry_X=15, 		bestTry_Y=92+32;
//==================================================================== Player Car Data
u32 player_ID=14,		player_Pal=0;
int player_SizeX=16,	player_SizeY=32;
int player_X=112,		player_Y=126;
bool isShield;
//==================================================================== Pick_Ups Data
u32 pickup_ID=86, 		pickup_Pal=0;
int pickup_SizeX=16,	pickup_SizeY=16;
int pickup_X, 			pickup_Y;
bool isLucky;			int	pickup_Speed;		
//==================================================================== Traffic Data
u32 traffic_ID[8], 		traffic_Pal[8];
int traffic_SizeX[8],	traffic_SizeY[8];
int traffic_X[8], 		traffic_Y[8];
bool isBig[8];			int	traffic_Speed[8];
//==================================================================== Weapons Data
u32 weapon_ID[2],		weapon_Pal[2];
int	weapon_Y[2],		weapon_Ammo[2];
bool isWield[2];
//==================================================================== Bullets Data
u32 bullet_ID[2],		bullet_Pal[2];
int bullet_SizeX[2],	bullet_SizeY[2];
int bullet_X[2],		bullet_Y[2];
bool isShot[2];			int	bullet_Speed[2];
//==================================================================== Explosions Data
u32 explosion_ID[30], 	explosion_Pal[30];
int explosion_X[30], 	explosion_Y[30];
bool isExploding[30];	int	explosion_Speed[30];

int main()
{
	//===================================================================================================== Initialize Every Objects
	initialize();									
	//===================================================================================================== Set Mode & BackGround & Display
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_2D;
	//===================================================================================================== Set Background Position
	REG_BG1HOFS= scroll_X;	REG_BG2HOFS= scroll_X;
	REG_BG1VOFS= scroll_Y;	REG_BG2VOFS= scroll_Y;	
	//===================================================================================================== Gmae Start	
	while(1)
	{
		switch(gamestate)
		{
			case 0://====================================================================================== Start Menu
				while(1)
				{
					vid_vsync();										//Frame Sync
					key_poll();											//Key Input Handler
					//===================================================================================== Traffic Respawn
					
					for (i=0;i<6;i++)
					{
						traffic_X[i]=(rand()%168)+28,	traffic_Y[i]=0;	//Respawn Cars
					}
					for (i=6;i<8;i++)
					{
						traffic_X[i]=(rand()%152)+23,	traffic_Y[i]=0;	//Respawn Trucks
						
					}
					//===================================================================================== Fresh Start
					newScore=0,damage=0;
					//===================================================================================== Main Menu
					if(key_hit(KEY_START))
					{
						gamestate=1;									//Gameplay
						break;											//Exit Loop
					}
					//===================================================================================== Display Objects
					objectsDisplay();									//Display Every Objects
					//===================================================================================== Text
					tte_write("#{es}");									//Clear Text Screen
					tte_write("#{P:78,20}");							//Text Location
					tte_write("#{cx:0x1000}MOTOR  MURDER");			//Text Output
					tte_write("#{P:65,60}");							//Text Location
					tte_write("#{cx:0x1000}(PRESS  START)");			//Text Output
					
					if(restart==false)
					{
						tte_write("#{P:35,102}");							//Text Location
						tte_write("#{cx:0x1000}USE (A)(B)   TO SHOOT ");	//Text Output
						tte_write("#{P:35,112}");							//Text Location
						tte_write("#{cx:0x1000}USE ARROWS   TO MOVE ");		//Text Output
					}
					
					sprintf(HighScore,"#{cx:0x1000}%d",newScore);		//Text Content
					tte_write("#{P:46,150}");							//Text Location
					tte_write(HighScore);								//Text Output
					
				}
			case 1://====================================================================================== Gameplay
				while(1)
				{
					vid_vsync();										//Frame Sync
					inputDetect();										//Input	Detection
					//===================================================================================== Background Scroll
					scroll_Y--;timer++;								//Scroll Y & Timer Increase
					REG_BG1HOFS= scroll_X;	REG_BG2HOFS= scroll_X;
					REG_BG1VOFS= scroll_Y;	REG_BG2VOFS= scroll_Y;
					//===================================================================================== Gameplay	
					if(gamestate!=1)
					{
						break;											//Exit Loop
					}
					//===================================================================================== Display Objects
					objectsDisplay();									//Display Every Objects
					//===================================================================================== Text
					tte_write("#{es}");									//Clear Text Screen
					sprintf(HighScore,"#{cx:0x1000}%d",newScore);		//Text Content
					tte_write("#{P:46,150}");							//Text Location
					tte_write(HighScore);								//Text Output
				}
				break;
			case 2://====================================================================================== Pause
				while(1)
				{
					vid_vsync();										//Frame Sync
					key_poll();											//Key Input Handler
					//===================================================================================== Pause Menu
					if((key_hit(KEY_START))||(key_hit(KEY_SELECT)))
					{
						gamestate=1;									//Gameplay
						break;											//Exit Loop
					}
					//===================================================================================== Display Objects
					objectsDisplay();									//Display Every Objects
					//===================================================================================== Text
					tte_write("#{es}");									//Clear Screen
					tte_write("#{P:75,60}");							//Text Location
					tte_write("#{cx:0x1000}(GAME  PAUSED)");			//Text Output
					
					sprintf(HighScore,"#{cx:0x1000}%d",newScore);		//Text Content
					tte_write("#{P:46,150}");							//Text Location
					tte_write(HighScore);								//Text Output
				}
				break;
			case 3://====================================================================================== Game Over
				while(1)
				{
					vid_vsync();										//Frame Sync
					key_poll();											//Key Input Handler
					//===================================================================================== Game Over Menu
					if(key_hit(KEY_START))
					{
						resetGameplay();								//Main Menu & Reset
						objectsDisplay();								//Display Every Objects
						gameBorders();									//Check Positions
						break;											//Exit Loop
					}
					//===================================================================================== Display Objects
					objectsDisplay();									//Display Every Objects
					//===================================================================================== Text
					tte_write("#{es}");									//Clear Screen
					tte_write("#{P:75,60}");							//Text Location
					tte_write("#{cx:0x1000}(GAME  OVER)");				//Text Output
					
					sprintf(HighScore,"#{cx:0x1000}%d",newScore);		//Text Content
					tte_write("#{P:46,150}");							//Text Location
					tte_write(HighScore);								//Text Output
				}
				break;
		}
	}
	return 0;
}
void initialize()
{	
	//================================================================ Seed
	srand(time(NULL));			//Set Time Main As Seed
	//===================================================================================================== Sprite Sheet
	memcpy(&tile_mem[4][0],SpriteSheetTiles,SpriteSheetTilesLen);	//Load Tiles Into CBB 4/Tiles 0-1024
	memcpy(pal_obj_mem,SpriteSheetPal,SpriteSheetPalLen);			//Load Background1 Palette
	//===================================================================================================== Load Background Data
	memcpy(&tile_mem[0][0],BackgroundTiles,BackgroundTilesLen);	//Load Tiles Into CBB 0/Tiles 0-1024
	memcpy(pal_bg_mem,BackgroundPal,BackgroundPalLen);          	//Load Background Palette
	memcpy(&se_mem[29][0],BackgroundMap,BackgroundMapLen);			//Load Background Into SBB 29
	//===================================================================================================== Bind Backgrounds
	REG_BG1CNT= BG_CBB(0) | BG_SBB(29) | BG_4BPP | BG_REG_32x32 | 	BG_PRIO(1);	//Set BackGround1 (4bpp 32x32T Map) 
	//===================================================================================================== Initialize Text
	tte_init_se(0, BG_CBB(1)|BG_SBB(30),0,CLR_GREEN,14,NULL,NULL);//Set BackGround0 As Text Renderer
	//===================================================================================================== Initialize Sprites	
	oam_init(obj_buffer,2048);
	//================================================================ Pick-UP
	pickup_X=offScreen,	pickup_Y=0;
	//================================================================ Traffic 1
	for (i=0;i<2;i++)
	{
		traffic_ID[i]=142,		traffic_Pal[i]=0;
		traffic_SizeX[i]=16,	traffic_SizeY[i]=32;
		traffic_Speed[i]=0;		isBig[i]=false;
	}
	//================================================================ Traffic 2
	for (i=2;i<4;i++)
	{
		traffic_ID[i]=144,		traffic_Pal[i]=0;
		traffic_SizeX[i]=16,	traffic_SizeY[i]=32;
		traffic_Speed[i]=0;		isBig[i]=false;
	}
	//================================================================ Traffic 3
	for (i=4;i<6;i++)
	{
		traffic_ID[i]=146,		traffic_Pal[i]=0;
		traffic_SizeX[i]=16,	traffic_SizeY[i]=32;
		traffic_Speed[i]=0;		isBig[i]=false;
	}
	//================================================================ Traffic 4 Big
	for (i=6;i<8;i++)
	{
		traffic_ID[i]=10,		traffic_Pal[i]=0;
		traffic_SizeX[i]=32,	traffic_SizeY[i]=64;
		traffic_Speed[i]=0;		isBig[i]=true;
	}
	for (i=0;i<2;i++)
	{	//============================================================ Weapons
		weapon_ID[i]=50+i,			weapon_Pal[i]=0;
		weapon_Y[i]=player_Y+10;	weapon_Ammo[i]=10;
		//============================================================ Bullets
		bullet_ID[i]=18+i,			bullet_Pal[i]=0;
		bullet_SizeX[i]=8,			bullet_SizeY[i]=8;
		bullet_Y[i]=player_Y+8,		bullet_Speed[i]=0;
	}
	//================================================================ Explosions
	for (i=0;i<30;i++)
	{
		explosion_ID[i]=256, 	explosion_Pal[i]=0;
	}
}
void inputDetect()
{
	//================================================================ Scoring
	if(timer%60==0){	newScore++;	}
	//================================================================ Key Input
	key_poll();								//Key Input Handler
	//================================================================ Player Move Right
	if(key_hit(KEY_RIGHT))
	{
		player_X+=2;
	}
	if(key_held(KEY_RIGHT))
	{
		player_X+=2;
	}
	//================================================================ Player Move Left
	if(key_hit(KEY_LEFT))
	{
		player_X-=2;
	}
	if(key_held(KEY_LEFT))
	{
		player_X-=2;
	}
	//================================================================ PLayer Move Up
	if(key_hit(KEY_UP))
	{
		player_Y-=2;
	}
	if(key_held(KEY_UP))
	{
		player_Y-=2;
	}
	//================================================================ Player Move Down
	if(key_hit(KEY_DOWN))
	{
		player_Y+=2;
		
	}
	if(key_held(KEY_DOWN))
	{
		player_Y+=2;
	}
	//================================================================ Pick-Up Frequency
	if((timer%300==0)&&(isLucky==false))
	{
		isLucky=true;	isShield=false;
	}
	//================================================================ Pick-Up Auto-Movement
	if(isLucky==true)
	{
		pickup_Speed+=3;
	}
	for (i=0;i<8;i++)
	{	//============================================================ RaceCars Auto-Movement
		if(i<2){traffic_Speed[i]+=3;}
		//============================================================ Motorcicles Auto-Movement
		if((newScore>20)&&(i>=2)&&(i<4)){	traffic_Speed[i]+=3;	}
		//============================================================ FlashyCars Auto-Movement
		if((newScore>40)&&(i>=4)&&(i<6)){	traffic_Speed[i]+=3;	}
		//============================================================ Trucks Auto-Movement
		if((newScore>60)&&(i>=6)&&(i<8)){	traffic_Speed[i]+=2;	}
	}
	//================================================================ Weapon Left Shooting System
	if((key_hit(KEY_A))&&(isShot[0]==false)&&(isWield[0]==true)&&(weapon_Ammo[0]>0))
	{
		isShot[0]=true;
		bullet_X[0]=player_X,	bullet_Y[0]=player_Y+8;
	}
	//================================================================ Weapon Right Shooting System
	if((key_hit(KEY_B))&&(isShot[1]==false)&&(isWield[1]==true)&&(weapon_Ammo[1]>0))
	{
		isShot[1]=true;
		bullet_X[1]=player_X+8,	bullet_Y[1]=player_Y+8;
	}
	//================================================================ Bullets Auto-Movements
	for (i=0;i<2;i++)
	{
		if(isShot[i]==true)
		{
			bullet_Speed[i]+=6;
		}
	}
	//================================================================ Explosions Animation
	for (i=0;i<30;i++)
	{
		if(isExploding[i]==true)
		{
			explosion_ID[i]+=4;
		}
	}
	//================================================================ Game Pause
	if(key_hit(KEY_SELECT))
	{
		gamestate=2;
	}
	//================================================================ Other Checks
	gameBorders();
	collisionDetect();
}
void objectsDisplay()
{
	//===================================================================================================== Damage Bar
	damagebar(damagebar_ID,damagebar_Pal,DamageBar,DamageBar2,damagebar_X,damagebar_Y,damage,0);
	//===================================================================================================== Score Bar
	scorebar(scorebar_ID,scorebar_Pal,ScoreBar,ScoreBar2,scorebar_X,scorebar_Y,2);
	//===================================================================================================== High Score
	bestTry(bestTry_ID,bestTry_Pal,BestTry,BestTry2,bestTry_X,bestTry_Y,bestScore,newScore,4);
	//===================================================================================================== Player
	player(player_ID,player_Pal,Player,player_X,player_Y,isShield,40);
	//===================================================================================================== Pick-Ups
	pickups(pickup_ID,pickup_Pal,Pickup,pickup_X,pickup_Y,isLucky,pickup_Speed,49);
	
	for (i=0;i<8;i++)
	{	//================================================================================================= Race Car
		if(i<2){	traffic(traffic_ID[i],traffic_Pal[i]+i-0,Racecar,traffic_X[i],traffic_Y[i],isBig[i],traffic_Speed[i],i+41);	}
		
		//================================================================================================= Motorcicle
		if((newScore>20)&&(i>=2)&&(i<4)){	traffic(traffic_ID[i],traffic_Pal[i]+i-2,Motorcicle,traffic_X[i],traffic_Y[i],isBig[i],traffic_Speed[i],i+41);	}
		
		//================================================================================================= Flashy Car
		if((newScore>40)&&(i>=4)&&(i<6)){	traffic(traffic_ID[i],traffic_Pal[i]+i-4,Flashycar,traffic_X[i],traffic_Y[i],isBig[i],traffic_Speed[i],i+41);	}
		
		//================================================================================================= Truck
		if((newScore>60)&&(i>=6)&&(i<8)){	traffic(traffic_ID[i],traffic_Pal[i]+i-6,Truck,traffic_X[i],traffic_Y[i],isBig[i],traffic_Speed[i],i+41);	}
	}
	//===================================================================================================== Weapons & Bullets
	for (i=0;i<2;i++)
	{
		weapons(weapon_ID[i],weapon_Pal[i],Weapon,player_X+(i*8),player_Y+10,isWield[i],i,i+36);
	}
	//===================================================================================================== Explosions
	for (i=0;i<30;i++)
	{
		explosions(explosion_ID[i],explosion_Pal[i],Explosion,explosion_X[i],explosion_Y[i],isExploding[i],explosion_Speed[i],i+6);
	}
	//===================================================================================================== Biffer All Sprites
	oam_copy(oam_mem,obj_buffer,50);
}
void gameBorders()
{	
	//================================================================ Player Top Side Limit
	if(player_Y<0)
	{
		player_Y+=2;
	}
	//================================================================ Player Bottom Side Limit
	if(player_Y>126)
	{
		player_Y-=2;
	}
	//================================================================ Player Left Side Limit
	if(player_X<28)
	{
		player_X+=2;
	}
	//================================================================ Player Right Side Limit
	if(player_X>196)
	{
		player_X-=2;
	}
	//================================================================ Pick-Up Bottom Side Limit
	if (pickup_Speed>176)
	{
		pickup_ID=((rand()%4)+41)*2;				
		pickup_X=(rand()%168)+28;
		pickup_Speed=-72;
		isLucky=false;
	}
	//================================================================ Cars Bottom Side Limit
	for (i=0;i<6;i++)
	{
		if(traffic_Speed[i]>192)
		{
			traffic_Speed[i]=-56;
			traffic_X[i]=(rand()%168)+28;
		}
	}
	//================================================================ Trucks Bottom Side Limit
	for (i=6;i<8;i++)
	{
		if(traffic_Speed[i]>224)
		{
			traffic_Speed[i]=-24;
			traffic_X[i]=(rand()%152)+23;
		}
	}
	//================================================================ Ammunition Limit
	for (i=0;i<2;i++)
	{		
		if(weapon_Ammo[i]==0)
		{
			isWield[i]=false;
		}
		//============================================================ Bullet Top Side Limit
		if(bullet_Speed[i]>160)
		{
			bullet_Speed[i]=0;
			isShot[i]=false;
			weapon_Ammo[i]--;
		}
	}
	//================================================================Explosions Frame Limit
	for (i=0;i<30;i++)
	{
		if(explosion_ID[i]>284)
		{
			explosion_ID[i]=256;
			isExploding[i]=false;
		}
	}
}
void collisionDetect()
{
	//===================================================================================================== Player With Pick-Ups
	if (collider(	player_X,player_X+player_SizeX,player_Y,player_Y+player_SizeY,
					pickup_X,pickup_X+pickup_SizeX,pickup_Speed-pickup_SizeY,pickup_Speed	)==true)	
	{
		if(pickup_ID==82)	{isShield=true;}									//Get Shield
		if(pickup_ID==84)	{damage=0;}											//Get Full Repair
		if(pickup_ID==86)	{damage-=1;	if(damage<0)	{damage=0;}}			//Get 1 Damage Repair
		if(pickup_ID==88)	{for (i=0;i<2;i++)
								{if(isWield[i]==false)
									{isWield[i]=true;weapon_Ammo[i]=10;i=2;}}}//Get Weapon & Ammo
		//============================================================ Spawn Pick-Up
		isLucky=false;							
		pickup_Speed=-72;
		pickup_ID=((rand()%4)+41)*2;				
		pickup_X=(rand()%168)+28;
	}
	//===================================================================================================== Player With Cars
	for (i=0;i<8;i++)
	{	
		//============================================================ Level Limiter
		if((newScore<=20)&&(i>=2)&&(i<4)){i=8;}	
		if((newScore<=40)&&(i>=4)&&(i<6)){i=8;}	
		if((newScore<=60)&&(i>=6)&&(i<8)){i=8;}	
		
		if(i!=8)
		{
			if(i<6)
			{	if (collider(	player_X,player_X+player_SizeX,player_Y,player_Y+player_SizeY,
								traffic_X[i],traffic_X[i]+traffic_SizeX[i],traffic_Speed[i]-traffic_SizeY[i],traffic_Speed[i]	)==true)	
				{	
					for (j=0;j<30;j++)
					{
						if(isExploding[j]==false)
						{
							explosion_X[j]=traffic_X[i]-(traffic_SizeX[i]/2);
							explosion_Y[j]=traffic_Speed[i]-traffic_SizeY[i];
							explosion_Pal[j]=0;
							isExploding[j]=true;
							
							j=30;
						}
					}
					for (j=0;j<30;j++)
					{
						if(isExploding[j]==false&&isShield==false)
						{	
							damage++; if(damage>4) {damage=4;gamestate=3;}
							explosion_X[j]=player_X-(player_SizeX/2);
							explosion_Y[j]=player_Y;
							explosion_Pal[j]=1;
							isExploding[j]=true;
							
							j=30;
						}
					}
					traffic_Speed[i]=-56;traffic_X[i]=(rand()%168)+28;
					newScore++;
				}
			}
			//================================================================================================= Player With Trucks
			if(i>5)
			{	if (collider(	player_X,player_X+player_SizeX,player_Y,player_Y+player_SizeY,
								traffic_X[i]+4,traffic_X[i]+traffic_SizeX[i]-4,traffic_Speed[i]-traffic_SizeY[i],traffic_Speed[i]	)==true)	
				{	
					for (j=0;j<30;j++)
					{
						if(isExploding[j]==false)
						{
							explosion_X[j]=traffic_X[i];
							explosion_Y[j]=traffic_Speed[i]-(traffic_SizeY[i]/2);
							explosion_Pal[j]=0;
							isExploding[j]=true;
							
							j=30;
						}
					}
					for (j=0;j<30;j++)
					{
						if(isExploding[j]==false&&isShield==false)
						{
							damage+=3; if(damage>4) {damage=4;gamestate=3;}
							explosion_X[j]=player_X-(player_SizeX/2);
							explosion_Y[j]=player_Y;
							explosion_Pal[j]=1;
							isExploding[j]=true;
							
							j=30;
						}
					}
					traffic_Speed[i]=-24;traffic_X[i]=(rand()%152)+23;
					newScore++;
				}
			}
			//================================================================================================= Cars With Bullets
			for (k=0;k<8;k++)
			{
				if(k<2)
				{
					if (collider(	bullet_X[k]+2,bullet_X[k]+bullet_SizeX[k]-2,bullet_Y[k]-bullet_Speed[k],bullet_Y[k]-bullet_Speed[k]+bullet_SizeY[k],
									traffic_X[i],traffic_X[i]+traffic_SizeX[i],traffic_Speed[i]-traffic_SizeY[i],traffic_Speed[i]	)==true)	
					{
						if(i<6)
						{	
							for (j=0;j<30;j++)
							{
								if(isExploding[j]==false)
								{
									explosion_X[j]=traffic_X[i]-(traffic_SizeX[i]/2);
									explosion_Y[j]=traffic_Speed[i]-traffic_SizeY[i];
									explosion_Pal[j]=0;
									isExploding[j]=true;
									
									j=30;
								}
							}
							traffic_Speed[i]=-56;traffic_X[i]=(rand()%168)+28;
							newScore++;
						}
						//===================================================================================== Trucks With Bullets
						if(i>5)
						{	
							for (j=0;j<30;j++)
							{
								if(isExploding[j]==false)
								{
									explosion_X[j]=traffic_X[i];
									explosion_Y[j]=traffic_Speed[i]-(traffic_SizeY[i]/2);
									explosion_Pal[j]=0;
									isExploding[j]=true;
									
									j=30;
								}
							}
							traffic_Speed[i]=-24;traffic_X[i]=(rand()%152)+23;
							newScore+=3;
						}
					}
				}
				//==================================================== Level Limiter
				if((newScore<=20)&&(k>=2)&&(i<4)){k=8;}	
				if((newScore<=40)&&(k>=4)&&(i<6)){k=8;}	
				if((newScore<=60)&&(k>=6)&&(i<8)){k=8;}	
				
				if(k!=8)
				{
					//============================================================================================= Cars With Trucks
					if(i!=k)
					{	if (collider(	traffic_X[i],traffic_X[i]+traffic_SizeX[i],traffic_Speed[i]-traffic_SizeY[i],traffic_Speed[i],
										traffic_X[k],traffic_X[k]+traffic_SizeX[k],traffic_Speed[k]-traffic_SizeY[k],traffic_Speed[k]	)==true)
						{
							if(i<6)
							{	
								for (j=0;j<30;j++)
								{
									if(isExploding[j]==false)
									{
										explosion_X[j]=traffic_X[i]-(traffic_SizeX[i]/2);
										explosion_Y[j]=traffic_Speed[i]-traffic_SizeY[i];
										explosion_Pal[j]=0;
										isExploding[j]=true;
										
										j=30;
									}
								}
								traffic_Speed[i]=-56;traffic_X[i]=(rand()%168)+28;	
							}
							//===================================================================================== Trucks With Trucks
							if(i>5)
							{	
								for (j=0;j<30;j++)
								{
									if(isExploding[j]==false)
									{
										explosion_X[j]=traffic_X[i];
										explosion_Y[j]=traffic_Speed[i]-(traffic_SizeY[i]/2);
										explosion_Pal[j]=0;
										isExploding[j]=true;
										
										j=30;
									}
								}
								traffic_Speed[i]=-24;traffic_X[i]=(rand()%152)+23;	
							}
						}
					}
				}
			}
		}
	}
}
//========================================================================================================= Restarter
void resetGameplay()
{
	restart=true;
	gamestate=0,		timer=0;
	player_X=112,		player_Y=126;
	pickup_X=offScreen,	pickup_Speed=0;
	
	if(newScore>bestScore)
	{
		bestScore=newScore;
	}
	for(i=0;i<30;i++)
	{	
		if (i<8)
		{
			if (i<2)	{	weapon_Ammo[i]=0;		bullet_X[i]=offScreen;	}
			if (i<6)	{	traffic_Speed[i]=-56;	traffic_X[i]=offScreen;	}
			if (i>5)	{	traffic_Speed[i]=-24;	traffic_X[i]=offScreen;	}
		}	explosion_X[i]=offScreen;
	}
}
//========================================================================================================= Collision Handler
bool collider(int A1,int B1,int C1,int D1,int A2,int B2,int C2,int D2)
{
	if((((A1>=A2)&&(A1<=B2))||((B1>=A2)&&(B1<=B2)))&&(((C1>=C2)&&(C1<=D2))||((D1>=C2)&&(D1<=D2))))
	{
		return true;
	}
	else return false;
}
//========================================================================================================= Initialize Damage Bar
void damagebar(u32 ID,u32 Pal,OBJ_ATTR* Name,OBJ_ATTR* Name2,int X,int Y,int Damage,int Number)
{
	
	Name= &obj_buffer[Number];								//#	
	Name2= &obj_buffer[Number+1];							//#	
	obj_set_attr(Name,ATTR0_TALL,ATTR1_SIZE_32,ATTR2_PALBANK(Pal)|(ID+(Damage*2)));
	obj_set_attr(Name2,ATTR0_TALL,ATTR1_SIZE_32,ATTR2_PALBANK(Pal)|(ID+(Damage*2)+(32*4)));
	Name->attr2= ATTR2_BUILD(ID+(Damage*2),Pal,1);			//Set Explosion Attributes
	Name2->attr2= ATTR2_BUILD(ID+(Damage*2)+(32*4),Pal,1);	//Set Explosion Attributes
	obj_set_pos(Name,X,Y);									//Set Explosion Position
	obj_set_pos(Name2,X,Y+32);								//Set Explosion Position
}
//========================================================================================================= Initialize Score Bar
void scorebar(u32 ID,u32 Pal,OBJ_ATTR* Name,OBJ_ATTR* Name2,int X,int Y,int Number)
{
	Name= &obj_buffer[Number];						//#	
	Name2= &obj_buffer[Number+1];					//#	
	obj_set_attr(Name,ATTR0_WIDE,ATTR1_SIZE_32,ATTR2_PALBANK(Pal)|(ID));
	obj_set_attr(Name2,ATTR0_WIDE,ATTR1_SIZE_32,ATTR2_PALBANK(Pal)|(ID+4));
	Name->attr2= ATTR2_BUILD(ID,Pal,1);			//Set Explosion Attributes
	Name2->attr2= ATTR2_BUILD(ID+4,Pal,1);			//Set Explosion Attributes
	obj_set_pos(Name,X,Y);							//Set Explosion Position
	obj_set_pos(Name2,X+32,Y);						//Set Explosion Position
}
//========================================================================================================= Initialize Score Bar
void bestTry(u32 ID,u32 Pal,OBJ_ATTR* Name,OBJ_ATTR* Name2,int X,int Y,int Last,int New,int Number)
{
	
	Name= &obj_buffer[Number];						//#	
	Name2= &obj_buffer[Number+1];					//#
	obj_set_attr(Name,ATTR0_WIDE,ATTR1_SIZE_32,ATTR2_PALBANK(Pal)|(ID));
	obj_set_attr(Name2,ATTR0_WIDE,ATTR1_SIZE_32,ATTR2_PALBANK(Pal)|(ID+4));
	Name->attr2= ATTR2_BUILD(ID,Pal,1);		//Set Explosion Attributes
	Name2->attr2= ATTR2_BUILD(ID+4,Pal,1);	//Set Explosion Attributes
	obj_set_pos(Name,offScreen,Y);					//Set Explosion Position
	obj_set_pos(Name2,offScreen,Y);					//Set Explosion Position
	
	if(New>Last)
	{
		obj_set_pos(Name,X,Y);							//Set Explosion Position
		obj_set_pos(Name2,X+32,Y);						//Set Explosion Position
	}
}
//========================================================================================================= Initialize Player
void player(u32 ID,u32 Pal,OBJ_ATTR* Name,int X,int Y,bool Shield,int Number)
{
	Name= &obj_buffer[Number];					//#
	
	if(Shield==false)
	{
		obj_set_attr(Name,ATTR0_TALL,ATTR1_SIZE_32,ATTR2_PALBANK(Pal)|(ID));
		Name->attr2= ATTR2_BUILD(ID,Pal,1); 	//Set Player Attributes
	}				
	if(Shield==true)
	{
		obj_set_attr(Name,ATTR0_TALL,ATTR1_SIZE_32,ATTR2_PALBANK(Pal)|(ID+2));
		Name->attr2= ATTR2_BUILD(ID+2,Pal,1); 	//Set Player Attributes
	}
	
	obj_set_pos(Name,X,Y);						//Set Player Position
}
//========================================================================================================= Initialize Pick-Ups
void pickups(u32 ID,u32 Pal,OBJ_ATTR* Name,int X,int Y,bool Chance,int Speed,int Number)
{
	Name= &obj_buffer[Number];					//#
	obj_set_attr(Name,ATTR0_SQUARE,ATTR1_SIZE_16,ATTR2_PALBANK(Pal)|(ID));
	Name->attr2= ATTR2_BUILD(ID,Pal,1);		//Set PickUp Attributes
	obj_set_pos(Name,offScreen,Y-16+Speed);	//Set PickUp Position
	
	if(Chance==true)
	{  
		obj_set_pos(Name,X,Y-16+Speed);		//Set PickUp Position
	}
}
//========================================================================================================= Initialize Traffic
void traffic(u32 ID,u32 Pal,OBJ_ATTR* Name,int X,int Y,bool Big,int Speed,int Number)
{
	Name= &obj_buffer[Number];					//#
	
	if(Big==false)
	{	
		obj_set_attr(Name,ATTR0_TALL,ATTR1_SIZE_32,ATTR2_PALBANK(Pal)|(ID));
		Name->attr2= ATTR2_BUILD(ID,Pal,1); 	//Set Traffic Attributes
		obj_set_pos(Name,X,Y-32+Speed);		//Set Traffic Position
	}
	if(Big==true)
	{
		obj_set_attr(Name,ATTR0_TALL,ATTR1_SIZE_64,ATTR2_PALBANK(Pal)|(ID));
		Name->attr2= ATTR2_BUILD(ID,Pal,1); 	//Set Traffic Attributes
		obj_set_pos(Name,X,Y-64+Speed);		//Set Traffic Position
	}
}
//========================================================================================================= Initialize Weapons
void weapons(u32 ID,u32 Pal,OBJ_ATTR* Name,int X,int Y,bool Wield,int Side,int Number)
{
	Name= &obj_buffer[Number];					//#
	obj_set_attr(Name,ATTR0_SQUARE,ATTR1_SIZE_8,ATTR2_PALBANK(Pal)|(ID));
	Name->attr2= ATTR2_BUILD(ID,Pal,1);		//Set Weapon Attributes
	obj_set_pos(Name,offScreen,Y);				//Set Weapon Position 
	
	if(Wield==true)
	{
		obj_set_pos(Name,X,Y);					//Set Weapon Position
		
		bullets(bullet_ID[Side],bullet_Pal[Side],Bullet,bullet_X[Side],bullet_Y[Side],isShot[Side],bullet_Speed[Side],Number+2);
	}	
}
//========================================================================================================= Initialize Bullets
void bullets(u32 ID,u32 Pal,OBJ_ATTR* Name,int X,int Y,bool Shot,int Speed,int Number)
{
	Name= &obj_buffer[Number];					//#
	obj_set_attr(Name,ATTR0_SQUARE,ATTR1_SIZE_8,ATTR2_PALBANK(Pal)|(ID));
	Name->attr2= ATTR2_BUILD(ID,Pal,1);		//Set Bullet Attributes
	obj_set_pos(Name,offScreen,Y-Speed);		//Set Bullet Position

	if(Shot==true)
	{
		obj_set_pos(Name,X,Y-Speed);			//Set Bullet Position
	}
}
//========================================================================================================= Initialize Explosions
void explosions(u32 ID,u32 Pal,OBJ_ATTR* Name,int X,int Y,bool Boom,int Speed,int Number)
{
	Name= &obj_buffer[Number];					//#	
	obj_set_attr(Name,ATTR0_SQUARE,ATTR1_SIZE_32,ATTR2_PALBANK(Pal)|(ID));
	Name->attr2= ATTR2_BUILD(ID,Pal,1);		//Set Explosion Attributes
	obj_set_pos(Name,offScreen,Y+Speed);		//Set Explosion Position
	
	if(Boom==true)
	{
		obj_set_pos(Name,X,Y+Speed);			//Set Explosion Position
	}
}