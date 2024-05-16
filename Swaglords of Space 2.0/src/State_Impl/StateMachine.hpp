#pragma once

#include "Core/Core.hpp"
#include <stack>

#include "State.hpp"

namespace _Swag
{
	class _StateMachine
	{
	public:
		void AddState(Ref<State> newState, bool isReplacing);
		void RemoveState();

		void ProcessStateChanges();

		Ref<State>& GetActiveState();

		const bool IsEmpty() const;

	private:
		std::stack<Ref<State>> _states;
		Ref<State> _newState;

		bool _isRemoving = false;
		bool _isAdding = false;
		bool _isReplacing = false;
	};
}