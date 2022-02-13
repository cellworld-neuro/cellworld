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
        Transformation (double size, double rotation);
        double theta() const;
        double size{};
        double rotation{};
        Json_object_members({
                                Add_member(size);
                                Add_member(rotation);
                            });
    };

    struct Location : json_cpp::Json_object {
        Location();
        Location(double x, double y);
        double x{}, y{};
        bool operator==(const Location &) const;
        bool operator!=(const Location &) const;
        Location operator+=(const Location &);
        Location operator+(const Location &) const;
        Location operator-(const Location &) const;
        Location operator-() const;
        Location operator*(double) const;
        Location operator*(const Location &) const;
        Location operator/(double) const;
        double mod() const;
        double atan() const;
        Transformation transformation() const;
        Transformation transformation(const Location &) const;
        double atan(const Location &) const;
        double dist(const Location &) const;
        double dist() const;
        double dist(const Location &, const Location &) const;
        double manhattan(const Location &) const;
        Location move(double theta, double dist) const;
        void transform (const Transformation &);
        Json_object_members({
                         Add_member(x);
                         Add_member(y);
        })
    };

    struct Location_list : json_cpp::Json_vector<Location> {
        json_cpp::Json_vector<double> get_x();
        json_cpp::Json_vector<double> get_y();
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

    std::vector<int> histogram(std::vector<int>);
    double entropy(const std::vector<int>&);
    std::vector<unsigned int> new_index(unsigned int);
    std::vector<unsigned int> new_index(std::vector<double>, bool);
    double max(const std::vector<double> &);
    unsigned int sum(const std::vector<unsigned int>& );

    double angle_difference(double, double);
    int direction(double, double);
    bool angle_between(double value, double lim1, double lim2, bool inclusive = false);
    double to_radians(double);
    double to_degrees(double);
    double normalize(double);
    double normalize_degrees(double);
    bool segments_intersect(const Location &segment1_point1, const Location &segment1_point2, const Location &segment2_point1, const Location &segment2_point2);

    bool file_exists (const std::string &);
    bool file_exists (const std::vector<std::string> &);
    bool file_exists (const std::string &, const std::vector<std::string> &);
    bool folder_exists(const std::string &path);
    bool create_folder(const std::string &path);
}