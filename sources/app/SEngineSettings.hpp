#pragma once

#include <vector>
#include <glm/vec2.hpp>


struct SEngineSettings
{
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
