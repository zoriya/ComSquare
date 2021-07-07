//
// Created by Zoe Roux on 5/21/21.
//


#pragma once

#include <functional>
#include <utility>
#include <unordered_map>

namespace ComSquare
{
	//! @brief A callback where you can subscribe to and emit it.
	template<typename ...Types>
	class Callback
	{
	private:
		int _nextID = 0;
		//! @brief The list of functions to call.
		std::unordered_map<int, std::function<void (Types...)>> _functions = {};

	public:
		//! @brief Add a method to be called when this callback is invoked.
		//! @param callback The list of arguments of the callback method
		//! @return A unique ID for this callback. That can be used to remove the callback later.
		template<typename Func>
		int addCallback(Func callback)
		{
			int id = this->_nextID++;
			if constexpr(std::is_same_v<Func, std::function<void (Types...)>>)
				this->_functions[id] = std::move(callback);
			else
				this->_functions[id] = std::function<void (Types...)>(callback);
			return id;
		}

		//! @brief Remove a function from this callback.
		//! @param id The ID of the function.
		void removeCallback(int id)
		{
			this->_functions.erase(id);
		}

		void operator()(Types ...args) const
		{
			for (const auto &[_, callback] : this->_functions)
				callback(args...);
		}

		//! @brief A default constructor
		Callback() = default;
		//! @brief A default copy constructor
		Callback(const Callback &) = default;
		//! @brief A default destructor
		~Callback() = default;
		//! @brief A default assignment operator
		Callback &operator=(const Callback &) = default;

		//! @brief Implicitly transform a callable into a callback.
		template<typename Func>
		Callback(Func callback) // NOLINT(google-explicit-constructor)
		{
			this->addCallback(callback);
		}
	};
} // namespace WAL