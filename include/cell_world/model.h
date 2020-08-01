#pragma once
#include <cell_world/core.h>
#include <cell_world/world.h>
#include <cell_world/agent.h>
#include <cell_world/visibility.h>
#include <cell_world/map.h>

namespace cell_world{
    struct Model
    {
        Model(Cell_group &, unsigned int);
        explicit Model(Cell_group &);
        void add_agent(Agent_base &);
        bool update();
        void start_episode();
        void end_episode();
        const Model_state &get_state() const;
        void set_state(Model_state &);
        void set_public_state(Model_public_state &);
        void run();
        Map map;
    protected:
        Model_state _state;
        std::vector<std::reference_wrapper<Agent_base>> _agents;
    };
}