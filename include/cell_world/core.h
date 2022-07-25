#pragma once
#include <iostream>
#include <cinttypes>
#include <string>
#include <vector>
#include <json_cpp.h>

#ifdef DEBUG
#else
#endif

#define SIMILAR(A,B,M) ((A-M < B) && (A+M > B))

#define WAIT_UNTIL(EXPRESSION) while(!(EXPRESSION));

namespace cell_world{
    const int Not_found = -1;

    struct Coordinates : json_cpp::Json_object{
        Coordinates ();
        Coordinates (int x, int y);
        int x{},y{};
        bool is_origin() const;
        int rotation() const;
        bool operator ==(const Coordinates &) const;
        bool operator !=(const Coordinates &) const;
        Coordinates operator +=(const Coordinates &);
        Coordinates operator +(const Coordinates &) const;
        Coordinates operator -(const Coordinates &) const;
        Coordinates operator -() const;
        unsigned int manhattan(const Coordinates &) const;
        Json_object_members({
            Add_member(x);
            Add_member(y);
        })
    };

    using Coordinates_list = json_cpp::Json_vector<Coordinates>;

    using Move = Coordinates;

    using Move_list = json_cpp::Json_vector<Move>;

    struct Transformation : json_cpp::Json_object{
        Transformation ();
        Transformation (float size, float rotation);
        float theta() const;
        float size{};
        float rotation{};
        Json_object_members({
                                Add_member(size);
                                Add_member(rotation);
                            });
    };

    struct Location : json_cpp::Json_object {
        Location();
        Location(float x, float y);
        float x{}, y{};
        bool operator==(const Location &) const;
        bool operator!=(const Location &) const;
        Location operator+=(const Location &);
        Location operator+(const Location &) const;
        Location operator-(const Location &) const;
        Location operator-() const;
        Location operator*(float) const;
        Location operator*(const Location &) const;
        Location operator/(float) const;
        float mod() const;
        float atan() const;
        Transformation transformation() const;
        Transformation transformation(const Location &) const;
        float atan(const Location &) const;
        float dist(const Location &) const;
        float dist() const;
        float dist(const Location &, const Location &) const;
        float manhattan(const Location &) const;
        Location move(float theta, float dist) const;
        void transform (const Transformation &);
        Json_object_members({
                         Add_member(x);
                         Add_member(y);
        })
    };

    struct Location_list : json_cpp::Json_vector<Location> {
        json_cpp::Json_vector<float> get_x();
        json_cpp::Json_vector<float> get_y();
    };

    struct Cell : json_cpp::Json_object{
        Cell();
        Cell(const Cell&) = default;
        Cell(const Coordinates &);
        Cell(Coordinates, Location, bool);
        unsigned int id;
        Coordinates coordinates;
        Location location;
        bool occluded;
        static Cell &ghost_cell();
        bool operator != (const Cell&) const;
        bool operator == (const Cell&) const;
        Cell &operator = (const Cell&);
        operator bool () const{
            return this->id != ghost_cell().id;
        };
        Json_object_members({
            Add_member(id);
            Add_member(coordinates);
            Add_member(location);
            Add_member(occluded);
            Ignore_member("cell_type");
        })
    };

    using Cell_list = json_cpp::Json_vector<Cell>;

    std::vector<unsigned int> histogram(std::vector<unsigned int>);
    std::vector<unsigned int> new_index(unsigned int);
    std::vector<unsigned int> new_index(std::vector<float>, bool);

    template<typename T>
    T max(const std::vector<T> &values){
        T m = values[0];
        for (auto &v:values) m = v>m?v:m;
        return m;
    }

    template<typename T>
    T sum(const std::vector<T> &values){
        T s{};
        for (auto &v:values) s+=v;
        return s;
    }

    template<typename T>
    T min(const std::vector<T> &values){
        T m = values[0];
        for (auto &v:values) m = v<m?v:m;
        return m;
    }

    std::string get_variable(const std::string &, const std::string &);

    float angle_difference(float, float);
    int direction(float, float);
    bool angle_between(float value, float lim1, float lim2, bool inclusive = false);
    float to_radians(float);
    float to_degrees(float);
    float normalize(float);
    float normalize_degrees(float);
    bool segments_intersect(const Location &segment1_point1, const Location &segment1_point2, const Location &segment2_point1, const Location &segment2_point2);

    bool file_exists (const std::string &);
    bool file_exists (const std::vector<std::string> &);
    bool file_exists (const std::string &, const std::vector<std::string> &);
    bool folder_exists(const std::string &path);
    bool create_folder(const std::string &path);

    float entropy(std::vector<float> probabilities, float base = M_E);

    template<typename T>
    float weights_entropy(std::vector<T> weights, float base = M_E){
        if (weights.empty()) return 0;
        auto total = sum(weights);
        if (total == 0) return 0;
        std::vector<float> probs(weights.size());
        for (int i = 0 ; i < weights.size(); i++) probs[i] = (float)weights[i] / total ;
        return entropy(probs, base);
    }

    template<typename T>
    float labels_entropy(std::vector<T> labels, float base = M_E){
        if (labels.empty()) return 0;
        std::unordered_map<T, unsigned int> labels_weight;
        for (auto label:labels) {
            if (labels_weight.find(label) == labels_weight.end()) {
                labels_weight[label] = 1;
            } else {
                labels_weight[label]++;
            }
        }
        std::vector<unsigned int> weights;
        for (auto itr = labels_weight.begin(); itr != labels_weight.end(); itr++)  weights.push_back(itr->second);
        return weights_entropy(weights, base);
    }


}