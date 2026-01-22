#ifndef QUAD_H
#define QUAD_H
#include "hittable.h"

class quad : public hittable {
    public:
        quad(const point3& Q, const vec3& u, vec3& v, shared_ptr<material> mat)
        : Q(Q), u(u), v(v), mat(mat) {
            auto n = cross(u, v);
            normal = unit_vector(n);
            D = dot(normal, Q);
            set_bounding_box();
        }

        virtual void set_bounding_box() {
            auto bbox_diagonal1 = aabb(Q, Q + u + v);
            auto bbox_diagonal2 = aabb(Q + u, Q + v);
            bbox = aabb(bbox_diagonal1, bbox_diagonal2);
        }

        aabb bounding_box() const override {
            return bbox;
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
            auto denom = dot(normal, r.direction());
            if(std::fabs(denom) < 1e-8) {
                return false; // Ray is parallel to the quad
            }

            auto t = (D - dot(normal, r.origin())) / denom;

            if(!ray_t.contains(t)) {
                return false; // Intersection is outside the ray's valid t interval
            }

            auto intersection = r.at(t);

            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;

            rec.set_face_normal(r, normal);

            return true;
        }
    private:
        point3 Q; // One corner of the quad
        vec3 u;   // Edge vector from Q
        vec3 v;   // Edge vector from Q
        shared_ptr<material> mat;   // Material of the quad
        aabb bbox; // Axis-aligned bounding box
        vec3 normal; // Normal vector of the quad
        double D; // Plane constant
};

#endif