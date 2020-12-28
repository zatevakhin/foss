#pragma once

/*!
 * @file
 * @brief File, with the class implementing the \b registry to store various data in the format key-value.
 * */

#include <boost/noncopyable.hpp>
#include <boost/any.hpp>
#include <iostream>
#include <map>


/*!
 * @brief Class which is implementing \b registry with data in the format key-value.
 * */
class CRegistry
{

public:
    /*!
     * @brief The deleted constructor forbidding the creation of objects of this class.
     * */
    CRegistry() = delete;

    /*!
     * @brief Delete the copy constructor forbidding the copying objects of this class.
     * */
    CRegistry(const CRegistry&) = delete;

    /*!
     * @brief Stores a value by key in the registry
     * @param key - Registry key
     * @param value - The value which can be any type
     * */
    inline static void set(const std::string & key, boost::any value)
    {
        CRegistry::registry[key] = value;
    }

    /*!
     * @brief Returns the value stored in the registry with the conversion it to the specified type
     * @param key - Registry key
     * @return The value which is stored in the registry
     * */
    template <typename Type>
    inline static Type get(const std::string & key)
    {
        return boost::any_cast<Type>(CRegistry::registry[key]);
    }


private:
    /*!
     * @brief Keeps pairs key-value.
     * */
    static std::map<std::string, boost::any> registry;
};
