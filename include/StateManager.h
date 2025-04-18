#pragma once
#include <vector>
#include <unordered_map>
#include "State_Intro.h"
#include "State_MainMenu.h"
#include "State_Game.h"
#include "State_Paused.h"
#include "State_GameOver.h"
#include "State_LevelCompleted.h"
#include "State_ChooseMap.h"
#include "State_YesNoMenu.h"
#include "SharedContext.h"


enum class StateType{ Intro = 1, MainMenu, Game, Paused, GameOver, Credits, LevelCompleted, ChooseMap, YesNoMenu };

// State container.
using StateContainer = std::vector<std::pair<StateType, BaseState*>>;
// Type container.
using TypeContainer = std::vector<StateType>;
// State factory.
using StateFactory = std::unordered_map<StateType, std::function<BaseState*(void)>>;

class StateManager{
public:
	StateManager(SharedContext* l_shared);
	~StateManager();

	void Update(const sf::Time& l_time);
	void Draw();

	void ProcessRequests();

	SharedContext* GetContext();
	bool HasState(const StateType& l_type);

	void SwitchTo(const StateType& l_type);
	void Remove(const StateType& l_type);
private:
	// Methods.
	void CreateState(const StateType& l_type);
	void RemoveState(const StateType& l_type);

	template<class T>
	void RegisterState(const StateType& l_type){
		m_stateFactory[l_type] = [this]() -> BaseState*
		{
			return new T(this);
		};
	}

	// Members.
	SharedContext* m_shared;
	StateContainer m_states;
	TypeContainer m_toRemove;
	StateFactory m_stateFactory;

	sf::Music m_musicMain;

	sf::SoundBuffer m_bufferUIsound;
	sf::Sound m_UIsound;

};