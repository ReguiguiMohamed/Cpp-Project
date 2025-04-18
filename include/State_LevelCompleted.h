#pragma once
#include "BaseState.h"
#include "EventManager.h"

class State_LevelCompleted : public BaseState{
public:
	State_LevelCompleted(StateManager* l_stateManager);
	~State_LevelCompleted();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& l_time);
	void Draw();

    void MouseClick(EventDetails* l_details);

private:
	sf::Font m_font;
	sf::Text m_text;

	sf::Vector2f m_buttonSize;
	sf::Vector2f m_buttonPos;
	unsigned int m_buttonPadding;

	// Changed array size from 2 to 1
	sf::RectangleShape m_rects[1]; 
	sf::Text m_labels[1];

	sf::SoundBuffer m_bufferSuccessSound;
	sf::Sound m_SuccessSound;
};