#pragma once
#include <cell_world/core.h>
#include <cell_world/chance.h>
#include <cell_world/connection.h>
#include <atomic>

namespace cell_world {
    struct Agent_public_state : json_cpp::Json_object{
        Agent_public_state();
        Agent_public_state(unsigned int);
        unsigned int agent_index{};
        unsigned int iteration{};
        Cell cell;
        Json_object_members({
            Add_member(agent_index);
            Add_member(iteration);
            Add_member(cell);
        })
    };

    using Agent_state_list = json_cpp::Json_vector<Agent_public_state>;

    struct Agent_internal_state {
        explicit Agent_internal_state(size_t);
        void *get_address();
    private:
        std::vector<char> _mem_blob;
    };

    using Agent_internal_state_list = std::vector<Agent_internal_state>;

    struct Model_public_state : json_cpp::Json_object{
        enum Status{
            Idle,
            Running,
            Stopped
        };
        Status status;
        unsigned int current_turn;
        unsigned int iterations;
        Agent_state_list agents_state;
        Json_object_members({
            Add_member(status);
            Add_member(current_turn);
            Add_member(iterations);
            Add_member(agents_state);
        })
    };

    struct Model_state {
        Model_public_state public_state;

    private:
        Agent_internal_state_list _agents_internal_state;
        friend class Model;
    };

}