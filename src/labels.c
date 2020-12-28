#include "labels.h"

LPWSTR WindowLabels[NBOFWINDOWLABELS] = {
	L"To Play Harry Potter click on New Game or Load Game. Click on Options for Video, Sounds and Joystick options.",
	L"Main Menu",
	L"Choose Video, Sound or Joystick options.",
	L"Options",
	L"Choose the desired renderer and resolution for the game to use.",
	L"Start a NEW game by clicking on the slot you wish to use.",
	L"Load a game by clicking on a previously used slot.",
	L"Choose your desired sound options.",
	L"Video Options",
	L"Sound Options",
	L"Custom Resolution",
	L"Desired Resolution",
	L"New Game",
	L"Loading Game",
	L"Sound Volume",
	L"Music Volume",
	L"Disable all sound",
	L"Custom Resolution",
	L"Maximum Resolution"
};

char* ErrorLabels[NBOFERRORLABELS] = {
	"Error: Couldn't oppen file :\n>%s< !\n",
	"Error: Couldn't read file : \n>%s< !\n",
	"Error: Couldn't write file : \n>%s< !\n",
	"Error: Invalid Render Device !\n",
	"Error: Couldn't patch the game settings !\n", 
	"Error: Can't read the game's sound settings !\n",
	"Error: Can't read the game's video settings !\n",
	"Error: Can't read the game's render device !\n",
	"Error: Can't allocate more memory !\n"
};

char* Labels[NBOFLABELS] = {
	"Loading config file >%s<\n",
	"Setting game resolution to: %dx%d:%d\n",
	"Setting DGVoodoo2 resolution to: ",
	"Setting render device to ",
	"Patching game settings\n",
	"Game starting : %s%s\n",
	"Freeing ressources\n",
	"Setting sound settings\n",
	"Resetting %s config to default\n",
	"Rebuilding config directory\n"
};