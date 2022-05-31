#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
	sphere() noexcept {}
	sphere(point3 cen, double r, shared_ptr<material> m) noexcept
	 : center(cen), radius(r), mat_ptr(m) {};

	bool hit(
		const ray& r, double t_min, double t_max, hit_record& rec) const override;

public: 
	point3 center;
	double radius{};
	shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	const vec3 oc = r.origin() - center;
	const auto a = r.direction().length_squared();
	const auto half_b = dot(oc, r.direction());
	const auto c = oc.length_squared() - radius*radius;

	const auto discriminant = half_b*half_b - a*c;
	if (discriminant < 0) return false;
	const auto sqrtd = sqrt(discriminant);
	t_min *= a;
	t_max *= a;

	//Find the nearest root that lies in the acceptable range
	auto root_without_a = (-half_b - sqrtd);
	if (root_without_a < t_min || t_max < root_without_a) {
		root_without_a = (-half_b + sqrtd);
		if (root_without_a < t_min || t_max < root_without_a)
			return false;
	}

	const auto root = root_without_a / a;
	rec.t = root;
	rec.p = r.at(rec.t);
	const vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mat_ptr;

	return true;
}

#endif
