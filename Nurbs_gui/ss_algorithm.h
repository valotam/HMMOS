#ifndef _SS_VETEX_H_
#define _SS_VETEX_H_
namespace SS
{
    struct Vertex2f
    {
        float x, y;
    };
}
#endif

#ifndef _SS_ALGORITHM_H_
#define _SS_ALGORITHM_H_

#include <cmath>
#include <vector>
#include <functional>
#include <iostream>
#include <iomanip>

namespace SS
{
    using namespace std;
    using Parameter_Function = std::function<float(Vertex2f&, Vertex2f&, float, int)>;
    using Knot_Vector_Function = std::function<float(vector<float>&, int, int)>;

    
    class Nurbs
    {
    private:
        Parameter_Function parameter_function;
        Knot_Vector_Function knot_vector_function;

    public:
        unsigned int degree;
        vector<Vertex2f> control_points;
        vector<float> weights;
        vector<Vertex2f> curve;
        vector<float> knots;
        vector<float> parameters;
        float parameter_u_limit;

    public:
        Nurbs() : degree(0), parameter_u_limit(1.0f) { }
        ~Nurbs() { }

        void clear_curve();

        /* Set parameter function and knot vector function
         *
         */
        void set_parameter_function(Parameter_Function para_func);
        void set_knot_vector_function(Knot_Vector_Function knot_func);

        /* Calculate curve points
         *
         */
        int get_curve();

    private:
        /* Algorithm A2.1 Find span (i) p.68
         *
         */
        int find_span(
            int index_of_last_internal_knot, 
            int degree, 
            float parameter_u, 
            vector<float>& knot_vector);
        /* Algorithm A2.2 Compute basis function (N_i_p) p.70
         *
         */
        void basis_functions(
            int index_basis, 
            float parameter_u,
            int degree, 
            vector<float> &knot_vector,
            vector<float> &basis);

        /* Algorithm A3.1 Compute curve point p.82
         * Algorithm A4.1 Rational (weight, homogeneous)
         */
        Vertex2f curve_point(
            int index_of_last_internal_knot,
            int degree,
            vector<float> &knot_vector,
            vector<float> &basis,
            vector<Vertex2f> &control_points,
            vector<float> &weights,
            float parameter_u);

        /* Centripetal to get parameter values (U) (Material 7.1 p.19)
         * (Equally spaced)/(chord length)/(centripetal)
         */
        Vertex2f get_delta(
            Vertex2f &vertex_initial, 
            Vertex2f &vertex_final);

        vector<float> parameter_values(vector<Vertex2f> &control_points);

        /* Knot Vector (Material 7.1 p.22)
         * (Equal spaing)/(Averaging)
         */
        vector<float> get_knot_vector(
            vector<float> &parameters_U, 
            int degree);

    };

    class Para
    {
    private:
        static Vertex2f get_delta(
            Vertex2f &vertex_initial, 
            Vertex2f &vertex_final);

    public:
        /* Centripetal to get parameter values (U) (Material 7.1 p.19)
         * (Equally spaced)/(chord length)/(centripetal)
         */
        static float equally_spaced(
            Vertex2f &vertex_initial, 
            Vertex2f &vertex_final, 
            float total_chord_length, 
            int num_of_parameters);

        static float chord_length(
            Vertex2f &vertex_initial, 
            Vertex2f &vertex_final, 
            float total_chord_length, 
            int num_of_parameters);

        static float centripetal(
            Vertex2f &vertex_initial, 
            Vertex2f &vertex_final, 
            float total_chord_length, 
            int num_of_parameters);

    };

    
    class Knot
    {
    public:
        /* Knot Vector (Material 7.1 p.22)
         * (Equal spaing)/(Averaging)
         */
        static float equal_spacing(
            vector<float> &parameters_U, 
            int degree, 
            int index_k);

        static float averageing(
            vector<float> &parameters_U, 
            int degree, 
            int index_k);

    };
}
#endif