#pragma once

#include <memory>
#include <stack>

#include "Layer.h"

namespace Swag
{
	typedef std::unique_ptr<Layer> LayerRef;

	class LayerMachine
	{
	public:
		~LayerMachine() { }

		void AddLayer(LayerRef newLayer, bool isReplacing);
		void RemoveLayer();

		void ProcessLayerChanges();

		LayerRef& GetActiveLayer();

	private:
		std::stack<LayerRef> _layers;
		LayerRef _newLayer;

		bool _isRemoving = false;
		bool _isAdding = false;
		bool _isReplacing = false;
	};
}