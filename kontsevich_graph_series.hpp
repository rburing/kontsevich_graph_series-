#ifndef INCLUDED_KONTSEVICH_GRAPH_SERIES_H_
#define INCLUDED_KONTSEVICH_GRAPH_SERIES_H_

#include "kontsevich_graph_sum.hpp"
#include <ostream>
#include <map>
#include <limits>

template<class T> class KontsevichGraphSeries;
template<class T> std::ostream& operator<<(std::ostream&, const KontsevichGraphSeries<T>&);

template<class T>
class KontsevichGraphSeries : public std::map< size_t, KontsevichGraphSum<T> >
{
    size_t d_precision = std::numeric_limits<std::size_t>::max();

    using std::map< size_t, KontsevichGraphSum<T> >::map; // inherit constructors

    public:
    size_t precision() const;
    size_t precision(size_t new_precision);
    void reduce();
    KontsevichGraphSeries<T> operator()(std::vector< KontsevichGraphSeries<T> > arguments);
    KontsevichGraphSeries<T>& operator+=(const KontsevichGraphSeries<T>& rhs);
    KontsevichGraphSeries<T>& operator-=(const KontsevichGraphSeries<T>& rhs);

    friend std::ostream& operator<< <>(std::ostream& os, const KontsevichGraphSeries<T>& series);
};

template <class T>
KontsevichGraphSeries<T> operator+(KontsevichGraphSeries<T> lhs, const KontsevichGraphSeries<T>& rhs);
template <class T>
KontsevichGraphSeries<T> operator-(KontsevichGraphSeries<T> lhs, const KontsevichGraphSeries<T>& rhs);

#include "kontsevich_graph_series.tpp"

#endif
