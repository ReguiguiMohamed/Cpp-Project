#include "State_LevelCompleted.h"
#include "StateManager.h"

State_LevelCompleted::State_LevelCompleted(StateManager* l_stateManager)
    : BaseState(l_stateManager){}

State_LevelCompleted::~State_LevelCompleted(){}

void State_LevelCompleted::OnCreate(){

	m_bufferSuccessSound.loadFromFile(Utils::GetResourceDirectory() + "SoundEffects/chipquest.wav");
	m_SuccessSound.setBuffer(m_bufferSuccessSound);
	m_SuccessSound.setVolume(50.0f);

	m_font.loadFromFile(Utils::GetResourceDirectory() + "media/Fonts/arial.ttf");
	m_text.setFont(m_font);
	m_text.setString(sf::String("Level Completed"));
	m_text.setCharacterSize(20);

	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);

	m_text.setPosition(400,100);
	
	// Keep original button size, position, and padding
	m_buttonSize = sf::Vector2f(300.0f,32.0f);
	m_buttonPos = sf::Vector2f(400,200); // Position for the single button
	m_buttonPadding = 4; // Not really needed now, but keep for consistency

	// Only one label needed
	std::string str = "Exit to Main Menu"; 

	// Setup the single button (index 0)
	m_rects[0].setSize(m_buttonSize);
	m_rects[0].setFillColor(sf::Color::Red); // Keep color, you might want to change it
	m_rects[0].setOrigin(m_buttonSize.x / 2.0f, m_buttonSize.y / 2.0f);
	m_rects[0].setPosition(m_buttonPos); // Use the defined position

	m_labels[0].setFont(m_font);
	m_labels[0].setString(sf::String(str));
	m_labels[0].setCharacterSize(20);

	sf::FloatRect rect = m_labels[0].getLocalBounds();
	m_labels[0].setOrigin(
		rect.left + rect.width / 2.0f,
		rect.top + rect.height / 2.0f);

	m_labels[0].setPosition(m_buttonPos); // Position label at the same spot

	// Register the callback
	EventManager* evMgr = m_stateMgr->
		GetContext()->m_eventManager;
	evMgr->AddCallback(StateType::LevelCompleted, "Mouse_Left", &State_LevelCompleted::MouseClick, this);
}

void State_LevelCompleted::OnDestroy(){
	EventManager* evMgr = m_stateMgr->
		GetContext()->m_eventManager;

	evMgr->RemoveCallback(StateType::LevelCompleted, "Mouse_Left");
}


void State_LevelCompleted::MouseClick(EventDetails* l_details){
	SharedContext* context = m_stateMgr->GetContext();
	sf::Vector2i mousePos = l_details->m_mouse;

	float halfX = m_buttonSize.x / 2.0f;
	float halfY = m_buttonSize.y / 2.0f;
	
	// Check only the first button (index 0)
	if(mousePos.x >= m_rects[0].getPosition().x - halfX &&
	   mousePos.x <= m_rects[0].getPosition().x + halfX &&
	   mousePos.y >= m_rects[0].getPosition().y - halfY &&
	   mousePos.y <= m_rects[0].getPosition().y + halfY)
	{
		// Always switch to Main Menu when the button is clicked
		m_stateMgr->SwitchTo(StateType::MainMenu);
	}
}

void State_LevelCompleted::Draw(){
	sf::RenderWindow* window = m_stateMgr->
		GetContext()->m_wind->GetRenderWindow();
	window->draw(m_text);
	
	// Draw only the first button (index 0)
	window->draw(m_rects[0]);
	window->draw(m_labels[0]);
}


void State_LevelCompleted::Activate(){
	m_SuccessSound.play();
}

void State_LevelCompleted::Deactivate(){}
void State_LevelCompleted::Update(const sf::Time& l_time){}