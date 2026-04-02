------------------------------------------------------------------------------------------------

# PROGRAM NAME:

	TCP Card Game

------------------------------------------------------------------------------------------------

## SYNOPSIS: 

	This program creates a two-player turn-based card game that uses TCP client and server connection and communication. The server side manages most of everything including player connections, class menu 	and selection, deck distribution, turn logic, game logic such as damage, healing, and resources. Two clients connect to the server, choose a class in which they will receive their card deck before the 	match begins. Gameplay and setting up is driven by message exchanges. Clients send commands to the server in which the server processes these actions and returns any updates or confirmation back to both 	clients.

------------------------------------------------------------------------------------------------

# FILES:

	This project contains 10 files.

	class_project4_team.c: Main folder of programs
	This includes:
                      server.c
                      client.c
                      Player.c
                      Player.h
                      GameManager.c
                      GameManager.h	
                      Cards.c
                      Cards.h

        	  Readme.txt: Contains the overview of the project.
    	      DesignFile.pdf: Contains a blueprint on how the project was created.

------------------------------------------------------------------------------------------------

## INSTRUCTIONS TO USE:

	Using your choice of compiler, run make to compile all files at once. If needed, run make clean to clean up any old executables.

	To run the server, use ./server.

	To run  the clients, open two clients and run ./client 127.0.0.1 on both clients.

	The players will then be prompted to select which card deck class they would like to use (Mage or Jack). Players can then type 1 or 2 to pick these classes. Once their class is picked, they may type 	READY to ready up. Once both players are READY, the match will commence.


	During the match, the server will indicate whose turn it is with either "YOUR TURN" or "OPPONENT TURN". Once it is your turn, your card deck will be displayed with numbers applied to each different 	card. Players can play their card with "PLAY 1" (2, 3, 4, etc.). After each action the players take, the server will return the GAMESTATE which includes your health and resources.

	To win the game, a players health must drop to zero. If you win you will receive "YOU WIN!", if you lose, you will receive "YOU LOSE!" messages from the server, then the match will end.

------------------------------------------------------------------------------------------------

# DESIGNERS:

	     Group: Gerardo Cossio, Tyler Meredith, Gage Wolston, Colin Hardin, Justin Osuna, Eddie Rios
	     Class: Network Programming
	Assignment: TCP Card Game
	  Due Date: 12/10/2025
	Instructor: Dr. Zhao

------------------------------------------------------------------------------------------------

# TEAM ASSIGNMENTS:

	Gerardo Cossio: Team Captain, PowerPoint, Overall files,
	Tyler Meredith: Design File, Readme, Player files,
	  Gage Wolston: GameManager files, Cards files, deck creation, Overall files
	  Colin Hardin: Server and client connection setup, Player files, Bug fixes
	  Justin Osuna:
	    Eddie Rios:

------------------------------------------------------------------------------------------------

# SOFTWARE AND HARDWARE DESCRIPTIONS:

	Development and Testing Environments:

	PuTTY SSH client connected to ada2
	Ubuntu Linux (WSL) 64-bit on Windows
	Visual Studio Code
	Miro (Collaborative whiteboard)
	
	Software Tools:

	Standard UNIX socket libraries
	GNU Make
	GCC

	Hardware / Architecture:

	x86_64 architecture
	Local Network testing (127.0.0.1)

------------------------------------------------------------------------------------------------
