#include "ss_algorithm.h"

using namespace std;
using namespace std::placeholders;

/* Algorithm A2.1 Find span (i) p.68
 *
 */
int SS::Nurbs::find_span(
    int index_of_last_internal_knot, 
    int degree, 
    float parameter_u, 
    vector<float>& knot_vector)
{
    if (parameter_u == knot_vector[degree])
        return degree;
    if (parameter_u == knot_vector[index_of_last_internal_knot + 1])
        return index_of_last_internal_knot;

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
void SS::Nurbs::basis_functions(
    int index_basis, 
    float parameter_u,
    int degree, 
    vector<float> &knot_vector,
    vector<float> &basis)
{
    basis[0] = 1.0f;
    vector<float> left_numerator(degree);
    vector<float> right_numerator(degree);

    for(int j = 0; j < degree; j++)
    {
        left_numerator[j] = parameter_u - knot_vector[index_basis - j];
        right_numerator[j] = knot_vector[index_basis + j + 1] - parameter_u;

        float first_term = 0.0f;

        for(int r = 0; r < j + 1; r++)
        {
            float last_term_denominator = right_numerator[r] + left_numerator[j - r];
            float temp = basis[r] / last_term_denominator;
            basis[r] = first_term + right_numerator[r] * temp;
            first_term = left_numerator[j - r] * temp;
        }
        
        basis[j + 1] = first_term;
    }
}

/* Algorithm A3.1 Compute curve point p.82
 *
 */
SS::Vertex2f SS::Nurbs::curve_point(
    int index_of_last_internal_knot,
    int degree,
    vector<float> &knot_vector,
    vector<float> &basis,
    vector<SS::Vertex2f> &control_points,
    float parameter_u)
{
    int span = find_span(index_of_last_internal_knot, degree, parameter_u, knot_vector);
    basis_functions(span, parameter_u, degree, knot_vector, basis);
    SS::Vertex2f curve_point = { 0.0f, 0.0f };
    
    for(int i = 0; i <= degree; i++)
    {
        curve_point.x += basis[i] * control_points[span - degree + i].x;
        curve_point.y += basis[i] * control_points[span - degree + i].y;
    }
    
    return curve_point;
}

/* Centripetal to get parameter values (U) (Material 7.1 p.19)
 * (Equally spaced)/(chord length)/(centripetal)
 */
SS::Vertex2f SS::Nurbs::get_delta(
    SS::Vertex2f &vertex_initial, 
    SS::Vertex2f &vertex_final)
{
    float delta_x = vertex_final.x - vertex_initial.x;
    float delta_y = vertex_final.y - vertex_initial.y;
    SS::Vertex2f delta = { delta_x, delta_y };
    return delta;
}
SS::Vertex2f SS::Para::get_delta(
    SS::Vertex2f &vertex_initial, 
    SS::Vertex2f &vertex_final)
{
    float delta_x = vertex_final.x - vertex_initial.x;
    float delta_y = vertex_final.y - vertex_initial.y;
    SS::Vertex2f delta = { delta_x, delta_y };
    return delta;
}
float SS::Para::equally_spaced(
    SS::Vertex2f &vertex_initial, 
    SS::Vertex2f &vertex_final, 
    float total_chord_length, 
    int num_of_parameters)
{
    float result = 1.0f / (num_of_parameters - 1);
    return result;
}
float SS::Para::chord_length(
    SS::Vertex2f &vertex_initial, 
    SS::Vertex2f &vertex_final, 
    float total_chord_length, 
    int num_of_parameters)
{
    float result = 0;
    SS::Vertex2f delta = get_delta(vertex_initial, vertex_final);
    float norm = sqrtf(powf(delta.x, 2) + powf(delta.y, 2));
    result = norm / total_chord_length;

    return result;
}
float SS::Para::centripetal(
    SS::Vertex2f &vertex_initial, 
    SS::Vertex2f &vertex_final, 
    float total_chord_length, 
    int num_of_parameters)
{
    float result = 0;
    SS::Vertex2f delta = get_delta(vertex_initial, vertex_final);
    float norm = sqrtf(powf(delta.x, 2) + powf(delta.y, 2));
    result = sqrtf(norm) / total_chord_length;

    return result;
}
vector<float> SS::Nurbs::parameter_values(
    vector<SS::Vertex2f> &control_points, 
    Parameter_Function parameter_function)
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
        results[i] = results[i - 1] + parameter_function(control_points[i - 1], control_points[i],
            total_chord_length, size);
    }

    results[size - 1] = 1.0f;

    return results;
}

/* Knot Vector (Material 7.1 p.22)
 * (Equal spaing)/(Averaging)
 */
float SS::Knot::equal_spacing(
    vector<float> &parameters_U, 
    int degree, 
    int index_k)
{
    float knot_value = (float)(index_k-degree)/(parameters_U.size()- degree);
    return knot_value;
}
float SS::Knot::averageing(
    vector<float> &parameters_U, 
    int degree, 
    int index_k)
{
    float knot_value = 0;
    for(int i = index_k; i <= index_k + degree - 1; i++)
    {
        knot_value += parameters_U[i];
    }
    knot_value /= degree;

    return knot_value;
}
vector<float> SS::Nurbs::get_knot_vector(
    vector<float> &parameters_U, 
    int degree, 
    Knot_Vector_Function knot_function)
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

/* Calculate curve points
    *
    */
int SS::Nurbs::get_curve(Parameter_Function para_func, Knot_Vector_Function knot_func)
{
    curve.clear();
    curve.shrink_to_fit();

    vector<float> parameters = parameter_values(control_points, para_func);
    vector<float> knots = get_knot_vector(parameters, degree, knot_func);
    
    int index_of_last_internal_knot = knots.size() - 1 - degree - 1;
    vector<float> basis(degree + 1);

    for(float parameter_u = 0.0f; parameter_u < 1.0f; parameter_u += 0.001f)
    {
        Vertex2f point_on_curve = curve_point(index_of_last_internal_knot, degree, knots, 
            basis, control_points, parameter_u);
        curve.push_back(point_on_curve);
    }
        Vertex2f point_on_curve = curve_point(index_of_last_internal_knot, degree, knots, 
            basis, control_points, 1.0f);
        curve.push_back(point_on_curve);

    return 0;
}
