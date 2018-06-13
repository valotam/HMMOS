#include <vector>
#include <functional>
#include <iostream>

using namespace std;

void Print() {
    cout << endl;
}

template <typename T>
void Print(const T& t) {
    cout << t << endl;
}

template <typename First, typename... Rest>
void Print(const First& first, const Rest&... rest) {
    cout << first << ", ";
    print(rest...); // recursive call using pack expansion syntax
}

// auto Basis(const vector<float> &knot_vector, int degree) {
//     auto basis_function = [&](float parameter_u) ->function<float(float)>{
//         vector<int> N_zero;

//         for(int i = 0; i < knot_vector.size() - 1; i++)
//         {
//             if (knot_vector.at(i) == knot_vector.at(i)) {
//                 N_zero.push_back(0);
//             }
//             else {
//                 if (parameter_u >= knot_vector[i] && parameter_u < knot_vector[i+1])   N_zero.push_back(1);
//                 else    N_zero.push_back(0);                
//             }
//         }
        
//     };
    
// }


/* Algorithm A2.1 Find span (i) p.68
 *
 */
int Find_Span(
    int index_last_internal_knots, 
    int degree, 
    float parameter_u, 
    const vector<float>& knot_vector)
{
    if (parameter_u == knot_vector[degree])
        return degree;
    if (parameter_u == knot_vector[index_last_internal_knots + 1])
        return index_last_internal_knots + 1;

    int low = degree;
    int high = index_last_internal_knots + 1;
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
int Basis_Functions(
    int index_basis_functions, 
    float parameter_u,
    int degree, 
    const vector<float> &knot_vector,
    vector<float> &basis_functions)
{
    basis_functions[0] = 1.0f;
    vector<float> left(degree + 1);
    vector<float> right(degree  + 1);

    for(int j = 1; j <= degree; j++)
    {
        left[j] = parameter_u - knot_vector[index_basis_functions + 1 - j];
        right[j] = knot_vector[index_basis_functions + j] - parameter_u;

        float saved = 0.0f;

        for(int r = 0; r < j; r++)
        {
            float temp = basis_functions[r] / (right[r + 1] + left[j - r]);
            basis_functions[r] = saved + right[r + 1] * temp;
            saved = left[j - r] * temp;
        }
        
        basis_functions[j] = saved;
    }
    
}


int main(int argc, char const *argv[])
{
    const vector<float> knot_vector = { 0, 0, 0, 1, 2, 3, 4, 4, 5, 5, 5 };
    int degree = 2;
    int index_last_internal_knots = knot_vector.size() - 1 - degree - 1;
    float parameter_u = 1.0f;
    int i = Find_Span(index_last_internal_knots, degree, parameter_u, knot_vector);
    cout << i << endl;

    vector<float> basis_functions(knot_vector.size()-1);
    int temp = Basis_Functions(i, parameter_u, degree, knot_vector, basis_functions);
    cout << temp << endl;

    return 0;
}
