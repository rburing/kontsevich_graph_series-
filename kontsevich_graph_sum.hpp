#ifndef INCLUDED_KONTSEVICH_GRAPH_SUM_H_
#define INCLUDED_KONTSEVICH_GRAPH_SUM_H_

#include <vector>
#include <utility>
#include <ostream>
#include <kontsevich_graph.hpp>

template<class T> class KontsevichGraphSum;
template<class T> std::ostream& operator<<(std::ostream&, const KontsevichGraphSum<T>&);

template<class T>
class KontsevichGraphSum : public std::vector< std::pair<T, KontsevichGraph> >
{
    using std::vector< std::pair<T, KontsevichGraph> >::vector; // inherit constructors
    public:
    void reduce();

    friend std::ostream& operator<< <>(std::ostream& os, const KontsevichGraphSum<T>& gs);
};

#endif
