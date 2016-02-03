#include "kontsevich_graph.hpp"
#include "util/sort_pairs.hpp"
#include "util/cartesian_product.hpp"
#include <algorithm>
#include <tuple>

KontsevichGraph::KontsevichGraph()
: d_sign(1)
{}

KontsevichGraph::KontsevichGraph(size_t internal, size_t external, std::vector< std::pair<size_t, size_t> > targets, int sign, bool normalized)
: d_internal(internal), d_external(external), d_targets(targets), d_sign(sign)
{
    if (!normalized)
        normalize();
}

void KontsevichGraph::normalize()
{
    std::vector< std::pair<size_t, size_t> > global_minimum = d_targets;
    size_t exchanges = sort_pairs(global_minimum.begin(), global_minimum.end());
    std::vector<size_t> vertices(d_external + d_internal);
    std::iota(vertices.begin(), vertices.end(), 0);
    while (std::next_permutation(vertices.begin() + d_external, vertices.end()))
    {
        std::vector< std::pair<size_t, size_t> > local_minimum = d_targets;
        // Relabel elements of target pairs
        for (size_t i = 0; i != d_internal; ++i) {
            local_minimum[i].first = vertices[local_minimum[i].first];
            local_minimum[i].second = vertices[local_minimum[i].second];
        }
        // Apply permutation to list of target pairs
        std::vector< std::pair<size_t, size_t> > permuted(d_internal);
        for (size_t i = 0; i != d_internal; ++i)
        {
            permuted[vertices[d_external + i] - d_external] = local_minimum[i];
        }
        local_minimum.swap(permuted);
        // Sort elements of target pairs
        size_t local_exchanges = sort_pairs(local_minimum.begin(), local_minimum.end());
        if (local_minimum < global_minimum) {
            global_minimum = local_minimum;
            exchanges = local_exchanges;
        }
    }
    d_targets = global_minimum;
    d_sign *= (exchanges % 2 == 0) ? 1 : -1;
}

std::vector<size_t> KontsevichGraph::internal_vertices() const
{
    std::vector<size_t> vertices(d_internal);
    std::iota(vertices.begin(), vertices.end(), d_external);
    return vertices;
}

std::vector< std::pair<size_t, size_t> > KontsevichGraph::targets() const
{
    return d_targets;
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

size_t KontsevichGraph::vertices() const
{
    return d_internal + d_external;
}

std::vector<size_t> KontsevichGraph::in_degrees() const
{
    std::vector<size_t> indegrees(d_external);
    for (auto& target_pair : d_targets)
    {
        if (target_pair.first < d_external)
            indegrees[target_pair.first]++;
        if (target_pair.second < d_external)
            indegrees[target_pair.second]++;
    }
    return indegrees;
}

std::vector<size_t> KontsevichGraph::neighbors_in(size_t vertex) const
{
    std::vector<size_t> neighbors;
    for (size_t idx = 0; idx < d_internal; ++idx)
    {
        if (d_targets[idx].first == vertex || d_targets[idx].second == vertex)
            neighbors.push_back(d_external + idx);
    }
    return neighbors;
}

bool KontsevichGraph::operator<(const KontsevichGraph& rhs) const
{
    return std::tie(this->d_external, this->d_internal, this->d_targets, this->d_sign) < std::tie(rhs.d_external, rhs.d_internal, rhs.d_targets, rhs.d_sign);
}

std::set<KontsevichGraph> KontsevichGraph::graphs(size_t internal, size_t external, bool modulo_signs)
{
    std::set<KontsevichGraph> result;
    std::vector<size_t> ends(2*internal);
    for (size_t i = 0; i != 2*internal; ++i)
    {
        ends[i] = internal + external;
    }
    CartesianProduct graph_encodings(ends);
    std::vector< std::pair <size_t, size_t> > targets(internal);
    for (auto graph_encoding = graph_encodings.begin(); graph_encoding != graph_encodings.end(); ++graph_encoding)
    {
        bool skip = false;
        for (size_t i = 0; i != internal; ++i)
        {
            std::pair<size_t, size_t> target_pair = { (*graph_encoding)[2*i], (*graph_encoding)[2*i + 1] };
            // Avoid double edges and tadpoles:
            if (target_pair.first == target_pair.second || target_pair.first == external + i || target_pair.second == external + i)
            {
                skip = true;
                break;
            }
            targets[i] = target_pair;
        }
        if (!skip)
        {
            KontsevichGraph graph(internal, external, targets);
            if (modulo_signs)
                graph.sign(1);
            result.insert(graph);
        }
    }
    return result;
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

std::istream& operator>>(std::istream& is, KontsevichGraph& g)
{
    is >> g.d_external;
    is >> g.d_internal;
    is >> g.d_sign;
    g.d_targets.clear();
    std::string line;
    std::pair<size_t, size_t> target_pair;
    size_t pair_count = 0;
    while (pair_count++ < g.d_internal && is >> target_pair.first >> target_pair.second)
        g.d_targets.push_back(target_pair);
    g.d_internal = g.d_targets.size();
    g.normalize();
    return is;
}
