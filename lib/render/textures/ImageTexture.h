#ifndef RENDER_IMAGE_TEXTURE_H
#define RENDER_IMAGE_TEXTURE_H

#include "Texture.h"

#include <vector>

namespace render
{

class ImageTexture : public Texture
{
public:
    ImageTexture(int cols, int rows, std::unique_ptr<uint8_t>& data, int bytesPerPixel);

    virtual geom::Vec3 value(float u, float v, const geom::Vec3& p) const;

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
