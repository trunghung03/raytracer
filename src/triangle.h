#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"

class triangle : public hittable {
public:
    triangle() noexcept {}
    triangle(point3 a, point3 b, point3 c, shared_ptr<material> m) noexcept
     : pointA(a), pointB(b), pointC(c), mat_ptr(m) {};

    bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;
public:
    point3 pointA;
    point3 pointB;
    point3 pointC;
    shared_ptr<material> mat_ptr;
};

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
//     Line equation: p = l0 + ld
//     If l . n = 0 (line para plane)
//         if ((p0 - l0) . n = 0)
//             line is in plane
//         else no intersection
//     Else if l . n != 0
//         1 intersection
//     Solve d = (p0 - l0) . n / l . n
//     calculate point of intersection: p = l0 + ld
//     see if poi is in circle
    const auto plane_norm = cross(pointA-pointB, pointB-pointC);
    const auto dot_pl_n = dot(pointA - r.origin(), plane_norm);
    const auto dot_ln = dot(r.direction(), plane_norm);

    if (dot_ln == 0) {
        if (dot_pl_n == 0)
            return true;
        else return false;
    } else {
        const auto d = dot_pl_n / dot_ln;
        if (d < t_min || t_max < d) {
			return false;
        }
        const auto intersection = r.at(d);

        const auto area = cross(pointA-pointB, pointA-pointC).length() / 2;
        const auto area1 = cross(intersection-pointA, intersection-pointB).length() / 2;
        const auto area2 = cross(intersection-pointB, intersection-pointC).length() / 2;
        const auto area3 = cross(intersection-pointC, intersection-pointA).length() / 2;

        if (area != (area1 + area2 + area3)) return false;

        rec.t = d;
        rec.p = intersection;
        rec.mat_ptr = mat_ptr;
        rec.front_face = true;
        return true;
    }
}

#endif
