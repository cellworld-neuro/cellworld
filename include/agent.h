#pragma once
#include <core.h>
#include <chance.h>
#include <connection.h>
#include <atomic>

namespace cell_world{

    using Move  = Coordinates;

    enum Agent_status{
        Started,
        Action_pending,
        Action_ready,
        Finished
    };

     struct Stochastic_move {
         Stochastic_move(const Connection_pattern&, std::vector<uint32_t>);
         Move get_move();
         Connection_pattern destinations;
         std::vector<uint32_t> chances;
    };

    struct Agent_type{
        std::string name = "";
        uint32_t version = 0;
    };

    struct Agent_data{
        Agent_type type;
        Color color;
        Icon icon = No_icon;
        Cell cell;
    };

    struct State{
        uint32_t iteration;
        uint32_t iterations;
        std::vector<Agent_data> agents_data;
        int32_t find(const std::string&) const;
    };

    struct Agent_message{
        Agent_type from;
        Agent_type to;
        std::string body;
    };

    struct Agent{
        explicit Agent(Agent_type);
        virtual const Cell &start_episode(uint32_t) = 0;
        virtual void update_state(const State &) = 0;
        virtual Move get_move() = 0;
        virtual void end_episode(const State &) = 0;
        virtual void receive_message(const Agent_message&);
        void send_message(const Agent_type&, const std::string &);
        void send_message(const std::string &);
        const Cell &cell() const;
        void set_status(Agent_status);
        void set_color(Color);

    protected:
        Agent_data data;
        std::atomic<Agent_status> status;
    private:
        uint32_t _message_group;
        friend class Model;
        friend struct Agent_broadcaster;
    };

    struct Agent_broadcaster{
        static void add(Agent *agent, uint32_t group);
        static void send(const Agent_message &, uint32_t group);
        static uint32_t new_message_group();
    };

}