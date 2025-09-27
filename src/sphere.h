#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
# include "interval.h"
class sphere : public hittable {
    public:
        // Stationary Sphere
        sphere(const point3& static_center, double radius, shared_ptr<material> mat)
            : center(static_center, vec3(0,0,0)),radius(std::fmax(0, radius)), mat(mat) {
                auto rvec = vec3(radius, radius, radius);
                bbox = aabb(static_center - rvec, static_center + rvec);
            }

        // Moving Sphere
        sphere(const point3& center0, const point3& center1, double radius, shared_ptr<material> mat)
            : center(center0, center1 - center0), radius(std::fmax(0, radius)), mat(mat) {
                auto rvec = vec3(radius, radius, radius);
                aabb bbox1 = aabb(center.at(0) - rvec, center.at(0) + rvec);
                aabb bbox2 = aabb(center.at(1) - rvec, center.at(1) + rvec);
                bbox = aabb(bbox1, bbox2);
            }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
            point3 curr_center = center.at(r.time());
            vec3 oc = curr_center - r.origin();
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
                rec.mat = mat; // Assign the material of the sphere to the hit record
                vec3 outward_normal = (rec.p - curr_center) / radius; // Normal at the intersection point
                rec.set_face_normal(r, outward_normal); // Set the normal and front face
                return true; // Intersection found
            
        }
        
        aabb bounding_box() const override {return bbox;}

    private:
        ray center;
        double radius;
        shared_ptr<material> mat; // Pointer to the material of the sphere
        aabb bbox;
};

#endif // SPHERE_H