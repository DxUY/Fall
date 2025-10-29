#pragma once

#include "FallEnginePCH.h"

namespace FallEngine
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = 1 << 0,
		EventCategoryInput = 1 << 1,
		EventCategoryKeyboard = 1 << 2,
		EventCategoryMouse = 1 << 3,
		EventCategoryMouseButton = 1 << 4
	};

#define EVENT_CLASS_TYPE(type) \
	static EventType GetStaticType() { return EventType::type; } \
	EventType GetEventType() const override { return GetStaticType(); } \
	const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
	int GetCategoryFlags() const override { return static_cast<int>(category); }

	class  Event
	{
	protected:
		bool m_Handled = false;

	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category) const
		{
			return GetCategoryFlags() & static_cast<int>(category);
		}

		bool IsHandled() const { return m_Handled; }
		void SetHandled(bool handled) { m_Handled = handled; }

		virtual ~Event() = default;
	};

	class  EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event) {
		}

		template<typename T>
		using EventFn = std::function<bool(T&)>;

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.SetHandled(func(static_cast<T&>(m_Event)));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}