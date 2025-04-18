#include "State_Game.h"
#include "StateManager.h"

State_Game::State_Game(StateManager* l_stateManager)
	: BaseState(l_stateManager){}

State_Game::~State_Game(){}

void State_Game::OnCreate(){
	EventManager* evMgr = m_stateMgr->
		GetContext()->m_eventManager;

	evMgr->AddCallback(StateType::Game, "Key_Escape", &State_Game::Pause, this);
	evMgr->AddCallback(StateType::Game, "Key_O", &State_Game::ToggleOverlay, this);

	sf::Vector2u size = m_stateMgr->GetContext()->m_wind->GetWindowSize();
	m_view.setSize(size.x,size.y);
	m_view.setCenter(size.x/2,size.y/2);
	m_view.zoom(0.6f);
	m_stateMgr->GetContext()->m_wind->GetRenderWindow()->setView(m_view);

	m_gameMap = new Map(m_stateMgr->GetContext(), this);

	int level = m_stateMgr->GetContext()->m_mapNumber;

	std::cout<<"Will load map "<<level<<std::endl;

	// i know this is not ideal 
	if(level==0){
		m_gameMap->LoadMap("media/Maps/map0.map");
		m_gameMap->LoadBackGround("media/Maps/map0.map.background");
	}
	else if(level==1)
		m_gameMap->LoadMap("media/Maps/map1.map");
	else if(level==2){
		m_gameMap->LoadMap("media/Maps/map2.map");
		m_gameMap->LoadBackGround("media/Maps/map2.map.background");
	}
		

	
	music.openFromFile(Utils::GetResourceDirectory() + "MusicTracks/" + m_gameMap->GetMusicName());
	music.setVolume(50.0f);
	music.setLoop(true);	
}

void State_Game::OnDestroy(){
	EventManager* evMgr = m_stateMgr->
		GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Game, "Key_Escape");
	evMgr->RemoveCallback(StateType::Game, "Key_O");
	
	delete m_gameMap;
	m_gameMap = nullptr;
}

void State_Game::Update(const sf::Time& l_time){
	SharedContext* context = m_stateMgr->GetContext();
	EntityBase* player = context->m_entityManager->Find("Player");
	if(!player){
		std::cout << "You died" << std::endl;
		m_stateMgr->SwitchTo(StateType::GameOver);
		return;
	} else {
		// printing player's position (DEBUG ONLY)
		//std::cout<<player->GetPosition().x << ' ' << player->GetPosition().y <<'\n';
		m_view.setCenter(player->GetPosition());
		context->m_wind->GetRenderWindow()->setView(m_view);
	}

	// Check if screen leaves the boundaries of the map.
	sf::FloatRect viewSpace = context->m_wind->GetViewSpace();
	// Check the top-left corner of the viewSpace
	if(viewSpace.left <= 0){
		// place top-left at the edge of the screen
		m_view.setCenter(viewSpace.width / 2, m_view.getCenter().y);
		context->m_wind->GetRenderWindow()->setView(m_view);
	} else if (viewSpace.left + viewSpace.width > (m_gameMap->GetMapSize().x) * Sheet::Tile_Size){
		// view is outside of the map in the opposite direction
		m_view.setCenter(((m_gameMap->GetMapSize().x) * Sheet::Tile_Size) - (viewSpace.width / 2), m_view.getCenter().y);
		context->m_wind->GetRenderWindow()->setView(m_view);
	}

	// std::cout << "viewSpace" << '\n';
	// std::cout << "viewSpace.top: "<< viewSpace.top << '\n';

	 if(viewSpace.height + viewSpace.top > (m_gameMap->GetMapSize().y) * Sheet::Tile_Size){
		m_view.setCenter(m_view.getCenter().x, ((m_gameMap->GetMapSize().y) * Sheet::Tile_Size) - (viewSpace.height / 2));
		context->m_wind->GetRenderWindow()->setView(m_view);
	}

	m_gameMap->Update(l_time.asSeconds());
	m_stateMgr->GetContext()->m_entityManager->Update(l_time.asSeconds());
}

void State_Game::Draw(){
	m_gameMap->Draw();
	m_stateMgr->GetContext()->m_entityManager->Draw();
}

void State_Game::Pause(EventDetails* l_details){
	m_stateMgr->SwitchTo(StateType::Paused);
}

void State_Game::Activate(){
	music.play();
}
void State_Game::Deactivate(){
	music.pause();
}
// Test/debug methods.
void State_Game::ToggleOverlay(EventDetails* l_details){
	m_stateMgr->GetContext()->m_debugOverlay.SetDebug(!m_stateMgr->GetContext()->m_debugOverlay.Debug());
}