#include "Game.h"
#include <iostream>
#include "Utils.h"
#include "Button.h"
#include "Player.h"
#include "Background.h"
#include "Alien.h"

// Button dimensions
static int buttonWidth = 300;
static int buttonHeight = 150;

// Get screen dimensions
static int windowWidth = 1710;
static int windowHeight = 980;

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr), lastFrameTime(0), deltaTime(0.0f), currentState(GameState::MENU), scoreAdded(false) {}

Game::~Game() {
    Mix_FreeChunk(shootSound);
	Mix_FreeChunk(playerHitSound);
	Mix_FreeChunk(alienHitSound);
	Mix_FreeChunk(powerUpSound);
    Mix_FreeChunk(gameOverSound);
	Mix_FreeMusic(backgroundMusic);
	Mix_FreeMusic(menuMusic);
    delete startButton;
    delete quitButton;
    delete goToMenuButton;
    delete scoreButton;
	delete replayButton;
    delete startBackground;
    delete gameBackground;
	delete pauseGoToMenuButton;
	delete singleButton;
	delete duoButton;
    delete border;
	delete P1Win;
	delete P2Win;
    delete gameTitle;
    delete gameOverTitle;
    delete player1;
    delete player2;
    delete alienSwarm1;
	delete alienSwarm2;
    if (!bullets.empty()) {
        for (Bullet* bullet : bullets) {
            delete bullet;
        }
    }
    bullets.clear();
    for (Bullet* bullet : enemiesBullets) {
        delete bullet;
    }
    enemiesBullets.clear();
    delete livesManager1;
    delete livesManager2;
	delete highScore;
    delete score;
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    //Initialize SDL_Image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
    }

    //Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
    } 

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }


    // Create window
    Uint32 flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Set renderer color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 

    // Create the all entities
    initializeAll();

    isRunning = true;
    lastFrameTime = SDL_GetTicks();
    deltaTime = 0.0f;

    return true;
}

void Game::initializeAll() {
    // Calculate button positions
    int centeredX = (windowWidth - buttonWidth) / 2; // Centered for button only
	int centeredY = (windowHeight - buttonHeight) / 2; // Centered for button only
    int menuButtonsY = 750;
    int startButtonX = centeredX - buttonWidth - 100;
    int scoreButtonX = centeredX;
    int quitButtonX = centeredX + buttonWidth + 100;
    int goToMenuButtonX = centeredX - buttonWidth - 100;
	

    // Initialize the background
    startBackground = new Background(0, 0, windowWidth, windowHeight, "Resource/Background/Menu_background.png", renderer);
    gameBackground = new Background(0, 0, windowWidth, windowHeight, "Resource/Background/background.png", renderer);
    gameOverBackground = new Background(0, 0, windowWidth, windowHeight, "Resource/Background/Game_over_background.png", renderer);


    // Initialize sound effects and music
	shootSound = Mix_LoadWAV("Resource/Sound/Shoot.wav");
	Mix_VolumeChunk(shootSound, 12);
	playerHitSound = Mix_LoadWAV("Resource/Sound/Player_hit.wav");
	Mix_VolumeChunk(playerHitSound, 12);
	alienHitSound = Mix_LoadWAV("Resource/Sound/Alien_hit.wav");
	Mix_VolumeChunk(alienHitSound, 12);
	powerUpSound = Mix_LoadWAV("Resource/Sound/Power_up.wav");
	Mix_VolumeChunk(powerUpSound, 12);
    gameOverSound = Mix_LoadWAV("Resource/Sound/Game_over.wav");
    Mix_VolumeChunk(gameOverSound, 20);
	menuMusic = Mix_LoadMUS("Resource/Sound/Menu_music.mp3");
	backgroundMusic = Mix_LoadMUS("Resource/Sound/Background_music.mp3");
	Mix_VolumeMusic(30);
    if (Mix_PlayMusic(menuMusic, -1) == -1) {
        std::cerr << "Mix_PlayMusic error: " << Mix_GetError() << std::endl;
    }

    // Initialize the buttons
    startButton = new Button(startButtonX, menuButtonsY, buttonWidth, buttonHeight, "Resource/Button/start_button.png", this, Button::ButtonType::START);
    quitButton = new Button(quitButtonX, menuButtonsY, buttonWidth, buttonHeight, "Resource/Button/quit_button.png", this, Button::ButtonType::QUIT);
    goToMenuButton = new Button(goToMenuButtonX, menuButtonsY, buttonWidth, buttonHeight, "Resource/Button/go_to_menu_button.png", this, Button::ButtonType::GO_TO_MENU);
	scoreButton = new Button(scoreButtonX, menuButtonsY, buttonWidth, buttonHeight, "Resource/Button/highscore_button.png", this, Button::ButtonType::SCORE);
	pauseGoToMenuButton = new Button(centeredX, centeredY, buttonWidth, buttonHeight, "Resource/Button/go_to_menu_button.png", this, Button::ButtonType::GO_TO_MENU);
	replayButton = new Button(centeredX, menuButtonsY, buttonWidth, buttonHeight, "Resource/Button/replay_button.png", this, Button::ButtonType::REPLAY);
	singleButton = new Button(startButtonX, centeredY, buttonWidth, buttonHeight, "Resource/Button/1_player.png", this, Button::ButtonType::SINGLEPLAYER);
	duoButton = new Button(quitButtonX, centeredY, buttonWidth, buttonHeight, "Resource/Button/2_player.png", this, Button::ButtonType::DUOPLAYER);
    arrow = new Button(50, 50, 100, 50, "Resource/Button/Arrow_1.png", "Resource/Button/Arrow_2.png", this, Button::ButtonType::ARROW);

	// Initialize the win screen and game title (as background for easy handling)
	P1Win = new Background((windowWidth - 600)/2, (windowHeight - 300) / 2, 600, 300, "Resource/Title/P1_win.png", renderer);
	P2Win = new Background((windowWidth - 600) / 2, (windowHeight - 300) / 2, 600, 300, "Resource/Title/P2_win.png", renderer);
    gameTitle = new Background((windowWidth - 800) / 2, (windowHeight - 300) / 2, 800, 250, "Resource/Title/Game_title.png", renderer);
    gameOverTitle = new Background((windowWidth - 800) / 2, 200, 800, 400, "Resource/Title/Game_over_title.png", renderer);

	// Initialize the border for duo mode
    border = new SDL_Rect;
	border->x = 850;
	border->y = 0;
	border->w = 10;
	border->h = windowHeight;

    // Initialize font, high score class and score counter
    gameFont = TTF_OpenFont("Resource/ARCADECLASSIC.ttf", 28);
    if (gameFont == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }
    scoreFont = TTF_OpenFont("Resource/ARCADECLASSIC.ttf", 60);
    if (scoreFont == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }

	// Initialize score and high score
    score = new Score(gameFont, renderer);
	highScore = new HighScore("highScore.txt", scoreFont, renderer);
	highScore->load();

    // Initialize player ship, alien, lives manager
    player1 = new Player(300, 900, 60, 60, "Resource/Player/player1.png", renderer, 300, bullets, SDLK_a, SDLK_d, SDLK_SPACE, true, this);
    player2 = new Player(900, 900, 60, 60, "Resource/Player/player2.png", renderer, 300, bullets, SDLK_LEFT, SDLK_RIGHT, SDLK_KP_ENTER, false, this);
    alienSwarm1 = new AlienSwarm(renderer, enemiesBullets, this, true);
    alienSwarm2 = new AlienSwarm(renderer, enemiesBullets, this, false);
    livesManager1 = new LivesManager(3, gameFont, renderer);
    livesManager2 = new LivesManager(3, gameFont, renderer);
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        switch (currentState) {
        case GameState::MENU:
            // Handle menu input 
            startButton->handleEvent(event); 
            quitButton->handleEvent(event);  
			scoreButton->handleEvent(event);
            break;
		case GameState::CHOOSE_MODE:
			// Handle choose mode input
			singleButton->handleEvent(event);
			duoButton->handleEvent(event);
            arrow->handleEvent(event);
            break;
        case GameState::SINGLE:
            // Handle gameplay input 
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                currentState = GameState::PAUSE;
				Mix_PauseMusic();
            }
            player1->handleEvent(event);
            break;
		case GameState::DUO:
			// Handle gameplay input
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                currentState = GameState::PAUSE;
                Mix_PauseMusic();
            }
            player1->handleEvent(event);
			player2->handleEvent(event);
            break;
        case GameState::PAUSE:
            // Handle pause menu input
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                if (singlePlayer == true) {
                    currentState = GameState::SINGLE;
                }
                else {
                    currentState = GameState::DUO;
                }
                Mix_ResumeMusic();
            }
            pauseGoToMenuButton->handleEvent(event);
            break;
        case GameState::GAME_OVER:
            // Handle game over input 
			goToMenuButton->handleEvent(event);
            quitButton->handleEvent(event);
			replayButton->handleEvent(event);
            break;
		case GameState::HIGHSCORE:
			// Handle highscore input
            arrow->handleEvent(event);
			break;
        }
    }
}

void Game::update() {
    // Game logic
    Uint32 currentFrameTime = SDL_GetTicks();
    deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f; // Convert to seconds
    // Use delta time for smooth movement instead of jagged one
    switch (currentState) {
    case GameState::MENU:
        // Reset bullets
        for (Bullet* bullet : bullets) {
            delete bullet;
        }
        bullets.clear();
        // Reset enemies bullets
        for (Bullet* bullet : enemiesBullets) {
            delete bullet;
        }
        enemiesBullets.clear();
        // Reset the alien swarms
        if (initialResetSwarm == true) {
            alienSwarm1->reset();
            alienSwarm1->resetResetCounter();
            alienSwarm1->resetNumOfShooter();
            alienSwarm2->reset();
            alienSwarm2->resetResetCounter();
            alienSwarm2->resetNumOfShooter();
			initialResetSwarm = false;
        }
        // Reset score
        score->reset();
        // Reset lives manager
		livesManager1->reset(); 
		livesManager2->reset(); 
        // Set resetPlayerPositon flag to false
        if (resetPlayerPosition == true) {
			resetPlayerPosition = false;
        }
        // Reset player speed
        if (speedResetted == true) {
            player1->resetSpeed();
            player2->resetSpeed();
            speedResetted = false;
        }
        break;
    case GameState::SINGLE: {
        // Update high score flag 
		scoreAdded = false;
        // Reset the alien swarms
        if (initialResetSwarm == false) {
            alienSwarm1->reset();
            alienSwarm1->resetResetCounter();
            alienSwarm1->resetNumOfShooter();
            initialResetSwarm = true;
        }
        // Reset player speed
        if (speedResetted == false) {
            player1->resetSpeed();
            speedResetted = true;
        }
        // Reset player to initial position in single mode
        if (resetPlayerPosition == false) {
            player1->setPosition(825);
            resetPlayerPosition = true;
        }
        // Update player and alien
        player1->update(deltaTime);
        alienSwarm1->update(deltaTime); 
        // Handle shooting (put in update so that can fire by holding shoot key)
        player1->shoot();
        // Update player bullets
        for (size_t i = 0; i < bullets.size(); ++i) {
            bullets[i]->update(deltaTime);
            if (bullets[i]->shouldRemove) {
                delete bullets[i];
                bullets.erase(bullets.begin() + i);
                --i; // Adjust index after erasing
            }
        }
        //Update enemy bullets
        for (size_t i = 0; i < enemiesBullets.size(); ++i) {
            enemiesBullets[i]->update(deltaTime);
            if (enemiesBullets[i]->shouldRemove) {
                delete enemiesBullets[i];
                enemiesBullets.erase(enemiesBullets.begin() + i);
                --i; // Adjust index after erasing
            }
        }
        // Collision detection (bullets and aliens)
        for (int i = 0; i < bullets.size(); i++) {
            for (int j = 0; j < alienSwarm1->getAliens().size(); j++) {
                if (bullets[i]->isColliding(*alienSwarm1->getAliens()[j])) {
                    // Spawn power-up
                    PowerUp* newPowerUp = PowerUp::spawnPowerUp(alienSwarm1->getAliens()[j]->getRect().x,
                        alienSwarm1->getAliens()[j]->getRect().y,
                        renderer);
                    if (newPowerUp != nullptr) {
                        powerUps.push_back(newPowerUp);
                    }
                    // Remove bullet and alien
                    bullets[i]->shouldRemove = true;
                    alienSwarm1->getAliens()[j]->shouldRemove = true;
                    // Increase score
                    score->increaseScore(10);
					Mix_PlayChannel(-1, alienHitSound, 0); // Play hit sound
                    break; // Break after a collision
                }
            }
        }
        // Collision detection (bullets and player)
        if (player1->isInvincible == false) {
            for (int i = 0; i < enemiesBullets.size(); i++) {
                if (enemiesBullets[i]->isColliding(*player1)) {
                    enemiesBullets[i]->shouldRemove = true; // Remove bullet
                    livesManager1->loseLife(); // Self explanatory
                    // Give player iframes (invincibility frames to not get hit immediately) and reset player position
                    player1->isInvincible = true;
                    player1->invincibilityTime = 0.0f;
					player1->setPosition(825);
					Mix_PlayChannel(-1, playerHitSound, 0); // Play hit sound
                    break; // Break after a collision
                }
            }
        }
        // Power-up updates and collisions
        for (size_t i = 0; i < powerUps.size(); ++i) {
            if (powerUps[i]) { 
                powerUps[i]->update(deltaTime);
                // Collision detection with player
                if (powerUps[i]->isColliding(*player1)) {
                    // Apply power-up effect
                    switch (powerUps[i]->getType()) {
                        case PowerUp::Type::EXTRA_LIFE:
							livesManager1->addLife();
							break;
						case PowerUp::Type::SPEED_BOOST:
							player1->speed += 50;
							break;
						case PowerUp::Type::RAPID_FIRE:
							player1->shootCooldown -= 0.01f;
							break;
						case PowerUp::Type::INVINCIBILITY:
							player1->isInvincible = true;
							player1->invincibilityTime = 0.0f;
							break;
                    }
					Mix_PlayChannel(-1, powerUpSound, 0); // Play power-up sound
                    delete powerUps[i];  // Delete the power-up
                    powerUps.erase(powerUps.begin() + i);
                    --i; // Adjust index
                    continue; // Skip to the next power-up
                }
                // Remove power-up if it should be removed 
                if (powerUps[i]->shouldRemove) {
                    delete powerUps[i];     // Delete the power-up
                    powerUps.erase(powerUps.begin() + i);
                    --i; // Adjust index
                }
            }
        }
        // Create new alien swarm when previous is defeated
        if (alienSwarm1->getAliens().empty())
        {
			alienSwarm1->increaseResetCounter(); // Increase the reset counter
            alienSwarm1->increaseSpeed(0.1f); // Increase speed
            alienSwarm1->reset(); // Spawn new swarm
			alienSwarm1->increaseShootingSpeed(0.1f); // Increase shooting speed
			alienSwarm1->increaseNumOfShooter(); // Increase number of shooters (if reset counter is 2 and the numOfShooter is less than 15)
        }
        //check if aliens reached the bottom
        for (Alien* alien : alienSwarm1->getAliens()) {
            if (alien->getRect().y + alien->getRect().h >= player1->getRect().y) {
                setGameState(GameState::GAME_OVER);
                break;
            }
        }
        // Check if player's lives is 0
        if (livesManager1->isGameOver())
        {
            gameOver = true;
            setGameState(GameState::GAME_OVER);
        }
        break;
    } 
    case GameState::DUO:
		scoreAdded = true; // Update high score flag so that we don't count score for duo player
		// Reset the alien swarms
        if (initialResetSwarm == false) {
            alienSwarm1->reset();
            alienSwarm1->resetResetCounter();
            alienSwarm1->resetNumOfShooter();
            alienSwarm2->reset();
            alienSwarm2->resetResetCounter();
            alienSwarm2->resetNumOfShooter();
            initialResetSwarm = true;
        }
        // Reset player speed
        if (speedResetted == false) {
            player1->resetSpeed();
            player2->resetSpeed();
            speedResetted = true;
        }
		// Reset players to initial position in duo mode
        if (resetPlayerPosition == false) {
            player1->setPosition(397);
            player2->setPosition(1252);
			resetPlayerPosition = true;
        }
        // Update player and alien
        player1->update(deltaTime);
		player2->update(deltaTime);
        alienSwarm1->update(deltaTime);
		alienSwarm2->update(deltaTime);
        // Handle shooting (put in update so that can fire by holding shoot key)
        player1->shoot();
		player2->shoot();
        // Update player bullets
        for (size_t i = 0; i < bullets.size(); ++i) {
            bullets[i]->update(deltaTime);
            if (bullets[i]->shouldRemove) {
                delete bullets[i];
                bullets.erase(bullets.begin() + i);
                --i; // Adjust index after erasing
            }
        }
        //Update enemy bullets
        for (size_t i = 0; i < enemiesBullets.size(); ++i) {
            enemiesBullets[i]->update(deltaTime);
            if (enemiesBullets[i]->shouldRemove) {
                delete enemiesBullets[i];
                enemiesBullets.erase(enemiesBullets.begin() + i);
                --i; // Adjust index after erasing
            }
        }
        // Collision detection (bullets and aliens)
        for (int i = 0; i < bullets.size(); i++) {
            for (int j = 0; j < alienSwarm1->getAliens().size(); j++) {
                if (bullets[i]->isColliding(*alienSwarm1->getAliens()[j])) {
                    // Spawn power-up (with null check)
                    PowerUp* newPowerUp = PowerUp::spawnPowerUp(alienSwarm1->getAliens()[j]->getRect().x,
                        alienSwarm1->getAliens()[j]->getRect().y,
                        renderer);
                    if (newPowerUp != nullptr) {
                        powerUps.push_back(newPowerUp);
                    }
                    bullets[i]->shouldRemove = true;
                    alienSwarm1->getAliens()[j]->shouldRemove = true;
                    Mix_PlayChannel(-1, alienHitSound, 0); // Play hit sound
                    break; // Break after a collision
                }
            }
        }
        for (int i = 0; i < bullets.size(); i++) {
            for (int j = 0; j < alienSwarm2->getAliens().size(); j++) {
                if (bullets[i]->isColliding(*alienSwarm2->getAliens()[j])) {
                    // Spawn power-up (with null check)
                    PowerUp* newPowerUp = PowerUp::spawnPowerUp(alienSwarm2->getAliens()[j]->getRect().x,
                        alienSwarm2->getAliens()[j]->getRect().y,
                        renderer);
                    if (newPowerUp != nullptr) {
                        powerUps.push_back(newPowerUp);
                    }
                    bullets[i]->shouldRemove = true;
                    alienSwarm2->getAliens()[j]->shouldRemove = true;
                    Mix_PlayChannel(-1, alienHitSound, 0); // Play hit sound
                    break; // Break after a collision
                }
            }
        }
        // Collision detection (bullets and player)
        if (player1->isInvincible == false) {
            for (int i = 0; i < enemiesBullets.size(); i++) {
                if (enemiesBullets[i]->isColliding(*player1)) {
                    enemiesBullets[i]->shouldRemove = true;
                    livesManager1->loseLife();
                    player1->isInvincible = true;
                    player1->invincibilityTime = 0.0f;
                    player1->setPosition(397);
                    Mix_PlayChannel(-1, playerHitSound, 0); // Play hit sound
                    break; // Break after a collision
                }
            }
        }
        if (player2->isInvincible == false) {
            for (int i = 0; i < enemiesBullets.size(); i++) {
                if (enemiesBullets[i]->isColliding(*player2)) {
                    enemiesBullets[i]->shouldRemove = true;
                    livesManager2->loseLife();
                    player2->isInvincible = true;
                    player2->invincibilityTime = 0.0f;
                    player2->setPosition(1252);
                    Mix_PlayChannel(-1, playerHitSound, 0); // Play hit sound
                    break; // Break after a collision
                }
            }
        }
        // Power-up Updates and Collisions 
        for (size_t i = 0; i < powerUps.size(); ++i) {
            if (powerUps[i]) { // Always check for null
                powerUps[i]->update(deltaTime);
                // Collision detection with player
                if (powerUps[i]->isColliding(*player1)) {
                    // Apply power-up effect
                    switch (powerUps[i]->getType()) {
                    case PowerUp::Type::EXTRA_LIFE:
                        livesManager1->addLife();
                        break;
                    case PowerUp::Type::SPEED_BOOST:
                        player1->speed += 50;
                        break;
                    case PowerUp::Type::RAPID_FIRE:
                        player1->shootCooldown -= 0.01f;
                        break;
                    case PowerUp::Type::INVINCIBILITY:
                        player1->isInvincible = true;
                        player1->invincibilityTime = 0.0f;
                        break;
                    }
                    Mix_PlayChannel(-1, powerUpSound, 0); // Play power-up sound
                    delete powerUps[i];  // Delete the power-up
                    powerUps.erase(powerUps.begin() + i);
                    --i; // Adjust index
                    continue; // Skip to the next power-up
                }
                // Remove power-up if it should be removed 
                if (powerUps[i]->shouldRemove) {
                    delete powerUps[i];     // Delete the power-up
                    powerUps.erase(powerUps.begin() + i);
                    --i; // Adjust index
                }
            }
        }
        for (size_t i = 0; i < powerUps.size(); ++i) {
            if (powerUps[i]) { // Always check for null
                powerUps[i]->update(deltaTime);
                // Collision detection with player
                if (powerUps[i]->isColliding(*player2)) {
                    // Apply power-up effect
                    switch (powerUps[i]->getType()) {
                    case PowerUp::Type::EXTRA_LIFE:
                        livesManager2->addLife();
                        break;
                    case PowerUp::Type::SPEED_BOOST:
                        player2->speed += 50;
                        break;
                    case PowerUp::Type::RAPID_FIRE:
                        player2->shootCooldown -= 0.01f;
                        break;
                    case PowerUp::Type::INVINCIBILITY:
                        player2->isInvincible = true;
                        player2->invincibilityTime = 0.0f;
                        break;
                    }
                    Mix_PlayChannel(-1, powerUpSound, 0); // Play power-up sound
                    delete powerUps[i];  // Delete the power-up
                    powerUps.erase(powerUps.begin() + i);
                    --i; // Adjust index
                    continue; // Skip to the next power-up
                }
                // Remove power-up if it should be removed 
                if (powerUps[i]->shouldRemove) {
                    delete powerUps[i];     // Delete the power-up
                    powerUps.erase(powerUps.begin() + i);
                    --i; // Adjust index
                }
            }
        }
        // Create new alien swarm when previous is defeated
        if (alienSwarm1->getAliens().empty())
        {
            alienSwarm1->increaseResetCounter(); // Increase the reset counter
            alienSwarm1->increaseSpeed(0.1f); // Increase speed
            alienSwarm1->reset(); // Spawn new swarm
            alienSwarm1->increaseShootingSpeed(0.1f); // Increase shooting speed
            alienSwarm1->increaseNumOfShooter(); // Increase number of shooters (if reset counter is 2 and the numOfShooter is less than 15)
        }
        if (alienSwarm2->getAliens().empty())
        {
            alienSwarm2->increaseResetCounter(); // Increase the reset counter
            alienSwarm2->increaseSpeed(0.1f); // Increase speed
            alienSwarm2->reset(); // Spawn new swarm
            alienSwarm2->increaseShootingSpeed(0.1f); // Increase shooting speed
            alienSwarm2->increaseNumOfShooter(); // Increase number of shooters (if reset counter is 2 and the numOfShooter is less than 15)
        }
        //check if aliens reached the bottom
        for (Alien* alien : alienSwarm1->getAliens()) {
            if (alien->getRect().y + alien->getRect().h >= player1->getRect().y) {
                setGameState(GameState::GAME_OVER);
                break;
            }
        }
        for (Alien* alien : alienSwarm2->getAliens()) {
            if (alien->getRect().y + alien->getRect().h >= player2->getRect().y) {
                setGameState(GameState::GAME_OVER);
                break;
            }
        }
        // Check if player's lives is 0
        if (livesManager1->isGameOver())
        {
            player1win = false;
            gameOver = true;
            setGameState(GameState::GAME_OVER);	
        }
		if (livesManager2->isGameOver())
		{
            player1win = true;
            gameOver = true;
			setGameState(GameState::GAME_OVER);
		}
        break;
    case GameState::PAUSE:
        break;
    case GameState::GAME_OVER:
        if (gameOver == true) {
            Mix_HaltMusic();
            Mix_PlayChannel(-1, gameOverSound, 0);
            gameOver = false;
        }
        if (resetPlayerPosition == true) {
            resetPlayerPosition = false;
        }

        if (initialResetSwarm == true) {
            initialResetSwarm = false;
        }
        // Clear and reset all game entities
        for (Bullet* bullet : bullets) {
            delete bullet;
        }
        bullets.clear();
        for (Bullet* bullet : enemiesBullets) {
            delete bullet;
        }
        enemiesBullets.clear();
        alienSwarm1->reset(); 
		alienSwarm2->reset(); 
        livesManager1->reset(); 
        livesManager2->reset(); 
        // Add score if played in single player mode
        if (singlePlayer == true) {
            if (!scoreAdded) {
                highScore->addScore(score->getScore());
                highScore->save();
                scoreAdded = true;
                score->reset();
            }
        }
        break;
    }
    lastFrameTime = currentFrameTime;
}

void Game::render() {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set clear color to black
    SDL_RenderClear(renderer);
    // Draw game objects 
    switch (currentState) {
    case GameState::MENU:
        // Render menu elements
        startBackground->render(renderer);
        gameTitle->render(renderer);
        startButton->render(renderer);
		scoreButton->render(renderer);
        quitButton->render(renderer);
        break;
	case GameState::CHOOSE_MODE:
		// Render choose mode elements
		startBackground->render(renderer);
        arrow->render(renderer);
		singleButton->render(renderer);
		duoButton->render(renderer);
        break;
    case GameState::SINGLE:
        // Render game elements 
        gameBackground->render(renderer);
        player1->render(renderer);
        alienSwarm1->render(renderer); 
		for (PowerUp* powerUp : powerUps) {
			powerUp->render(renderer);
		}
        for (Bullet* bullet : bullets) {
            bullet->render(renderer);
        }
        for (Bullet* bullet : enemiesBullets) {
            bullet->render(renderer);
        }
        livesManager1->render(10, 10); 
        score->render(1510, 10);
        break;
    case GameState::DUO:
        // Render game elements 
        gameBackground->render(renderer);
        player1->render(renderer);
		player2->render(renderer);
        alienSwarm1->render(renderer);
		alienSwarm2->render(renderer);
        for (PowerUp* powerUp : powerUps) {
            powerUp->render(renderer);
        }
        for (Bullet* bullet : bullets) {
            bullet->render(renderer);
        }
        for (Bullet* bullet : enemiesBullets) {
            bullet->render(renderer);
        }
        livesManager1->render(10, 10);
        livesManager2->render(875, 10);
        // Draw the border
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
		SDL_RenderFillRect(renderer, border);
        break;
    case GameState::PAUSE:
        // Render pause menu
        gameBackground->render(renderer);
        player1->render(renderer);
		livesManager1->render(10, 10);
        alienSwarm1->render(renderer);
        for (PowerUp* powerUp : powerUps) {
            powerUp->render(renderer);
        }
        for (Bullet* bullet : bullets) {
            bullet->render(renderer);
        }
        for (Bullet* bullet : enemiesBullets) {
            bullet->render(renderer);
        }
		if (singlePlayer == true) {
            score->render(1510, 10);
		} else {
            player2->render(renderer);
			alienSwarm2->render(renderer);
            livesManager2->render(875, 10);
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
            SDL_RenderFillRect(renderer, border);
        }
        pauseGoToMenuButton->render(renderer);
        break;
    case GameState::GAME_OVER:
        // Render game over screen elements
        gameOverBackground->render(renderer);
        quitButton->render(renderer);
		goToMenuButton->render(renderer);
		replayButton->render(renderer);
        if (singlePlayer == true) {
            gameOverTitle->render(renderer);
        }
        else {
            if (player1win == true) {
                P1Win->render(renderer);
            }
            else {
                P2Win->render(renderer);
            }
        }
        break;
	case GameState::HIGHSCORE:
		// Render highscore screen elements
        startBackground->render(renderer);
        arrow->render(renderer);
		highScore->render(windowWidth/2, 100, scoreFont);
    }
    // Update the screen
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    // Destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // Release font
    if (gameFont)
    {
        TTF_CloseFont(gameFont);
        gameFont = nullptr;
    }
    if (scoreFont)
    {
        TTF_CloseFont(scoreFont);
        scoreFont = nullptr;
    }
    // Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}