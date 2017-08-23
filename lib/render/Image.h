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

    void clear(int32_t clearColor = 0);

    const int32_t& value(int row, int col) const;
    void setValue(int row, int col, int32_t value);

    int rows() const { return mRows; }
    int cols() const { return mCols; }
    float aspectRatio() const { return mAspectRatio; }

    const void* bytes() const { return (void*)mValues.data(); }

private:
    int mRows;
    int mCols;
    float mAspectRatio;
    std::vector<int32_t> mValues;
};

}
#endif // RENDER_IMAGE_H
