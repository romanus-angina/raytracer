#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
#include <vector>
#include "hittable.h"
#include<memory>

using std::make_shared;
using std::shared_ptr;

class hittable_list: public hittable {
    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }
        std::vector<shared_ptr<hittable>> objects;

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override{
            hit_record temp_rec;
            bool hit_anything = false;
            double closest_so_far = t_max;

            for (const auto& object : objects) {
                if (object->hit(r, t_min, closest_so_far, temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec; // Update the record with the closest hit
                }
            }
            return hit_anything; // Return true if any object was hit
        }
};

#endif // HITTABLE_LIST_H