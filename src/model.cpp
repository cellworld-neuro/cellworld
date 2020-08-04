#include <cell_world/model.h>
#include <iostream>

using namespace std;

namespace cell_world {

    bool Model::update()
    {
        if (_state.public_state.status != Model_public_state::Status::Running) throw logic_error("Model::update - "
                                                                                                "model is not "
                                                                                   "running.");
        Agent_base &agent = _agents[_state.public_state.current_turn];
        auto &agent_state = _state.public_state.agents_state[_state.public_state.current_turn];
        agent_state.iteration++;
        auto move = agent.get_move(_state.public_state);
        auto &agent_cell = agent_state.cell;
        int destination_index = map.find(agent_cell.coordinates + move);
        _state.public_state.current_turn++;
        if (destination_index != Not_found) agent_cell = map.cells[destination_index];
        if (_state.public_state.current_turn == _agents.size()) _state.public_state.current_turn = 0;
        auto agent_status = agent.update_state(_state.public_state);
        return _state.public_state.agents_state[_state.public_state.current_turn].iteration < _state.public_state.iterations &&
               agent_status == Agent_status_code::Running;
    }

    Model::Model(const Cell_group &cells, unsigned int iterations) :
            map(cells.free_cells()),
            state(_state){
        _state.public_state.status = Model_public_state::Status::Idle;
        _state.public_state.iterations = iterations;
    }

    Model::Model(const Cell_group &cg) : Model(cg, 50) {}

    void Model::end_episode() {
        if (_state.public_state.status != Model_public_state::Status::Running) throw logic_error("Model::end_episode - model is not running.");
        _state.public_state.status = Model_public_state::Status::Stopped;
        for (Agent_base &agent:_agents) agent.end_episode(_state.public_state);
    }

    void Model::start_episode() {
        if (_state.public_state.status == Model_public_state::Status::Running) throw logic_error("Model::start_episode - model is already running.");
        if (_agents.empty()) throw logic_error("Model::start_episode - can't start an episode with no agents.");
        _state.public_state.current_turn = 0;
        unsigned int agent_index = 0;
        for (Agent_base &agent:_agents) {
            Cell c = agent.start_episode();
            if (map.cells.find(c)==Not_found) throw logic_error("Model::start_episode - agent start cell not available.");
            auto &agent_state = _state.public_state.agents_state[agent_index];
            agent_state.iteration = 0;
            agent_state.cell = c;
            agent_index++;
        }
        _state.public_state.status = Model_public_state::Status::Running;
        agent_index = 0;
        for (Agent_base &agent:_agents) {
            agent.set_public_state(_state.public_state.agents_state.data() + agent_index);
            agent.set_internal_state(_state._agents_internal_state[agent_index],true);
            agent.update_state(_state.public_state);
            agent_index++;
        }
    }

    void Model::add_agent(Agent_base &agent) {
        unsigned int agent_index = _agents.size();
        _agents.emplace_back(agent);
        _state.public_state.agents_state.emplace_back(agent_index);
        _state._agents_internal_state.emplace_back(agent.get_internal_state_size());
    }

    void Model::run() {
        start_episode();
        while (update());
        end_episode();
    }

    void Model::set_public_state(const Model_public_state &mps) {
        _state.public_state = mps;
        //reset all internal states
        unsigned int agent_index = 0;
        for (Agent_base &agent:_agents) {
            agent.set_internal_state(_state._agents_internal_state[agent_index], true);
            agent_index++;
        }
    }

    void Model::set_state(Model_state &ms) {
        _state = ms;
        unsigned int agent_index = 0;
        for (Agent_base &agent:_agents){
            agent.set_internal_state(_state._agents_internal_state[agent_index],false);
            agent_index++;
        }
    }

    const Model_state &Model::get_state() const {
        return _state;
    }
}