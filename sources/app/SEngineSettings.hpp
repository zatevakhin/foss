
#ifndef FOSS_SENGINESETTINGS_HPP
#define FOSS_SENGINESETTINGS_HPP

#include <vector>

struct SEngineSettings
{

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

#endif //FOSS_SENGINESETTINGS_HPP
