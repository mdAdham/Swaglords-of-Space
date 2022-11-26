#include "LayerMachine.h"
#include <iostream>

namespace Swag
{
	void LayerMachine::AddLayer(LayerRef newLayer, bool isReplacing)
	{
		this->_isAdding = true;
		this->_isReplacing = isReplacing;

		this->_newLayer = std::move(newLayer);
	}

	void LayerMachine::RemoveLayer()
	{
		this->_isRemoving = true;
	}

	void LayerMachine::ProcessLayerChanges()
	{
		if (this->_isRemoving && !this->_layers.empty())
		{
			this->_layers.pop();

			if (!this->_layers.empty())
			{
				this->_layers.top()->Resume();
			}

			this->_isRemoving = false;
		}

		if (this->_isAdding)
		{
			if (!this->_layers.empty())
			{
				if (this->_isReplacing)
				{
					this->_layers.pop();
				}
				else
				{
					this->_layers.top()->Pause();
				}
			}

			this->_layers.push(std::move(this->_newLayer));
			this->_layers.top()->Init();
			this->_isAdding = false;
		}
	}

	LayerRef& LayerMachine::GetActiveLayer()
	{
		return this->_layers.top();
	}
}