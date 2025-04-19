#include "Player.h"
#include "EntityManager.h"
#include "StateManager.h"

Player::Player(EntityManager *l_entityMgr)
	: Character(l_entityMgr)
{
	Load("Player.char");
	m_type = EntityType::Player;

	EventManager *events = m_entityManager->GetContext()->m_eventManager;
	events->AddCallback<Player>(StateType::Game, "Player_MoveLeft", &Player::React, this);
	events->AddCallback<Player>(StateType::Game, "Player_MoveRight", &Player::React, this);
	events->AddCallback<Player>(StateType::Game, "Player_Jump", &Player::React, this);
	events->AddCallback<Player>(StateType::Game, "Player_Attack", &Player::React, this);

	m_bufferPickup1.loadFromFile(Utils::GetResourceDirectory() + "SoundEffects/coin.wav");
	m_soundPickup1.setBuffer(m_bufferPickup1);

	m_bufferPickup2.loadFromFile(Utils::GetResourceDirectory() + "SoundEffects/1up2-Sound_effects_Pack_2.wav");
	m_soundPickup2.setBuffer(m_bufferPickup2);

	m_bufferPickup3.loadFromFile(Utils::GetResourceDirectory() + "SoundEffects/1up3-Sound_effects_Pack_2.wav");
	m_soundPickup3.setBuffer(m_bufferPickup3);
}

Player::~Player()
{
	EventManager *events = m_entityManager->GetContext()->m_eventManager;
	events->RemoveCallback(StateType::Game, "Player_MoveLeft");
	events->RemoveCallback(StateType::Game, "Player_MoveRight");
	events->RemoveCallback(StateType::Game, "Player_Jump");
	events->RemoveCallback(StateType::Game, "Player_Attack");
}

void Player::React(EventDetails *l_details)
{
	if (l_details->m_name == "Player_MoveLeft")
	{
		Character::Move(Direction::Left);
	}
	else if (l_details->m_name == "Player_MoveRight")
	{
		Character::Move(Direction::Right);
	}
	else if (l_details->m_name == "Player_Jump")
	{
		Character::Jump();
	}
	else if (l_details->m_name == "Player_Attack")
	{
		Character::Attack();
	}
}

void Player::OnEntityCollision(EntityBase *l_collider, bool l_attack)
{
	if (m_state == EntityState::Dying)
	{
		return;
	}

	// Only process if flagged as an attack, the player is in the Attacking state,
	// and the other entity is an Enemy or even a Player (if intended)
	if (l_attack && GetState() == EntityState::Attacking &&
		(l_collider->GetType() == EntityType::Enemy || l_collider->GetType() == EntityType::Player))
	{
		Character *opponent = (Character *)l_collider;

		// Increase the attack hitbox "just a bit" compared to before.
		// (Previous values were: left offset: -10; width extra: +20)
		sf::FloatRect playerHitbox(
			m_position.x - 8, // slightly less offset than before
			m_position.y - 5, // unchanged vertical expansion
			m_size.x + 28,	  // slightly larger width (was +20)
			m_size.y + 10	  // unchanged height expansion
		);

		sf::FloatRect opponentHitbox(
			opponent->GetPosition().x,
			opponent->GetPosition().y,
			opponent->GetSize().x,
			opponent->GetSize().y);

		// Only perform an attack if the opponent is within this adjusted range.
		if (!playerHitbox.intersects(opponentHitbox))
		{
			return; // Opponent not in adjusted attack range.
		}

		opponent->GetHurt(1);
		if (m_position.x > opponent->GetPosition().x)
		{
			opponent->AddVelocity(-32, 0);
		}
		else
		{
			opponent->AddVelocity(32, 0);
		}
	}
	else
	{
		// If not an attack collision, do nothing.
	}
}