//
// Copyright © 2017 Undernones. All rights reserved.
//

#ifndef RENDER_FLIP_NORMALS_H
#define RENDER_FLIP_NORMALS_H

#include <render/Transform.h>

namespace render
{

class FlipNormals : public Transform
{
public:
    FlipNormals(std::shared_ptr<Hitable>& object);
    FlipNormals(std::shared_ptr<Hitable>&& object);
    virtual ~FlipNormals() = default;

protected:
    virtual void transform(Ray& ray);
};

}

#endif // RENDER_FLIP_NORMALS_H
