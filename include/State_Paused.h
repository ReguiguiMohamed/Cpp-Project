#pragma once
#include "BaseState.h"
#include "EventManager.h"

class State_Paused : public BaseState{
public:
	State_Paused(StateManager* l_stateManager);
	~State_Paused();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& l_time);
	void Draw();

	void Unpause(EventDetails* l_details);

	void MouseClick(EventDetails* l_details);
	
private:
	sf::Font m_font;
	sf::Text m_text;

	sf::Vector2f m_buttonSize;
	sf::Vector2f m_buttonPos;
	unsigned int m_buttonPadding;

	sf::RectangleShape m_rects[3];
	sf::Text m_labels[3];
	sf::RectangleShape m_rect;
};