#pragma once

#include <memory>
#include <stack>

#include "State.h"

namespace Swag
{
	typedef std::unique_ptr<State> StateRef;

	class StateMachine
	{
	public:
		//StateMachine() { }
		~StateMachine() { }

		void AddState(StateRef newState, bool isReplacing);
		void RemoveState();

		void ProcessStateChanges();

		void OnDontDestroyOnLoad();

		StateRef& GetActiveState();

	private:
		std::stack<StateRef> _states;
		StateRef _newState;

		bool _isRemoving = false;
		bool _isAdding = false;
		bool _isReplacing = false;

	};
}