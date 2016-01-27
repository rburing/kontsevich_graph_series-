#include "kontsevich_graph.hpp"
#include <algorithm>

inline std::pair<size_t, size_t> exchange_pair(std::pair<size_t, size_t> p)
{
    return { p.second, p.first };
}

KontsevichGraph::KontsevichGraph(size_t internal, size_t external, std::vector< std::pair<size_t, size_t> > targets, bool normalized)
: d_internal(internal), d_external(external), d_targets(targets)
{
    if (!normalized)
    {
        size_t exchanges = 0;
        for (auto target = d_targets.begin(); target != d_targets.end(); target++)
        {
            std::pair<size_t, size_t> exchanged = exchange_pair(*target);
            if (exchanged < *target)
            {
                *target = exchanged;
                exchanges++;
            }
        }
        sort(d_targets.begin(), d_targets.end());
        d_sign = (exchanges % 2 == 0) ? 1 : -1;
    }
    else
        d_sign = 1;
}

std::vector<size_t> KontsevichGraph::internal_vertices() const
{
    std::vector<size_t> vertices(d_internal);
    std::iota(vertices.begin(), vertices.end(), d_external);
    return vertices;
}

std::pair<size_t, size_t> KontsevichGraph::targets(size_t internal_vertex) const
{
    return d_targets[internal_vertex - d_external];
}

int KontsevichGraph::sign() const
{
    return d_sign;
}

int KontsevichGraph::sign(int new_sign)
{
    return d_sign = new_sign;
}

std::pair< size_t, std::vector< std::pair<size_t, size_t> > > KontsevichGraph::abs() const
{
    return { d_external, d_targets };
}

size_t KontsevichGraph::internal() const
{
    return d_internal;
}

size_t KontsevichGraph::external() const
{
    return d_external;
}

bool operator==(const KontsevichGraph &lhs, const KontsevichGraph &rhs)
{
    return (lhs.d_external == rhs.d_external) && (lhs.d_sign == rhs.d_sign) && (lhs.d_targets == rhs.d_targets);
}

bool operator!=(const KontsevichGraph &lhs, const KontsevichGraph &rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream &os, const KontsevichGraph& g)
{
    return os << "Kontsevich graph with " << g.d_internal << " vertices on " << g.d_external << " ground vertices";
}

