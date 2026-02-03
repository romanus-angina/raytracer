#ifndef HITTABLE_H
#define HITTABLE_H
# include "interval.h"
# include "aabb.h"
class material;
class hit_record{
    public:
        point3 p; // Point of intersection
        vec3 normal; // Normal at the intersection point
        double t; // Parameter t for the ray equation
        double u; // U texture coordinate
        double v; // V texture coordinate
        shared_ptr<material> mat; // Pointer to the material of the object hit
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

        virtual aabb bounding_box() const = 0;
};

class translate: public hittable {
    public:
        translate(shared_ptr<hittable> object, const vec3& offset): object(object), offset(offset) {
            bbox = object->bounding_box() + offset;
        }

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            // Move ray by offset backwards
            ray offset_r(r.origin() - offset, r.direction(), r.time());
            // Check for intersectio on offset ray
            if(!object->hit(offset_r, ray_t, rec)) return false;
            // Mobe intersection forward by offset
            rec.p += offset;
            return true;
        }

        aabb bounding_box() const override {return bbox; }
    private:
        shared_ptr<hittable> object;
        vec3 offset;
        aabb bbox;

};

class rotate_y: public hittable {
    public:
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            auto origin = point3(cos_theta * r.origin().x() - sin_theta * r.origin().z(),
                                r.origin().y(),
                                sin_theta * r.origin().x() + cos_theta * r.origin().z());
            
            auto direction = vec3(cos_theta * r.direction().x() - sin_theta * r.direction().z(),
                                r.direction().y(),
                                sin_theta * r.direction().x() + cos_theta * r.direction().z());

            ray rotated_r(origin, direction, r.time());

            if(!object->hit(rotated_r, ray_t, rec)) return false;

            rec.p = point3(cos_theta * rec.p.x() + sin_theta * rec.p.z(),
                            rec.p.y(),
                            -sin_theta * rec.p.x() + cos_theta * rec.p.z());

            rec.normal = vec3(cos_theta * rec.normal.x() + sin_theta * rec.normal.z(),
                                rec.normal.y(),
                                -sin_theta * rec.normal.x() + cos_theta * rec.normal.z());

            return true;
        }
}
#endif // HITTABLE_H