#include "ImageTexture.h"

#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace render
{

ImageTexture::ImageTexture(int cols, int rows, std::unique_ptr<uint8_t>& data, int bytesPerPixel)
    : Texture()
    , mCols(cols)
    , mRows(rows)
    , mData(std::move(data))
    , mBytesPerPixel(bytesPerPixel)
{
    assert(bytesPerPixel == 1 || bytesPerPixel == 3);

    mOffsetR = 0;
    mOffsetG = mBytesPerPixel == 1 ? 0 : 1;
    mOffsetB = mBytesPerPixel == 1 ? 0 : 2;
}

geom::Vec3
ImageTexture::value(float u, float v, const geom::Vec3& p) const
{
    u = std::max(0.f, std::min(u, 1.f));
    v = std::max(0.f, std::min(v, 1.f));

    auto i = int((  u) * mCols);
    auto j = int((1-v) * mRows - 0.001);

    auto bpp = mBytesPerPixel;
    auto indexR = bpp*i + bpp*mCols*j + mOffsetR;
    auto indexG = bpp*i + bpp*mCols*j + mOffsetG;
    auto indexB = bpp*i + bpp*mCols*j + mOffsetB;

    auto r = int(mData.get()[indexR]) / 255.f;
    auto g = int(mData.get()[indexG]) / 255.f;
    auto b = int(mData.get()[indexB]) / 255.f;

    return geom::Vec3(r, g, b);
}

std::shared_ptr<Texture>
ImageTexture::loadFromFile(const char* fileName)
{
    int nx, ny, nn;
    auto bytes = std::unique_ptr<uint8_t>(stbi_load(fileName, &nx, &ny, &nn, 0));
    return std::make_shared<ImageTexture>(nx, ny, bytes, nn);
}

}
