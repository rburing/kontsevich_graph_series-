#include <vector>
#include <utility>
#include <ostream>
#include <kontsevich_graph.hpp>

template<class T> class KontsevichGraphSum;
template<class T> std::ostream& operator<<(std::ostream&, const KontsevichGraphSum<T>&);

template<class T>
class KontsevichGraphSum
{
    std::vector< std::pair<T, KontsevichGraph> > d_terms;

    public:
    KontsevichGraphSum(std::vector< std::pair<T, KontsevichGraph> > terms);
    size_t size() const;

    friend std::ostream& operator<< <>(std::ostream& os, const KontsevichGraphSum<T>& gs);
};
