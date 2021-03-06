//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_IMAGE_TEXTURE_H
#define RENDER_IMAGE_TEXTURE_H

#include <render/Texture.h>

namespace render
{

class ImageTexture : public Texture
{
public:
    ImageTexture(int cols, int rows, std::unique_ptr<uint8_t>& data, int bytesPerPixel);

    virtual geom::Vec3 value(const geom::Vec2& uv, const geom::Vec3& p) const;

    static std::shared_ptr<Texture> loadFromFile(const char* fileName);

private:
    int mCols;
    int mRows;
    std::unique_ptr<uint8_t> mData;
    int mBytesPerPixel;

    int mOffsetR;
    int mOffsetG;
    int mOffsetB;
};

}
#endif // RENDER_IMAGE_TEXTURE_H
