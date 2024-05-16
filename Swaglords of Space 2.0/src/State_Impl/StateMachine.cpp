#include "StateMachine.hpp"

namespace _Swag {

	void _StateMachine::AddState(Ref<State> newState, bool isReplacing = false)
	{
		this->_isAdding = true;
		this->_isReplacing = isReplacing;

		this->_newState = std::move(newState);
	}

	void _StateMachine::RemoveState()
	{
		this->_isRemoving = true;
	}

	void _StateMachine::ProcessStateChanges()
	{
		if (this->_isRemoving && !this->_states.empty())
		{
			this->_states.top()->OnDestroy();
			this->_states.pop();

			if (!this->_states.empty())
			{
				this->_states.top()->OnResume();
			}

			this->_isRemoving = false;
		}

		if (this->_isAdding)
		{
			if (!this->_states.empty())
			{
				if (this->_isReplacing)
				{
					this->_states.top()->OnDestroy();
					this->_states.pop();
				}
				else
				{
					this->_states.top()->OnPause();
				}
			}


			this->_states.push(std::move(this->_newState));
			this->_states.top()->Init();
			
			this->_isAdding = false;
		}
	}

	Ref<State>& _StateMachine::GetActiveState()
	{
		if(!_states.empty())
			return this->_states.top();
		else
		{
			_SWAG_CRITICAL("No State Was Found");
			__debugbreak();
			exit(-232);
		}
	}

	const bool _StateMachine::IsEmpty() const
	{
		return this->_states.empty();
	}

}