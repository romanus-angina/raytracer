#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
# include "interval.h"
class sphere : public hittable {
    public:
        sphere(const point3& center, double radius): center(center), radius(std::fmax(0, radius)) {
            //TODO: Initialize material pointer mat
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
            vec3 oc = center - r.origin();
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), (oc));
            auto c = oc.length_squared() - radius * radius;
            auto discriminant = h * h - a * c;
                if (discriminant < 0) {
                    return false; // No intersection
                } 
                auto sqrtd = std::sqrt(discriminant);

                //Calculate nearest root in acceptable range
                auto root = (h - sqrtd) / a;
                if (!ray_t.surrounds(root)) {
                    root = (h + sqrtd) / a;
                    if (!ray_t.surrounds(root)) {
                        return false; // No valid intersection
                    }
                }
                rec.t = root;
                rec.p = r.at(rec.t);
                vec3 outward_normal = (rec.p - center) / radius; // Normal at the intersection point
                rec.set_face_normal(r, outward_normal); // Set the normal and front face
                return true; // Intersection found
            
        }
        

    private:
        point3 center;
        double radius;
        shared_ptr<material> mat; // Pointer to the material of the sphere
};

#endif // SPHERE_H