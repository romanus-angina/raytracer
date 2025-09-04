#ifndef HITTABLE_H
#define HITTABLE_H
# include "interval.h"

class hit_record{
    public:
        point3 p; // Point of intersection
        vec3 normal; // Normal at the intersection point
        double t; // Parameter t for the ray equation
        bool front_face; // Indicates if the ray hit the front or back face of the object

        void set_face_normal(const ray& r, const vec3& outward_normal) {
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif // HITTABLE_H