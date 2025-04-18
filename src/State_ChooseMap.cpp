#include "State_ChooseMap.h"
#include "StateManager.h"

State_ChooseMap::State_ChooseMap(StateManager* l_stateManager)
    : BaseState(l_stateManager){}

State_ChooseMap::~State_ChooseMap(){}

void State_ChooseMap::Activate(){
	m_stateMgr->Remove(StateType::Game); // Destroy 'Game' state before Map selection
}

void State_ChooseMap::OnCreate(){

	m_font.loadFromFile(Utils::GetResourceDirectory() + "media/Fonts/FORCED_SQUARE.ttf");
	m_text.setFont(m_font);
	m_text.setString(sf::String("Choose Level:"));
	m_text.setCharacterSize(20);

	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);

	m_text.setPosition(410,100);

	m_buttonSize = sf::Vector2f(300.0f,32.0f);
	m_buttonPos = sf::Vector2f(420,200);
	m_buttonPadding = 4; // 4px.

	// Only 2 buttons now
	std::string str[2]; 
	str[0] = "MAP 1";
	str[1] = "BACK";

	// Loop only 2 times
	for(int i = 0; i < 2; ++i){ 
		sf::Vector2f buttonPosition(
			m_buttonPos.x,m_buttonPos.y + 
			(i * (m_buttonSize.y + m_buttonPadding)));
		m_rects[i].setSize(m_buttonSize);
		m_rects[i].setFillColor(sf::Color(58,28,63,255));

		m_rects[i].setOrigin(
			m_buttonSize.x / 2.0f, m_buttonSize.y / 2.0f);
		m_rects[i].setPosition(buttonPosition);

		m_labels[i].setFont(m_font);
		m_labels[i].setString(sf::String(str[i]));
		m_labels[i].setCharacterSize(19);

		sf::FloatRect rect = m_labels[i].getLocalBounds();
		m_labels[i].setOrigin(
			rect.left + rect.width / 2.0f,
			rect.top + rect.height / 2.0f);

		m_labels[i].setPosition(buttonPosition);
	}

	EventManager* evMgr = m_stateMgr->
		GetContext()->m_eventManager;
	evMgr->AddCallback(StateType::ChooseMap, "Mouse_Left",&State_ChooseMap::MouseClick,this);
}

void State_ChooseMap::MouseClick(EventDetails* l_details){
	SharedContext* context = m_stateMgr->GetContext();
	sf::Vector2i mousePos = l_details->m_mouse;

	float halfX = m_buttonSize.x / 2.0f;
	float halfY = m_buttonSize.y / 2.0f;
	
	// Loop only 2 times
	for(int i = 0; i < 2; ++i){ 
		if(mousePos.x >= m_rects[i].getPosition().x - halfX &&
		   mousePos.x <= m_rects[i].getPosition().x + halfX &&
		   mousePos.y >= m_rects[i].getPosition().y - halfY &&
		   mousePos.y <= m_rects[i].getPosition().y + halfY)
		{
			if(i == 0){ // MAP 1 button
				std::cout<<"Player chose map 1"<<std::endl;
				m_stateMgr->GetContext()->m_mapNumber = 1; // Set map number to 1
				m_stateMgr->SwitchTo(StateType::Game);
			} else if(i == 1){ // BACK button
				m_stateMgr->SwitchTo(StateType::MainMenu);
			}
			// Removed the else if for i == 1 (which was MAP 2)
		}
	}
}

void State_ChooseMap::OnDestroy(){
	EventManager* evMgr = m_stateMgr->
		GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::ChooseMap, "Mouse_Left");
}

void State_ChooseMap::Draw(){
	sf::RenderWindow* window = m_stateMgr->
		GetContext()->m_wind->GetRenderWindow();
	window->draw(m_text);
	
	// Loop only 2 times
	for(int i = 0; i < 2; ++i){ 
		window->draw(m_rects[i]);
		window->draw(m_labels[i]);
	}
}

void State_ChooseMap::Deactivate(){}
void State_ChooseMap::Update(const sf::Time& l_time){}