#ifndef OSRM_STORAGE_IO_HPP_
#define OSRM_STORAGE_IO_HPP_

#include "util/fingerprint.hpp"
#include "util/simple_logger.hpp"

#include <boost/filesystem/fstream.hpp>

#include <tuple>

namespace osrm
{
namespace storage
{
namespace io
{

// Returns the checksum and the number of nodes and number of edges
std::tuple<unsigned, std::size_t, std::size_t> readHSGRSize(boost::filesystem::ifstream &input_stream)
{
    const util::FingerPrint fingerprint_valid = util::FingerPrint::GetValid();
    util::FingerPrint fingerprint_loaded;
    input_stream.read(reinterpret_cast<char *>(&fingerprint_loaded), sizeof(util::FingerPrint));
    if (!fingerprint_loaded.TestGraphUtil(fingerprint_valid))
    {
        util::SimpleLogger().Write(logWARNING) << ".hsgr was prepared with different build.\n"
                                                  "Reprocess to get rid of this warning.";
    }

    unsigned check_sum = 0;
    unsigned number_of_nodes = 0;
    unsigned number_of_edges = 0;
    BOOST_ASSERT_MSG(0 != number_of_nodes, "number of nodes is zero");
    BOOST_ASSERT_MSG(0 != number_of_edges, "number of edges is zero");
    input_stream.read(reinterpret_cast<char *>(check_sum), sizeof(unsigned));
    input_stream.read(reinterpret_cast<char *>(&number_of_nodes), sizeof(unsigned));
    input_stream.read(reinterpret_cast<char *>(&number_of_edges), sizeof(unsigned));

    return std::make_tuple(check_sum, number_of_nodes, number_of_edges);
}

// Needs to be called after getHSGRSize() to get the correct offset in the stream
template <typename NodeT, typename EdgeT>
void readHSGR(boost::filesystem::ifstream &input_stream,
              NodeT *node_buffer,
              std::size_t number_of_nodes,
              EdgeT *edge_buffer,
              std::size_t number_of_edges)
{
    input_stream.read(reinterpret_cast<char *>(node_buffer), number_of_nodes * sizeof(NodeT));
    input_stream.read(reinterpret_cast<char *>(edge_buffer), number_of_edges * sizeof(EdgeT));
}

}
}
}

#endif
