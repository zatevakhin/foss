#pragma once

#include <glm/vec2.hpp>
#include <vector>


struct SEngineSettings
{
    glm::ivec2 mVersion;
    glm::ivec2 mWindowSize;

    struct SComboBoxData
    {
        int mIndex;
        std::vector<unsigned int> mItems;
        std::vector<const char*> mItemsNames;
    };

    // flags
    bool mBlend;
    bool mDepthTest;
    bool mDepthMask;
    bool mCullFace;
    bool mPolygonOffsetFill;

    // Combo
    SComboBoxData mPolygonMode;
};
