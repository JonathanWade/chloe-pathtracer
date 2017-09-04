#pragma once

#include <vector>
#include <memory>

using std::vector;

class hitableList : public hitable {
    public:
        hitableList() {}
        virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const override;
        vector<std::unique_ptr<hitable> > list;
};


bool hitableList::hit(const ray& r, float tMin, float tMax, hitRecord& rec) const {
    hitRecord tempRecord;
    bool hitAnything = false;
    double closest = tMax;
    for (auto& hitable : list) {
        if(hitable->hit(r, tMin, closest, tempRecord)) {
            hitAnything = true;
            closest = tempRecord.t;
            rec = tempRecord;
        }
    }

    return hitAnything;
}
