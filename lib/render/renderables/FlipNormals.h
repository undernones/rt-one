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
    FlipNormals(std::shared_ptr<Renderable>& object);
    FlipNormals(std::shared_ptr<Renderable>&& object);
    virtual ~FlipNormals() = default;

protected:
    virtual void transform(Ray& ray) const;
};

}

#endif // RENDER_FLIP_NORMALS_H
