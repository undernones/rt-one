//
// Copyright © 2017 Undernones. All rights reserved.
//

#include "Image.h"

#include <assert.h>

namespace render
{

Image::Image()
    : Image(0, 0)
{
}
Image::Image(int rows, int cols)
    : mRows(rows)
    , mCols(cols)
    , mAspectRatio(static_cast<float>(cols) / rows)
    , mValues(rows * cols)
{
}

void
Image::clear(int32_t clearColor)
{
    for (auto& value: mValues) {
        value = clearColor;
    }
}

const int32_t&
Image::value(int row, int col) const
{
    assert(row >= 0 && row < mRows);
    assert(col >= 0 && col < mCols);
    row = (mRows - 1) - row;
    return mValues[mCols * row + col];
}

void
Image::setValue(int row, int col, int32_t value)
{
    assert(row >= 0 && row < mRows);
    assert(col >= 0 && col < mCols);
    row = (mRows - 1) - row;
    mValues[mCols * row + col] = value;
}

}
