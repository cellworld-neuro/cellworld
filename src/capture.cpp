#include <cell_world/capture.h>

namespace cell_world {

    Capture::Capture(const Capture_parameters &parameters, const World &world):
            dist_max (parameters.distance * world.cell_transformation.size),
            theta_diff_max(to_radians(parameters.angle / 2)),
            visibility(world.create_cell_group() ,world.cell_shape, world.cell_transformation),
            cool_down(parameters.cool_down){
    }

    bool Capture::is_captured(const Location &predator_location, double predator_theta, const Location &prey_location) {
        if (!cool_down.time_out()) return false;
        if (predator_location.dist(prey_location) > dist_max) return false;
        if (angle_difference(predator_location.atan(prey_location),predator_theta) > theta_diff_max) return false;
        if (visibility.is_visible(predator_location, prey_location)) {
            cool_down.reset();
            return true;
        } else {
            return false;
        }
    }

    Peeking::Peeking(const Peeking_parameters &parameters, const World &world):
    peeking_visibility(world.create_cell_group() ,world.cell_shape, Transformation((1.0 + parameters.margin) * world.cell_transformation.size,world.cell_transformation.rotation)),
    peeking_timer(parameters.time_limit) {

    }

    void Peeking::not_visible() {
        peeking_timer.reset();
    }

    bool Peeking::is_seen(const Location &predator_location, const Location &prey_location) {
        if (peeking_visibility.is_visible(predator_location, prey_location)){
            return true;
        }
        if (peeking_timer.time_out()) {
            return true;
        }
        return false;
    }
}