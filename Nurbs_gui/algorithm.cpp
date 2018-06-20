#include <vector>
#include <functional>
#include <iostream>

using namespace std;

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

int main(int argc, char const *argv[])
{
    const vector<float> knot_vector = { 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5 };
    vector<float> basis(knot_vector.size()-1);
    const vector<float> control_points = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int degree = 2;
    int index_of_last_internal_knot = knot_vector.size() - 1 - degree - 1;
    float parameter_u = 1.0f;

    float point_on_curve = curve_point(index_of_last_internal_knot, degree, knot_vector, basis, control_points, parameter_u);

    cout << point_on_curve << endl;

    return 0;
}
