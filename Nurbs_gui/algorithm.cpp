#include <cmath>
#include <vector>
#include <functional>
#include <iostream>

#include "vertex.h"

using namespace std;
using namespace std::placeholders;
using Parameter_Function = std::function<float(SS::Vertex2f, SS::Vertex2f, float, int)>;
using Knot_Vector_Function = std::function<float(vector<float>, int, int)>;


/* Algorithm A2.1 Find span (i) p.68
 *
 */
int find_span(
    int index_of_last_internal_knot, 
    int degree, 
    float parameter_u, 
    const vector<float>& knot_vector)
{
    if (parameter_u == knot_vector[degree])
        return degree;
    if (parameter_u == knot_vector[index_of_last_internal_knot + 1])
        return index_of_last_internal_knot + 1;

    int low = degree;
    int high = index_of_last_internal_knot + 1;
    int mid = (low + high) / 2;

    while(parameter_u < knot_vector[mid] || parameter_u >= knot_vector[mid + 1]) {
        if (parameter_u < knot_vector[mid]) high = mid;
        else    low = mid;
    
        mid = (low + high) / 2;
    }

    return mid;
}

/* Algorithm A2.2 Compute basis function (N_i_p) p.70
 *
 */
void basis_functions(
    int index_basis, 
    float parameter_u,
    int degree, 
    const vector<float> &knot_vector,
    vector<float> &basis)
{
    basis[0] = 1.0f;
    vector<float> left(degree + 1);
    vector<float> right(degree  + 1);

    for(int j = 1; j <= degree; j++)
    {
        left[j] = parameter_u - knot_vector[index_basis + 1 - j];
        right[j] = knot_vector[index_basis + j] - parameter_u;

        float saved = 0.0f;

        for(int r = 0; r < j; r++)
        {
            float temp = basis[r] / (right[r + 1] + left[j - r]);
            basis[r] = saved + right[r + 1] * temp;
            saved = left[j - r] * temp;
        }
        
        basis[j] = saved;
    }
    
}

/* Algorithm A3.1 Compute curve point p.82
 *
 */
float curve_point(
    int index_of_last_internal_knot,
    int degree,
    const vector<float> &knot_vector,
    vector<float> &basis,
    const vector<float> &control_points,
    float parameter_u)
{
    int span = find_span(index_of_last_internal_knot, degree, parameter_u, knot_vector);
    basis_functions(span, parameter_u, degree, knot_vector, basis);
    float curve_point = 0;
    
    for(int i = 0; i < degree; i++)
        curve_point += basis[i] * control_points[span - degree + i];
    
    return curve_point;
}

/* Centripetal to get parameter values (U) (Material 7.1 p.19)
 * (Equally spaced)/(chord length)/(centripetal)
 */
SS::Vertex2f get_delta(SS::Vertex2f vertex_initial, SS::Vertex2f vertex_final)
{
    float delta_x = vertex_final.x - vertex_initial.x;
    float delta_y = vertex_final.y - vertex_initial.y;
    SS::Vertex2f delta = { delta_x, delta_y };
    return delta;
}
static float equally_spaced(SS::Vertex2f vertex_initial, SS::Vertex2f vertex_final, float total_chord_length, int num_of_parameters)
{
    float result = 1.0f / (num_of_parameters - 1);
    return result;
}
static float chord_length(SS::Vertex2f vertex_initial, SS::Vertex2f vertex_final, float total_chord_length, int num_of_parameters)
{
    float result = 0;
    SS::Vertex2f delta = get_delta(vertex_initial, vertex_final);
    float norm = sqrtf(powf(delta.x, 2) + powf(delta.y, 2));
    result = norm / total_chord_length;

    return result;
}
static float centripetal(SS::Vertex2f vertex_initial, SS::Vertex2f vertex_final, float total_chord_length, int num_of_parameters)
{
    float result = 0;
    SS::Vertex2f delta = get_delta(vertex_initial, vertex_final);
    float norm = sqrtf(powf(delta.x, 2) + powf(delta.y, 2));
    result = sqrtf(norm) / total_chord_length;

    return result;
}
vector<float> parameter_values(vector<SS::Vertex2f> control_points, Parameter_Function parameter_function)
{
    int size = control_points.size();
    vector<float> results(size);
    parameter_function = bind(parameter_function, _1, _2, _3, _4);

    float total_chord_length = 0;
    for(int i = 1; i < size; i++)
    {
        SS::Vertex2f delta = get_delta(control_points[i - 1], control_points[i]);
        total_chord_length += sqrtf(powf(delta.x, 2) + powf(delta.y, 2));
    }
    
    results[0] = 0.0f; // inital u_0 = 0;

    for(int i = 1; i < size - 1; i++)
    {
        results[i] = results[i - 1] + parameter_function(control_points[i - 1], control_points[i], total_chord_length, size);
    }

    results[size - 1] = 1.0f;

    return results;
}

/* Knot Vector (Material 7.1 p.22)
 * (Equal spaing)/(Averaging)
 */
static float equal_spacing(vector<float> parameters_U, int degree, int index_k)
{
    float knot_value = (float)(index_k-degree)/(parameters_U.size()- degree);
    return knot_value;
}
static float averageing(vector<float> parameters_U, int degree, int index_k)
{
    float knot_value = 0;
    for(int i = index_k; i <= index_k + degree - 1; i++)
    {
        knot_value += parameters_U[i];
    }
    knot_value /= degree;

    return knot_value;
}
vector<float> get_knot_vector(vector<float> parameters_U, int degree, Knot_Vector_Function knot_function)
{
    const int num_of_control_points = parameters_U.size();
    const int num_of_knots = num_of_control_points + degree + 1;
    vector<float> results(num_of_knots);
    knot_function = bind(knot_function, _1, _2, _3);

    for(int i = 0; i <= degree; i++)
    {
        results[i] = 0.0f;
        results[num_of_knots - 1 - i] = 1.0f;
    }
    
    for(int k = 1+degree; k <= num_of_control_points - 1; k++)
    {
        results[k] = knot_function(parameters_U, degree, k);
    }
    
    return results;
}

int main(int argc, char const *argv[])
{
    vector<float> knot_vector = { 0, 0, 0, 1, 2, 3, 4, 4, 5, 1, 1 };
    vector<float> basis(knot_vector.size()-1);
    vector<float> control_points = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int degree = 2;
    int index_of_last_internal_knot = knot_vector.size() - 1 - degree - 1;
    float parameter_u = 1.0f;

    float point_on_curve = curve_point(index_of_last_internal_knot, degree, knot_vector, basis, control_points, parameter_u);

    cout << point_on_curve << endl;

    vector<SS::Vertex2f> cp = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {2.0f, 0.0f},
        {4.0f, 0.0f},
        {5.0f, 0.0f}
    };

    vector<float> u = parameter_values(cp, equally_spaced);
    vector<float> v = parameter_values(cp, chord_length);
    vector<float> w = parameter_values(cp, centripetal);

    cout << u.size() << endl;
    cout << v.size() << endl;
    cout << w.size() << endl;

    vector<float> knots = get_knot_vector(u, degree, equal_spacing);
    vector<float> knots2 = get_knot_vector(u, degree, averageing);
    vector<float> knots3 = get_knot_vector(u, degree, averageing);

    return 0;
}
