
// #include "CTextureCache.hpp"
// #include "CTexture2D.hpp"


// CTextureSharedPtr CTextureCache::get(const fs::path& path) const
// {
//     const auto & it = mCache.find(path.native());
//     if (it != mCache.end())
//     {
//         return it->second.lock();
//     }

//     return nullptr;
// }


// void CTextureCache::add(const fs::path& path, const CTextureSharedPtr& tex)
// {
//     mCache[path.native()] = tex;
// }
