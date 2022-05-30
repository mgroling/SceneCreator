import numpy as np
import time


class Plane:
    def __init__(self, center, vec1, vec2) -> None:
        self.center = center
        self.vec1 = vec1
        self.vec2 = vec2
        self.normal = np.cross(vec1, vec2)

    def intersection_usinginverse(self, ray_origin, ray_direction):
        a = np.array([self.vec1, self.vec2, -ray_direction]).T
        b = ray_origin - self.center
        gamma_phi_t = np.matmul(np.linalg.pinv(a), b)

        return gamma_phi_t

    def intersection(self, ray_origin, ray_direction):
        t = -(np.dot(self.normal, ray_origin - self.center)) / np.dot(
            self.normal, ray_direction
        )

        a = np.array([self.vec1, self.vec2, -self.normal]).T
        b = (ray_origin + t * ray_direction) - self.center
        gamma_phi_t = np.matmul(np.linalg.inv(a), b)
        gamma_phi_t[2] = t

        return gamma_phi_t


if __name__ == "__main__":
    u = np.array([0, 0, 0])
    v = np.array([0, 50, 0])
    w = np.array([0, 0, 50])

    # v, w = v / np.linalg.norm(v), w / np.linalg.norm(w)

    ray_origin = np.array([800, 0, 0])
    ray_direction = np.array([-400, 199, 200])
    ray_direction = ray_direction / np.linalg.norm(ray_direction)

    p = Plane(u, v, w)

    times = 100000

    cur_time = time.time()
    # for i in range(times):
    #     p.intersection_usinginverse(np.random.sample((3,)), np.random.sample((3,)))
    print("Using inverse:", p.intersection_usinginverse(ray_origin, ray_direction))
    print(time.time() - cur_time)
    cur_time = time.time()
    # for i in range(times):
    #     p.intersection(np.random.sample((3,)), np.random.sample((3,)))
    print("Other method:", p.intersection(ray_origin, ray_direction))
    print(time.time() - cur_time)
