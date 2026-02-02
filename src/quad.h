#ifndef QUAD_H
#define QUAD_H
#include "hittable.h"
#include "hittable_list.h"

class quad : public hittable {
    public:
        quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat)
        : Q(Q), u(u), v(v), mat(mat) {
            auto n = cross(u, v);
            normal = unit_vector(n);
            D = dot(normal, Q);
            w = n / dot(n, n); // Precompute w vector
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
            vec3 planar_hitpt_vector = intersection - Q;
            auto alpha = dot(w, cross(planar_hitpt_vector, v));
            auto beta = dot(w, cross(u, planar_hitpt_vector));

            if(!is_interior(alpha, beta, rec)) {
                return false; // Intersection is outside the quad
            }

            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;

            rec.set_face_normal(r, normal);

            return true;
        }

        virtual bool is_interior(double alpha, double beta, hit_record& rec) const {
            interval unit_interval = interval(0.0, 1.0);

            if(!unit_interval.contains(alpha) || !unit_interval.contains(beta)) {
                return false;
            }

            rec.u = alpha;
            rec.v = beta;
            return true;
        }
    private:
        point3 Q; // One corner of the quad
        vec3 u;   // Edge vector from Q
        vec3 v;   // Edge vector from Q
        vec3 w;   // Precomputed vector for barycentric coordinates
        shared_ptr<material> mat;   // Material of the quad
        aabb bbox; // Axis-aligned bounding box
        vec3 normal; // Normal vector of the quad
        double D; // Plane constant
};

inline shared_ptr<hittable_list> box(const point3& a, const point3& b, shared_ptr<material> mat){
            // Return a 3D box/cube withh opposite vertices a and b
            auto sides = make_shared<hittable_list>();

            // Construct a and b with minimum nad maximum vertices
            auto min = point3(fmin(a.x(),b.x()), fmin(a.y(),b.y()), fmin(a.z(),b.z()));
            auto max = point3(fmax(a.x(),b.x()), fmax(a.y(),b.y()), fmax(a.z(),b.z()));

            auto dx = vec3(max.x() - min.x(), 0, 0);
            auto dy = vec3(0, max.y() - min.y(), 0);
            auto dz = vec3(0, 0, max.z() - min.z());

            sides->add(make_shared<quad>(point3(min.x(),min.y(),max.z()),dx,dy,mat)); //front
            sides->add(make_shared<quad>(point3(max.x(),min.y(),max.z()),-dz,dy,mat)); //right
            sides->add(make_shared<quad>(point3(max.x(),min.y(),min.z()),-dx,dy,mat)); //back
            sides->add(make_shared<quad>(point3(min.x(),min.y(),min.z()),dz,dy,mat)); //left
            sides->add(make_shared<quad>(point3(min.x(),max.y(),max.z()),dx,-dz,mat)); //top
            sides->add(make_shared<quad>(point3(min.x(),min.y(),min.z()),dx,dz,mat)); //bottom

            return sides;
        }

#endif