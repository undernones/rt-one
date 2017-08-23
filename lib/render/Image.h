//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_IMAGE_H
#define RENDER_IMAGE_H

#include <stdint.h>
#include <vector>

namespace render
{

class Image
{
public:
    Image();
    Image(int rows, int cols);
    Image(const Image& other) = default;
    ~Image() = default;

    Image& operator =(const Image& rhs) = default;

    void clear(uint32_t clearColor = 0x000000FF);

    const uint32_t& value(int row, int col) const;
    void setValue(int row, int col, uint32_t value);

    int rows() const { return mRows; }
    int cols() const { return mCols; }
    float aspectRatio() const { return mAspectRatio; }

    const std::vector<uint32_t>& getRgba() const { return mValues; }
    std::vector<uint32_t> rToRgba() const;
    std::vector<uint32_t> gToRgba() const;
    std::vector<uint32_t> bToRgba() const;
    std::vector<uint32_t> zToRgba() const;

private:
    int mRows;
    int mCols;
    float mAspectRatio;
    std::vector<uint32_t> mValues;
};

}
#endif // RENDER_IMAGE_H
