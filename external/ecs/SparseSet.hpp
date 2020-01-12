#pragma once

#include <vector>

namespace ecs
{

template<typename Id, typename T>
class SparseSet
{
    static constexpr auto Undefined = std::numeric_limits<std::size_t>::max();

public:
    template<typename ...Args>
    std::pair<Id, T&> emplace(Args&& ...args)
    {
        // Add object
        std::size_t i = mObjects.size();
        auto& object = mObjects.emplace_back(std::forward<Args>(args)...);
        // Get the correct id and set the links
        Id id;
        if (mFreeIds.empty())
        {
            id = static_cast<Id>(mIdToIndex.size());
            mIdToIndex.push_back(i);
        }
        else
        {
            id = mFreeIds.back();
            mFreeIds.pop_back();
            mIdToIndex[static_cast<std::size_t>(id)] = i;
        }
        mIndexToId.push_back(id);
        return std::pair<Id, T&>(id, object);
    }

    bool has(Id id) const
    {
        return static_cast<std::size_t>(id) < mIdToIndex.size() &&
            mIdToIndex[static_cast<std::size_t>(id)] != Undefined;
    }

    T& get(Id id)
    {
        return mObjects[mIdToIndex[static_cast<std::size_t>(id)]];
    }

    const T& get(Id id) const
    {
        return mObjects[mIdToIndex[static_cast<std::size_t>(id)]];
    }

    void erase(Id id)
    {
        // Get the index of the object to destroy
        std::size_t i = mIdToIndex[static_cast<std::size_t>(id)];
        // Swap with the last object and update its index
        // Moving std::unordered_map may causes a segfault
        std::swap(mObjects[i], mObjects.back());
        //mObjects[i] = std::move(mObjects.back());
        Id lastObjectId = mIndexToId.back();
        mIdToIndex[static_cast<std::size_t>(lastObjectId)] = i;
        mIndexToId[i] = lastObjectId;
        // Erase the last object and its index
        mObjects.pop_back();
        mIndexToId.pop_back();
        // Assign Undefined to the id
        mIdToIndex[static_cast<std::size_t>(id)] = Undefined;
        // Add the deleted Id to the free Ids
        mFreeIds.push_back(id);
    }

    std::vector<T>& getObjects()
    {
        return mObjects;
    }

    const std::vector<T>& getObjects() const
    {
        return mObjects;
    }

    std::size_t getSize() const
    {
        return mObjects.size();
    }

    void reserve(std::size_t size)
    {
        mIdToIndex.reserve(size);
        mFreeIds.reserve(size);
        mObjects.reserve(size);
        mIndexToId.reserve(size);
    }

  private:
    std::vector<std::size_t> mIdToIndex;
    std::vector<Id> mFreeIds;
    std::vector<T> mObjects;
    std::vector<Id> mIndexToId;
};

}