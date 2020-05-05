#include <plonk/proof_system/verification_key/verification_key.hpp>
#include "../client_proofs/join_split/join_split.hpp"

namespace rollup {
namespace rollup_proofs {

using namespace rollup::client_proofs::join_split;
using namespace plonk::stdlib::types::turbo;
using namespace rollup::rollup_proofs;

struct inner_circuit_data {
    std::shared_ptr<waffle::verification_key> verification_key;
    size_t proof_size;
};

inner_circuit_data compute_inner_circuit_data()
{
    std::cerr << "Generating join-split circuit keys..." << std::endl;

    // Junk data required just to create keys.
    join_split_tx tx;
    tx.input_path[0].resize(32);
    tx.input_path[1].resize(32);

    Composer composer = Composer("../srs_db/ignition");
    join_split_circuit(composer, tx);

    std::cerr << "Circuit size: " << composer.get_num_gates() << std::endl;
    auto proving_key = composer.compute_proving_key();
    auto verification_key = composer.compute_verification_key();
    // TODO: Avoid?
    auto prover = composer.create_prover();
    auto proof = prover.construct_proof();
    std::cerr << "Done." << std::endl;

    return { verification_key, proof.proof_data.size() };
}

} // namespace rollup_proofs
} // namespace rollup