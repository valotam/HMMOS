/* File:
 *     deCasteljau.cpp
 *
 * Purpose:
 *     Implement the algorithm in C and
 *     test the results using arbitrary control points.
 *
 * Compile:
 *    g++ -g -Wall -o deCasteljau deCasteljau.cpp
 * Run:
 *    ./deCasteljau
 *
 * Input:
 *
 * Output:
 *    Coordinates at some value of t by Bezier and
 *    de Casteljau's algorithm.
 *
 */
#include <iostream>
#include <cmath>

#define DEGREE      3
#define PARAMETER   0.5

/**
 * Class:       Vertex
 * Purpose:     Generating vertice and printing their own coordinates.
 */
class Vertex {
public:
    double m_x, m_y, m_z;

/**
 * Function:    Default constructor
 */
    Vertex()
        : m_x(0.0), m_y(0.0), m_z(0.0) {};

/**
 * Function:    Copy constructor
 */
    Vertex( const double x,
            const double y,
            const double z)
        : m_x(x), m_y(y), m_z(z) {};

/**
 * Operator:    << (overloading)
 * Purpose:     Print coordinates to output stream.
 */
    friend std::ostream &operator<<(std::ostream& strm,
                                    const Vertex& obj) {
        strm << "(" << obj.m_x << ", "
                    << obj.m_y << ", "
                    << obj.m_z << ")";
    };

/**
 * Function:    Destructor
 */
    ~Vertex() {};
};

/* Binomial Coefficient */
int N_Choose_K (int degree_n,
                int index_k);

/* Curve Algorithms */
Vertex Bezier(  const Vertex* arr_control_points,
                int degree_n,
                double parameter_t);
Vertex DeCasteljau( const Vertex* arr_control_points,
                    int degree_n,
                    double parameter_t);

/**
 * Main
 */
 int main(int argc, char* argv[]) {
    /* Initialize control points as constant variable */
    const Vertex P0(0.0, 0.0, 0.0);
    const Vertex P1(0.0, 3.0, 0.0);
    const Vertex P2(1.0, 3.0, 0.0);
    const Vertex P3(1.0, 0.0, 0.0);
    /* Insert control points to array */
    const Vertex arr_control_points_list[DEGREE+1] = {P0, P1, P2, P3};

    std::cout << "Degree of curve: " << DEGREE
                                     << std::endl;
    std::cout << "Control points: "  << P0 << ", "
                                     << P1 << ", "
                                     << P2 << ", "
                                     << P3
                                     << std::endl;
    std::cout << "Parameter: "       << PARAMETER
                                     << std::endl;

    /* Calculate vertex on Bezier curve */
    Vertex curve = Bezier(arr_control_points_list, DEGREE, PARAMETER);
    std::cout << "Bezier curve: " << curve << std::endl;
    /* Calculate vertex by using de Casteljau algorithm */
    curve = DeCasteljau(arr_control_points_list, DEGREE, PARAMETER);
    std::cout << "de Casteljau: " << curve << std::endl;

    return 0;
} /* main */

/**
 * Function:    Bezier
 * Purpose:     Calculation of coordinates on Bezier curve
 *              w.r.t. the value of t.
 * In args:     arr_control_points  (list of control points)
 *              degree_n            (degree of curve)
 *              parameter_t         (the value of parameter)
 * Out arg:     trace_on_curve      (vertex on Bezier curve)
 */
Vertex Bezier(  const Vertex* arr_control_points,
                int degree_n,
                double parameter_t) {
    Vertex trace_on_curve(0.0, 0.0, 0.0);

    /* Check parameter's errors */
    if (parameter_t < 0 || parameter_t > 1) {
        std::cerr   << "ERROR::FUNC::Bezier: "
                    << "Variable parameter_t must be the value from 0 to 1."
                    << std::endl;
    }
    /* Check degree's errors */
    if (degree_n < 2) {
        std::cerr   << "ERROR::FUNC::Bezier: "
                    << "Control points must be more than or equal to 3 for Bezier curve."
                    << std::endl;
    }
    /* Calculate coordinates of Bezier curve */
    else {
        for (int k = 0; k <= degree_n; k++) {
            /* Calculate Bernstein basis function */
            double bernstein =   N_Choose_K(degree_n, k)
                                * std::pow(1 - parameter_t  , degree_n - k)
                                * std::pow(parameter_t      , k);
            /* Multiply control point and berstein basis */
            trace_on_curve.m_x += arr_control_points[k].m_x * bernstein;
            trace_on_curve.m_y += arr_control_points[k].m_y * bernstein;
            trace_on_curve.m_z += arr_control_points[k].m_z * bernstein;
        }
    }

    return trace_on_curve;
} /* Bezier */

/**
 * Function:    N_Choose_K
 * Purpose:     Calculation of binomial coefficient
 * In args:     degree_n            (degree of binomial)
 *              index_k             ('k'th picking value)
 * Out arg:     result              (binomial coefficient)
 */
int N_Choose_K (int degree_n,
                int index_k) {
    int result, denominator = 1, numerator = 1;

    /* Check degree's errors */
    if (degree_n < 0) {
        std::cerr   << "ERROR::FUNC::N_Choose_K: "
                    << "Variable degree_n must be greater than 0"
                    << std::endl;
        result = 0;
    }
    /* Check restriction */
    if (degree_n < index_k) {
        std::cerr   << "ERROR::FUNC::N_Choose_K: "
                    << "Variable degree_n must be greater than index_k."
                    << std::endl;
        result = 0;
    }
    /* Calculate N choose K */
    else {
        if (degree_n == 0 || index_k == 0) {
            result = 1;
        }
        else if (index_k == 1) {
            result = degree_n;
        }
        /* Compare which of two values, N and K, is greater */
        else if (degree_n > index_k<<1) {
            /* n*(n-1)*...*(n-k+1) / k*(k-1)*...*1 */
            for (int i = 1; i <= index_k; i++) {
                denominator *= i;
                numerator *= degree_n - (i-1);
            }
            result = numerator / denominator;
        }
        else {
            /* n*(n-1)*...*(k+1) / (n-k)*(n-k-1)*...*1 */
            for (int i = 1; i <= degree_n - index_k; i++) {
                denominator *= i;
                numerator *= degree_n - (i-1);
            }
            result = numerator / denominator;
        }
    }

    return result;
} /* N_Choose_K */

/**
 * Function:    DeCasteljau
 * Purpose:     Calculation of de Casteljau algorithm
 * In args:     arr_control_points  (list of control points)
 *              degree_n            (degree of curve)
 *              parameter_t         (the value of parameter)
 * Out arg:     trace_on_curve      (vertex on Bezier curve)
 */
Vertex DeCasteljau( const Vertex* arr_control_points,
                    int degree_n,
                    double parameter_t) {
    Vertex trace_on_curve(0.0, 0.0, 0.0);

    /* Check parameter's errors */
    if (parameter_t < 0 || parameter_t > 1) {
        std::cerr   << "ERROR::FUNC::DeCasteljau: "
                    << "Variable parameter_t must be the value from 0 to 1."
                    << std::endl;
    }
    /* Check degree's errors */
    if (degree_n < 2) {
        std::cerr   << "ERROR::FUNC::DeCasteljau: "
                    << "Control points must be more than or equal to 3 for Bezier curve."
                    << std::endl;
    }
    /* Calculate coordinates of Bezier curve by using de Casteljau algorithm */
    else {
        /* Initialize list of points for curve fitting */
        Vertex* arr_curve_fitting_points = new Vertex[degree_n + 1];
        for (size_t i = 0; i <= degree_n; i++) {
            arr_curve_fitting_points[i] = arr_control_points[i];
        }
        /* 'k'th curve fitting point at step 'r' =
         *                      (1 - t) * ('k-1'th curve fitting point at step 'r-1')
         *                          +t  * ('k'th curve fitting point at step 'r-1')
        */
        for (int r = 1; r <= degree_n; r++) {
            for (int k = 1; k <= degree_n - r + 1; k++) {
                arr_curve_fitting_points[k - 1].m_x
                        =   (1 - parameter_t) * arr_curve_fitting_points[k - 1].m_x
                            + parameter_t     * arr_curve_fitting_points[k].m_x;
                arr_curve_fitting_points[k - 1].m_y
                        =   (1 - parameter_t) * arr_curve_fitting_points[k - 1].m_y
                            + parameter_t     * arr_curve_fitting_points[k].m_y;
                arr_curve_fitting_points[k - 1].m_z
                        =   (1 - parameter_t) * arr_curve_fitting_points[k - 1].m_z
                            + parameter_t     * arr_curve_fitting_points[k].m_z;
            }
        }

        trace_on_curve = arr_curve_fitting_points[0];
        delete[] arr_curve_fitting_points;
    }

    return trace_on_curve;
} /* DeCasteljau */

/**
 * Function:    CoxDeBoor
 * Purpose:     Calculation of de Casteljau algorithm
 * In args:     arr_control_points  (list of control points)
 *              degree_N            (degree of curve)
 *              knots_n             (the number of knots)
 *              parameter_t         (the value of parameter)
 * Out arg:     trace_on_curve      (vertex on Bezier curve)
 */
Vertex CoxDeBoor( const Vertex* arr_control_points,
                    int degree_n,
                    int control_point_N,
                    const float parameter_t,
                    const Vertex* v_knots) {
    Vertex trace_on_curve(0.0, 0.0, 0.0);

    /* Check parameter's errors */
    if (parameter_t < 0 || parameter_t > 1) {
        std::cerr   << "ERROR::FUNC::DeCasteljau: "
                    << "Variable parameter_t must be the value from 0 to 1."
                    << std::endl;
    }
    /* Check degree's errors */
    if (degree_n < 2) {
        std::cerr   << "ERROR::FUNC::DeCasteljau: "
                    << "Control points must be more than or equal to 3 for Bezier curve."
                    << std::endl;
    }
    /* Calculate coordinates of Bezier curve by using de Casteljau algorithm */
    else {
        /* Initialize list of points for curve fitting */
        Vertex* arr_curve_fitting_points = new Vertex[degree_n + 1];
        for (size_t i = 0; i <= degree_n; i++) {
            arr_curve_fitting_points[i] = arr_control_points[i];
        }
        /* 'k'th curve fitting point at step 'r' =
         *                      (1 - t) * ('k-1'th curve fitting point at step 'r-1')
         *                          +t  * ('k'th curve fitting point at step 'r-1')
        */
        for (int r = 1; r <= degree_n; r++) {
            for (int i = control_point_N - degree_n + r; i <= control_point_N; k++) {
                float tou =
                    (parameter_t - v_knots[i]) /
                    (v_knots[i - degree_n + control_point_N + 1] - v_knots[i]);
                arr_curve_fitting_points[k - 1].m_x
                        =   (1 - tou) * arr_curve_fitting_points[k - 1].m_x
                            + tou     * arr_curve_fitting_points[k].m_x;
                arr_curve_fitting_points[k - 1].m_y
                        =   (1 - tou) * arr_curve_fitting_points[k - 1].m_y
                            + tou     * arr_curve_fitting_points[k].m_y;
                arr_curve_fitting_points[k - 1].m_z
                        =   (1 - tou) * arr_curve_fitting_points[k - 1].m_z
                            + tou     * arr_curve_fitting_points[k].m_z;
            }
        }

        trace_on_curve = arr_curve_fitting_points[0];
        delete[] arr_curve_fitting_points;
    }

    return trace_on_curve;
} /* DeCasteljau */
