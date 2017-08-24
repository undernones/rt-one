//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_IMAGE_H
#define RENDER_IMAGE_H

#include <stdint.h>
#include <vector>

#include <geom/Vec3.h>

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

    void clear(geom::Vec3 clearColor = geom::Vec3(0, 0, 0));

    const geom::Vec3& value(int row, int col) const;
    void setValue(int row, int col, const geom::Vec3& value);

    int rows() const { return mRows; }
    int cols() const { return mCols; }
    float aspectRatio() const { return mAspectRatio; }

    std::vector<uint32_t> getRgba() const;
    std::vector<uint32_t> rToRgba() const;
    std::vector<uint32_t> gToRgba() const;
    std::vector<uint32_t> bToRgba() const;
    std::vector<uint32_t> zToRgba() const;

private:
    int mRows;
    int mCols;
    float mAspectRatio;
    std::vector<geom::Vec3> mValues;
};

}
#endif // RENDER_IMAGE_H
