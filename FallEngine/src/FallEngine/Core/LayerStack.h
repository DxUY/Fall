#pragma once

#include <vector>
#include "FallEngine/Core/Base.h" // for Scope

namespace FallEngine {

	class Layer;

	class LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Scope<Layer> layer);
		void PushOverlay(Scope<Layer> overlay);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		class Iterator {
		public:
			using VecIt = std::vector<Scope<Layer>>::iterator;
			Iterator(VecIt it) : m_It(it) {}
			Layer* operator*() const { return m_It->get(); }
			Iterator& operator++() { ++m_It; return *this; }
			bool operator!=(const Iterator& other) const { return m_It != other.m_It; }
		private:
			VecIt m_It;
		};

		Iterator begin() { return Iterator(m_Layers.begin()); }
		Iterator end() { return Iterator(m_Layers.end()); }

	private:
		std::vector<Scope<Layer>> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}